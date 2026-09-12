#include<iostream>
#include<unordered_map>
#include "headers/cube_solving.hpp"
using namespace std;

//scramble R2UB2R'L'U2RD'FB' white as top and red as front

CubeSolver::CubeSolver(Cube cube){
    this->cube=cube;
    this->whiteCrossVerboseMode=false;
    this->f2lVerboseMode=false;
    // std::vector<std::pair<std::pair<std::vector<int>,std::vector<int>>,color>>
    // vector of 4 edges{pair{pair{edge face1,edge face2},color}}
    this->WhiteEdges={{{{0,1,2},{2,0,1}},color::Green},
                      {{{0,2,1},{1,0,1}},color::Red},
                      {{{0,1,0},{4,0,1}},color::Blue},
                      {{{0,0,1},{3,0,1}},color::Orange}};
    this->YellowEdges={{{{5,1,2},{2,2,1}},color::Green},
                       {{{5,2,1},{3,2,1}},color::Orange},
                       {{{5,1,0},{4,2,1}},color::Blue},
                       {{{5,0,1},{1,2,1}},color::Red}};
    // std::vector<std::pair<std::vector<int>,std::vector<int>>> 
    //vector of 4 edges<pair<edge face1,edge face2>>
    this->MiddleEdges={{{4,1,2},{1,1,0}},
                       {{1,1,2},{2,1,0}},
                       {{2,1,2},{3,1,0}},
                       {{3,1,2},{4,1,0}}};
    // std::vector<std::pair<std::vector<std::vector<int>>,std::vector<color>>>
    // std::vector<color> part seems redundant- may use in future, currently not being used
    this->whiteCorners={{{{0,0,0},{3,0,2},{4,0,1}},{color::Orange,color::Blue}},
                        {{{0,0,2},{2,0,2},{3,0,1}},{color::Green,color::Orange}},
                        {{{0,2,0},{1,0,0},{4,0,2}},{color::Red,color::Blue}},
                        {{{0,2,2},{1,0,2},{2,0,0}},{color::Red,color::Green}}};
    this->yellowCorners={{{{5,0,0},{1,2,0},{4,2,2}},{color::Red,color::Blue}},
                         {{{5,0,2},{1,2,2},{2,2,0}},{color::Red,color::Green}},
                         {{{5,2,0},{3,2,2},{4,2,0}},{color::Orange,color::Blue}},
                         {{{5,2,2},{2,2,2},{3,2,0}},{color::Green,color::Orange}}};
    unordered_map<color,int> f2lColorNumbering;
    f2lColorNumbering[color::Red]=1;
    f2lColorNumbering[color::Blue]=2;
    f2lColorNumbering[color::Orange]=3;
    f2lColorNumbering[color::Green]=4;
}

pair<pair<vector<int>,vector<int>>,WhiteEdgeType> CubeSolver::findUnmatchedWhiteEdge(){
    vector<int> temp;
    vector<int> temp1;
    vector<int> modifiedTemp1;
    //checking type1 and half of type2 edges
    for(pair<pair<vector<int>,vector<int>>,color> element:WhiteEdges){
        temp=element.first.first;
        temp1=element.first.second;
        modifiedTemp1=temp1;
        modifiedTemp1[1]+=1;
        if(cube.locationToColor(temp)==color::White && 
            cube.locationToColor(temp1)!=cube.locationToColor(modifiedTemp1)){
                return {{temp,temp1},WhiteEdgeType::type1};
        }
        else if(cube.locationToColor(temp1)==color::White){
            return {{temp1,temp},WhiteEdgeType::type2};
        }
    }
    // vector<pair<pair<vector<int>,vector<int>>,color>> YellowEdges={{{{5,1,2},{2,2,1}},color::Green},{{{5,2,1},{3,2,1}},color::Orange},{{{5,1,0},{4,2,1}},color::Blue},{{{5,0,1},{1,2,1}},color::Red}};
    //checking type4 and rest half of type2 edges
    for(pair<pair<vector<int>,vector<int>>,color> element:YellowEdges){
        temp=element.first.first;
        temp1=element.first.second;
        if(cube.locationToColor(temp)==color::White){
                return {{temp,temp1},WhiteEdgeType::type4};
        }
        else if(cube.locationToColor(temp1)==color::White){
            return {{temp1,temp},WhiteEdgeType::type2};
        }
    }
    //checking type3
    for(pair<vector<int>,vector<int>> Edge:MiddleEdges){
        temp=Edge.first;
        temp1=Edge.second;
        if(cube.locationToColor(temp)==color::White){
            return {{temp,temp1},WhiteEdgeType::type3};
        }
        else if(cube.locationToColor(temp1)==color::White){
            return {{temp1,temp},WhiteEdgeType::type3};
        }
    }
    return {{{0,0,0},{0,0,0}},WhiteEdgeType::done};
} 

