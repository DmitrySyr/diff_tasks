#include <map>
#include "lib.h"

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>




BOOST_AUTO_TEST_SUITE(Making_String)


BOOST_AUTO_TEST_CASE(TestingSplit) {
  const std::vector<std::string> Asking{
					{"345.45.54.45"},
                    {"..."},
                    {"sdf.567.56.5"},
                    {""}};

  const std::vector<std::vector<std::string>> Answering{
					{"345", "45", "54", "45"},
                    {"", "", "", ""},
                    {"sdf", "567", "56", "5"},
                    {""}};

  for(const auto& i : std::vector<int> {0, 1, 2, 3}) {

	auto result = split(Asking[i], '.');

	//std::cout << "Compear: " << *result.begin() << " and " << *Answering[i].begin() << "\n";

	BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), Answering[i].begin(), Answering[i].end());

  }
}

BOOST_AUTO_TEST_CASE(TestingConvert_Correct) {
  const std::vector<std::string> Asks{
					"345", "45", "54", "2555"
                    //,""
                    , "67", "678"};

  std::map<std::string, unsigned int> Ans{
      std::make_pair("345", 345)
      , std::make_pair("45", 45)
      , std::make_pair("54", 54)
      , std::make_pair("2555", 2555)
      //, std::make_pair("", 0)
      , std::make_pair("67", 67)
      , std::make_pair("678", 678) };

  for(const auto& i : Asks) {

    //std::cout << "Check: " << Ans[i] << " with " << i << "\n";

	BOOST_CHECK_EQUAL(convert<unsigned int>(i) , Ans[i]);

  }
}


BOOST_AUTO_TEST_CASE(check_conv_throw){

  BOOST_CHECK_THROW( convert<unsigned int>(""), std::runtime_error);

}


BOOST_AUTO_TEST_CASE(check_filter){

  const std::vector<ip_t> for_filtering{
					{79, 184, 223, 137}
					,{34, 214, 12, 226}
					, {74, 96, 79, 230}
					, {88, 166, 41, 16235}
					, {117, 5, 169, 4309}
					, {0, 0, 0, 0}
                    };

	BOOST_CHECK_EQUAL(filter(for_filtering[0], -1, -1, -1, 137), 1);
	BOOST_CHECK_EQUAL(filter(for_filtering[1], 32), 0);
	BOOST_CHECK_EQUAL(filter(for_filtering[2], 74, 96), 1);
	BOOST_CHECK_EQUAL(filter(for_filtering[3], 88, 166, 41, 0), 0);
	BOOST_CHECK_EQUAL(filter(for_filtering[4], 117, 5, -1, 4309), 1);
	BOOST_CHECK_EQUAL(filter(for_filtering[5], -1, -1, -1, -1), 1);


}


//BOOST_AUTO_TEST_CASE(check_filter_throw){
//
//  const std::vector<ip_t> for_filtering{
//					  {117, 5, 169, 4309}
//					, {0, 0, 0, 0}
//                    };
//
//  BOOST_CHECK_THROW( filter(for_filtering[0], 117, 5, 169, 4309, -1), std::runtime_error);
//  BOOST_CHECK_THROW( filter(for_filtering[1], -1, -1, -1, -1, -1), std::runtime_error);
//
//}


BOOST_AUTO_TEST_CASE(check_filter_any){

    const std::vector<ip_t> for_filtering{
					  {79, 184, 223, 137}
					, {34, 214, 12, 226}
					, {74, 96, 79, 230}
					, {88, 166, 41, 16235}
					, {117, 5, 169, 4309}
					, {0, 0, 0, 0}
                    };

    BOOST_CHECK_EQUAL(filter_any(for_filtering[0], -1), 1);
	BOOST_CHECK_EQUAL(filter_any(for_filtering[1], 226), 1);
	BOOST_CHECK_EQUAL(filter_any(for_filtering[2], 115), 0);
	BOOST_CHECK_EQUAL(filter_any(for_filtering[3], 16235), 1);
	BOOST_CHECK_EQUAL(filter_any(for_filtering[4], -5), 1);
	BOOST_CHECK_EQUAL(filter_any(for_filtering[5], 1), 0);

}



BOOST_AUTO_TEST_SUITE_END()
