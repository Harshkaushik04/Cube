#include<iostream>
#include "headers/cube_solving.hpp"
using namespace std;

//scramble R2UB2R'L'U2RD'FB' white as top and red as front

CubeSolver::CubeSolver(Cube cube){
    this->cube=cube;
    // vector of 4 edges{pair{pair{edge face1,edge face2},color}}
    this->WhiteEdges={{{{0,1,2},{2,0,1}},color::Green},{{{0,2,1},{1,0,1}},color::Red},{{{0,1,0},{4,0,1}},color::Blue},{{{0,0,1},{3,0,1}},color::Orange}};
    this->YellowEdges={{{{5,1,2},{2,2,1}},color::Green},{{{5,2,1},{3,2,1}},color::Orange},{{{5,1,0},{4,2,1}},color::Blue},{{{5,0,1},{1,2,1}},color::Red}};
    //vector of 4 edges<pair<edge face1,edge face2>>
    this->MiddleEdges={{{4,1,2},{1,1,0}},{{1,1,2},{2,1,0}},{{2,1,2},{3,1,0}},{{3,1,2},{4,1,0}}};
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

void CubeSolver::whiteEdgeType4(pair<vector<int>,vector<int>> edge){
    vector<int> Face=edge.first;
    vector<int> Face1=edge.second;
    color face1Color=this->cube.locationToColor(Face1);
    while(this->cube.locationToColor({5,1,2})!=color::White||this->cube.locationToColor({2,2,1})!=face1Color){
        this->cube.D(0);
        cout<<"D ";
        this->cube.printCube();
    }
    if(face1Color==color::Orange){
        this->cube.D(0);
        this->cube.F(0,3);
        this->cube.F(0,3);
        cout<<"D F(0,3) F(0,3) ";
        this->cube.printCube();
    }
    else if(face1Color==color::Red){
        this->cube.Dprime(0);
        this->cube.F(0,1);
        this->cube.F(0,1);
        cout<<"Dprime F F ";
        this->cube.printCube();
    }
    else if(face1Color==color::Blue){
        this->cube.D(0);
        this->cube.D(0);
        this->cube.F(0,4);
        this->cube.F(0,4);
        cout<<"D D F(0,4) F(0,4) ";
        this->cube.printCube();
    }
    else{ //Green
        this->cube.F(0,2);
        this->cube.F(0,2);
        cout<<"F(0,2) F(0,2) ";
        this->cube.printCube();
    }
    cout<<endl;
}

void CubeSolver::whiteEdgeType1(pair<vector<int>,vector<int>> edge){
    vector<int> Face=edge.first;
    vector<int> Face1=edge.second;
    color face1Color=this->cube.locationToColor(Face1);
    int faceMoved=0;
    while(this->cube.cube3DArray[0][1][2]!=color::White || this->cube.cube3DArray[2][0][1]!=face1Color){
        this->cube.U(0);
        faceMoved++;
        cout<<"U ";
        this->cube.printCube();
    }
    this->cube.R(0,1);
    this->cube.R(0,1);
    cout<<"R R ";
    this->cube.printCube();
    for(int i=0;i<faceMoved;i++){
        this->cube.Uprime(0);
        cout<<"W ";
        this->cube.printCube();
    }
    cout<<endl;
    whiteEdgeType4({{5,1,2},{2,2,1}});
}

void CubeSolver::whiteEdgeType3(pair<vector<int>,vector<int>> edge){
    vector<int> Face=edge.first;
    vector<int> Face1=edge.second;
    color face1Color=this->cube.locationToColor(Face1);
    int faceMoved=0;
    //this->MiddleEdges={{{4,1,2},{1,1,0}},{{1,1,2},{2,1,0}},{{2,1,2},{3,1,0}},{{3,1,2},{4,1,0}}};
    if(Face==MiddleEdges[0].first){ //{4,1,2}->{0,2,1},Fprime(0,1)
        while(this->cube.locationToColor({0,2,1})==color::White){
            this->cube.U(0);
            cout<<"U "<<endl;
            this->cube.printCube();
            faceMoved++;
        }
        this->cube.Fprime(0,1);
        cout<<"Fprime "<<endl;
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            cout<<"Uprime "<<endl;
        }
        whiteEdgeType4({{5,0,1},{1,2,1}});
    }
    else if(Face==MiddleEdges[0].second){ //{1,1,0}->{0,1,0},L(0,1)
        while(this->cube.locationToColor({0,1,0})==color::White){
            this->cube.U(0);
            cout<<"U "<<endl;
            this->cube.printCube();
            faceMoved++;
        }
        this->cube.L(0,1);
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            cout<<"Uprime "<<endl;
            this->cube.printCube();
        }
        whiteEdgeType4({{5,1,0},{4,2,1}});
    }
    else if(Face==MiddleEdges[1].first){ //{1,1,2}->{0,1,2},Rprime(0,1)
        while(this->cube.locationToColor({0,1,2})==color::White){
            this->cube.U(0);
            cout<<"U "<<endl;
            faceMoved++;
        }
        this->cube.Rprime(0,1);
        cout<<"Rprime "<<endl;
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            cout<<"Uprime "<<endl;
            this->cube.printCube();
        }
        whiteEdgeType4({{5,1,2},{2,2,1}});
    }
    else if(Face==MiddleEdges[1].second){ //{2,1,0}->{0,2,1},F(0,1)
        while(this->cube.locationToColor({0,2,1})==color::White){
            this->cube.U(0);
            faceMoved++;
        }
        this->cube.F(0,1);
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
        }
        whiteEdgeType4({{5,0,1},{1,2,1}});
    }
    else if(Face==MiddleEdges[2].first){ //{2,1,2}->{0,0,1},Rprime(0,2)
        while(this->cube.locationToColor({0,0,1})==color::White){
            this->cube.U(0);
            faceMoved++;
        }
        this->cube.Rprime(0,2);
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
        }
        whiteEdgeType4({{5,2,1},{3,2,1}});
    }
    else if(Face==MiddleEdges[2].second){ //{3,1,0}->{0,1,2},R(0,1)
        while(this->cube.locationToColor({0,1,2})==color::White){
            this->cube.U(0);
            faceMoved++;
        }
        this->cube.R(0,1);
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
        }
        whiteEdgeType4({{5,1,2},{2,2,1}});
    }
    else if(Face==MiddleEdges[3].first){ //{3,1,2}->{0,1,0},Lprime(0,1)
        while(this->cube.locationToColor({0,1,0})==color::White){
            this->cube.U(0);
            faceMoved++;
        }
        this->cube.Lprime(0,1);
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
        }
        whiteEdgeType4({{5,1,0},{4,2,1}});
    }
    else if(Face==MiddleEdges[3].second){ //{4,1,0}->{0,0,1},F(0,3)
        while(this->cube.locationToColor({0,0,1})==color::White){
            this->cube.U(0);
            faceMoved++;
        }
        this->cube.F(0,3);
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
        }
        whiteEdgeType4({{5,1,1},{3,2,1}});
    }
}

