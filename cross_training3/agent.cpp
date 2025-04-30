#include "headers/agent.hpp"
#include<random>
#include<fstream>
#include<chrono>
#include<unordered_map>
using namespace std;

ReplayBuffer::ReplayBuffer(long capacity,torch::DeviceType device,torch::DeviceType largerMemoryDevice){
    this->capacity=capacity;
    this->device=device;
    this->memory=std::deque<std::tuple<torch::Tensor,torch::Tensor,torch::Tensor,torch::Tensor,torch::Tensor>>();
    this->largerMemoryDevice=largerMemoryDevice;
}



void ReplayBuffer::insert(const std::tuple<torch::Tensor, torch::Tensor, torch::Tensor, torch::Tensor, torch::Tensor> transition) {
    // Move tensor to the correct device
    std::tuple<torch::Tensor, torch::Tensor, torch::Tensor, torch::Tensor, torch::Tensor> cpuTuple = std::make_tuple(
        get<0>(transition).to(this->largerMemoryDevice),
        get<1>(transition).to(this->largerMemoryDevice),
        get<2>(transition).to(this->largerMemoryDevice),
        get<3>(transition).to(this->largerMemoryDevice),
        get<4>(transition).to(this->largerMemoryDevice)
    );

    // Append at the end
    this->memory.push_back(cpuTuple);

    // Remove the oldest if capacity is exceeded
    if (memory.size() > capacity) {
        memory.pop_front();  // Removes oldest element in O(1) time
    }
}

// vector<torch::Tensor> ReplayBuffer::sample(int batchSize=64){
//     std::vector<std::tuple<torch::Tensor,torch::Tensor,torch::Tensor,torch::Tensor,torch::Tensor>> batch;
//         std::sample(memory.begin(), memory.end(), std::back_inserter(batch), batchSize,mt19937{std::random_device{}()});

//     std::vector<torch::Tensor> states, actions, rewards, done, next_states;
//     for (std::tuple<torch::Tensor,torch::Tensor,torch::Tensor,torch::Tensor,torch::Tensor> experience : batch) {
//         states.push_back(get<0>(experience).to(device));
//         actions.push_back(get<1>(experience).to(device));
//         rewards.push_back(get<2>(experience).to(device));
//         done.push_back(get<3>(experience).to(device));
//         next_states.push_back(get<4>(experience).to(device));
//     }
//     torch::Tensor states_tensor = torch::cat(states);
//     torch::Tensor actions_tensor = torch::cat(actions);
//     torch::Tensor rewards_tensor = torch::cat(rewards);
//     torch::Tensor done_tensor=torch::cat(done);
//     torch::Tensor next_states_tensor = torch::cat(next_states);
//     return {states_tensor, actions_tensor, rewards_tensor, done_tensor, next_states_tensor};
// }

vector<torch::Tensor> ReplayBuffer::sample(int batchSize = 64) {
    std::vector<size_t> indices(memory.size());
    std::iota(indices.begin(), indices.end(), 0);  // Fill with 0, 1, 2, ..., N-1
    std::shuffle(indices.begin(), indices.end(), std::mt19937{std::random_device{}()});

    std::vector<torch::Tensor> states(batchSize), actions(batchSize), rewards(batchSize), done(batchSize), next_states(batchSize);

    for (int i = 0; i < batchSize; ++i) {
        auto& experience = memory[indices[i]];  // Avoids copying entire batch
        states[i] = std::get<0>(experience).to(device);
        actions[i] = std::get<1>(experience).to(device);
        rewards[i] = std::get<2>(experience).to(device);
        done[i] = std::get<3>(experience).to(device);
        next_states[i] = std::get<4>(experience).to(device);
    }

    return {
        torch::cat(states),
        torch::cat(actions),
        torch::cat(rewards),
        torch::cat(done),
        torch::cat(next_states)
    };
}


bool ReplayBuffer::canSample(int batchSize){
    if(this->memory.size()>=5*batchSize){
        return true;
    }
    return false;
}

