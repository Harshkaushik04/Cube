#include<iostream>
#include "headers/agent.hpp"
using namespace std;

int main(){
    torch::DeviceType device=torch::kCUDA;
    Cube cube=Cube();
    Env env=Env(cube,Stage::Cross,0,0);
    CubeNetImpl model=CubeNetImpl(0.0001,0.2,16);
    model.loadModel("weights/best.pt");
    model.to(device);
    Agent agent=Agent(model,device,device,1.0,0.15,5000,16,100000,64,0.0001,0.99);
    agent.train(env,100000000,460002);
}

    
// int main(){
//     vector<vector<vector<color>>> cube3DArray(6, vector<vector<color>>(3, vector<color>(3)));
//     //after performing RU'F on solved cube
//     cube3DArray[0][0][0]=color::Red;
//     cube3DArray[0][0][1]=color::Red;
//     cube3DArray[0][0][2]=color::Red;
//     cube3DArray[0][1][0]=color::White;
//     cube3DArray[0][1][1]=color::White;
//     cube3DArray[0][1][2]=color::White;
//     cube3DArray[0][2][0]=color::Blue;
//     cube3DArray[0][2][1]=color::Blue;
//     cube3DArray[0][2][2]=color::Orange;

//     cube3DArray[1][0][0]=color::Red;
//     cube3DArray[1][0][1]=color::Red;
//     cube3DArray[1][0][2]=color::Blue;
//     cube3DArray[1][1][0]=color::Red;
//     cube3DArray[1][1][1]=color::Red;
//     cube3DArray[1][1][2]=color::Blue;
//     cube3DArray[1][2][0]=color::Yellow;
//     cube3DArray[1][2][1]=color::Yellow;
//     cube3DArray[1][2][2]=color::Blue;

//     cube3DArray[2][0][0]=color::White;
//     cube3DArray[2][0][1]=color::Red;
//     cube3DArray[2][0][2]=color::Yellow;
//     cube3DArray[2][1][0]=color::White;
//     cube3DArray[2][1][1]=color::Green;
//     cube3DArray[2][1][2]=color::Green;
//     cube3DArray[2][2][0]=color::White;
//     cube3DArray[2][2][1]=color::Green;
//     cube3DArray[2][2][2]=color::Green;

//     cube3DArray[3][0][0]=color::Green;
//     cube3DArray[3][0][1]=color::Green;
//     cube3DArray[3][0][2]=color::Green;
//     cube3DArray[3][1][0]=color::White;
//     cube3DArray[3][1][1]=color::Orange;
//     cube3DArray[3][1][2]=color::Orange;
//     cube3DArray[3][2][0]=color::White;
//     cube3DArray[3][2][1]=color::Orange;
//     cube3DArray[3][2][2]=color::Orange;

//     cube3DArray[4][0][0]=color::White;
//     cube3DArray[4][0][1]=color::Orange;
//     cube3DArray[4][0][2]=color::Yellow;
//     cube3DArray[4][1][0]=color::Blue;
//     cube3DArray[4][1][1]=color::Blue;
//     cube3DArray[4][1][2]=color::Yellow;
//     cube3DArray[4][2][0]=color::Blue;
//     cube3DArray[4][2][1]=color::Blue;
//     cube3DArray[4][2][2]=color::Orange;

//     cube3DArray[5][0][0]=color::Green;
//     cube3DArray[5][0][1]=color::Green;
//     cube3DArray[5][0][2]=color::Red;
//     cube3DArray[5][1][0]=color::Yellow;
//     cube3DArray[5][1][1]=color::Yellow;
//     cube3DArray[5][1][2]=color::Orange;
//     cube3DArray[5][2][0]=color::Yellow;
//     cube3DArray[5][2][1]=color::Yellow;
//     cube3DArray[5][2][2]=color::Orange;
//     // cout<<torch::cuda::is_available()<<endl;
//     Cube cube=Cube(cube3DArray);
//     cube.printCube();
//     cube.Uprime(5);
//     cube.printCube();
//     return 0;
// }