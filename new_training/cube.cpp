#include "headers/cube.hpp"
#include<random>
using namespace std;

Cube::Cube(){
    this->solvedCube3DArray={{{color::White,color::White,color::White},
    {color::White,color::White,color::White},
    {color::White,color::White,color::White}},
    {{color::Red,color::Red,color::Red},
    {color::Red,color::Red,color::Red},
    {color::Red,color::Red,color::Red}},
    {{color::Green,color::Green,color::Green},
    {color::Green,color::Green,color::Green},
    {color::Green,color::Green,color::Green}},
    {{color::Orange,color::Orange,color::Orange},
    {color::Orange,color::Orange,color::Orange},
    {color::Orange,color::Orange,color::Orange}},
    {{color::Blue,color::Blue,color::Blue},
    {color::Blue,color::Blue,color::Blue},
    {color::Blue,color::Blue,color::Blue}},
    {{color::Yellow,color::Yellow,color::Yellow},
    {color::Yellow,color::Yellow,color::Yellow},
    {color::Yellow,color::Yellow,color::Yellow}}
    };
    this->solvedOneHotEncodedArray=convertToOneHot(this->solvedCube3DArray);
    this->cube3DArray=this->solvedCube3DArray;
    this->oneHotEncodedArray=this->solvedOneHotEncodedArray;
}

Cube::Cube(vector<vector<vector<color>>>& cube3DArray){
    this->cube3DArray=cube3DArray;
    this->oneHotEncodedArray=Cube::convertToOneHot(cube3DArray);
    this->solvedCube3DArray={{{color::White,color::White,color::White},
    {color::White,color::White,color::White},
    {color::White,color::White,color::White}},
    {{color::Red,color::Red,color::Red},
    {color::Red,color::Red,color::Red},
    {color::Red,color::Red,color::Red}},
    {{color::Green,color::Green,color::Green},
    {color::Green,color::Green,color::Green},
    {color::Green,color::Green,color::Green}},
    {{color::Orange,color::Orange,color::Orange},
    {color::Orange,color::Orange,color::Orange},
    {color::Orange,color::Orange,color::Orange}},
    {{color::Blue,color::Blue,color::Blue},
    {color::Blue,color::Blue,color::Blue},
    {color::Blue,color::Blue,color::Blue}},
    {{color::Yellow,color::Yellow,color::Yellow},
    {color::Yellow,color::Yellow,color::Yellow},
    {color::Yellow,color::Yellow,color::Yellow}}
    };
    this->solvedOneHotEncodedArray=convertToOneHot(this->solvedCube3DArray);
}
Cube::Cube(vector<vector<vector<vector<int>>>>& oneHotEncodedArray){
    this->oneHotEncodedArray=oneHotEncodedArray;
    this->cube3DArray=Cube::fullyConvertBack(oneHotEncodedArray);
}

