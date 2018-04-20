#include <map>
#include <tuple>
#include <iostream>
#include <sstream>


#include "print_ip.h"

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

#include <boost/test/output_test_stream.hpp>

#include <boost/any.hpp>

using any = boost::any;

//using boost::test_tools::output_test_stream;


BOOST_AUTO_TEST_SUITE(test_printing_ip)

std::stringstream ss;

// structure for enabling changing streambuf
  struct cout_redirect {
    cout_redirect( std::streambuf * new_buffer )
        : old( std::cout.rdbuf( new_buffer ) )
    { }

    ~cout_redirect( ) {
        std::cout.rdbuf( old );
    }

  private:
    std::streambuf * old;
  };

  // funcs for getting from tuple
    struct helper
    {
        template<typename T>
        void operator()(T& t) const { HW4::print((T)t); }
    };

    template<std::size_t I = 0, typename FuncT, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    for_index(int, std::tuple<Tp...> &, FuncT)
    { }

    template<std::size_t I = 0, typename FuncT, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp), void>::type
    for_index(int index, std::tuple<Tp...>& t, FuncT f)
    {
        if (index == 0) f(std::get<I>(t));
        for_index<I + 1, FuncT, Tp...>(index-1, t, f);
    }

BOOST_AUTO_TEST_CASE(test_integrals) {



  std::tuple<char, char, short, short, short, int, int, long, long, long> Ask{
					static_cast<char>( -1 )
					,static_cast<char>( 128 )
					,static_cast<short>( 0 )
					,static_cast<short>( -32640 )
					,static_cast<short>( -1 )

					,static_cast<int>( 2130706433 )
					,static_cast<int>( -255 )

					,static_cast<long>( 3321954815 )
					,static_cast<long>( 8319119876597776512 )
					,static_cast<long>( -2139062144 )
				};

  const std::vector<std::string> Answering{
                    "255"
                    ,"128"

                    ,"0.0"
                    ,"128.128"
                    ,"255.255"

					,"127.0.0.1"
                    ,"255.255.255.1"

                    ,"0.0.0.0.198.1.1.255"
                    ,"115.115.115.115.128.128.128.128"
                    ,"255.255.255.255.128.128.128.128"
					};

  for(const auto& i : {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}) {


    {
        cout_redirect guard( ss.rdbuf() );
        for_index(i, Ask, helper());

    }


    BOOST_CHECK_EQUAL( ss.str(), Answering[i] );

    ss.str(std::string());
  }
}


BOOST_AUTO_TEST_CASE(test_containers) {



    std::vector<std::vector<int>> ask_int {
        {255,128,128,0,1}
        ,{0,0,0,0,0}
        ,{567, 890, 999, 375, 123455, 98989876}
        ,{}
        ,{0}
    };

    std::vector<std::vector<std::string>> ask_string {
        {"255","128","128","0","1"}
        ,{"0","0","0","0","0"}
        ,{"567", "890", "999", "375", "123455", "98989876"}
        ,{}
        ,{"0"}
    };

    std::vector<std::string> ask_string_II {
        {"255.128.128.0.1"}
        ,{"0.0.0.0.0"}
        ,{"567.890.999.375.123455.98989876"}
        ,{}
        ,{"0"}
    };

    std::vector<std::list<int>> ask_list {
        {255,128,128,0,1}
        ,{0,0,0,0,0}
        ,{567,890,999,375,123455,98989876}
        ,{}
        ,{0}
    };

  const std::vector<std::string> ans{
                    "255.128.128.0.1"
                    ,"0.0.0.0.0"

                    ,"567.890.999.375.123455.98989876"
                    ,""
                    ,"0"
					};

  int i = 0;
  for(const auto& el : ask_int) {


    {
        cout_redirect guard( ss.rdbuf() );
        HW4::print(el);

    }


    BOOST_CHECK_EQUAL( ss.str(), ans[i] );
    ++i;

    ss.str(std::string());
  }

  i = 0;
  for(const auto& el : ask_string) {


    {
        cout_redirect guard( ss.rdbuf() );
        HW4::print(el);

    }


    BOOST_CHECK_EQUAL( ss.str(), ans[i] );
    ++i;

    ss.str(std::string());
  }

  i = 0;
  for(const auto& el : ask_string_II) {


    {
        cout_redirect guard( ss.rdbuf() );
        HW4::print(el);

    }


    BOOST_CHECK_EQUAL( ss.str(), ans[i] );
    ++i;

    ss.str(std::string());
  }

  i = 0;
  for(const auto& el : ask_list) {


    {
        cout_redirect guard( ss.rdbuf() );
        HW4::print(el);

    }


    BOOST_CHECK_EQUAL( ss.str(), ans[i] );
    ++i;

    ss.str(std::string());
  }
}

BOOST_AUTO_TEST_CASE(test_tuples) {


  std::tuple<char> ask_0 {97};

  std::tuple<int, int, int, int> ask_1 {198,128,0,1};

  std::tuple<long, long, long, long> ask_2 {198,128,0,1};

  std::tuple<short, short, short, short> ask_3 {198,128,0,1};

  std::tuple<short, int, long, unsigned long> ask_4 {198,128,0,1};

  const std::string ans {"198.128.0.1"};


  {     cout_redirect guard( ss.rdbuf() );
        HW4::print(ask_0);
  }

  BOOST_CHECK_EQUAL( ss.str(), "a" );
  ss.str(std::string());

  //-------------------------------------
  {     cout_redirect guard( ss.rdbuf() );
        HW4::print(ask_1);
  }

  BOOST_CHECK_EQUAL( ss.str(), ans );
  ss.str(std::string());

  //-------------------------------------
  {     cout_redirect guard( ss.rdbuf() );
        HW4::print(ask_2);
  }

  BOOST_CHECK_EQUAL( ss.str(), ans );
  ss.str(std::string());

  //-------------------------------------
  {     cout_redirect guard( ss.rdbuf() );
        HW4::print(ask_3);
  }

  BOOST_CHECK_EQUAL( ss.str(), ans );
  ss.str(std::string());

  //-------------------------------------
  {     cout_redirect guard( ss.rdbuf() );
        HW4::print(ask_4);
  }

  BOOST_CHECK_EQUAL( ss.str(), ans );
  ss.str(std::string());

}

BOOST_AUTO_TEST_CASE(test_cont_cont_numbers) {


  std::vector<std::list<int>> ask_0 {
                                    {198, 128, 0, 1}
                                    , {255, 255, 255, 0}
  };


  {     cout_redirect guard( ss.rdbuf() );
        HW4::print( ask_0 );
  }

  BOOST_CHECK_EQUAL( ss.str(), "198.128.0.1\n255.255.255.0\n" );
  ss.str(std::string());

  //-------------------------------------
  std::list<std::vector<int>> ask_1 {
                                    {198, 128, 0, 1}
                                    , {255, 255, 255, 0}
                                    };

  {     cout_redirect guard( ss.rdbuf() );
        HW4::print(ask_1);
  }

    BOOST_CHECK_EQUAL( ss.str(), "198.128.0.1\n255.255.255.0\n" );
  ss.str(std::string());

  //-------------------------------------


}


BOOST_AUTO_TEST_SUITE_END()
