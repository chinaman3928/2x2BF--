#pragma once

#include <vector>
#include <string>
#include <iostream>

class Solver
{
private:
    static constexpr int LR = 0;
    static constexpr int FB = 1;
    static constexpr int TB = 2;

    static constexpr char LETTERS[8][3] = { {'E','R','A'}, {'N','Q','B'}, {'F','I','D'}, {'M','J','C'}, {'H','S','X'}, {'O','T','W'}, {'G','L','U'}, {'P','K','V'} };

    static constexpr int ZMAP[8] =      {1, 5, 3, 7, 0, 4, 2, 6};
    static constexpr int YMAP[8] =      {1, 3, 0, 2, 5, 7, 4, 6};
    static constexpr int YPRIMEMAP[8] = {2, 0, 3, 1, 6, 4, 7, 5};

    struct TreeNode
    {
        std::string s;
        std::vector<TreeNode*> vec;
        TreeNode() {}
        TreeNode(char c) : s{c} {}
    };

    //TODO destructor for TreeNodes
    struct AnchorEntry
    {
        int anchor;
        char set;

        TreeNode* solutions[8][3];
        AnchorEntry(int anchor, char set) : anchor{anchor}, set{set}, solutions() {}
    };

    struct BufferPiece
    {
        char colors[3];
        char swapper;

        void operator= (const BufferPiece& other)
        {
            colors[0] = other.colors[0];
            colors[1] = other.colors[1];
            colors[2] = other.colors[2];
            swapper = other.swapper;
        }
    };

    struct NodePrintEntry
    {
        TreeNode* tn;
        bool done;

        NodePrintEntry(TreeNode* tn) : tn{tn}, done{false} {}
    };

    char (&c)[8][3];
    std::vector<AnchorEntry> anchorEntries;

    bool solvedRelTo(int i, int j);
    void rotateZ();
    void rotateY();
    void rotateYPrime();
    void solveThisOrientationForEachAnchor();
    void solveThisOrientationAnchorDepthFirst(const int anchor, const int newCycleIf, TreeNode* sol, char solved, BufferPiece bufferPiece);
    void computeNextStep(const int anchor, const int newCycleIf, const BufferPiece& bufferPiece, int& whereBelongs, BufferPiece& nextBufferPiece, char& letterToAppend);
    void newCycle(const int anchor, TreeNode* spawner, char solved);
    void printTree(std::vector<NodePrintEntry>& stack);

public:
    Solver(char (&c)[8][3]);
    void solve();
    void print();
};


//TODO i think i will have the cube population in this constructor? maybe
Solver::Solver(char (&c)[8][3])
    : c{c}
{
    bool open[8] = {true, true, true, true, true, true, true, true};
    for (int i = 0; i < 8; ++i)
    {
        if (open[i])
        {
            anchorEntries.emplace_back(i, 1 << i);
            for (int j = i + 1; j < 8; ++j)
            {
                if (open[j] && solvedRelTo(i, j))
                {
                    anchorEntries.back().set |= (1 << j);
                    open[j] = false;
                }
            }
            open[i] = false;
        }
    }
}


void Solver::solve()
{
    solveThisOrientationForEachAnchor();
    for (int i = 0; i < 3; ++i) {rotateZ(); solveThisOrientationForEachAnchor();}

    rotateY();
    solveThisOrientationForEachAnchor();
    for (int i = 0; i < 3; ++i) {rotateZ(); solveThisOrientationForEachAnchor();}

    rotateY();
    solveThisOrientationForEachAnchor();
    for (int i = 0; i < 3; ++i) {rotateZ(); solveThisOrientationForEachAnchor();}

    rotateYPrime();
    solveThisOrientationForEachAnchor();
    for (int i = 0; i < 3; ++i) {rotateZ(); solveThisOrientationForEachAnchor();}

    rotateYPrime();
    solveThisOrientationForEachAnchor();
    for (int i = 0; i < 3; ++i) {rotateZ(); solveThisOrientationForEachAnchor();}

    rotateY();
    solveThisOrientationForEachAnchor();
    for (int i = 0; i < 3; ++i) {rotateZ(); solveThisOrientationForEachAnchor();}
}