void CubeSolver::whiteEdgeType4(pair<vector<int>,vector<int>>& edge){
    vector<int> Face=edge.first;
    vector<int> Face1=edge.second;
    color face1Color=this->cube.locationToColor(Face1);
    while(this->cube.locationToColor({5,1,2})!=color::White||this->cube.locationToColor({2,2,1})!=face1Color){
        this->cube.D(0);
        if(whiteCrossVerboseMode){
            cout<<"D ";
            this->cube.printCube();
        }
    }
    if(face1Color==color::Orange){
        this->cube.D(0);
        this->cube.F(0,3);
        this->cube.F(0,3);
        if(whiteCrossVerboseMode){
            cout<<"D F(0,3) F(0,3) ";
            this->cube.printCube();
        }
    }
    else if(face1Color==color::Red){
        this->cube.Dprime(0);
        this->cube.F(0,1);
        this->cube.F(0,1);
        if(whiteCrossVerboseMode){
            cout<<"Dprime F F ";
            this->cube.printCube();
        }
    }
    else if(face1Color==color::Blue){
        this->cube.D(0);
        this->cube.D(0);
        this->cube.F(0,4);
        this->cube.F(0,4);
        if(whiteCrossVerboseMode){
            cout<<"D D F(0,4) F(0,4) ";
            this->cube.printCube();
        }
    }
    else{ //Green
        this->cube.F(0,2);
        this->cube.F(0,2);
        if(whiteCrossVerboseMode){
            cout<<"F(0,2) F(0,2) ";
            this->cube.printCube();
        }
    }
    if(whiteCrossVerboseMode) cout<<endl;
}

void CubeSolver::whiteEdgeType1(pair<vector<int>,vector<int>>& edge){
    vector<int> Face=edge.first;
    vector<int> Face1=edge.second;
    color face1Color=this->cube.locationToColor(Face1);
    int faceMoved=0;
    while(this->cube.cube3DArray[0][1][2]!=color::White || this->cube.cube3DArray[2][0][1]!=face1Color){
        this->cube.U(0);
        faceMoved++;
        if(whiteCrossVerboseMode){
            cout<<"U ";
            this->cube.printCube();
        }
    }
    this->cube.R(0,1);
    this->cube.R(0,1);
    if(whiteCrossVerboseMode){
        cout<<"R R ";
        this->cube.printCube();
    }
    for(int i=0;i<faceMoved;i++){
        this->cube.Uprime(0);
        if(whiteCrossVerboseMode){
            cout<<"W ";
            this->cube.printCube();
        }
    }
    if(whiteCrossVerboseMode){
        cout<<endl;
    }
    pair<vector<int>,vector<int>> edge={{5,1,2},{2,2,1}};
    whiteEdgeType4(edge);
}

