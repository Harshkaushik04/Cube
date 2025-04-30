#include<iostream>
#include "headers/agent.hpp"
using namespace std;

int main(){
    torch::DeviceType device=torch::kCUDA;
    Cube cube=Cube();
    Env env=Env(cube,Stage::Cross1,0,0);
    CubeNetImpl model=CubeNetImpl(0.0001,0.2,16);
    // model.loadModel("weights/best.pt");
    model.to(device);
    Agent agent=Agent(model,device,device,1,0.1,5000,16,100000,128,0.0001,0.99,env);
    agent.train(env,10000000,0);
}
