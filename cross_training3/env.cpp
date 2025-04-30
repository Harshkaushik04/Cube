#include "headers/cube.hpp"
#include "headers/env.hpp"
#include<fstream>
#include<random>
using namespace std;


Env::Env(Cube cube,Stage stage,int stageSteps=0,int totalSteps=0):cube(cube){ //cube(cube) because no default constructor for Cube hence this->cube object was not able to form initially
    this->cube=cube;
    this->stage=stage;
    this->stageSteps=stageSteps;
    this->totalSteps=totalSteps;
    this->threshold=10;
    this->movesList=vector<int>();
    this->originalScramble=this->cube.solvedCube3DArray;
    this->scrambleMoves=vector<int>();
    this->solvingMoves=vector<int>();
}

void saveVectorToFile(const std::vector<int>& vec, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for writing!\n";
        return;
    }

    for (int num : vec) {
        file << num << " ";  // Save numbers separated by space
    }
    file.close();
    std::cout << "Vector saved to " << filename << "\n";
}

std::vector<int> loadVectorFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for reading!\n";
        return {};
    }

    std::vector<int> vec;
    int num;
    while (file >> num) {
        vec.push_back(num);  // Read numbers from file
    }
    file.close();
    return vec;
}

tuple<torch::Tensor,torch::Tensor,torch::Tensor,Stage> Env::step(torch::Tensor action,bool stepInfo=false){
    float reward=0;
    bool done=false;
    reward-=0.5;
    int numAction = action[0][0].item<int>();
    this->cube.takeAction(numAction);
    this->solvingMoves.push_back(numAction);
    this->stageSteps++;
    this->totalSteps++;
    int solvingMovesSize=this->solvingMoves.size();
    if(solvingMovesSize>2){
        if(this->solvingMoves[solvingMovesSize-1]==this->solvingMoves[solvingMovesSize-2] && this->solvingMoves[solvingMovesSize-1]==this->solvingMoves[solvingMovesSize-3]){
            reward-=0.5;
        }
    }
    if(this->stage==Stage::Cross1){
        if(this->cube.checkCross1()){
            if(stepInfo){
                cout<<"1 edge piece aligned!"<<endl;
            }
            this->stage=Stage::Cross2;
            reward+=25;
            this->stageSteps=0;
            this->threshold=10;
        }
    }
    else if(this->stage==Stage::Cross2){
        if(this->cube.checkCross2()){
            if(stepInfo){
                cout<<"2 edge pieces aligned!"<<endl;
            }
            this->stage=Stage::Cross3;
            reward+=25;
            this->stageSteps=0;
            this->threshold=10;
        }
    }
    else if(this->stage==Stage::Cross3){
        if(this->cube.checkCross3()){
            if(stepInfo){
                cout<<"3 edge pieces aligned!"<<endl;
            }
            this->stage=Stage::Cross;
            reward+=25;
            this->stageSteps=0;
            this->threshold=10;
        }
    }
    else if(this->stage==Stage::Cross){
        if(this->cube.checkWhiteCross()){
            if(stepInfo){
                cout<<"White cross made!"<<endl;
            }
            this->stage=Stage::F2L1pair;
            reward+=40;
            this->stageSteps=0;
            this->threshold=15;
        }
    }
    else if(this->stage==Stage::F2L1pair){
        if(cube.checkF2L1pair()){
            if(stepInfo){
                cout<<"first F2L pair inserted!"<<endl;
            }
            this->stage=Stage::F2L2pair;
            reward+=40;
            this->stageSteps=0;
            this->threshold=15;
        }
    }
    else if(this->stage==Stage::F2L2pair){
        if(cube.checkF2L2pair()){
            if(stepInfo){
                cout<<"2 F2L pairs inserted!"<<endl;
            }
            this->stage=Stage::F2L3pair;
            reward+=40;
            this->stageSteps=0;
            this->threshold=15;
        }
    }
    else if(this->stage==Stage::F2L3pair){
        if(cube.checkF2L3pair()){
            if(stepInfo){
                cout<<"3 F2L pairs inserted!"<<endl;
            }
            this->stage=Stage::F2L4pair;
            reward+=40;
            this->stageSteps=0;
            this->threshold=15;
        }
    }
    else if(this->stage==Stage::F2L4pair){
        if(cube.checkF2Lcompletion()){
            if(stepInfo){
                cout<<"F2L completed!"<<endl;
            }
            this->stage=Stage::YellowCross;
            reward+=55;
            this->stageSteps=0;
            this->threshold=20;
        }
    }
    else if(this->stage==Stage::YellowCross){
        if(cube.checkTillYellowCross()){
            if(stepInfo){
                cout<<"Till Yellow cross completed!"<<endl;
            }
            this->stage=Stage::OLL;
            reward+=55;
            this->stageSteps=0;
            this->threshold=20;
        }
    }
    else if(this->stage==Stage::OLL){
        if(cube.checkTillOLL()){
            if(stepInfo){
                cout<<"Till OLL completed!"<<endl;
            }
            this->stage=Stage::PLL;
            reward+=75;
            this->stageSteps=0;
            this->threshold=25;
        }
    }
    else if(this->stage==Stage::PLL){
        if(cube.checkCubeCompletion()){
            if(stepInfo){
                cout<<"Cube completed!"<<endl;
            }
            this->stage=Stage::complete;
            reward+=150;
            this->stageSteps=0;
        }
    }
    if(this->stageSteps>this->threshold){ 
        done=true;
    }
    if(this->stage==Stage::complete){
        done=true;
        cout<<"====================================================================";
        cout<<"Total number of steps:"<<this->totalSteps<<endl;
        cout<<"Original scramble:"<<endl;
        printOriginalScramble();
        cout<<"solved sube:"<<endl;
        this->cube.printCube();
        cout<<"scramble moves:"<<endl;
        vector<string> stringScrambleMoves=intMovesListToStringMovesList(this->scrambleMoves);
        for(string s:stringScrambleMoves){
            cout<<s<<" ";
        }
        cout<<endl;
        vector<string> stringSolvingMoves=intMovesListToStringMovesList(this->solvingMoves);
        for(string s:stringSolvingMoves){
            cout<<s<<" ";
        }
        cout<<endl;
        cout<<"====================================================================";
        random_device rd;  
        mt19937 gen(rd()); 
        uniform_int_distribution<long> dist(0, 120000); 
        long num=dist(gen);
        saveVectorToFile(this->solvingMoves,"solvedCubes/solvingMoves"+to_string(num)+".txt");
        saveVectorToFile(this->scrambleMoves,"solvedCubes/scramblesCube"+to_string(num)+".txt");
    }
    // if(agentStage==Stage::Cross1){
    //     if(this->stage!=Stage::Cross1){
    //         done=true;
    //     }
    // }
    // else if(agentStage==Stage::Cross2){
    //     if(this->stage!=Stage::Cross2){
    //         done=true;
    //     }
    // }
    // else if(agentStage==Stage::Cross3){
    //     if(this->stage!=Stage::Cross3){
    //         done=true;
    //     }
    // }
    // else if(agentStage==Stage::Cross){
    //     if(this->stage!=Stage::Cross){
    //         done=true;
    //     }
    // }
    // else if(agentStage==Stage::F2L1pair){
    //     if(this->stage!=Stage::F2L1pair){
    //         done=true;
    //     }
    // }
    // else if(agentStage==Stage::F2L2pair){
    //     if(this->stage!=Stage::F2L2pair){
    //         done=true;
    //     }
    // }
    // else if(agentStage==Stage::F2L3pair){
    //     if(this->stage!=Stage::F2L3pair){
    //         done=true;
    //     }
    // }
    // else if(agentStage==Stage::F2L4pair){
    //     if(this->stage!=Stage::F2L4pair){
    //         done=true;
    //     }
    // }
    // else if(agentStage==Stage::OLL){
    //     if(this->stage!=Stage::OLL){
    //         done=true;
    //     }
    // }
    // else if(agentStage==Stage::PLL){
    //     if(this->stage!=Stage::PLL){
    //         done=true;
    //     }
    // }
    torch::Tensor observation=processArray(this->cube.oneHotEncodedArray);
    observation=observation.to(torch::kCUDA);
    torch::Tensor rewardTensor = torch::tensor({reward}).view({1, -1}).to(torch::kFloat);
    rewardTensor=rewardTensor.to(torch::kCUDA);
    torch::Tensor doneTensor = torch::tensor({done}).view({1, -1}).to(torch::kInt);
    doneTensor=doneTensor.to(torch::kCUDA);
    tuple<torch::Tensor,torch::Tensor,torch::Tensor,Stage> result={observation,rewardTensor,doneTensor,this->stage};
    return result;
}