Agent::Agent(CubeNetImpl model,torch::DeviceType device,torch::DeviceType largerMemoryDevice,float epsilon,float minEpsilon,
    int numWarmup,int numActions,long capacity,int batchSize,float learningRate,float gamma){
        this->model = make_shared<CubeNetImpl>(model);
        this->targetModel = make_shared<CubeNetImpl>(model);  
        this->targetModel->eval();  
        this->epsilon=epsilon;
        this->epsilonDecay=epsilonDecay;
        this->minEpsilon=minEpsilon;
        this->batchSize=batchSize;
        this->numActions=numActions;
        this->gamma=gamma;
        this->learningRate=learningRate;
        this->device=device;
        this->largerMemoryDevice=largerMemoryDevice;
        this->optimizer=make_shared<torch::optim::Adam>(this->model->parameters(),torch::optim::AdamOptions(this->learningRate));
        this->memory=make_shared<ReplayBuffer>(capacity,this->device,this->largerMemoryDevice);
        this->model->to(this->device);
        this->targetModel->to(this->device);
        cout<<"Starting epsilon is "<<this->epsilon<<endl;
        cout<<"epsilon decay is "<<this->epsilonDecay<<endl;
}

torch::Tensor Agent::getAction(torch::Tensor state,long epoch){
    if(torch::rand({1}).item<float>() < this->epsilon){
        return torch::randint(0,this->numActions,{1,1});
    }
    else{
        torch::Tensor av=this->model->forward(state).detach();
        if(epoch%4000==0){
            cout<<"Q values:"<<av<<endl;
        cout<<"[R(0,1),R(0,2),R(0,3),R(0,4),Rprime(0,1),Rprime(0,2),Rprime(0,3),Rprime(0,4),U(0),U(5),Uprime(0),Uprime(5),M(0,1),M(0,2),Mprime(0,1),Mprime(0,2)]"<<endl;
        cout<<"max: "<<torch::argmax(av, 1,true).item<long>()<<endl;
        }
        return torch::argmax(av, 1,true);
    }
}

