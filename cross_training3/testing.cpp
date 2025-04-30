#include "headers/agent.hpp"
#include "headers/cube.hpp"
#include "headers/model.hpp"
#include<iostream>
#include<iostream>
using namespace std;

int main(){
    torch::DeviceType device=torch::kCUDA;
    cout<<torch::cuda::is_available()<<endl;
    Cube cube=Cube();
    Env env=Env(cube,Stage::Cross1,0,0);
    CubeNetImpl model=CubeNetImpl(0.0001,0.2,16);
    model.loadModel("/home/harsh/RL/Cube/cross_training2/model_itter/330001.pt");
    model.to(device);
    Agent agent=Agent(model,device,device,0,0,5000,16,100000,128,0.0001,0.99);
    agent.test(env);
    return 0;
}