std::vector<std::string> Env::intMovesListToStringMovesList(vector<int> movesList){
    vector<string> result;
    for(int numAction:movesList){
        if(numAction==0){
            result.push_back("R(0,1)");
        }
        else if(numAction==1){
            result.push_back("R(0,2)");
        }
        else if(numAction==2){
            result.push_back("R(0,3)");
        }
        else if(numAction==3){
            result.push_back("R(0,4)");
        }
        else if(numAction==4){
            result.push_back("Rprime(0,1)");
        }
        else if(numAction==5){
            result.push_back("Rprime(0,2)");
        }
        else if(numAction==6){
            result.push_back("Rprime(0,3)");
        }
        else if(numAction==7){
            result.push_back("Rprime(0,4)");
        }
        else if(numAction==8){
            result.push_back("U(0)");
        }
        else if(numAction==9){
            result.push_back("U(5)");
        }
        else if(numAction==10){
            result.push_back("Uprime(0)");
        }
        else if(numAction==11){
            result.push_back("Uprime(5)");
        }
        else if(numAction==12){
            result.push_back("M(0,1)");
        }
        else if(numAction==13){
            result.push_back("M(0,2)");
        }
        else if(numAction==14){
            result.push_back("Mprime(0,1)");
        }
        else if(numAction==15){
            result.push_back("Mprime(0,2)");
        }
    }
    return result;
}