void Cube::R(int top,int front){
    if(top==0){ //color::White on top
        if(front==1){ //red in front
            //rotating layer changes
            vector<color> temp={cube3DArray[1][0][2],cube3DArray[1][1][2],cube3DArray[1][2][2]};
            vector<color> temp1={cube3DArray[0][0][2],cube3DArray[0][1][2],cube3DArray[0][2][2]};
            vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{0,2},order::second},{{3,0},order::secondplusreverse},{{5,2},order::secondplusreverse},{{1,2},order::second}};
            for(int i=0;i<4;i++){
                for(int j=0;j<3;j++){
                    if(layerTransitionsPairWithOrdering[i].second==order::second){
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[j];
                    }
                    else{
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[2-j];
                    }
                }
                temp=temp1;
                if(i<3){
                    for(int j=0;j<3;j++){
                        temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][j][layerTransitionsPairWithOrdering[i+1].first.second];
                    }
                }
            }
            //side layer corner changes
            color tempColor=cube3DArray[2][0][0];
            color tempColor1=cube3DArray[2][0][2];
            vector<pair<int,int>> layerTransitionsPair={{0,2},{2,2},{2,0},{0,0}};
            for(int i=0;i<4;i++){
                cube3DArray[2][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[2][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
            //side layer edge changes
            tempColor=cube3DArray[2][0][1];
            tempColor1=cube3DArray[2][1][2];
            layerTransitionsPair={{1,2},{2,1},{1,0},{0,1}};
            for(int i=0;i<4;i++){
                cube3DArray[2][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[2][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
        }
        else if(front==2){ //green in front
            //rotating layer changes
            vector<color> temp={cube3DArray[2][0][2],cube3DArray[2][1][2],cube3DArray[2][2][2]};
            vector<color> temp1={cube3DArray[0][0][0],cube3DArray[0][0][1],cube3DArray[0][0][2]};
            vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{0,0},order::third},{{4,0},order::secondplusreverse},{{5,2},order::third},{{2,2},order::secondplusreverse}};
            for(int i=0;i<4;i++){
                for(int j=0;j<3;j++){
                    if(layerTransitionsPairWithOrdering[i].second==order::secondplusreverse){
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[2-j];
                    }
                    else{ // ==order::third
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][layerTransitionsPairWithOrdering[i].first.second][j]=temp[j];
                    }
                }
                temp=temp1;
                if(i<3){
                    if(layerTransitionsPairWithOrdering[i+1].second==order::secondplusreverse){
                        for(int j=0;j<3;j++){
                            temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][j][layerTransitionsPairWithOrdering[i+1].first.second];
                        }
                    }
                    else{ // ==order::third
                        for(int j=0;j<3;j++){
                            temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][layerTransitionsPairWithOrdering[i+1].first.second][j];
                        }
                    }
                }
            }
            //side layer corner changes
            color tempColor=cube3DArray[3][0][0];
            color tempColor1=cube3DArray[3][0][2];
            vector<pair<int,int>> layerTransitionsPair={{0,2},{2,2},{2,0},{0,0}};
            for(int i=0;i<4;i++){
                cube3DArray[3][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[3][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
            //side layer edge changes
            tempColor=cube3DArray[3][0][1];
            tempColor1=cube3DArray[3][1][2];
            layerTransitionsPair={{1,2},{2,1},{1,0},{0,1}};
            for(int i=0;i<4;i++){
                cube3DArray[3][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[3][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
        }
        else if(front==3){ //orange in front
            //rotating layer changes
            vector<color> temp={cube3DArray[3][0][2],cube3DArray[3][1][2],cube3DArray[3][2][2]};
            vector<color> temp1={cube3DArray[0][0][0],cube3DArray[0][1][0],cube3DArray[0][2][0]};
            vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{0,0},order::secondplusreverse},{{1,0},order::second},{{5,0},order::second},{{3,2},order::secondplusreverse}};
            for(int i=0;i<4;i++){
                for(int j=0;j<3;j++){
                    if(layerTransitionsPairWithOrdering[i].second==order::second){
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[j];
                    }
                    else{
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[2-j];
                    }
                }
                temp=temp1;
                if(i<3){
                    for(int j=0;j<3;j++){
                        temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][j][layerTransitionsPairWithOrdering[i+1].first.second];
                    }
                }
            }
            //side layer corner changes
            color tempColor=cube3DArray[4][0][0];
            color tempColor1=cube3DArray[4][0][2];
            vector<pair<int,int>> layerTransitionsPair={{0,2},{2,2},{2,0},{0,0}};
            for(int i=0;i<4;i++){
                cube3DArray[4][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[4][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
            //side layer edge changes
            tempColor=cube3DArray[4][0][1];
            tempColor1=cube3DArray[4][1][2];
            layerTransitionsPair={{1,2},{2,1},{1,0},{0,1}};
            for(int i=0;i<4;i++){
                cube3DArray[4][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[4][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
        }
        else if(front==4){ //blue in front
            //rotating layer changes
            vector<color> temp={cube3DArray[4][0][2],cube3DArray[4][1][2],cube3DArray[4][2][2]};
            vector<color> temp1={cube3DArray[0][2][0],cube3DArray[0][2][1],cube3DArray[0][2][2]};
            vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{0,2},order::thirdplusreverse},{{2,0},order::second},{{5,0},order::thirdplusreverse},{{4,2},order::second}};
            for(int i=0;i<4;i++){
                for(int j=0;j<3;j++){
                    if(layerTransitionsPairWithOrdering[i].second==order::thirdplusreverse){
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][layerTransitionsPairWithOrdering[i].first.second][j]=temp[2-j];
                    }
                    else{ // ==second
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[j];
                    }
                }
                temp=temp1;
                if(i<3){
                    if(layerTransitionsPairWithOrdering[i+1].second==order::thirdplusreverse){
                        for(int j=0;j<3;j++){
                            temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][layerTransitionsPairWithOrdering[i+1].first.second][j];
                        }
                    }
                    else{ // ==second
                        for(int j=0;j<3;j++){
                            temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][j][layerTransitionsPairWithOrdering[i+1].first.second];
                        }
                    }
                }
            }
            //side layer corner changes
            color tempColor=cube3DArray[1][0][0];
            color tempColor1=cube3DArray[1][0][2];
            vector<pair<int,int>> layerTransitionsPair={{0,2},{2,2},{2,0},{0,0}};
            for(int i=0;i<4;i++){
                cube3DArray[1][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[1][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
            //side layer edge changes
            tempColor=cube3DArray[1][0][1];
            tempColor1=cube3DArray[1][1][2];
            layerTransitionsPair={{1,2},{2,1},{1,0},{0,1}};
            for(int i=0;i<4;i++){
                cube3DArray[1][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[1][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
        }
    }
    else if(top==5){
        if(front==1){
            R(0,3);
        }
        else if(front==2){
           R(0,4);
        }
        else if(front==3){
            R(0,1);
        }
        else if(front==4){
            R(0,2);
        }
    }
    this->oneHotEncodedArray=convertToOneHot(cube3DArray);
}

void Cube::Rprime(int top=0,int front=1){
    if(top==0){
        if(front==1){
            //rotating layer changes
            vector<color> temp1={cube3DArray[1][0][2],cube3DArray[1][1][2],cube3DArray[1][2][2]};
            vector<color> temp={cube3DArray[0][0][2],cube3DArray[0][1][2],cube3DArray[0][2][2]};
            vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{1,2},order::second},{{5,2},order::second},{{3,0},order::secondplusreverse},{{0,2},order::secondplusreverse}};
            for(int i=0;i<4;i++){
                for(int j=0;j<3;j++){
                    if(layerTransitionsPairWithOrdering[i].second==order::second){
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[j];
                    }
                    else{
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[2-j];
                    }
                }
                temp=temp1;
                if(i<3){
                    for(int j=0;j<3;j++){
                        temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][j][layerTransitionsPairWithOrdering[i+1].first.second];
                    }
                }
            }
            //side layer corner changes
            color tempColor1=cube3DArray[2][0][0];
            color tempColor=cube3DArray[2][0][2];
            vector<pair<int,int>> layerTransitionsPair={{0,0},{2,0},{2,2},{0,2}};
            for(int i=0;i<4;i++){
                cube3DArray[2][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[2][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
            //side layer edge changes
            tempColor1=cube3DArray[2][0][1];
            tempColor=cube3DArray[2][1][2];
            layerTransitionsPair={{0,1},{1,0},{2,1},{1,2}};
            for(int i=0;i<4;i++){
                cube3DArray[2][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[2][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
        }
        else if(front==2){
            //rotating layer changes
            vector<color> temp1={cube3DArray[2][0][2],cube3DArray[2][1][2],cube3DArray[2][2][2]};
            vector<color> temp={cube3DArray[0][0][0],cube3DArray[0][0][1],cube3DArray[0][0][2]};
            vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{2,2},order::second},{{5,2},order::thirdplusreverse},{{4,0},order::second},{{0,0},order::thirdplusreverse}};
            for(int i=0;i<4;i++){
                for(int j=0;j<3;j++){
                    if(layerTransitionsPairWithOrdering[i].second==order::second){
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[j];
                    }
                    else{ // ==order::thirdplusreverse
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][layerTransitionsPairWithOrdering[i].first.second][j]=temp[2-j];
                    }
                }
                temp=temp1;
                if(i<3){
                    if(layerTransitionsPairWithOrdering[i+1].second==order::second){
                        for(int j=0;j<3;j++){
                            temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][j][layerTransitionsPairWithOrdering[i+1].first.second];
                        }
                    }
                    else{ // ==order::thirdplusreverse
                        for(int j=0;j<3;j++){
                            temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][layerTransitionsPairWithOrdering[i+1].first.second][j];
                        }
                    }
                }
            }
            //side layer corner changes
            color tempColor1=cube3DArray[3][0][0];
            color tempColor=cube3DArray[3][0][2];
            vector<pair<int,int>> layerTransitionsPair={{0,0},{2,0},{2,2},{0,2}};
            for(int i=0;i<4;i++){
                cube3DArray[3][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[3][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
            //side layer edge changes
            tempColor1=cube3DArray[3][0][1];
            tempColor=cube3DArray[3][1][2];
            layerTransitionsPair={{0,1},{1,0},{2,1},{1,2}};
            for(int i=0;i<4;i++){
                cube3DArray[3][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[3][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
        }
        else if(front==3){
            //rotating layer changes
            vector<color> temp1={cube3DArray[3][0][2],cube3DArray[3][1][2],cube3DArray[3][2][2]};
            vector<color> temp={cube3DArray[0][0][0],cube3DArray[0][1][0],cube3DArray[0][2][0]};
            vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{3,2},order::secondplusreverse},{{5,0},order::secondplusreverse},{{1,0},order::second},{{0,0},order::second}};
            for(int i=0;i<4;i++){
                for(int j=0;j<3;j++){
                    if(layerTransitionsPairWithOrdering[i].second==order::second){
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[j];
                    }
                    else{
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[2-j];
                    }
                }
                temp=temp1;
                if(i<3){
                    for(int j=0;j<3;j++){
                        temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][j][layerTransitionsPairWithOrdering[i+1].first.second];
                    }
                }
            }
            //side layer corner changes
            color tempColor1=cube3DArray[4][0][0];
            color tempColor=cube3DArray[4][0][2];
            vector<pair<int,int>> layerTransitionsPair={{0,0},{2,0},{2,2},{0,2}};
            for(int i=0;i<4;i++){
                cube3DArray[4][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[4][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
            //side layer edge changes
            tempColor1=cube3DArray[4][0][1];
            tempColor=cube3DArray[4][1][2];
            layerTransitionsPair={{0,1},{1,0},{2,1},{1,2}};
            for(int i=0;i<4;i++){
                cube3DArray[4][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[4][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
        }
        else if(front==4){
            //rotating layer changes
            vector<color> temp1={cube3DArray[4][0][2],cube3DArray[4][1][2],cube3DArray[4][2][2]};
            vector<color> temp={cube3DArray[0][2][0],cube3DArray[0][2][1],cube3DArray[0][2][2]};
            vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{4,2},order::secondplusreverse},{{5,0},order::third},{{2,0},order::secondplusreverse},{{0,2},order::third}};
            for(int i=0;i<4;i++){
                for(int j=0;j<3;j++){
                    if(layerTransitionsPairWithOrdering[i].second==order::third){
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][layerTransitionsPairWithOrdering[i].first.second][j]=temp[j];
                    }
                    else{ // ==order::secondplusreverse
                        cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[2-j];
                    }
                }
                temp=temp1;
                if(i<3){
                    if(layerTransitionsPairWithOrdering[i+1].second==order::third){
                        for(int j=0;j<3;j++){
                            temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][layerTransitionsPairWithOrdering[i+1].first.second][j];
                        }
                    }
                    else{ // ==order::secondplusreverse
                        for(int j=0;j<3;j++){
                            temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][j][layerTransitionsPairWithOrdering[i+1].first.second];
                        }
                    }
                }
            }
            //side layer corner changes
            color tempColor1=cube3DArray[1][0][0];
            color tempColor=cube3DArray[1][0][2];
            vector<pair<int,int>> layerTransitionsPair={{0,0},{2,0},{2,2},{0,2}};
            for(int i=0;i<4;i++){
                cube3DArray[1][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[1][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
            //side layer edge changes
            tempColor1=cube3DArray[1][0][1];
            tempColor=cube3DArray[1][1][2];
            layerTransitionsPair={{0,1},{1,0},{2,1},{1,2}};
            for(int i=0;i<4;i++){
                cube3DArray[1][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
                tempColor=tempColor1;
                if(i<3){
                    tempColor1=cube3DArray[1][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
                }
            }
        }
    }
    else if(top==5){
        if(front==1){
            Rprime(0,3);
        }
        else if(front==2){
            Rprime(0,4);
        }
        else if(front==3){
            Rprime(0,1);
        }
        else if(front==4){
            Rprime(0,2);
        }
    }
    this->oneHotEncodedArray=convertToOneHot(cube3DArray);
}

void Cube::L(int top=0,int front=1){
    if(top==0){
        if(front==1){
            R(0,3);
        }
        else if(front==2){
            R(0,4);
        }
        else if(front==3){
            R(0,1);
        }
        else if(front==4){
            R(0,2);
        }
    }
    else if(top==5){
        if(front==1){
            R(0,1);
        }
        else if(front==2){
            R(0,2);
        }
        else if(front==3){
            R(0,3);
        }
        else if(front==4){
            R(0,4);
        }
    }
}

void Cube::Lprime(int top=0,int front=1){
    if(top==0){
        if(front==1){
            Rprime(0,3);
        }
        else if(front==2){
            Rprime(0,4);
        }
        else if(front==3){
            Rprime(0,1);
        }
        else if(front==4){
            Rprime(0,2);
        }
    }
    else if(top==5){
        if(front==1){
            Rprime(0,1);
        }
        else if(front==2){
            Rprime(0,2);
        }
        else if(front==3){
            Rprime(0,3);
        }
        else if(front==4){
            Rprime(0,4);
        }
    }
}

void Cube::F(int top=0,int front=1){
    if(top==0){
        if(front==1){
            R(0,4);
        }
        else if(front==2){
            R(0,1);
        }
        else if(front==3){
            R(0,2);
        }
        else if(front==4){
            R(0,3);
        }
    }
    else if(top==5){
        if(front==1){
            R(5,2);
        }
        else if(front==2){
            R(5,3);
        }
        else if(front==3){
            R(5,4);
        }
        else if(front==4){
            R(5,1);
        }
    }
}

void Cube::Fprime(int top=0,int front=1){
    if(top==0){
        if(front==1){
            Rprime(0,4);
        }
        else if(front==2){
            Rprime(0,1);
        }
        else if(front==3){
            Rprime(0,2);
        }
        else if(front==4){
            Rprime(0,3);
        }
    }
    else if(top==5){
        if(front==1){
            Rprime(5,2);
        }
        else if(front==2){
            Rprime(5,3);
        }
        else if(front==3){
            Rprime(5,4);
        }
        else if(front==4){
            Rprime(5,1);
        }
    }
}

void Cube::U(int top=0){
    if(top==0){
         //rotating layer changes
         vector<color> temp1={cube3DArray[4][0][0],cube3DArray[4][0][1],cube3DArray[4][0][2]};
         vector<color> temp={cube3DArray[1][0][0],cube3DArray[1][0][1],cube3DArray[1][0][2]};
         vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{4,0},order::third},{{3,0},order::third},{{2,0},order::third},{{1,0},order::third}};
         for(int i=0;i<4;i++){
             for(int j=0;j<3;j++){
                cube3DArray[layerTransitionsPairWithOrdering[i].first.first][layerTransitionsPairWithOrdering[i].first.second][j]=temp[j];
             }
             temp=temp1;
             if(i<3){
                 for(int j=0;j<3;j++){
                     temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][layerTransitionsPairWithOrdering[i+1].first.second][j];
                 }
             }
         }
         //side layer corner changes
         color tempColor1=cube3DArray[0][0][2];
         color tempColor=cube3DArray[0][0][0];
         vector<pair<int,int>> layerTransitionsPair={{0,2},{2,2},{2,0},{0,0}};
         for(int i=0;i<4;i++){
             cube3DArray[0][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
             tempColor=tempColor1;
             if(i<3){
                 tempColor1=cube3DArray[0][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
             }
         }
         //side layer edge changes
         tempColor=cube3DArray[0][0][1];
         tempColor1=cube3DArray[0][1][2];
         layerTransitionsPair={{1,2},{2,1},{1,0},{0,1}};
         for(int i=0;i<4;i++){
             cube3DArray[0][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
             tempColor=tempColor1;
             if(i<3){
                 tempColor1=cube3DArray[0][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
             }
         }
    }
    else if(top==5){
        //rotating layer changes
        vector<color> temp1={cube3DArray[2][2][0],cube3DArray[2][2][1],cube3DArray[2][2][2]};
        vector<color> temp={cube3DArray[1][2][0],cube3DArray[1][2][1],cube3DArray[1][2][2]};
        vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{2,2},order::third},{{3,2},order::third},{{4,2},order::third},{{1,2},order::third}};
        for(int i=0;i<4;i++){
            for(int j=0;j<3;j++){
               cube3DArray[layerTransitionsPairWithOrdering[i].first.first][layerTransitionsPairWithOrdering[i].first.second][j]=temp[j];
            }
            temp=temp1;
            if(i<3){
                for(int j=0;j<3;j++){
                    temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][layerTransitionsPairWithOrdering[i+1].first.second][j];
                }
            }
        }
        //side layer corner changes
        color tempColor1=cube3DArray[5][0][2];
        color tempColor=cube3DArray[5][0][0];
        vector<pair<int,int>> layerTransitionsPair={{0,2},{2,2},{2,0},{0,0}};
        for(int i=0;i<4;i++){
            cube3DArray[5][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
            tempColor=tempColor1;
            if(i<3){
                tempColor1=cube3DArray[5][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
            }
        }
        //side layer edge changes
        tempColor=cube3DArray[5][0][1];
        tempColor1=cube3DArray[5][1][2];
        layerTransitionsPair={{1,2},{2,1},{1,0},{0,1}};
        for(int i=0;i<4;i++){
            cube3DArray[5][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
            tempColor=tempColor1;
            if(i<3){
                tempColor1=cube3DArray[5][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
            }
        }
    }
    this->oneHotEncodedArray=convertToOneHot(cube3DArray);
}

void Cube::Uprime(int top=0){
    if(top==0){
        //rotating layer changes
        vector<color> temp={cube3DArray[4][0][0],cube3DArray[4][0][1],cube3DArray[4][0][2]};
        vector<color> temp1={cube3DArray[1][0][0],cube3DArray[1][0][1],cube3DArray[1][0][2]};
        vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{1,0},order::third},{{2,0},order::third},{{3,0},order::third},{{4,0},order::third}};
        for(int i=0;i<4;i++){
            for(int j=0;j<3;j++){
               cube3DArray[layerTransitionsPairWithOrdering[i].first.first][layerTransitionsPairWithOrdering[i].first.second][j]=temp[j];
            }
            temp=temp1;
            if(i<3){
                for(int j=0;j<3;j++){
                    temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][layerTransitionsPairWithOrdering[i+1].first.second][j];
                }
            }
        }
        //side layer corner changes
        color tempColor=cube3DArray[0][0][2];
        color tempColor1=cube3DArray[0][0][0];
        vector<pair<int,int>> layerTransitionsPair={{0,0},{2,0},{2,2},{0,2}};
        for(int i=0;i<4;i++){
            cube3DArray[0][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
            tempColor=tempColor1;
            if(i<3){
                tempColor1=cube3DArray[0][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
            }
        }
        //side layer edge changes
        tempColor1=cube3DArray[0][0][1];
        tempColor=cube3DArray[0][1][2];
        layerTransitionsPair={{0,1},{1,0},{2,1},{1,2}};
        for(int i=0;i<4;i++){
            cube3DArray[0][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
            tempColor=tempColor1;
            if(i<3){
                tempColor1=cube3DArray[0][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
            }
        }
    }
    else if(top==5){
        //rotating layer changes
        vector<color> temp={cube3DArray[2][2][0],cube3DArray[2][2][1],cube3DArray[2][2][2]};
        vector<color> temp1={cube3DArray[1][2][0],cube3DArray[1][2][1],cube3DArray[1][2][2]};
        vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{1,2},order::third},{{4,2},order::third},{{3,2},order::third},{{2,2},order::third}};
        for(int i=0;i<4;i++){
            for(int j=0;j<3;j++){
               cube3DArray[layerTransitionsPairWithOrdering[i].first.first][layerTransitionsPairWithOrdering[i].first.second][j]=temp[j];
            }
            temp=temp1;
            if(i<3){
                for(int j=0;j<3;j++){
                    temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][layerTransitionsPairWithOrdering[i+1].first.second][j];
                }
            }
        }
        //side layer corner changes
        color tempColor=cube3DArray[5][0][2];
        color tempColor1=cube3DArray[5][0][0];
        vector<pair<int,int>> layerTransitionsPair={{0,0},{2,0},{2,2},{0,2}};
        for(int i=0;i<4;i++){
            cube3DArray[5][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
            tempColor=tempColor1;
            if(i<3){
                tempColor1=cube3DArray[5][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
            }
        }
        //side layer edge changes
        tempColor1=cube3DArray[5][0][1];
        tempColor=cube3DArray[5][1][2];
        layerTransitionsPair={{0,1},{1,0},{2,1},{1,2}};
        for(int i=0;i<4;i++){
            cube3DArray[5][layerTransitionsPair[i].first][layerTransitionsPair[i].second]=tempColor;
            tempColor=tempColor1;
            if(i<3){
                tempColor1=cube3DArray[5][layerTransitionsPair[i+1].first][layerTransitionsPair[i+1].second];
            }
        }
    }
    this->oneHotEncodedArray=convertToOneHot(cube3DArray);
}

void Cube::D(int top=0){
    if(top==0){
        U(5);
    }
    else if(top==5){
        U(0);
    }
}

void Cube::Dprime(int top=0){
    if(top==0){
        Uprime(5);
    }
    else if(top==5){
        Uprime(0);
    }
}

// void Cube::M(int top=0,int front=1){
//     if(top==0){ //color::White on top
//         if(front==1){ //red in front
//             //rotating layer changes
//             vector<color> temp={cube3DArray[1][0][1],cube3DArray[1][1][1],cube3DArray[1][2][1]};
//             vector<color> temp1={cube3DArray[0][0][1],cube3DArray[0][1][1],cube3DArray[0][2][1]};
//             vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{0,1},order::second},{{3,1},order::secondplusreverse},{{5,1},order::secondplusreverse},{{1,1},order::second}};
//             for(int i=0;i<4;i++){
//                 for(int j=0;j<3;j++){
//                     if(layerTransitionsPairWithOrdering[i].second==order::second){
//                         cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[j];
//                     }
//                     else{
//                         cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[2-j];
//                     }
//                 }
//                 temp=temp1;
//                 if(i<3){
//                     for(int j=0;j<3;j++){
//                         temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][j][layerTransitionsPairWithOrdering[i+1].first.second];
//                     }
//                 }
//             }
//         }
//         else if(front==2){ //green in front
//             //rotating layer changes
//             vector<color> temp={cube3DArray[2][0][1],cube3DArray[2][1][1],cube3DArray[2][2][1]};
//             vector<color> temp1={cube3DArray[0][1][0],cube3DArray[0][1][1],cube3DArray[0][1][2]};
//             vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{0,1},order::third},{{4,1},order::secondplusreverse},{{5,1},order::third},{{2,1},order::secondplusreverse}};
//             for(int i=0;i<4;i++){
//                 for(int j=0;j<3;j++){
//                     if(layerTransitionsPairWithOrdering[i].second==order::secondplusreverse){
//                         cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[2-j];
//                     }
//                     else{ // ==order::third
//                         cube3DArray[layerTransitionsPairWithOrdering[i].first.first][layerTransitionsPairWithOrdering[i].first.second][j]=temp[j];
//                     }
//                 }
//                 temp=temp1;
//                 if(i<3){
//                     if(layerTransitionsPairWithOrdering[i+1].second==order::secondplusreverse){
//                         for(int j=0;j<3;j++){
//                             temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][j][layerTransitionsPairWithOrdering[i+1].first.second];
//                         }
//                     }
//                     else{ // ==order::third
//                         for(int j=0;j<3;j++){
//                             temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][layerTransitionsPairWithOrdering[i+1].first.second][j];
//                         }
//                     }
//                 }
//             }
//         }
//         else if(front==3){ //orange in front
//             //rotating layer changes
//             vector<color> temp={cube3DArray[3][0][1],cube3DArray[3][1][1],cube3DArray[3][2][1]};
//             vector<color> temp1={cube3DArray[0][0][1],cube3DArray[0][1][1],cube3DArray[0][2][1]};
//             vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{0,1},order::secondplusreverse},{{1,1},order::second},{{5,1},order::second},{{3,1},order::secondplusreverse}};
//             for(int i=0;i<4;i++){
//                 for(int j=0;j<3;j++){
//                     if(layerTransitionsPairWithOrdering[i].second==order::second){
//                         cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[j];
//                     }
//                     else{
//                         cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[2-j];
//                     }
//                 }
//                 temp=temp1;
//                 if(i<3){
//                     for(int j=0;j<3;j++){
//                         temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][j][layerTransitionsPairWithOrdering[i+1].first.second];
//                     }
//                 }
//             }
//         }
//         else if(front==4){ //blue in front
//             //rotating layer changes
//             vector<color> temp={cube3DArray[4][0][1],cube3DArray[4][1][1],cube3DArray[4][2][1]};
//             vector<color> temp1={cube3DArray[0][1][0],cube3DArray[0][1][1],cube3DArray[0][1][2]};
//             vector<pair<pair<int,int>,order>> layerTransitionsPairWithOrdering={{{0,1},order::thirdplusreverse},{{2,1},order::second},{{5,1},order::thirdplusreverse},{{4,1},order::second}};
//             for(int i=0;i<4;i++){
//                 for(int j=0;j<3;j++){
//                     if(layerTransitionsPairWithOrdering[i].second==order::thirdplusreverse){
//                         cube3DArray[layerTransitionsPairWithOrdering[i].first.first][layerTransitionsPairWithOrdering[i].first.second][j]=temp[2-j];
//                     }
//                     else{ // ==second
//                         cube3DArray[layerTransitionsPairWithOrdering[i].first.first][j][layerTransitionsPairWithOrdering[i].first.second]=temp[j];
//                     }
//                 }
//                 temp=temp1;
//                 if(i<3){
//                     if(layerTransitionsPairWithOrdering[i+1].second==order::thirdplusreverse){
//                         for(int j=0;j<3;j++){
//                             temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][layerTransitionsPairWithOrdering[i+1].first.second][j];
//                         }
//                     }
//                     else{ // ==second
//                         for(int j=0;j<3;j++){
//                             temp1[j]=cube3DArray[layerTransitionsPairWithOrdering[i+1].first.first][j][layerTransitionsPairWithOrdering[i+1].first.second];
//                         }
//                     }
//                 }
//             }
//         }
//     }
//     else if(top==5){
//         if(front==1){
//             M(0,3);
//         }
//         else if(front==2){
//            M(0,4);
//         }
//         else if(front==3){
//             M(0,1);
//         }
//         else if(front==4){
//             M(0,2);
//         }
//     }
//     this->oneHotEncodedArray=convertToOneHot(cube3DArray);
// }

void Cube::M(int top=0,int front=1){
    if(top==0){ //color::White on top
        if(front==1){ //red in front
            Rprime(0,1);
            L(0,1);
        }
        else if(front==2){ //green in front
            Rprime(0,2);
            L(0,2);
        }
        else if(front==3){ //orange in front
            Rprime(0,3);
            L(0,3);
        }
        else if(front==4){ //blue in front
            Rprime(0,4);
            L(0,4);
        }
    }
    else if(top==5){
        if(front==1){
            M(0,3);
        }
        else if(front==2){
           M(0,4);
        }
        else if(front==3){
            M(0,1);
        }
        else if(front==4){
            M(0,2);
        }
    }
    this->oneHotEncodedArray=convertToOneHot(cube3DArray);
}

void Cube::Mprime(int top=0,int front=1){
    if(top==0){
        if(front==1){
            M(5,1);
        }
        else if(front==2){
            M(5,2);
        }
        else if(front==3){
            M(5,3);
        }
        else if(front==4){
            M(5,4);
        }
    }
    else if(top==5){
        if(front==1){
            M(0,1);
        }
        else if(front==2){
            M(0,2);
        }
        else if(front==3){
            M(0,3);
        }
        else if(front==4){
            M(0,4);
        }
    }
}

void Cube::printCube(){
    cout<<endl;
    for(int i=0;i<6;i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<3;k++){
                if(cube3DArray[i][j][k]==color::White){
                    cout<<"W ";
                }
                else if(cube3DArray[i][j][k]==color::Orange){
                    cout<<"O ";
                }
                else if(cube3DArray[i][j][k]==color::Red){
                    cout<<"R ";
                }
                else if(cube3DArray[i][j][k]==color::Yellow){
                    cout<<"Y ";
                }
                else if(cube3DArray[i][j][k]==color::Green){
                    cout<<"G ";
                }
                else if(cube3DArray[i][j][k]==color::Blue){
                    cout<<"B ";
                }
            }
            cout<<endl;
        }
        cout<<"================================="<<endl;
    }
    cout<<endl<<endl<<endl;
}

vector<int> oneHotEncode(color c) {
    vector<int> oneHot(6,0);
    if(c==color::White){
        oneHot[0]=1;
    }
    else if(c==color::Red){
        oneHot[1]=1;
    }
    else if(c==color::Green){
        oneHot[2]=1;
    }
    else if(c==color::Orange){
        oneHot[3]=1;
    }
    else if(c==color::Blue){
        oneHot[4]=1;
    }
    else if(c==color::Yellow){
        oneHot[5]=1;
    }
    return oneHot;
}

color convertBack(vector<int> oneHot) {
    color c;
    if(oneHot[0]=1){
        c=color::White;
    }
    else if(oneHot[1]=1){
        c==color::Red;
    }
    else if(oneHot[0]=1){
        c==color::Green;
    }
    else if(oneHot[0]=1){
        c==color::Orange;
    }
    else if(oneHot[0]=1){
        c==color::Blue;
    }
    else if(oneHot[0]=1){
        c==color::Yellow;
    }
    return c;
}


// Function to convert 6*3*3 color cube to 6*3*3*6 one-hot encoded cube
vector<vector<vector<vector<int>>>> Cube::convertToOneHot(vector<vector<vector<color>>>& cube) {
    vector<vector<vector<vector<int>>>> oneHotCube(6, vector<vector<vector<int>>>(3, vector<vector<int>>(3, vector<int>(6, 0))));
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                oneHotCube[i][j][k] = oneHotEncode(cube[i][j][k]);
            }
        }
    }
    return oneHotCube;
}

vector<vector<vector<color>>> Cube::fullyConvertBack(vector<vector<vector<vector<int>>>>& OneHotCube) {
    vector<vector<vector<color>>> cube(6, vector<vector<color>>(3, vector<color>(3, color::White)));
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                cube[i][j][k] = convertBack(OneHotCube[i][j][k]);
            }
        }
    }
    return cube;
}

void Cube::takeAction(int numAction){
    // [R(0,1),R(0,2),R(0,3),R(0,4),R'(0,1),R'(0,2),R'(0,3),R'(0,4),U(0),U(5),U'(0),U'(5),M(0,1),M(0,2),M'(0,1),M'(0,2)] -> 0-15
    if(numAction==0){
        R(0,1);
    }
    else if(numAction==1){
        R(0,2);
    }
    else if(numAction==2){
        R(0,3);
    }
    else if(numAction==3){
        R(0,4);
    }
    else if(numAction==4){
        Rprime(0,1);
    }
    else if(numAction==5){
        Rprime(0,2);
    }
    else if(numAction==6){
        Rprime(0,3);
    }
    else if(numAction==7){
        Rprime(0,4);
    }
    else if(numAction==8){
        U(0);
    }
    else if(numAction==9){
        U(5);
    }
    else if(numAction==10){
        Uprime(0);
    }
    else if(numAction==11){
        Uprime(5);
    }
    else if(numAction==12){
        M(0,1);
    }
    else if(numAction==13){
        M(0,2);
    }
    else if(numAction==14){
        Mprime(0,1);
    }
    else if(numAction==15){
        Mprime(0,2);
    }
}

bool Cube::checkWhiteCross(){
    if(cube3DArray[0][0][1]==color::White &&
    cube3DArray[3][0][1]==color::Orange &&
    cube3DArray[0][1][2]==color::White &&
    cube3DArray[4][0][1]==color::Blue &&
    cube3DArray[0][2][1]==color::White &&
    cube3DArray[1][0][1]==color::Red &&
    cube3DArray[0][1][0]==color::White &&
    cube3DArray[2][0][1]==color::Green ){
        return true;
    }
    return false;
}

bool Cube::checkCross1(){
    if((cube3DArray[0][0][1]==color::White &&
    cube3DArray[3][0][1]==color::Orange) ||
    (cube3DArray[0][1][2]==color::White &&
    cube3DArray[4][0][1]==color::Blue) ||
    (cube3DArray[0][2][1]==color::White &&
    cube3DArray[1][0][1]==color::Red) ||
    (cube3DArray[0][1][0]==color::White &&
    cube3DArray[2][0][1]==color::Green) ){
        return true;
    }
    return false;
}

bool Cube::checkCross2(){
    if(((cube3DArray[0][0][1]==color::White &&
    cube3DArray[3][0][1]==color::Orange)&& (cube3DArray[0][1][2]==color::White &&
        cube3DArray[4][0][1]==color::Blue)) ||
        ((cube3DArray[0][2][1]==color::White &&
            cube3DArray[1][0][1]==color::Red)&&((cube3DArray[0][0][1]==color::White &&
                cube3DArray[3][0][1]==color::Orange))) ||
        ((cube3DArray[0][0][1]==color::White &&
            cube3DArray[3][0][1]==color::Orange)&&(cube3DArray[0][1][0]==color::White &&
                cube3DArray[2][0][1]==color::Green)) ||
    ((cube3DArray[0][1][2]==color::White &&
    cube3DArray[4][0][1]==color::Blue)&&(cube3DArray[0][2][1]==color::White &&
        cube3DArray[1][0][1]==color::Red)) ||
        ((cube3DArray[0][1][2]==color::White &&
            cube3DArray[4][0][1]==color::Blue)&&
            (cube3DArray[0][1][0]==color::White &&
                cube3DArray[2][0][1]==color::Green)) ||
    ((cube3DArray[0][2][1]==color::White &&
    cube3DArray[1][0][1]==color::Red) &&
    (cube3DArray[0][1][0]==color::White &&
    cube3DArray[2][0][1]==color::Green)) ){
        return true;
    }
    return false;
}

bool Cube::checkCross3(){
    if(((cube3DArray[0][0][1]==color::White &&
    cube3DArray[3][0][1]==color::Orange)&&(cube3DArray[0][1][2]==color::White &&
        cube3DArray[4][0][1]==color::Blue)&&(cube3DArray[0][1][0]==color::White &&
            cube3DArray[2][0][1]==color::Green)) ||
        ((cube3DArray[0][0][1]==color::White &&
            cube3DArray[3][0][1]==color::Orange) &&
            (cube3DArray[0][1][2]==color::White &&
            cube3DArray[4][0][1]==color::Blue)&&(cube3DArray[0][1][0]==color::White &&
                cube3DArray[2][0][1]==color::Green))||
        ((cube3DArray[0][0][1]==color::White &&
            cube3DArray[3][0][1]==color::Orange) &&
            (cube3DArray[0][2][1]==color::White &&
                cube3DArray[1][0][1]==color::Red) &&
                (cube3DArray[0][1][0]==color::White &&
                cube3DArray[2][0][1]==color::Green))||
        ((cube3DArray[0][1][2]==color::White &&
            cube3DArray[4][0][1]==color::Blue) &&
            (cube3DArray[0][2][1]==color::White &&
            cube3DArray[1][0][1]==color::Red) &&
            (cube3DArray[0][1][0]==color::White &&
            cube3DArray[2][0][1]==color::Green))) {
        return true;
    }
    return false;
}

bool Cube::checkF2Lpair1(){
    if(cube3DArray[0][0][0]==color::White && cube3DArray[3][0][2]==color::Orange 
        && cube3DArray[3][1][2]==color::Orange && cube3DArray[4][0][0]==color::Blue 
        && cube3DArray[4][1][0]==color::Blue){
            return true;
        }
    return false;
} 

bool Cube::checkF2Lpair2(){
    if(cube3DArray[0][2][0]==color::White
        && cube3DArray[1][0][0]==color::Red && cube3DArray[1][1][0]==color::Red &&
        cube3DArray[4][0][2]==color::Blue && cube3DArray[4][1][2]==color::Blue){
            return true;
        }
    return false;
} 

bool Cube::checkF2Lpair3(){
    if(cube3DArray[0][2][2]==color::White && cube3DArray[1][0][2]==color::Red &&
        cube3DArray[1][1][2]==color::Red && cube3DArray[2][0][0]==color::Green &&
        cube3DArray[2][1][0]==color::Green){
            return true;
        }
    return false;
} 

bool Cube::checkF2Lpair4(){
    if(cube3DArray[0][0][2]==color::White &&
        cube3DArray[2][0][2]==color::Green && cube3DArray[2][1][2]==color::Green &&
        cube3DArray[3][0][0]==color::Orange && cube3DArray[3][1][0]==color::Orange){
            return true;
        }
    return false;
} 

bool Cube::checkF2L1pair(){
    if(checkWhiteCross() &&(checkF2Lpair1()||checkF2Lpair2()||checkF2Lpair3()||checkF2Lpair4())){
        return true;
    }
    return false;
}

bool Cube::checkF2L2pair(){
    if(checkWhiteCross() &&((checkF2Lpair1()&&checkF2Lpair2())||(checkF2Lpair1()&&checkF2Lpair3())
        ||(checkF2Lpair1()&&checkF2Lpair4())||(checkF2Lpair2()&&checkF2Lpair3())||
        (checkF2Lpair2()&&checkF2Lpair4())||(checkF2Lpair3()&&checkF2Lpair4()))){
        return true;
    }
    return false;
}

bool Cube::checkF2L3pair(){
    if(checkWhiteCross()&&((checkF2Lpair1()&&checkF2Lpair2()&&checkF2Lpair3())||
    (checkF2Lpair1()&&checkF2Lpair2()&&checkF2Lpair4())||(checkF2Lpair1()&&checkF2Lpair3()&&checkF2Lpair4())
    ||(checkF2Lpair2()&&checkF2Lpair3()&&checkF2Lpair4()))){
        return true;
    }
    return false;
}

bool Cube::checkF2Lcompletion(){
    // if(checkF2Lpair1()){
    //     cout<<"1"<<endl;
    // }
    // if(checkF2Lpair2()){
    //     cout<<"2"<<endl;
    // }
    // if(checkF2Lpair3()){
    //     cout<<"3"<<endl;
    // }
    // if(checkF2Lpair4()){
    //     cout<<"4"<<endl;
    // }
    if(checkWhiteCross()&&checkF2Lpair1()&&checkF2Lpair2()&&checkF2Lpair3()&&checkF2Lpair4()){
        return true;
    }
    return false;
}

bool Cube::checkTillYellowCross(){
    if(checkF2Lcompletion() && cube3DArray[5][0][1]==color::Yellow && cube3DArray[5][1][2]==color::Yellow
        && cube3DArray[5][2][1]==color::Yellow && cube3DArray[5][1][0]==color::Yellow){
        return true;
    }
    return false;
}

bool Cube::checkTillOLL(){
    if(checkTillYellowCross() && cube3DArray[5][0][0]==color::Yellow && cube3DArray[5][0][2]==color::Yellow
        && cube3DArray[5][2][2]==color::Yellow && cube3DArray[5][2][0]==color::Yellow){
        return true;
    }
    return false;
}

bool Cube::checkCubeCompletion(){
    if(checkTillOLL() && cube3DArray[1][2][0]==color::Red && cube3DArray[1][2][1]==color::Red &&
        cube3DArray[1][2][2]==color::Red && cube3DArray[2][2][0]==color::Green && 
        cube3DArray[2][2][1]==color::Green && cube3DArray[2][2][2]==color::Green &&
        cube3DArray[3][2][0]==color::Orange && cube3DArray[3][2][1]==color::Orange &&
        cube3DArray[3][2][2]==color::Orange && cube3DArray[4][2][0]==color::Blue &&
        cube3DArray[4][2][1]==color::Blue && cube3DArray[4][2][2]==color::Blue){
            return true;
    }
    return false;
}

void Cube::resetToSolved(){
    this->cube3DArray=this->solvedCube3DArray;
    this->oneHotEncodedArray=this->solvedOneHotEncodedArray;
}

vector<int> Cube::randomScramble(int numMoves=22){
    random_device rd;  
    mt19937 gen(rd()); 
    uniform_int_distribution<int> dist(0, 15); 
    vector<int> moves;
    int numAction;
    this->resetToSolved();
    for(int i=0;i<numMoves;i++){
        numAction=dist(gen);
        this->takeAction(numAction);
        moves.push_back(numAction);
    }
    return moves;
}

void Cube::testing(bool verbose=false){
    random_device rd;  
    mt19937 gen(rd()); 
    uniform_int_distribution<int> dist(0, 15); 
    int numAction;
    int numMoves=22;
    this->resetToSolved();
    for(int i=0;i<numMoves;i++){
        numAction=dist(gen);
        if(verbose){
            if(numAction==0){
                cout<<"R(0,1)"<<endl;
            }
            else if(numAction==1){
                cout<<"R(0,2)"<<endl;
            }
            else if(numAction==2){
                cout<<"R(0,3)"<<endl;
            }
            else if(numAction==3){
                cout<<"R(0,4)"<<endl;
            }
            else if(numAction==4){
                cout<<"Rprime(0,1)"<<endl;
            }
            else if(numAction==5){
                cout<<"Rprime(0,2)"<<endl;
            }
            else if(numAction==6){
                cout<<"Rprime(0,3)"<<endl;
            }
            else if(numAction==7){
                cout<<"Rprime(0,4)"<<endl;
            }
            else if(numAction==8){
                cout<<"U(0)"<<endl;
            }
            else if(numAction==9){
                cout<<"U(5)"<<endl;
            }
            else if(numAction==10){
                cout<<"Uprime(0)"<<endl;
            }
            else if(numAction==11){
                cout<<"Uprime(5)"<<endl;
            }
            else if(numAction==12){
                cout<<"M(0,1)"<<endl;
            }
            else if(numAction==13){
                cout<<"M(0,2)"<<endl;
            }
            else if(numAction==14){
                cout<<"Mprime(0,1)"<<endl;
            }
            else if(numAction==15){
                cout<<"Mprime(0,2)"<<endl;
            }
        }
        this->takeAction(numAction);
        if(verbose){
            printCube();
        }
    }
}

color Cube::locationToColor(vector<int> location){
    return this->cube3DArray[location[0]][location[1]][location[2]];
}

// void Cube::execueteCommands(vector<string> commands){
//     for(string command:commands){
//         if(command=="R")
//     }
// }