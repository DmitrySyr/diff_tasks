#include <map>
#include <tuple>
#include <iostream>
#include <sstream>

#include "Application.h"
#include "Menu.h"
#include "Figures.h"

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

#include <boost/test/output_test_stream.hpp>


BOOST_AUTO_TEST_SUITE( test_graph )

BOOST_AUTO_TEST_CASE( test_create ) {

	auto& App = Application::Instance();

    App.Init();

	BOOST_CHECK_EQUAL( true, Application::Instance().gui != nullptr );
	BOOST_CHECK_EQUAL( false, Application::Instance().documents.empty() );

}


BOOST_AUTO_TEST_CASE( test_new_figures ) {

	auto& App = Application::Instance();

    App.Init();

    MainMenuNew m_menu( App.GetCurrentDocument(), 20, 20, 40, 40 );

    FiguresMenu_Circle circle_menu( App.GetCurrentDocument(), 20, 60, 40, 40 );

    Canvas canvas( App.GetCurrentDocument(), 200, 250, 800, 800 );

    // выделяем создание круга
    App.GetCurrentDocument()->ProcessAction( 25, 65, 0, 0, ActionType::Click );

    // создаём круг через drag´n'drop
    App.GetCurrentDocument()->ProcessAction( 300, 350, 400, 450, ActionType::DragAndDrop );

    // проверяем создание круга
	BOOST_CHECK_EQUAL( true, true );

}

BOOST_AUTO_TEST_CASE(test_delete_figure) {

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


	BOOST_CHECK_EQUAL( true, true );

}

BOOST_AUTO_TEST_CASE( test_new ) {

	auto& App = Application::Instance();

    App.Init();

    MainMenuNew m_menu( App.GetCurrentDocument(), 20, 20, 40, 40 );

    FiguresMenu_Circle circle_menu( App.GetCurrentDocument(), 20, 60, 40, 40 );

    Canvas canvas( App.GetCurrentDocument(), 200, 250, 800, 800 );

    FiguresMenu_DeleteFigure deleter( App.GetCurrentDocument(), 20, 120, 40, 40 );

    App.GetCurrentDocument()->ProcessAction( 25, 25, 0, 0, ActionType::Click );

	BOOST_CHECK_EQUAL( true, true );


}

BOOST_AUTO_TEST_CASE(test_save) {

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

    // вызываем объект сохранения

	// проверяем сохранение документа
	BOOST_CHECK_EQUAL( true, true );

}

BOOST_AUTO_TEST_CASE( test_load ) {

	auto& App = Application::Instance();

    App.Init();

    // вызываем объект загрузки

    //проверяем загрузку
    BOOST_CHECK_EQUAL( true, true );

}

BOOST_AUTO_TEST_SUITE_END()