void CubeSolver::whiteEdgeType2(pair<vector<int>,vector<int>> edge){
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
            cout<<"U "<<endl;
            this->cube.printCube();
            faceMoved++;
        }
        this->cube.F(0,1);
        cout<<"F "<<endl;
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            cout<<"Uprime ";
        }
        this->cube.printCube();
        whiteEdgeType3({{1,1,2},{2,1,0}});
    }
    else if(Face[1]==2){
        while(this->cube.locationToColor({1,2,1})!=color::White||this->cube.locationToColor({5,0,1})!=face1Color){
            this->cube.D(0);
            cout<<"D "<<endl;
            this->cube.printCube();
        }
        while(this->cube.locationToColor({0,2,1})==color::White){
            this->cube.U(0);
            cout<<"D "<<endl;
            this->cube.printCube();
            faceMoved++;
        }
        this->cube.Fprime(0,1);
        cout<<"Fprime "<<endl;
        this->cube.printCube();
        for(int i=0;i<faceMoved;i++){
            this->cube.Uprime(0);
            cout<<"Uprime "<<endl;
        }
        this->cube.printCube();
        whiteEdgeType3({{1,1,2},{2,1,0}});
    }
}

void CubeSolver::whiteCross(){
    for(int i=0;i<4;i++){
        pair<pair<vector<int>,vector<int>>,WhiteEdgeType> whiteEdgeInfo=findUnmatchedWhiteEdge();
        pair<vector<int>,vector<int>> edge=whiteEdgeInfo.first;
        WhiteEdgeType type=whiteEdgeInfo.second;
        if(type==WhiteEdgeType::type1){
            cout<<"hi1"<<endl;
            whiteEdgeType1(edge);
            this->cube.printCube();
        }
        else if(type==WhiteEdgeType::type2){
            cout<<"hi2"<<endl;
            whiteEdgeType2(edge);
            this->cube.printCube();
        }
        else if(type==WhiteEdgeType::type3){
            cout<<"hi3"<<endl;
            whiteEdgeType3(edge);
            this->cube.printCube();
        }
        else if(type==WhiteEdgeType::type4){
            cout<<"hi4"<<endl;
            whiteEdgeType4(edge);
            this->cube.printCube();
        }
        else{ //type==done
            continue;
        }
    }
}