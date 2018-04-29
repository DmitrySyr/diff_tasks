#include <iostream>

#include "Application.h"



int main()
{

    Application::Instance().Init();

    Application::Instance().gui -> figuresMenu -> SetActiveFigure( 1 );

    Application::Instance().gui -> canva -> AddFigure();

    Application::Instance().gui -> figuresMenu -> SetActiveFigure( 2 );

    Application::Instance().gui -> canva -> AddFigure();

    Application::Instance().gui -> mainMenu -> SaveDrawing();

    Application::Instance().gui -> canva -> DeleteFigure( 1 );

    Application::Instance().gui -> mainMenu -> LoadDrawing();

    Application::Instance().gui -> mainMenu -> Quit();


    std::cout << "Hello world!" << std::endl;
    return 0;
}
