#pragma once

#include <iostream>


class Cube
{
private:
    //numbers are in the order of alphabetical memorization scheme for that face
    static constexpr int TOPFACE[4]     = {0, 1, 2, 3};
    static constexpr int LEFTFACE[4]    = {0, 3, 7, 4};
    static constexpr int FRONTFACE[4]   = {3, 2, 7, 6};
    static constexpr int RIGHTFACE[4]   = {2, 1, 5, 6};
    static constexpr int BACKFACE[4]    = {1, 0, 4, 5};
    static constexpr int BOTTOMFACE[4]  = {7, 6, 5, 4};

    struct Piece
    {
        char topbottom;
        char leftright;
        char frontback;
    };
    Piece pieces[8];

public:
    Cube();
    Cube(const Cube&);

    Cube& makeFromInput();
    Cube& rotateZ();
    Cube& rotateY();
    Cube& rotateYPrime();
};

Cube::Cube(const Cube& c)
{
    for (int i = 0; i < 8; ++i) pieces[i] = c.pieces[i];
}

//TODO assumes good input
Cube& Cube::makeFromInput()
{
    for (int i : TOPFACE)       std::cin >> pieces[i].topbottom;
    for (int i : LEFTFACE)      std::cin >> pieces[i].leftright;
    for (int i : FRONTFACE)     std::cin >> pieces[i].frontback;
    for (int i : RIGHTFACE)     std::cin >> pieces[i].leftright;
    for (int i : BACKFACE)      std::cin >> pieces[i].frontback;
    for (int i : BOTTOMFACE)    std::cin >> pieces[i].topbottom;

    return *this;
}

Cube& Cube::rotateZ()
{
    const char topColor = pieces[0].topbottom;
    for (int i : TOPFACE)       pieces[i].topbottom = pieces[0].leftright;
    for (int i : LEFTFACE)      pieces[i].leftright = pieces[4].topbottom;
    for (int i : BOTTOMFACE)    pieces[i].topbottom = pieces[1].leftright;
    for (int i : RIGHTFACE)     pieces[i].leftright = topColor;

    return *this;
}

Cube& Cube::rotateY()
{
    const char frontColor = pieces[2].frontback;
    for (int i : FRONTFACE)     pieces[i].frontback = pieces[1].leftright;
    for (int i : RIGHTFACE)     pieces[i].leftright = pieces[0].frontback;
    for (int i : BACKFACE)      pieces[i].frontback = pieces[0].leftright;
    for (int i : LEFTFACE)      pieces[i].leftright = frontColor;

    return *this;
}

Cube& Cube::rotateYPrime()
{
    const char frontColor = pieces[2].frontback;
    for (int i : FRONTFACE)     pieces[i].frontback = pieces[0].leftright;
    for (int i : LEFTFACE)      pieces[i].leftright = pieces[0].frontback;
    for (int i : BACKFACE)      pieces[i].frontback = pieces[1].leftright;
    for (int i : RIGHTFACE)     pieces[i].leftright = frontColor;

    return *this;
}