void CubeSolver::whiteEdgeType3(pair<vector<int>,vector<int>>& edge){
    vector<int> Face=edge.first;
    vector<int> Face1=edge.second;
    color face1Color=this->cube.locationToColor(Face1);
    int faceMoved=0;
    //this->MiddleEdges={{{4,1,2},{1,1,0}},{{1,1,2},{2,1,0}},{{2,1,2},{3,1,0}},{{3,1,2},{4,1,0}}};
    if(Face==MiddleEdges[0].first){ //{4,1,2}->{0,2,1},Fprime(0,1)
        while(this->cube.locationToColor({0,2,1})==color::White){
            this->cube.U(0);
            if(whiteCrossVerboseMode){
                cout<<"U "<<endl;
                this->cube.printCube();
            }
            faceMoved++;
        }
        this->cube.Fprime(0,1);
        if(whiteCrossVerboseMode){
            cout<<"Fprime "<<endl;
        }
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            if(whiteCrossVerboseMode){
                cout<<"Uprime "<<endl;
            }
        }
        pair<vector<int>,vector<int>> edge={{5,0,1},{1,2,1}};
        whiteEdgeType4(edge);
    }
    else if(Face==MiddleEdges[0].second){ //{1,1,0}->{0,1,0},L(0,1)
        while(this->cube.locationToColor({0,1,0})==color::White){
            this->cube.U(0);
            if(whiteCrossVerboseMode){
                cout<<"U "<<endl;
                this->cube.printCube();
            }
            faceMoved++;
        }
        this->cube.L(0,1);
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            if(whiteCrossVerboseMode){
                cout<<"Uprime "<<endl;
                this->cube.printCube();
            }
        }
        pair<vector<int>,vector<int>> edge={{5,1,0},{4,2,1}};
        whiteEdgeType4(edge);
    }
    else if(Face==MiddleEdges[1].first){ //{1,1,2}->{0,1,2},Rprime(0,1)
        while(this->cube.locationToColor({0,1,2})==color::White){
            this->cube.U(0);
            if(whiteCrossVerboseMode){
                cout<<"U "<<endl;
            }
            faceMoved++;
        }
        this->cube.Rprime(0,1);
        if(whiteCrossVerboseMode){
            cout<<"Rprime "<<endl;
        }
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            if(whiteCrossVerboseMode){
                cout<<"Uprime "<<endl;
                this->cube.printCube();
            }
        }
        pair<vector<int>,vector<int>> edge={{5,1,2},{2,2,1}};
        whiteEdgeType4(edge);
    }
    else if(Face==MiddleEdges[1].second){ //{2,1,0}->{0,2,1},F(0,1)
        while(this->cube.locationToColor({0,2,1})==color::White){
            this->cube.U(0);
            if(whiteCrossVerboseMode){
                cout<<"U "<<endl;
            }
            faceMoved++;
        }
        this->cube.F(0,1);
        if(whiteCrossVerboseMode){
            cout<<"F "<<endl;
        }
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            if(whiteCrossVerboseMode){
                cout<<"Uprime "<<endl;
            }
        }
        if(whiteCrossVerboseMode){
            cube.printCube();
        }
        pair<vector<int>,vector<int>> edge={{5,0,1},{1,2,1}};
        whiteEdgeType4(edge);
    }
    else if(Face==MiddleEdges[2].first){ //{2,1,2}->{0,0,1},Rprime(0,2)
        while(this->cube.locationToColor({0,0,1})==color::White){
            if(whiteCrossVerboseMode){
                cout<<"U"<<endl;
            }
            this->cube.U(0);
            faceMoved++;
        }
        if(whiteCrossVerboseMode){
            cout<<"Rprime"<<endl;
        }
        this->cube.Rprime(0,2);
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            if(whiteCrossVerboseMode){
                cout<<"Uprime "<<endl;
            }
        }
        if(whiteCrossVerboseMode){
            cube.printCube();
        }
        pair<vector<int>,vector<int>> edge={{5,2,1},{3,2,1}};
        whiteEdgeType4(edge);
    }
    else if(Face==MiddleEdges[2].second){ //{3,1,0}->{0,1,2},R(0,1)
        while(this->cube.locationToColor({0,1,2})==color::White){
            this->cube.U(0);
            if(whiteCrossVerboseMode){
                cout<<"U"<<endl;
            }
            faceMoved++;
        }
        this->cube.R(0,1);
        if(whiteCrossVerboseMode){
            cout<<"R"<<endl;
        }
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            if(whiteCrossVerboseMode){
                cout<<"Uprime"<<endl;
            }
        }
        if(whiteCrossVerboseMode){
            cube.printCube();
        }
        pair<vector<int>,vector<int>> edge={{5,1,2},{2,2,1}};
        whiteEdgeType4(edge);
    }
    else if(Face==MiddleEdges[3].first){ //{3,1,2}->{0,1,0},Lprime(0,1)
        while(this->cube.locationToColor({0,1,0})==color::White){
            this->cube.U(0);
            if(whiteCrossVerboseMode){
                cout<<"U"<<endl;
            }
            faceMoved++;
        }
        this->cube.Lprime(0,1);
        if(whiteCrossVerboseMode){
            cout<<"Lprime"<<endl;
        }
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            if(whiteCrossVerboseMode){
                cout<<"Uprime"<<endl;
            }
        }
        if(whiteCrossVerboseMode){
            cube.printCube();
        }
        pair<vector<int>,vector<int>> edge={{5,1,0},{4,2,1}};
        whiteEdgeType4(edge);
    }
    else if(Face==MiddleEdges[3].second){ //{4,1,0}->{0,0,1},F(0,3)
        while(this->cube.locationToColor({0,0,1})==color::White){
            this->cube.U(0);
            if(whiteCrossVerboseMode){
                cout<<"U"<<endl;
            }
            faceMoved++;
        }
        this->cube.F(0,3);
        if(whiteCrossVerboseMode){
            cout<<"F(0,3)"<<endl;
        }
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            if(whiteCrossVerboseMode){
                cout<<"Uprime"<<endl;
            }
        }
        if(whiteCrossVerboseMode){
            cube.printCube();
        }
        pair<vector<int>,vector<int>> edge={{5,1,1},{3,2,1}};
        whiteEdgeType4(edge);
    }
}

