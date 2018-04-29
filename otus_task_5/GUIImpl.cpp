
#include "GUIImpl.h"

GUIImpl::GUIImpl() {

        canva       = new Canvas( this );
        mainWindow  = new MainWindow( this );
        mainMenu    = new MainMenu( this );
        figuresMenu = new FiguresMenu( this );
    }

 void GUIImpl::Refresh( IForm* form) {}
