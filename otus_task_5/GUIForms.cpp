#include "GUIForms.h"

MainWindow::MainWindow( GUIImpl* gui ) : IForm( gui ) {}

void MainWindow::Refresh() {

        this -> guiImpl -> Refresh( this );
    }

MainMenu::MainMenu( GUIImpl* gui ) : IForm( gui ) {}

void MainMenu::NewDrawing() {}
void MainMenu::SaveDrawing() {}
void MainMenu::LoadDrawing() {}
void MainMenu::Quit() {}

void MainMenu::Refresh() {

        this -> guiImpl -> Refresh( this );
    }

FiguresMenu::FiguresMenu( GUIImpl* gui ) : IForm( gui ) {}

void FiguresMenu::SetActiveFigure( int id ) {}

void FiguresMenu::Refresh() {

        this -> guiImpl -> Refresh( this );
    }


Canvas::Canvas( GUIImpl* gui ) : IForm( gui ), drawing( new Drawing( this ) ) {}

void Canvas::Refresh() {

        this -> guiImpl -> Refresh( this );
    }


void Canvas::AddFigure(){

    switch ( this -> guiImpl -> figuresMenu -> activeFigure ){

    case ActiveFigure::Line:
        this -> drawing -> AddFigure( new Line );
        this -> Refresh();
        break;

    case ActiveFigure::Circle:
        this -> drawing -> AddFigure( new Circle );
        this -> Refresh();
        break;
    }
}

void Canvas::DeleteFigure( int id ){

    this -> drawing -> DeleteFigure( id );
}