void CubeSolver::whiteEdgeType2(pair<vector<int>,vector<int>>& edge){
    /*2 types:
    {1,0,1},{2,0,1},{3,0,1},{4,0,1}
    and 
    {1,2,1},{2,2,1},{3,2,1},{4,2,1}*/
    vector<int> Face=edge.first;
    vector<int> Face1=edge.second;
    color face1Color=this->cube.locationToColor(Face1);
    int faceMoved=0;
    if(Face[1]==0){
        while(this->cube.locationToColor({1,0,1})!=color::White||this->cube.locationToColor({0,2,1})!=face1Color){
            this->cube.U(0);
            if(whiteCrossVerboseMode){
                cout<<"U "<<endl;
                this->cube.printCube();
            }
            faceMoved++;
        }
        this->cube.F(0,1);
        if(whiteCrossVerboseMode){
            cout<<"F "<<endl;
        }
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            if(whiteCrossVerboseMode){
                cout<<"Uprime ";
            }
        }
        if(whiteCrossVerboseMode){
            this->cube.printCube();
        }
        pair<vector<int>,vector<int>> edge={{1,1,2},{2,1,0}};
        whiteEdgeType3(edge);
    }
    else if(Face[1]==2){
        while(this->cube.locationToColor({1,2,1})!=color::White||this->cube.locationToColor({5,0,1})!=face1Color){
            this->cube.D(0);
            if(whiteCrossVerboseMode){
                cout<<"D "<<endl;
                this->cube.printCube();
            }
        }
        while(this->cube.locationToColor({0,2,1})==color::White){
            this->cube.U(0);
            if(whiteCrossVerboseMode){
                cout<<"D "<<endl;
                this->cube.printCube();
            }
            faceMoved++;
        }
        this->cube.Fprime(0,1);
        if(whiteCrossVerboseMode){
            cout<<"Fprime "<<endl;
            this->cube.printCube();
        }
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            if(whiteCrossVerboseMode){
                cout<<"Uprime "<<endl;  
            }
        }
        if(whiteCrossVerboseMode){
            this->cube.printCube();
        }
        pair<vector<int>,vector<int>> edge={{1,1,2},{2,1,0}};
        whiteEdgeType3(edge);
    }
}

