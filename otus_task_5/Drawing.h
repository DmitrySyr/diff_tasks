
#pragma once

#include <map>
#include "GUIForms.h"
#include "Figures.h"

class Canvas;

class Drawing{

public:

    Drawing( Canvas* canvas );
    Drawing( const Drawing& ) = delete;
    Drawing& operator=( Drawing& ) = delete;

    void AddFigure( IFigure* );
    void DeleteFigure( int id );
    void ClearAll();



private:
    Canvas * canva;
    std::map<int, IFigure*> collection;
};
