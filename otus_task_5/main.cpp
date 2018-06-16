

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

    MainMenuNew m_menu( App.GetCurrentDocument(), 20ul, 20ul, 40ul, 40ul );

    FiguresMenu_Circle circle_menu( App.GetCurrentDocument(), 20ul, 60ul, 40ul, 40ul );

    Canvas canvas( App.GetCurrentDocument(), 200ul, 250ul, 800ul, 800ul );

    FiguresMenu_DeleteFigure deleter( App.GetCurrentDocument(), 20ul, 120ul, 40ul, 40ul );

    // выделяем создание круга
    App.GetCurrentDocument()->ProcessAction( 25ul, 65ul, 0ul, 0ul, ActionType::Click );

    // создаём круг через drag´n'drop
    App.GetCurrentDocument()->ProcessAction( 300ul, 350ul, 400ul, 450ul, ActionType::DragAndDrop );

    // выделяем удаление круга
    App.GetCurrentDocument()->ProcessAction( 25ul, 130ul, 0ul, 0ul, ActionType::Click );

    // удаляем
    App.GetCurrentDocument()->ProcessAction( 310ul, 360ul, 0ul, 0ul, ActionType::Click );

    std::cout << "\nHello world!" << std::endl;
    return 0;
}
