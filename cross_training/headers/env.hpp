#pragma once
#include<iostream>
#include<vector>
#include<tuple>
#include<torch/torch.h>
#include<string>
#include "cube.hpp"

enum class Stage{Cross1,Cross2,Cross3,Cross,F2L1pair,F2L2pair,F2L3pair,F2L4pair,YellowCross,OLL,PLL,complete};

class Env{
public:
    Cube cube;
    Stage stage=Stage::Cross;
    int stageSteps=0;
    int totalSteps=0;
    int threshold;
    std::vector<int> movesList;
    std::vector<std::vector<std::vector<color>>> originalScramble;
    std::vector<int> scrambleMoves,solvingMoves;
    Env(Cube,Stage,int,int);
    std::tuple<torch::Tensor,torch::Tensor,torch::Tensor,Stage> step(torch::Tensor,bool);
    torch::Tensor processArray(std::vector<std::vector<std::vector<std::vector<int>>>>);
    torch::Tensor reset();
    std::vector<std::string> intMovesListToStringMovesList(std::vector<int> movesList);
    void printOriginalScramble();
};