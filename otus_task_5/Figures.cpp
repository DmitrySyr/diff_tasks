
#include "Figures.h"

void IFigure::HandClick( int x, int y ) {

        if( switcher == Switcher::FirstClick ) {
            this -> SetCenter( x, y );
            switcher = Switcher::SecondClick;
        }
        else {
            this -> SetTarget( x, y );
            switcher = Switcher::FirstClick;

        }
    }