void Agent::train(Env env,long epochs,long initialBias){
    unordered_map<string,vector<float>> stats={};
    stats["Returns"]=vector<float>();
    stats["AvgReturns"]=vector<float>();
    stats["EpsilonCheckpoint"]=vector<float>();
    tuple<torch::Tensor, torch::Tensor, torch::Tensor, Stage> result;
    vector<torch::Tensor> sampleResult;
    torch::Tensor state,action,reward,doneTensor,next_state,state_b,action_b,reward_b,done_b,target_q_value_b,next_state_b,predicted_q_value_b,next_q_value_b,loss;
    bool done;
    long i;
    float ep_return;
    for(int epoch=0;epoch<epochs;epoch++){
        state=env.reset();
        done=false;
        ep_return=0;
        int i=0;
        while(!done){
            action=this->getAction(state,epoch).to(device);
            result=env.step(action,false);
            next_state=get<0>(result);
            reward=get<1>(result);
            done=get<2>(result).item<bool>();
            doneTensor=get<2>(result);
            env.stage=get<3>(result);
            this->memory->insert(make_tuple(state,action,reward,doneTensor,next_state));
            if(this->memory->canSample(this->batchSize)){
                sampleResult=this->memory->sample(this->batchSize);
                state_b=sampleResult[0],action_b=sampleResult[1],reward_b=sampleResult[2];
                done_b=sampleResult[3],next_state_b=sampleResult[4];
                predicted_q_value_b = this->model->forward(state_b).gather(1, action_b);
                next_q_value_b = this->targetModel->forward(next_state_b);
                next_q_value_b =get<0>(next_q_value_b.max(-1, true)); 
                target_q_value_b = reward_b + (~done_b * gamma * next_q_value_b);
                loss = torch::mse_loss(predicted_q_value_b, target_q_value_b);
                this->model->zero_grad();
                loss.backward();
                this->optimizer->step();
            }
            state=next_state;
            ep_return+=reward.item<float>();
        }
        // cout<<env.totalSteps<<endl;
        // while (!done) {
        //     i++;
        //     auto start = std::chrono::high_resolution_clock::now();
        
        //     auto t1 = std::chrono::high_resolution_clock::now();
        //     action = this->getAction(state);
        //     auto t2 = std::chrono::high_resolution_clock::now();
        
        //     result = env.step(action);
        //     auto t3 = std::chrono::high_resolution_clock::now();
        
        //     next_state = get<0>(result);
        //     reward = get<1>(result);
        //     done = get<2>(result).item<bool>();
        //     doneTensor = get<2>(result);
        //     env.stage = get<3>(result);
        //     auto t4 = std::chrono::high_resolution_clock::now();
        
        //     this->memory->insert(make_tuple(state, action, reward, doneTensor, next_state));
        //     auto t5 = std::chrono::high_resolution_clock::now();
        
        //     if (this->memory->canSample(this->batchSize)) {
        //         auto t6 = std::chrono::high_resolution_clock::now();
        //         sampleResult = this->memory->sample(this->batchSize);
        //         state_b = sampleResult[0], action_b = sampleResult[1], reward_b = sampleResult[2];
        //         done_b = sampleResult[3], next_state_b = sampleResult[4];
        //         auto t7 = std::chrono::high_resolution_clock::now();
        
        //         predicted_q_value_b = this->model->forward(state_b).gather(1, action_b);
        //         auto t8 = std::chrono::high_resolution_clock::now();
        
        //         next_q_value_b = this->targetModel->forward(next_state_b);
        //         next_q_value_b = get<0>(next_q_value_b.max(-1, true)); 
        //         auto t9 = std::chrono::high_resolution_clock::now();
        
        //         target_q_value_b = reward_b + (~done_b * gamma * next_q_value_b);
        //         auto t10 = std::chrono::high_resolution_clock::now();
        
        //         loss = torch::mse_loss(predicted_q_value_b, target_q_value_b);
        //         auto t11 = std::chrono::high_resolution_clock::now();
        
        //         this->model->zero_grad();
        //         loss.backward();
        //         this->optimizer->step();
        //         auto t12 = std::chrono::high_resolution_clock::now();
        //         if(i%200==0){
        //             std::cout << "Sample Memory: " << std::chrono::duration<double>(t7 - t6).count() << "s\n";
        //         std::cout << "Model Forward Pass: " << std::chrono::duration<double>(t8 - t7).count() << "s\n";
        //         std::cout << "Target Model Forward Pass: " << std::chrono::duration<double>(t9 - t8).count() << "s\n";
        //         std::cout << "Calculate Target Q: " << std::chrono::duration<double>(t10 - t9).count() << "s\n";
        //         std::cout << "Loss Calculation: " << std::chrono::duration<double>(t11 - t10).count() << "s\n";
        //         std::cout << "Backward + Optimizer Step: " << std::chrono::duration<double>(t12 - t11).count() << "s\n";
        //         }
        //     }
        
            // state = next_state;
            // ep_return += reward.item<float>();
            
            // auto end = std::chrono::high_resolution_clock::now();
        
            // // Print time taken for each part
            // if(i==200){
            //     std::cout << "Get Action: " << std::chrono::duration<double>(t2 - t1).count() << "s\n";
            // std::cout << "Step: " << std::chrono::duration<double>(t3 - t2).count() << "s\n";
            // std::cout << "Processing Result: " << std::chrono::duration<double>(t4 - t3).count() << "s\n";
            // std::cout << "Insert into Memory: " << std::chrono::duration<double>(t5 - t4).count() << "s\n";
        
            // std::cout << "Total Step Time: " << std::chrono::duration<double>(end - start).count() << "s\n\n";
            // }
        // }
        stats["Returns"].push_back(ep_return);
        if(this->epsilon>this->minEpsilon){
            this->epsilon = max(0.1, 0.1 - (epoch * 0.00005));

        }
        if(env.stage==Stage::F2L1pair){
            cout<<"Stage:F2L pair 1"<<endl;
        }
        else if(env.stage==Stage::F2L2pair){
            cout<<"Stage:F2L pair 2"<<endl;
        }
        else if(env.stage==Stage::F2L3pair){
            cout<<"Stage:F2L pair 3"<<endl;
        }
        else if(env.stage==Stage::F2L4pair){
            cout<<"Stage:F2L pair 4"<<endl;
        }
        else if(env.stage==Stage::YellowCross){
            cout<<"Stage:Yellow cross"<<endl;
        }
        else if(env.stage==Stage::OLL){
            cout<<"Stage:OLL"<<endl;
        }
        else if(env.stage==Stage::PLL){
            cout<<"Stage:PLL"<<endl;
        }
        else if(env.stage==Stage::complete){
            cout<<"cube solved"<<endl;
        }
        if(epoch%1000==1){
            this->model->saveModel("weights/best.pt");
        }
        if (epoch % 400== 0) {
            // Compute the average of the last 100 elements in stats["Returns"]
            int size = stats["Returns"].size();
            int start_idx = std::max(0, size - 400); // Ensure we don't go out of bounds
    
            double sum = std::accumulate(stats["Returns"].begin() + start_idx, stats["Returns"].end(), 0.0);
            double average_returns = sum / std::min(400, size);
    
            stats["AvgReturns"].push_back(average_returns);
            stats["EpsilonCheckpoint"].push_back(epsilon);
    
            // Print stats
            if (size > 100) {
                std::cout << "Epoch: " << epoch
                          << ", Average Returns: " << stats["AvgReturns"].back()
                          << ", Return: " << stats["Returns"].back()
                          << ", Epsilon: " << epsilon << " ,Totalsteps:"<<env.totalSteps<<std::endl;
            } else {
                double sum_all = std::accumulate(stats["Returns"].begin(), stats["Returns"].end(), 0.0);
                double avg_all = sum_all / size;
    
                std::cout << "Epoch: " << epoch
                          << ", Average Returns: " << stats["AvgReturns"].back()
                          << ", Return: " << stats["Returns"].back()
                          << ", Epsilon: " << epsilon << " ,Totalsteps:"<<env.totalSteps<<std::endl;
            }
            if(env.stage==Stage::Cross1){
                cout<<"Stage:cross1"<<endl;
            }
            else if(env.stage==Stage::Cross2){
                cout<<"Stage:cross2"<<endl;
            }
            if(env.stage==Stage::Cross3){
                cout<<"Stage:cross3"<<endl;
            }
                else if(env.stage==Stage::Cross){
                    cout<<"Stage:cross"<<endl;
                }
                else if(env.stage==Stage::F2L1pair){
                    cout<<"Stage:F2L pair 1"<<endl;
                }
                else if(env.stage==Stage::F2L2pair){
                    cout<<"Stage:F2L pair 2"<<endl;
                }
                else if(env.stage==Stage::F2L3pair){
                    cout<<"Stage:F2L pair 3"<<endl;
                }
                else if(env.stage==Stage::F2L4pair){
                    cout<<"Stage:F2L pair 4"<<endl;
                }
                else if(env.stage==Stage::YellowCross){
                    cout<<"Stage:Yellow cross"<<endl;
                }
                else if(env.stage==Stage::OLL){
                    cout<<"Stage:OLL"<<endl;
                }
                else if(env.stage==Stage::PLL){
                    cout<<"Stage:PLL"<<endl;
                }
                else if(env.stage==Stage::complete){
                    cout<<"cube solved"<<endl;
                }
        }

        if(epoch%400==0){
            for (size_t i = 0; i < this->model->parameters().size(); i++) {
                this->targetModel->parameters()[i].data().copy_(this->model->parameters()[i].data());
            }
        }
        if (epoch % 4000 == 0) { 
            // Save data to CSV
            std::ofstream file("data.csv");
            if (!file.is_open()) {
                std::cerr << "Error opening file for writing!" << std::endl;
                return;
            }

            for (size_t i = 0; i < stats["AvgReturns"].size(); ++i) {
                file << stats["AvgReturns"][i] << "," << stats["EpsilonCheckpoint"][i] << "\n";
            }
            file.close();

            // Generate plot
            std::string output_file = "plots/plot_" + std::to_string(initialBias) + ".png";
            std::string command = "python3 plot.py data.csv " + output_file;
            int result = std::system(command.c_str());
            if (result != 0) {
                std::cerr << "Error executing Python script!" << std::endl;
            }
        }
        if(epoch%20000==1){
            this->model->saveModel("model_itter/"+to_string(epoch+initialBias)+".pt");
        }
        vector<string> scrambleMove;
        vector<string> solve;
        if(epoch%4000==0){
            cout<<"===================================="<<endl;
         cout<<"scramble:"<<endl;
         scrambleMove=env.intMovesListToStringMovesList(env.scrambleMoves);
         for(int i=0;i<scrambleMove.size();i++){
            cout<<scrambleMove[i]<<endl;
         }
         cout<<endl<<endl<<endl;
         solve=env.intMovesListToStringMovesList(env.solvingMoves);
         cout<<"solve:"<<endl;
         for(int i=0;i<solve.size();i++){
            cout<<solve[i]<<endl;
         }
         cout<<"====================================="<<endl;
        }
    }
}

