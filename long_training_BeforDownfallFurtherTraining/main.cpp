#include<iostream>
#include "headers/agent.hpp"
using namespace std;

int main(){
    torch::DeviceType device=torch::kCUDA;
    Cube cube=Cube();
    Env env=Env(cube,Stage::Cross1,0,0);
    CubeNetImpl model=CubeNetImpl(0.0001,0.2,16);
    model.loadModel("model_itter/62001.pt");
    model.to(device);
    Agent agent=Agent(model,device,device,1,0.1,5000,16,1000000,128,0.0001,0.99,env);
    agent.train(env,10000000,99000);
}
//22000
//74000
//91000