void CubeSolver::whiteCross(){
    for(int i=0;i<4;i++){
        pair<pair<vector<int>,vector<int>>,WhiteEdgeType> whiteEdgeInfo=findUnmatchedWhiteEdge();
        pair<vector<int>,vector<int>> edge=whiteEdgeInfo.first;
        WhiteEdgeType type=whiteEdgeInfo.second;
        if(type==WhiteEdgeType::type1){
            if(whiteCrossVerboseMode){
                cout<<"hi1"<<endl;
            }
            whiteEdgeType1(edge);
            if(whiteCrossVerboseMode){
                this->cube.printCube();
            }
        }
        else if(type==WhiteEdgeType::type2){
            if(whiteCrossVerboseMode){
                cout<<"hi1"<<endl;
            }
            whiteEdgeType2(edge);
            if(whiteCrossVerboseMode){
                this->cube.printCube();
            }
        }
        else if(type==WhiteEdgeType::type3){
            if(whiteCrossVerboseMode){
                cout<<"hi1"<<endl;
            }
            whiteEdgeType3(edge);
            if(whiteCrossVerboseMode){
                this->cube.printCube();
            }
        }
        else if(type==WhiteEdgeType::type4){
            if(whiteCrossVerboseMode){
                cout<<"hi1"<<endl;
            }
            whiteEdgeType4(edge);
            if(whiteCrossVerboseMode){
                this->cube.printCube();
            }
        }
        else{ //type==done
            continue;
        }
    }
}

pair<vector<vector<int>>,WhiteCornerType> CubeSolver::findUnmatchedWhiteCorner(){
    WhiteCornerType cornerType=WhiteCornerType::done;
    // type1 and type2
    for(pair<vector<vector<int>>,vector<color>> corner:whiteCorners){
        if(this->cube.locationToColor(corner.first[0])==color::White){
            color cornerFaceColor1=this->cube.locationToColor(corner.first[1]);
            color cornerFaceColor2=this->cube.locationToColor(corner.first[2]);
            color corner1CorrospondingCentreColor=this->cube.locationToColor(vector<int>{corner.first[1][0],1,1});
            color corner2CorrospondingCentreColor=this->cube.locationToColor(vector<int>{corner.first[2][0],1,1});
            color corner1CorrospondingEdgeColor=this->cube.locationToColor(vector<int>{corner.first[1][0],corner.first[1][1]+1,corner.first[1][2]});
            color corner2CorrospondingEdgeColor=this->cube.locationToColor(vector<int>{corner.first[2][0],corner.first[2][1]+1,corner.first[2][2]});
            if(cornerFaceColor1 == corner1CorrospondingCentreColor && corner1CorrospondingEdgeColor == corner1CorrospondingCentreColor &&
               cornerFaceColor2 == corner2CorrospondingCentreColor && corner2CorrospondingEdgeColor == corner2CorrospondingCentreColor){
                continue;
               }
            return {corner.first,WhiteCornerType::type1};
        }
        else if(this->cube.locationToColor(corner.first[1])==color::White || this->cube.locationToColor(corner.first[2])==color::White){
            return {corner.first,WhiteCornerType::type2};
        }
    }
    //type3 and type4
    for(pair<vector<vector<int>>,vector<color>> corner:yellowCorners){
        if(this->cube.locationToColor(corner.first[0])==color::White){
            return {corner.first,WhiteCornerType::type3};
        }
        else if(this->cube.locationToColor(corner.first[1])==color::White || this->cube.locationToColor(corner.first[2])==color::White){
            return {corner.first,WhiteCornerType::type4};
        }
    }
    return {{{0,0,0},{0,0,0},{0,0,0}},WhiteCornerType::done};
}

void CubeSolver::moveCorner(int initial,int final){ // initial is initial left side centre f2lNumbering and final is also calculated in similar way
    if(final-initial == 1 || final-initial == -3) this->cube.Uprime(5);
    else if(final-initial == 2 || final-initial == -2){
        this->cube.U(5);
        this->cube.U(5);
    }
    else if(final-initial == -1 || final-initial == 3) this->cube.U(5);
}

int CubeSolver::cornerToF2lRepresentative(vector<vector<int>>& corner){
    pair<color,color> centrePairs;
    color color1=this->cube.locationToColor(vector<int>{corner[1][0],1,1});
    color color2=this->cube.locationToColor(vector<int>{corner[2][0],1,1});
    centrePairs={color1,color2};
    if(centrePairs == pair<color,color>{color::Red,color::Green}) return f2lColorNumbering[color::Red];
    else if(centrePairs == pair<color,color>{color::Red,color::Blue}) return f2lColorNumbering[color::Blue];
    else if(centrePairs == pair<color,color>{color::Blue,color::Orange}) return f2lColorNumbering[color::Orange];
    else if(centrePairs == pair<color,color>{color::Orange,color::Green}) return f2lColorNumbering[color::Green];
    return 0;
}

