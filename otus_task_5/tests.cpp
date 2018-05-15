#include <map>
#include <tuple>
#include <iostream>
#include <sstream>

#include "GUIForms.h"
#include "Drawing.h"
#include "Figures.h"
#include "GUIImpl.h"

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

#include <boost/test/output_test_stream.hpp>


BOOST_AUTO_TEST_SUITE(test_graph)

BOOST_AUTO_TEST_CASE(test_new) {

	Application::Instance().Init();

	BOOST_CHECK_EQUAL( true, Application::Instance().gui != nullptr );

}


BOOST_AUTO_TEST_CASE(test_figures) {

	Application::Instance().Init();

	Application::Instance().gui -> figuresMenu -> SetActiveFigure( 1 );

	Application::Instance().gui -> canva -> AddFigure();

	BOOST_CHECK_EQUAL(  );


	Application::Instance().gui -> figuresMenu -> SetActiveFigure( 2 );

	Application::Instance().gui -> canva -> AddFigure();

	BOOST_CHECK_EQUAL( );


	Application::Instance().gui -> canva -> DeleteFigure( 1 );
	BOOST_CHECK_EQUAL( );
}

BOOST_AUTO_TEST_CASE(test_save_new_load) {

	Application::Instance().Init();

	Application::Instance().gui -> figuresMenu -> SetActiveFigure( 1 );

	Application::Instance().gui -> canva -> AddFigure();

	Application::Instance().gui -> mainMenu -> SaveDrawing();

	BOOST_CHECK_EQUAL( );


	Application::Instance().gui -> canva -> DeleteFigure( 1 );

	Application::Instance().gui -> mainMenu -> LoadDrawing();

	BOOST_CHECK_EQUAL( );

}




BOOST_AUTO_TEST_SUITE_END()
