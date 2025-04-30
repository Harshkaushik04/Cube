#include<iostream>
#include "headers/cube.hpp"
using namespace std;
int main(){
    //not sure
    // vector<vector<vector<color>>> cube3DArray = {
    //     // U face (White)
    //     {
    //         {color::Orange,    color::White, color::White},
    //         {color::White,  color::White, color::White},
    //         {color::Yellow,   color::White, color::Yellow}
    //     },
    //     // R face
    //     {
    //         {color::White,  color::Red,   color::Blue},
    //         {color::Red,    color::Red,   color::Yellow},
    //         {color::Orange, color::Red,   color::Green}
    //     },
    //     // F face
    //     {
    //         {color::Green,  color::Green, color::Green},
    //         {color::Red,    color::Green, color::Green},
    //         {color::Blue,   color::Blue,  color::Red}
    //     },
    //     // L face
    //     {
    //         {color::Orange, color::Orange, color::Orange},
    //         {color::Orange,   color::Orange, color::Orange},
    //         {color::Red,    color::Yellow, color::Orange}
    //     },
    //     // B face
    //     {
    //         {color::Yellow, color::Blue,   color::Green},
    //         {color::Yellow, color::Blue,   color::Blue},
    //         {color::White,  color::Orange, color::Yellow}
    //     },
    //     // D face (Yellow)
    //     {
    //         {color::Orange, color::Yellow, color::Red},
    //         {color::Yellow, color::Yellow, color::Green},
    //         {color::Blue,   color::Red,    color::Blue}
    //     }
    // };
    
    // Now construct the cube
    Cube cube=Cube();
    cout<<cube.checkF2L1pair()<<endl;
    cout<<cube.checkF2Lcompletion()<<endl;
    cout<<cube.checkTillOLL()<<endl;
    cout<<cube.checkCubeCompletion()<<endl;
    return 0;
}