bool Solver::solvedRelTo(int i, int j)
{
    return ( (c[j][LR] ^ ((i >> LR & 1) != (j >> LR & 1))) == c[i][LR] ) &&
           ( (c[j][FB] ^ ((i >> FB & 1) != (j >> FB & 1))) == c[i][FB] ) &&
           ( (c[j][TB] ^ ((i >> TB & 1) != (j >> TB & 1))) == c[i][TB] );
}


void Solver::rotateZ()
{
    //front
    const char old2Front = c[2][FB];
    c[2][FB] = c[6][FB];
    c[6][FB] = c[7][FB];
    c[7][FB] = c[3][FB];
    c[3][FB] = old2Front;

    //back
    const char old0Back = c[0][FB];
    c[0][FB] = c[4][FB];
    c[4][FB] = c[5][FB];
    c[5][FB] = c[1][FB];
    c[1][FB] = old0Back; 

    //the ring
    const char old0Top = c[0][TB];
    const char old2Top = c[2][TB];
    const char old1Top = c[1][TB];
    const char old3Top = c[3][TB];

    c[0][TB] = c[4][LR];
    c[2][TB] = c[6][LR];
    c[1][TB] = c[0][LR];
    c[3][TB] = c[2][LR];

    c[4][LR] = c[5][TB];
    c[6][LR] = c[7][TB];
    c[0][LR] = c[4][TB];
    c[2][LR] = c[6][TB];

    c[5][TB] = c[1][LR];
    c[7][TB] = c[3][LR];
    c[4][TB] = c[5][LR];
    c[6][TB] = c[7][LR];

    c[1][LR] = old0Top;
    c[3][LR] = old2Top;
    c[5][LR] = old1Top;
    c[7][LR] = old3Top;

    //anchors
    for (auto& ae : anchorEntries)
    {
        ae.anchor = ZMAP[ae.anchor];
        char newSet = 0;
        for (int i = 0; i < 8; ++i)
            newSet |= ((ae.set >> i & 1) << ZMAP[i]);
        ae.set = newSet;
    }
}


void Solver::rotateY()
{
    //top
    const char old0Top = c[0][TB];
    c[0][TB] = c[2][TB];
    c[2][TB] = c[3][TB];
    c[3][TB] = c[1][TB];
    c[1][TB] = old0Top;

    //bottom
    const char old6Bottom = c[6][TB];
    c[6][TB] = c[7][TB];
    c[7][TB] = c[5][TB];
    c[5][TB] = c[4][TB];
    c[4][TB] = old6Bottom;

    //the ring
    const char old2Front = c[2][FB];
    const char old6Front = c[6][FB];
    const char old3Front = c[3][FB];
    const char old7Front = c[7][FB];

    c[2][FB] = c[3][LR];
    c[6][FB] = c[7][LR];
    c[3][FB] = c[1][LR];
    c[7][FB] = c[5][LR];

    c[3][LR] = c[1][FB];
    c[7][LR] = c[5][FB];
    c[1][LR] = c[0][FB];
    c[5][LR] = c[4][FB];

    c[1][FB] = c[0][LR];
    c[5][FB] = c[4][LR];
    c[0][FB] = c[2][LR];
    c[4][FB] = c[6][LR];

    c[0][LR] = old2Front;
    c[4][LR] = old6Front;
    c[2][LR] = old3Front;
    c[6][LR] = old7Front;

    //anchors
    for (auto& ae : anchorEntries)
    {
        ae.anchor = YMAP[ae.anchor];
        char newSet = 0;
        for (int i = 0; i < 8; ++i)
            newSet |= ((ae.set >> i & 1) << YMAP[i]);
        ae.set = newSet;
    }
}


