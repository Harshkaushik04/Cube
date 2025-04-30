#include<iostream>
#include "headers/agent.hpp"
using namespace std;

int main(){
    torch::DeviceType device=torch::kCUDA;
    Cube cube=Cube();
    Env env=Env(cube,Stage::Cross1,0,0);
    CubeNetImpl model=CubeNetImpl(0.0001,0.2,16);
    model.loadModel("weights/best.pt");
    model.to(device);
    Agent agent=Agent(model,device,device,0.2,0.1,5000,16,100000,128,0.0001,0.95);
    agent.train(env,10000000,1010603);
}