#pragma once

#include "cube.h"


class Solver
{
private:
    Cube& c;
    int anchor[8];

public:
    Solver(Cube& c);
    void exec();
};


Solver::Solver(Cube& c)
    : c{c}
{
    for (int i = 0; i < 8; ++i) anchor[i] = i;
    for (int i = 0; i < 8; ++i)
    {
        if (anchor[i] != i) continue;
        for (int j = i + 1; j < 8; ++j)
        {
            if (anchor[j] == j && solvedRelTo(i, j)) anchor[j] = i;
        }
    }
}


void Solver::exec()
{
    solveThisOrientationForEachAnchor();
    for (int i = 0; i < 3; ++i) solveThisOrientationForEachAnchor(c.rotateZ());
    c.rotateY();

    solveThisOrientationForEachAnchor();
    for (int i = 0; i < 3; ++i) solve(c.rotateZ());
    c.rotateY();

    solve(c);
    for (int i = 0; i < 3; ++i) solve(c.rotateZ());
    c.rotateYPrime();

    solve(c);
    for (int i = 0; i < 3; ++i) solve(c.rotateZ());
    c.rotateYPrime();

    solve(c);
    for (int i = 0; i < 3; ++i) solve(c.rotateZ());
    c.rotateYPrime();

    solve(c);
    for (int i = 0; i < 3; ++i) solve(c.rotateZ());
}


void Solver::solveAll(Cube& c)
{
    solve(c);
    for (int i = 0; i < 3; ++i) solve(c.rotateZ());
    c.rotateY();

    solve(c);
    for (int i = 0; i < 3; ++i) solve(c.rotateZ());
    c.rotateY();

    solve(c);
    for (int i = 0; i < 3; ++i) solve(c.rotateZ());
    c.rotateYPrime();

    solve(c);
    for (int i = 0; i < 3; ++i) solve(c.rotateZ());
    c.rotateYPrime();

    solve(c);
    for (int i = 0; i < 3; ++i) solve(c.rotateZ());
    c.rotateYPrime();

    solve(c);
    for (int i = 0; i < 3; ++i) solve(c.rotateZ());
}