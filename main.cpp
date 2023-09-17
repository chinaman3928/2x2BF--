#include "solve.h"

//TODO probably move this to class
static constexpr char R = 0b000;
static constexpr char O = 0b001;
static constexpr char B = 0b010;
static constexpr char G = 0b011;
static constexpr char W = 0b100;
static constexpr char Y = 0b101;


int main()
{
    //red top, green front, yellow right
    // R U F' R2 U' R2 U F'
    //to bring to white top, blue front, red left
    // F U' R2 U R2 F U' R'
    //to solve
    char c[8][3] = {{G,W,R}, {O,G,W}, {R,B,W}, {B,Y,R}, {W,B,O}, {Y,G,R}, {Y,B,O}, {O,Y,G}};

    Solver solver(c);
    solver.solve();
    solver.print();
}