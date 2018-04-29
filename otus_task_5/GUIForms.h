#pragma once

#include "GUIImpl.h"
#include "Drawing.h"

class GUIImpl;
class Drawing;

enum class ActiveFigure{
    Line
    , Circle
    , Quadrat
    , Triangle
};

class IForm{
public:

    IForm( GUIImpl * gui ) : guiImpl( gui ) {}
    IForm( const IForm& ) = delete;
    IForm& operator=( IForm& ) = delete;

    virtual ~IForm(){}
    virtual void Refresh() = 0;


//private:

    GUIImpl* guiImpl;
};

class MainWindow final : public IForm {

public:

    MainWindow( GUIImpl* gui );

    void Refresh() override;

};


class MainMenu final : public IForm {

public:

    MainMenu( GUIImpl* gui );

    void Refresh() override;

    void NewDrawing();
    void SaveDrawing();
    void LoadDrawing();
    void Quit();

};


class FiguresMenu : public IForm {

public:

    FiguresMenu( GUIImpl* gui );

    void Refresh() override ;

    void SetActiveFigure( int id );


    ActiveFigure activeFigure = ActiveFigure::Line;

};

class Canvas : public IForm {

public:

    Canvas( GUIImpl* gui );

    Canvas( const Canvas& ) = delete;
    Canvas& operator=( Canvas& ) = delete;


    void AddFigure();
    void DeleteFigure( int id );


    void Refresh() override;

    Drawing* drawing;

};
