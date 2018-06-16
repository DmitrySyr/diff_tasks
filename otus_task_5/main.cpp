

/*
 *  В основе три класса, это Application, само приложение,
 *  затем GUI, это визуальное представление на экране,
 *  Document - представляет собой документ, хранит объекты,
 *  которые затем размещаются на экране редактора.
 *
 *  Все объекты в редакторе наследуются от одного класса IObject,
 *  это позволяет одинаково ообрабатывать обращения. В том числе
 *  все меню и все графические объекты.
 *
 *  Схема работы: приложение отправляет полученный от системы сигнал
 *  в Document, который через ProcessAction и struct ObjectsAreas
 *  определяет какому из объектов отдавать обработку и передает
 *  информацию.
 *  Каждый объект обрабатывает самостоятельно и при необходимости
 *  взаимодействует с рисовальщиком.
 */

#include <iostream>
#include <vector>
#include <memory>

#include "Application.h"
#include "Menu.h"
#include "Figures.h"




int main()
{

    auto& App = Application::Instance();

    App.Init();

    MainMenuNew m_menu( App.GetCurrentDocument(), 20, 20, 40, 40 );

    FiguresMenu_Circle circle_menu( App.GetCurrentDocument(), 20, 60, 40, 40 );

    Canvas canvas( App.GetCurrentDocument(), 200, 250, 800, 800 );

    FiguresMenu_DeleteFigure deleter( App.GetCurrentDocument(), 20, 120, 40, 40 );

    // выделяем создание круга
    App.GetCurrentDocument()->ProcessAction( 25, 65, 0, 0, ActionType::Click );

    // создаём круг через drag´n'drop
    App.GetCurrentDocument()->ProcessAction( 300, 350, 400, 450, ActionType::DragAndDrop );

    // выделяем удаление круга
    App.GetCurrentDocument()->ProcessAction( 25, 130, 0, 0, ActionType::Click );

    // удаляем
    App.GetCurrentDocument()->ProcessAction( 310, 360, 0, 0, ActionType::Click );

    std::cout << "\nHello world!" << std::endl;
    return 0;
}