void Env::printOriginalScramble(){
    for(int i=0;i<6;i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<3;k++){
                if(this->originalScramble[i][j][k]==color::White){
                    cout<<"W ";
                }
                else if(this->originalScramble[i][j][k]==color::Orange){
                    cout<<"O ";
                }
                else if(this->originalScramble[i][j][k]==color::Red){
                    cout<<"R ";
                }
                else if(this->originalScramble[i][j][k]==color::Yellow){
                    cout<<"Y ";
                }
                else if(this->originalScramble[i][j][k]==color::Green){
                    cout<<"G ";
                }
                else if(this->originalScramble[i][j][k]==color::Blue){
                    cout<<"B ";
                }
            }
            cout<<endl;
        }
        cout<<"================================="<<endl;
    }
    cout<<endl<<endl<<endl;
}

torch::Tensor Env::reset(){
    this->cube.resetToSolved();
    this->scrambleMoves=this->cube.randomScramble(22);
    if(this->cube.checkCross1()){
        this->stage=Stage::Cross2;
        if(this->cube.checkCross2()){
            this->stage=Stage::Cross3;
            if(this->cube.checkCross3()){
                this->stage=Stage::Cross;
                if(this->cube.checkWhiteCross()){
                    this->stage=Stage::F2L1pair;
                    if(this->cube.checkF2L1pair()){
                        this->stage=Stage::F2L2pair;
                        if(this->cube.checkF2L2pair()){
                            this->stage=Stage::F2L3pair;
                            if(this->cube.checkF2L3pair()){
                                this->stage=Stage::F2L4pair;
                                if(this->cube.checkF2Lcompletion()){
                                    this->stage=Stage::YellowCross;
                                    if(this->cube.checkTillYellowCross()){
                                        this->stage=Stage::OLL;
                                        if(this->cube.checkTillOLL()){
                                            this->stage=Stage::PLL;
                                            if(this->cube.checkCubeCompletion()){
                                                this->stage=Stage::complete;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else{
        this->stage=Stage::Cross1;
    }
    this->stageSteps=0;
    this->totalSteps=0;
    this->threshold=10; 
    this->originalScramble=this->cube.cube3DArray;
    this->solvingMoves=vector<int>();
    vector<vector<vector<vector<int>>>> obs=this->cube.oneHotEncodedArray;
    torch::Tensor observation=processArray(obs);
    observation=observation.to(torch::kCUDA);
    return observation;
}


torch::Tensor vectorToTensor(const std::vector<std::vector<std::vector<std::vector<int>>>> &vec) {
    int dim1 = vec.size();
    int dim2 = vec[0].size();
    int dim3 = vec[0][0].size();
    int dim4 = vec[0][0][0].size();

    std::vector<float> flat_data;
    for (const auto &mat3d : vec)
        for (const auto &mat2d : mat3d)
            for (const auto &row : mat2d)
                for (int val : row)
                    flat_data.push_back(static_cast<float>(val));

    auto options = torch::TensorOptions().dtype(at::kFloat);
    torch::Tensor tensor = torch::from_blob(flat_data.data(), {dim1, dim2, dim3, dim4}, options).clone();
    return tensor;
}

torch::Tensor Env::processArray(vector<vector<vector<vector<int>>>> oneHotVector){
    torch::Tensor observation=vectorToTensor(oneHotVector);
    observation = observation.unsqueeze(0);
    return observation;
}
