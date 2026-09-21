#pragma once
#include "cube.hpp"
#include<vector>
#include<unordered_map>
#include<tuple>

enum class WhiteEdgeType{type1,type2,type3,type4,done}; // for WhiteCross
/*type1:on white face
type2:on middle faces with non trivial solution to type1
type3:on middle face with trivial solution to type1
type4:on yellow face
type4->done
type2->type3->type4->done
type1->type4->done
*/
enum class WhiteCornerType{type1,type2,type3,type4,done}; // for F2L
/*type1:aligned corner with white
type2:white corner in the last layer(touching white)
type3:white corner on yellow
type4:white corner on top layer(ideal)
*/
enum class Type4WhiteCornerSubType{left,right}; // for F2L
/*
left: if you see white face on the front side,yellow on top, then it is in left side of top layer
right: if you see white face on the front side,yellow on top, then it is in right side of top layer
*/
enum class F2lEdgeType{type1,type2,type3,type4,type5,type6,type7,type8,type9};
/*
type1: Aligned same side/done
type2: middle layer
type3: misaligned same side type1[White corner on same face as edge]
type4: misaligned same side type2(1 U move)
type5: misaligned same side type3(2 U move)
type6: Aligned diff side
type7: misaligned diff side type1[White corner on same face as edge]
type8: misaligned diff side type2(1 U move)
type9: misaligned diff side type3(2 U move)
*/
using F2lPairDetails=std::tuple<std::vector<std::vector<int>>,std::pair<std::vector<int>,std::vector<int>>,F2lEdgeType,Type4WhiteCornerSubType>;
class CubeSolver{
public:
    Cube cube;
    bool whiteCrossVerboseMode;
    bool f2lVerboseMode;
    std::vector<std::pair<std::pair<std::vector<int>,std::vector<int>>,color>> WhiteEdges; // vector of 4 edges{std::pair{std::pair{edge face1,edge face2},color}}
    std::vector<std::pair<std::pair<std::vector<int>,std::vector<int>>,color>> YellowEdges; // std::vector of 4 edges{std::pair{std::pair{edge face1,edge face2},color}}
    std::vector<std::pair<std::vector<int>,std::vector<int>>> MiddleEdges;
    std::vector<std::pair<std::vector<std::vector<int>>,std::vector<color>>> whiteCorners;
    std::vector<std::pair<std::vector<std::vector<int>>,std::vector<color>>> yellowCorners;
    std::unordered_map<color,int> f2lColorNumbering;
    CubeSolver(Cube);
    std::pair<std::pair<std::vector<int>,std::vector<int>>,WhiteEdgeType> findUnmatchedWhiteEdge();
    void whiteEdgeType1(std::pair<std::vector<int>,std::vector<int>>& edge);
    void whiteEdgeType2(std::pair<std::vector<int>,std::vector<int>>& edge);
    void whiteEdgeType3(std::pair<std::vector<int>,std::vector<int>>& edge);
    void whiteEdgeType4(std::pair<std::vector<int>,std::vector<int>>& edge);
    void whiteCross();
    std::pair<std::vector<std::vector<int>>,WhiteCornerType> findUnmatchedWhiteCorner();
    void moveCorner(int initial,int final);
    int cornerToF2lRepresentative(std::vector<std::vector<int>>& corner);
    color cornerToF2lRepresentative(std::vector<std::vector<int>>& corner,bool redundantArg);
    std::vector<std::vector<int>> findUnfilledF2lSlot();
    void whiteCornerType3(std::vector<std::vector<int>>& corner);
    void whiteCornerType2(std::vector<std::vector<int>>& corner);
    void whiteCornerType1(std::vector<std::vector<int>>& corner);
    Type4WhiteCornerSubType findType4WhiteCornerSubType(std::vector<std::vector<int>>& corner);
    // {corner,edge,edgeType,cornerType4EdgeSubType}
    F2lPairDetails findF2lEdge(std::vector<std::vector<int>>& corner);
    void convertMiddleEdgeToTopEdge(std::pair<std::vector<int>,std::vector<int>>& edge);
    std::vector<std::vector<int>> findUnfilledF2lSlot();
    void makeF2Lpair(F2lPairDetails&);
    void F2lPair();
    void F2L();
    void YellowCross();
    void OLL();
    void PLL();
};