void Solver::rotateYPrime()
{
    //top
    const char old0Top = c[0][TB];
    c[0][TB] = c[1][TB];
    c[1][TB] = c[3][TB];
    c[3][TB] = c[2][TB];
    c[2][TB] = old0Top;

    //bottom
    const char old4Bottom = c[4][TB];
    c[4][TB] = c[5][TB];
    c[5][TB] = c[7][TB];
    c[7][TB] = c[6][TB];
    c[6][TB] = old4Bottom;

    //the ring
    const char old2Front = c[2][FB];
    const char old6Front = c[6][FB];
    const char old3Front = c[3][FB];
    const char old7Front = c[7][FB];

    c[2][FB] = c[0][LR];
    c[6][FB] = c[4][LR];
    c[3][FB] = c[2][LR];
    c[7][FB] = c[6][LR];

    c[0][LR] = c[1][FB];
    c[4][LR] = c[5][FB];
    c[2][LR] = c[0][FB];
    c[6][LR] = c[4][FB];

    c[1][FB] = c[3][LR];
    c[5][FB] = c[7][LR];
    c[0][FB] = c[1][LR];
    c[4][FB] = c[5][LR];

    c[3][LR] = old2Front;
    c[7][LR] = old6Front;
    c[1][LR] = old3Front;
    c[5][LR] = old7Front;

    //anchors
    for (auto& ae : anchorEntries)
    {
        ae.anchor = YPRIMEMAP[ae.anchor];
        char newSet = 0;
        for (int i = 0; i < 8; ++i)
            newSet |= ((ae.set >> i & 1) << YPRIMEMAP[i]);
        ae.set = newSet;
    }
}

    
void Solver::solveThisOrientationForEachAnchor()
{
    BufferPiece bufferPiece;
    bufferPiece.colors[c[0][LR] >> 1] = c[0][LR];
    bufferPiece.colors[c[0][FB] >> 1] = c[0][FB];
    bufferPiece.colors[c[0][TB] >> 1] = c[0][TB];
    bufferPiece.swapper = c[0][LR];

    for (AnchorEntry& ae : anchorEntries)
    {
        if (ae.set & 0b1) continue;

        int ori;
        if      (c[ae.anchor][LR] & 0b100) ori = LR;
        else if (c[ae.anchor][FB] & 0b100) ori = FB;
        else                               ori = TB;
        
        solveThisOrientationAnchorDepthFirst(ae.anchor, 0, ae.solutions[ae.anchor][ori] = new TreeNode(), ae.set, bufferPiece);
    }
}

void Solver::solveThisOrientationAnchorDepthFirst(const int anchor, const int newCycleIf, TreeNode* sol, char solved, BufferPiece bufferPiece)
{
    int whereBelongs;
    BufferPiece nextBufferPiece;
    char letterToAppend;
    while (solved & 0b11111110)
    {
        computeNextStep(anchor, newCycleIf, bufferPiece, whereBelongs, nextBufferPiece, letterToAppend);

        if (whereBelongs == 0) {newCycle(anchor, sol, solved); return;}
        sol->s.push_back(letterToAppend);
        solved |= (1 << whereBelongs);
        if (whereBelongs == newCycleIf) {newCycle(anchor, sol, solved); return;}
        bufferPiece = nextBufferPiece;
    }
}