void Agent::train2(Env env,long epochs,long initialBias){
    unordered_map<string,vector<float>> stats={};
    stats["Returns"]=vector<float>();
    stats["AvgReturns"]=vector<float>();
    stats["EpsilonCheckpoint"]=vector<float>();
    tuple<torch::Tensor, torch::Tensor, torch::Tensor, Stage> result;
    vector<torch::Tensor> sampleResult;
    torch::Tensor state,action,reward,doneTensor,next_state,state_b,action_b,reward_b,done_b,target_q_value_b,next_state_b,predicted_q_value_b,next_q_value_b,loss;
    bool done;
    long i;
    float ep_return;
    for(int epoch=0;epoch<epochs;epoch++){
        state=env.reset();
        done=false;
        ep_return=0;
        int i=0;
        while(!done){
            action=this->getAction(state,epoch).to(device);
            result=env.step(action,false);
            next_state=get<0>(result);
            reward=get<1>(result);
            done=get<2>(result).item<bool>();
            doneTensor=get<2>(result);
            env.stage=get<3>(result);
            this->memory->insert(make_tuple(state,action,reward,doneTensor,next_state));
            if(this->memory->canSample(this->batchSize)){
                sampleResult=this->memory->sample(this->batchSize);
                state_b=sampleResult[0],action_b=sampleResult[1],reward_b=sampleResult[2];
                done_b=sampleResult[3],next_state_b=sampleResult[4];
                predicted_q_value_b = this->model->forward(state_b).gather(1, action_b);
                next_q_value_b = this->targetModel->forward(next_state_b);
                next_q_value_b =get<0>(next_q_value_b.max(-1, true)); 
                target_q_value_b = reward_b + (~done_b * gamma * next_q_value_b);
                loss = torch::mse_loss(predicted_q_value_b, target_q_value_b);
                this->model->zero_grad();
                loss.backward();
                for (const auto& param : this->model->parameters()) {
                    std::cout << "Gradients:\n" << param.grad() << std::endl;
                }
                
                this->optimizer->step();
            }
            state=next_state;
            ep_return+=reward.item<float>();
        }
    }
}

