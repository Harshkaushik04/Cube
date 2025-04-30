#pragma once
#include<iostream>
#include<vector>
#include<torch/torch.h>

enum class color {White,Red,Blue,Orange,Green,Yellow};
enum class order{first,second,third,firstplusreverse,secondplusreverse,thirdplusreverse};

class Cube{
public:
    std::vector<std::vector<std::vector<color>>> cube3DArray; //6*3*3
    std::vector<std::vector<std::vector<std::vector<int>>>> oneHotEncodedArray;
    std::vector<std::vector<std::vector<color>>> solvedCube3DArray;
    std::vector<std::vector<std::vector<std::vector<int>>>> solvedOneHotEncodedArray;
    Cube();
    Cube(std::vector<std::vector<std::vector<color>>>&);
    Cube(std::vector<std::vector<std::vector<std::vector<int>>>>&);
    void R(int,int);
    void Rprime(int,int);
    void L(int,int);
    void Lprime(int,int);
    void F(int,int);
    void Fprime(int,int);
    void U(int);
    void Uprime(int);
    void D(int);
    void Dprime(int);
    void M(int,int);
    void Mprime(int,int);
    void printCube();
    std::vector<std::vector<std::vector<std::vector<int>>>> convertToOneHot(std::vector<std::vector<std::vector<color>>>&);
    std::vector<std::vector<std::vector<color>>> fullyConvertBack(std::vector<std::vector<std::vector<std::vector<int>>>>&);
    void takeAction(int);
    bool checkWhiteCross();
    bool checkF2L1pair();
    bool checkF2L2pair();
    bool checkF2L3pair();
    bool checkF2Lcompletion();
    void resetToSolved();
    std::vector<int> randomScramble(int);
    bool checkF2Lpair1();
    bool checkF2Lpair2();
    bool checkF2Lpair3();
    bool checkF2Lpair4();
    bool checkTillYellowCross();
    bool checkTillOLL();
    bool checkCubeCompletion();
    bool checkCross1();
    bool checkCross2();
    bool checkCross3();
    void testing();
    void solveCross1();
};
