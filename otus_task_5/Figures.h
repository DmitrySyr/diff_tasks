/*

File contains classes for graphical primitives.

*/

#pragma once

#include <vector>
#include <utility>

enum class Switcher{
    FirstClick, SecondClick
};


class IFigure{

public:
    IFigure() {}
    virtual ~IFigure(){}

    void HandClick( int x, int y );

    virtual std::vector< std::pair<int, int> > GetAllPoints() = 0;


private:

    Switcher switcher = Switcher::FirstClick;

    void SetCenter( int x, int y ) {}
    void SetTarget( int x, int y ) {}

    int CenterX, CenterY, TargetX, TargetY;


};

class Line final : public IFigure {

public:

    std::vector< std::pair<int, int> > GetAllPoints() override {}

};

class Circle final : public IFigure {

public:

    std::vector< std::pair<int, int> > GetAllPoints() override {}

};