void Agent::testing(Env env,long epochs,long initialBias){
    unordered_map<string,vector<float>> stats={};
    stats["Returns"]=vector<float>();
    stats["AvgReturns"]=vector<float>();
    stats["EpsilonCheckpoint"]=vector<float>();
    tuple<torch::Tensor, torch::Tensor, torch::Tensor, Stage> result;
    vector<torch::Tensor> sampleResult;
    torch::Tensor state,action,reward,doneTensor,next_state,state_b,action_b,reward_b,done_b,target_q_value_b,next_state_b,predicted_q_value_b,next_q_value_b,loss;
    bool done;
    long i;
    float ep_return;
    this->model->eval();
    this->targetModel->eval();
    for(int epoch=0;epoch<epochs;epoch++){
        cout<<"++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        cout<<"epoch:"<<epoch<<endl;
        state=env.reset();
        done=false;
        ep_return=0;
        int i=0;
        while(!done){
            i++;
            cout<<"move number:"<<i<<endl;
            cout<<"============================================"<<endl;
            action=this->getAction(state,epoch).to(device);
            result=env.step(action,false);
            next_state=get<0>(result).to(device);
            reward=get<1>(result).to(device);
            done=get<2>(result).item<bool>();
            doneTensor=get<2>(result).to(device);
            env.stage=get<3>(result);
            cout<<"==========================================="<<endl;
            cout<<"state:"<<endl<<state<<endl;
            cout<<"==========================================="<<endl;
            cout<<"next state:"<<endl<<next_state<<endl;
            cout<<"==========================================="<<endl;
            cout<<"reward tensor:"<<reward<<endl;
            cout<<"Action:"<<action<<endl;
            this->memory->insert(make_tuple(state,action,reward,doneTensor,next_state));
            state=next_state;
            ep_return+=reward.item<float>();
        }
        stats["Returns"].push_back(ep_return);
        if(this->epsilon>this->minEpsilon){
            this->epsilon = max(0.08, 0.08 - (epoch * 0.00005));

        }
        if (epoch % 400== 0) {
            // Compute the average of the last 100 elements in stats["Returns"]
            int size = stats["Returns"].size();
            int start_idx = std::max(0, size - 400); // Ensure we don't go out of bounds
    
            double sum = std::accumulate(stats["Returns"].begin() + start_idx, stats["Returns"].end(), 0.0);
            double average_returns = sum / std::min(400, size);
    
            stats["AvgReturns"].push_back(average_returns);
            stats["EpsilonCheckpoint"].push_back(epsilon);
    
            // Print stats
            if (size > 100) {
                std::cout << "Epoch: " << epoch
                          << ", Average Returns: " << stats["AvgReturns"].back()
                          << ", Return: " << stats["Returns"].back()
                          << ", Epsilon: " << epsilon << " ,Totalsteps:"<<env.totalSteps<<std::endl;
            } else {
                double sum_all = std::accumulate(stats["Returns"].begin(), stats["Returns"].end(), 0.0);
                double avg_all = sum_all / size;
    
                std::cout << "Epoch: " << epoch
                          << ", Average Returns: " << stats["AvgReturns"].back()
                          << ", Return: " << stats["Returns"].back()
                          << ", Epsilon: " << epsilon << " ,Totalsteps:"<<env.totalSteps<<std::endl;
            }
            if(env.stage==Stage::Cross1){
                cout<<"Stage:cross1"<<endl;
            }
            else if(env.stage==Stage::Cross2){
                cout<<"Stage:cross2"<<endl;
            }
            if(env.stage==Stage::Cross3){
                cout<<"Stage:cross3"<<endl;
            }
                else if(env.stage==Stage::Cross){
                    cout<<"Stage:cross"<<endl;
                }
                else if(env.stage==Stage::F2L1pair){
                    cout<<"Stage:F2L pair 1"<<endl;
                }
                else if(env.stage==Stage::F2L2pair){
                    cout<<"Stage:F2L pair 2"<<endl;
                }
                else if(env.stage==Stage::F2L3pair){
                    cout<<"Stage:F2L pair 3"<<endl;
                }
                else if(env.stage==Stage::F2L4pair){
                    cout<<"Stage:F2L pair 4"<<endl;
                }
                else if(env.stage==Stage::YellowCross){
                    cout<<"Stage:Yellow cross"<<endl;
                }
                else if(env.stage==Stage::OLL){
                    cout<<"Stage:OLL"<<endl;
                }
                else if(env.stage==Stage::PLL){
                    cout<<"Stage:PLL"<<endl;
                }
                else if(env.stage==Stage::complete){
                    cout<<"cube solved"<<endl;
                }
        }

        if(epoch%1000==0){
            for (size_t i = 0; i < this->model->parameters().size(); i++) {
                this->targetModel->parameters()[i].data().copy_(this->model->parameters()[i].data());
            }
        }
        vector<string> scrambleMove;
        vector<string> solve;
        if(epoch%4000==0){
            cout<<"===================================="<<endl;
         cout<<"scramble:"<<endl;
         scrambleMove=env.intMovesListToStringMovesList(env.scrambleMoves);
         for(int i=0;i<scrambleMove.size();i++){
            cout<<scrambleMove[i]<<endl;
         }
         cout<<endl<<endl<<endl;
         solve=env.intMovesListToStringMovesList(env.solvingMoves);
         cout<<"solve:"<<endl;
         for(int i=0;i<solve.size();i++){
            cout<<solve[i]<<endl;
         }
         cout<<"====================================="<<endl;
        }
    }
}

