#include "headers/cube.hpp"
#include "headers/cube_solving.hpp"
#include<iostream>
using namespace std;

int main(){
    Cube cube = Cube();
    // vector<pair<string,pair<int,int>>> moves={
    //     {"R",{0,1}},{"R",{0,1}},{"U",{0,1}},{"B",{0,1}},{"B",{0,1}},{"Rprime",{0,1}},
    //     {"Lprime",{0,1}},{"U",{0,1}},{"U",{0,1}},{"R",{0,1}},{"Dprime",{0,1}},{"F",{0,1}},{"Bprime",{0,1}}
    // };
    // cube.scramble(moves,false);
    int counter=0;
    for(int i=0;i<10000;i++){
        cube.randomScramble(22);
        CubeSolver cubeSolver = CubeSolver(cube);
        cubeSolver.whiteCross();
        cube=cubeSolver.cube;
        if(!cube.checkWhiteCross()) counter++;
    }
    cout<<"counter:"<<counter<<endl;
    return 0;
}
