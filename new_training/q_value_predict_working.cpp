#include<iostream>
#include "headers/agent.hpp"
using namespace std;

torch::Tensor vectorToTensor2(const std::vector<std::vector<std::vector<std::vector<int>>>> &vec) {
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

int main(){
    torch::DeviceType device=torch::kCUDA;
    std::vector<std::vector<std::vector<std::vector<int>>>> oneHotEncoded = {
        // Face 0
        {
            { {0,1,0,0,0,0}, {0,0,0,0,0,1}, {1,0,0,0,0,0} },
            { {0,0,1,0,0,0}, {0,0,0,1,0,0}, {0,0,0,0,1,0} },
            { {0,0,1,0,0,0}, {0,0,0,1,0,0}, {0,1,0,0,0,0} }
        },
        // Face 1
        {
            { {0,0,0,0,1,0}, {1,0,0,0,0,0}, {0,0,0,0,0,1} },
            { {0,0,0,1,0,0}, {0,0,1,0,0,0}, {0,0,0,0,1,0} },
            { {0,0,1,0,0,0}, {0,1,0,0,0,0}, {0,0,0,0,0,1} }
        },
        // Face 2
        {
            { {0,1,0,0,0,0}, {0,0,0,0,0,1}, {0,0,0,1,0,0} },
            { {0,0,1,0,0,0}, {0,0,0,0,1,0}, {0,1,0,0,0,0} },
            { {1,0,0,0,0,0}, {0,0,0,0,0,1}, {0,0,1,0,0,0} }
        },
        // Face 3
        {
            { {0,0,0,1,0,0}, {0,0,0,0,0,1}, {1,0,0,0,0,0} },
            { {0,0,1,0,0,0}, {0,0,0,1,0,0}, {0,1,0,0,0,0} },
            { {0,0,0,0,1,0}, {0,0,0,1,0,0}, {0,0,1,0,0,0} }
        },
        // Face 4
        {
            { {0,0,0,0,0,1}, {0,0,1,0,0,0}, {0,0,0,0,1,0} },
            { {1,0,0,0,0,0}, {0,0,0,0,0,1}, {0,0,0,1,0,0} },
            { {0,0,1,0,0,0}, {0,0,0,0,1,0}, {0,1,0,0,0,0} }
        },
        // Face 5
        {
            { {0,0,0,0,1,0}, {0,1,0,0,0,0}, {0,0,0,1,0,0} },
            { {0,0,0,0,0,1}, {0,0,1,0,0,0}, {0,0,0,0,1,0} },
            { {0,0,1,0,0,0}, {0,0,0,1,0,0}, {1,0,0,0,0,0} }
        }
    };    
    Cube cube=Cube(oneHotEncoded);
    torch::Tensor oneHotEncodedTensor=vectorToTensor2(oneHotEncoded);
    oneHotEncodedTensor=oneHotEncodedTensor.to(torch::kCUDA).unsqueeze(0);
    // Env env=Env(cube,Stage::Cross1,0,0);
    CubeNetImpl model=CubeNetImpl(0.0001,0.2,16);
    model.loadModel("model_itter/15001.pt");
    model.to(torch::kCUDA);
    std::shared_ptr<CubeNetImpl> our_model=make_shared<CubeNetImpl>(model);
    // Agent agent=Agent(model,device,device,0,0,5000,16,100000,128,0.0001,0.99,env);
    torch::Tensor predicted_q_values=our_model->forward(oneHotEncodedTensor);
    cout<<predicted_q_values<<endl;
    return 0;
}

//test command:
//g++ -I/home/harsh/libtorch/include -I/home/harsh/libtorch/include/torch/csrc/api/include test.cpp agent.cpp model.cpp cube.cpp env.cpp -o outputs/test -L/home/harsh/libtorch/lib -Wl,--no-as-needed -ltorch -ltorch_cuda -lc10 -ltorch_cpu
//export LD_LIBRARY_PATH=/home/harsh/libtorch/lib
