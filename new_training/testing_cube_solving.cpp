#include<iostream>
#include "headers/cube_solving.hpp"
using namespace std;

int main(){
    vector<vector<vector<color>>> cube3DArray = {
        {   // Top (White)
            {color::White, color::Red,   color::Blue},
            {color::Orange, color::White, color::White},
            {color::Orange, color::Blue,  color::Red}
        },
        {   // Front (Red)
            {color::Green, color::Yellow, color::White},
            {color::Green, color::Red,    color::Orange},
            {color::Blue,  color::Orange, color::Green}
        },
        {   // Right (Green)
            {color::Blue,  color::Orange, color::Orange},
            {color::Yellow,color::Green,  color::Yellow},
            {color::Yellow,color::Blue,   color::Yellow}
        },
        {   // Back (Orange)
            {color::Yellow,color::White,  color::Orange},
            {color::Green, color::Orange, color::Green},
            {color::Green, color::Red,    color::Green}
        },
        {   // Left (Blue)
            {color::Blue,  color::Blue,   color::White},
            {color::Red,   color::Blue,   color::White},
            {color::Red,   color::Red,    color::Yellow}
        },
        {   // Bottom (Yellow)
            {color::Red,   color::Green,  color::Red},
            {color::Yellow,color::Yellow, color::White},
            {color::White, color::Blue,   color::Orange}
        }
    };
    // for(int i=0;i<)
    Cube cube=Cube();
    CubeSolver cubesolver=CubeSolver(cube);
    for(int i=0;i<20;i++){
        cout<<i<<":"<<endl;
        cube.testing(false);
        cubesolver=CubeSolver(cube);
        cubesolver.whiteCrossVerboseMode=true;
        cout<<"cube initially:"<<endl;
        cube.printCube();
        cubesolver.whiteCross();
        cout<<"cube after algo:"<<endl;
        cube=cubesolver.cube;
        cube.printCube();
    }
    return 0;
}
//scramble R2UB2R'L'U2RD'FB' white as top and red as front
/*cube 3d array for this scramble:
vector<vector<vector<color>>> cube3DArray = {
    {   // Top (White)
        {color::White, color::Red,   color::Blue},
        {color::Orange, color::White, color::White},
        {color::Orange, color::Blue,  color::Red}
    },
    {   // Front (Red)
        {color::Green, color::Yellow, color::White},
        {color::Green, color::Red,    color::Orange},
        {color::Blue,  color::Orange, color::Green}
    },
    {   // Right (Green)
        {color::Blue,  color::Orange, color::Orange},
        {color::Yellow,color::Green,  color::Yellow},
        {color::Yellow,color::Blue,   color::Yellow}
    },
    {   // Back (Orange)
        {color::Yellow,color::White,  color::Orange},
        {color::Green, color::Orange, color::Green},
        {color::Green, color::Red,    color::Green}
    },
    {   // Left (Blue)
        {color::Blue,  color::Blue,   color::White},
        {color::Red,   color::Blue,   color::White},
        {color::Red,   color::Red,    color::Yellow}
    },
    {   // Bottom (Yellow)
        {color::Red,   color::Green,  color::Red},
        {color::Yellow,color::Yellow, color::White},
        {color::White, color::Blue,   color::Orange}
    }
};
Cube cube(cube3DArray);
*/