void Agent::test(Env env){
    torch::Tensor state,action,next_state,reward,doneTensor;
    tuple<torch::Tensor,torch::Tensor,torch::Tensor,Stage> result;
    bool done=false;
    int i=0;
    float totalReward=0;
    vector<string> scrambleMove;
    vector<string> solve;
    for(int epoch=0;epoch<10;epoch++){
         state=env.reset();
         done=false,i=0,totalReward=0;
         cout<<"==================================="<<endl;
         cout<<"EPOCH:"<<epoch<<endl;
         cout<<"epsilon:"<<epsilon<<endl;
         while(!done){
            action=this->getAction(state,epoch).to(device);
            cout<<action[0][0]<<endl;
            result=env.step(action,true);
            next_state=get<0>(result);
            reward=get<1>(result);
            done=get<2>(result).item<bool>();
            doneTensor=get<2>(result);
            env.stage=get<3>(result);
            if(done){
                break;
            }
            state=next_state;
         }
         cout<<"===================================="<<endl;
         cout<<"scramble:"<<endl;
         scrambleMove=env.intMovesListToStringMovesList(env.scrambleMoves);
         for(int i=0;i<scrambleMove.size();i++){
            cout<<scrambleMove[i]<<endl;
         }
         cout<<endl<<endl<<endl;
         solve=env.intMovesListToStringMovesList(env.solvingMoves);
         cout<<"solve:"<<endl;
         for(int i=0;i<solve.size();i++){
            cout<<solve[i]<<endl;
         }
         cout<<"====================================="<<endl;
         if(env.stage==Stage::Cross1){
            cout<<"Stage:cross1"<<endl;
        }
        else if(env.stage==Stage::Cross2){
            cout<<"Stage:cross2"<<endl;
        }
        if(env.stage==Stage::Cross3){
            cout<<"Stage:cross3"<<endl;
        }
            else if(env.stage==Stage::Cross){
                cout<<"Stage:cross"<<endl;
            }
            else if(env.stage==Stage::F2L1pair){
                cout<<"Stage:F2L pair 1"<<endl;
            }
            else if(env.stage==Stage::F2L2pair){
                cout<<"Stage:F2L pair 2"<<endl;
            }
            else if(env.stage==Stage::F2L3pair){
                cout<<"Stage:F2L pair 3"<<endl;
            }
            else if(env.stage==Stage::F2L4pair){
                cout<<"Stage:F2L pair 4"<<endl;
            }
            else if(env.stage==Stage::YellowCross){
                cout<<"Stage:Yellow cross"<<endl;
            }
            else if(env.stage==Stage::OLL){
                cout<<"Stage:OLL"<<endl;
            }
            else if(env.stage==Stage::PLL){
                cout<<"Stage:PLL"<<endl;
            }
            else if(env.stage==Stage::complete){
                cout<<"cube solved"<<endl;
            }
        
    }
}

