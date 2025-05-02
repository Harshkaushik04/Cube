#pragma once
#include "cube.hpp"
#include<vector>

enum class WhiteEdgeType{type1,type2,type3,type4,done};
/*type1:on white face
type2:on middle faces with non trivial solution to type1
type3:on middle face with trivial solution to type1
type4:on yellow face
type4->done
type2->type3->type4->done
type1->type4->done
*/
class CubeSolver{
public:
    Cube cube;
    bool whiteCrossVerboseMode;
    std::vector<std::pair<std::pair<std::vector<int>,std::vector<int>>,color>> WhiteEdges; // vector of 4 edges{std::pair{std::pair{edge face1,edge face2},color}}
    std::vector<std::pair<std::pair<std::vector<int>,std::vector<int>>,color>> YellowEdges; // std::vector of 4 edges{std::pair{std::pair{edge face1,edge face2},color}}
    std::vector<std::pair<std::vector<int>,std::vector<int>>> MiddleEdges;
    CubeSolver(Cube);
    std::pair<std::pair<std::vector<int>,std::vector<int>>,WhiteEdgeType> findUnmatchedWhiteEdge();
    void whiteEdgeType1(std::pair<std::vector<int>,std::vector<int>> edge);
    void whiteEdgeType2(std::pair<std::vector<int>,std::vector<int>> edge);
    void whiteEdgeType3(std::pair<std::vector<int>,std::vector<int>> edge);
    void whiteEdgeType4(std::pair<std::vector<int>,std::vector<int>> edge);
    void whiteCross();
    void findWhiteCorner();
    void findF2LEdge();
    void F2Lpair();
    void F2L();
    void YellowCross();
    void OLL();
    void PLL();
};



