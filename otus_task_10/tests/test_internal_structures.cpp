#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include "test_header.h"

BOOST_AUTO_TEST_SUITE ( principal_task )



BOOST_AUTO_TEST_CASE ( check_I )
{
    std::vector<std::exception_ptr> exc;

    // Инициализируем основной объект для получения и распределения данных
    auto receiver = std::make_shared<ReceivingBulk>( std::cout );

    // Инициализируем очередь для потоков под запись в файл
    auto LineToDisk    = std::make_shared<Queue>();
    auto LineToConsole = std::make_shared<Queue>();

    // Инициализируем два объекта, которые будут получать оповещения
    // и отправлять данные в потоки
    auto logger = std::make_shared<LoggingToFile>( *LineToDisk.get() );
    auto shower = std::make_shared<ShowOnDisplay>( *LineToConsole.get() );

    BOOST_CHECK_EQUAL ( receiver->GetNumberOfSubscribers(), 0 );

    receiver->AddProcessor( shower );

    BOOST_CHECK_EQUAL ( receiver->GetNumberOfSubscribers(), 1 );

    receiver->AddProcessor( logger );

    BOOST_CHECK_EQUAL ( receiver->GetNumberOfSubscribers(), 2 );
}

BOOST_AUTO_TEST_SUITE_END()


