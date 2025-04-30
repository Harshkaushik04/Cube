#include<iostream>
#include "headers/agent.hpp"
using namespace std;

int main(){
    torch::DeviceType device=torch::kCUDA;
    Cube cube=Cube();
    Env env=Env(cube,Stage::Cross1,0,0);
    CubeNetImpl model=CubeNetImpl(0.0001,0.2,16);
    model.loadModel("weights/best.pt");
    model.to(torch::kCUDA);
    Agent agent=Agent(model,device,device,0,0,5000,16,100000,128,0.0001,0.99,env);
    agent.test(env);
}

//test command:
//g++ -I/home/harsh/libtorch/include -I/home/harsh/libtorch/include/torch/csrc/api/include test.cpp agent.cpp model.cpp cube.cpp env.cpp -o outputs/test -L/home/harsh/libtorch/lib -Wl,--no-as-needed -ltorch -ltorch_cuda -lc10 -ltorch_cpu
//export LD_LIBRARY_PATH=/home/harsh/libtorch/lib