color CubeSolver::cornerToF2lRepresentative(vector<vector<int>>& corner,bool redundantArg){
    pair<color,color> centrePairs;
    color color1=this->cube.locationToColor(vector<int>{corner[1][0],1,1});
    color color2=this->cube.locationToColor(vector<int>{corner[2][0],1,1});
    centrePairs={color1,color2};
    if(centrePairs == pair<color,color>{color::Red,color::Green}) return color::Red;
    else if(centrePairs == pair<color,color>{color::Red,color::Blue}) return color::Blue;
    else if(centrePairs == pair<color,color>{color::Blue,color::Orange}) return color::Orange;
    else if(centrePairs == pair<color,color>{color::Orange,color::Green}) return color::Green;
    return color::White;
}

vector<vector<int>> CubeSolver::findUnfilledF2lSlot(){
    for(pair<vector<vector<int>>, vector<color>> corner:whiteCorners){
        color cornerFaceColor1=this->cube.locationToColor(corner.first[1]);
        color cornerFaceColor2=this->cube.locationToColor(corner.first[2]);
        color corner1CorrospondingCentreColor=this->cube.locationToColor(vector<int>{corner.first[1][0],1,1});
        color corner2CorrospondingCentreColor=this->cube.locationToColor(vector<int>{corner.first[2][0],1,1});
        color corner1CorrospondingEdgeColor=this->cube.locationToColor(vector<int>{corner.first[1][0],corner.first[1][1]+1,corner.first[1][2]});
        color corner2CorrospondingEdgeColor=this->cube.locationToColor(vector<int>{corner.first[2][0],corner.first[2][1]+1,corner.first[2][2]});
        if(this->cube.locationToColor(corner.first[0]) == color::White && cornerFaceColor1 == corner1CorrospondingCentreColor 
            && corner1CorrospondingEdgeColor == corner1CorrospondingCentreColor && cornerFaceColor2 == corner2CorrospondingCentreColor
            && corner2CorrospondingEdgeColor == corner2CorrospondingCentreColor) continue;
        else return corner.first;
    }
    return {{0,0,0},{0,0,0},{0,0,0}};
}

void CubeSolver::whiteCornerType3(vector<vector<int>>& corner){
    vector<vector<int>> unfilledF2lCornerSlot=findUnfilledF2lSlot();
    int unfilledF2lSlotRepresentative=cornerToF2lRepresentative(unfilledF2lCornerSlot);
    int initialCornerRepresentative=cornerToF2lRepresentative(corner);
    moveCorner(initialCornerRepresentative,unfilledF2lSlotRepresentative);
    // converting from type3 -> type4
    int top=5;
    int front=this->cube.colorToNumbering[cornerToF2lRepresentative(corner,true)];
    this->cube.R(top,front);
    this->cube.U(top);
    this->cube.U(top);
    this->cube.Rprime(top,front);
}

void CubeSolver::whiteCornerType2(vector<vector<int>>& corner){
    int top=5;
    int front=this->cube.colorToNumbering[cornerToF2lRepresentative(corner,true)];
    this->cube.R(top,front);
    this->cube.U(top);
    this->cube.Rprime(top,front);
}

void CubeSolver::whiteCornerType1(vector<vector<int>>& corner){
    //solved same way as type2 corner
    whiteCornerType2(corner);
}

Type4WhiteCornerSubType CubeSolver::findType4WhiteCornerSubType(vector<vector<int>>& corner){
    for(int i=1;i<=2;i++){
        if(this->cube.locationToColor(corner[i])==color::White){
            if(corner[i][2]==0) return Type4WhiteCornerSubType::left;
            else if(corner[i][2]==2) return Type4WhiteCornerSubType::right;
        }
    }
}

tuple<vector<vector<int>>,pair<vector<int>,vector<int>>,F2lEdgeType,Type4WhiteCornerSubType> CubeSolver::findF2lEdge(vector<vector<int>>& corner){
    for(pair<vector<int>,vector<int>> edge:MiddleEdges){

    }
}