//returns true iff no new cycle
//if new cycle, nextBufferPiece is garbage but whereBelongs and letterToAppend are still correctly used
void Solver::computeNextStep(const int anchor, const int newCycleIf, const BufferPiece& bufferPiece, int& whereBelongs, BufferPiece& nextBufferPiece, char& letterToAppend)
{
    const auto& an = c[anchor];
    whereBelongs = anchor;
    whereBelongs ^= (bufferPiece.colors[an[LR] >> 1] != an[LR]) << LR;
    whereBelongs ^= (bufferPiece.colors[an[FB] >> 1] != an[FB]) << FB;
    whereBelongs ^= (bufferPiece.colors[an[TB] >> 1] != an[TB]) << TB;

    const auto& next = c[whereBelongs];
    nextBufferPiece.colors[next[LR] >> 1] = next[LR];
    nextBufferPiece.colors[next[FB] >> 1] = next[FB];
    nextBufferPiece.colors[next[TB] >> 1] = next[TB];

    if      (bufferPiece.swapper >> 1 == an[LR] >> 1) {nextBufferPiece.swapper = next[LR]; letterToAppend = LETTERS[whereBelongs][LR];}
    else if (bufferPiece.swapper >> 1 == an[FB] >> 1) {nextBufferPiece.swapper = next[FB]; letterToAppend = LETTERS[whereBelongs][FB];}
    else                                              {nextBufferPiece.swapper = next[TB]; letterToAppend = LETTERS[whereBelongs][TB];}
}


void Solver::newCycle(const int anchor, TreeNode* spawner, char solved)
{
    for (int cycleStart = 1; cycleStart < 8; ++cycleStart)
    {
        if (!(solved >> cycleStart & 1))
        {
            BufferPiece newBufferPiece;
            newBufferPiece.colors[c[cycleStart][LR] >> 1] = c[cycleStart][LR];
            newBufferPiece.colors[c[cycleStart][FB] >> 1] = c[cycleStart][FB];
            newBufferPiece.colors[c[cycleStart][TB] >> 1] = c[cycleStart][TB];
            for (int face = 0; face < 3; ++face)
            {
                newBufferPiece.swapper = c[cycleStart][face];
                
                spawner->vec.emplace_back(new TreeNode(LETTERS[cycleStart][face])); //TODO this is not gonna have []
                solveThisOrientationAnchorDepthFirst(anchor, cycleStart, spawner->vec.back(), solved, newBufferPiece);
            }
        }
    }
}


void Solver::printTree(std::vector<NodePrintEntry>& stack)
{
    std::cout << stack.back().tn->s << std::endl;

    for (TreeNode* x : stack.back().tn->vec)
    {
        for (int i = 0; i < stack.size() - 1; ++i)
            std::cout << (stack[i].done ? "    " : "|   ");
        std::cout << "+---";
        stack.back().done = (x == stack.back().tn->vec.back());

        stack.emplace_back(x);
        printTree(stack);
    }
    //TODO if this loop never executes bc no children, then curr.done will never toggle to true
    
    stack.pop_back();
}


//assumes solve() has already been called
void Solver::print()
{
    int aeIndex = 0;
    for (const AnchorEntry& ae : anchorEntries)
    {
        std::cout << "anchor[" << aeIndex++ << "]" << std::endl;
        for (int piece = 0; piece < 8; ++piece)
        {
            std::cout << "in piece " << piece << std::endl;
            for (int ori = 0; ori < 3; ++ori)
            {
                std::cout << "White/Yellow facing " << ori << std::endl;

                //TODO nice to output which setpiece
                if (ae.solutions[piece][ori] == nullptr) std::cout << "no solution, there is a setpiece in the buffer location" << std::endl;
                else
                {
                    std::vector<NodePrintEntry> stack {{ae.solutions[piece][ori]}};
                    printTree(stack);
                }
            }
        }
    }
}

//big TODO list which may or may not already be mentioned
//  currently just says anchor[i], but i want it to output WY-BG-RO followed by set {ABC, DEF, GHI, ...}
//  better indent spacing
//  prettier writing for which piece and which side W/Y faces
//  per anchor, output stats (best worst, maybe even map)
//  per anchor+piece, output stats (best worst, maybe even map)
//  per anchor+piece+orientation, output stats
//  per leaf, output length (at braches its probably unecessary but good thought experiment to output best/worst at branches)
//  more info per branch (new cycle, what is being evicted into where) 