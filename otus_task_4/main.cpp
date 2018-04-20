#include <iostream>
#include <string>
#include <vector>


#include "print_ip.h"


using namespace std;

int main()
{
    try{
        char f = -1;

        HW4::print(f); std::cout << "\n";

        char f1 = -55;

        HW4::print(f1); std::cout << "\n";

        short sh1 = 0;

        HW4::print(sh1); std::cout << "\n";

        short shrt = -32640;

        HW4::print(shrt); std::cout << "\n";

        int t = 256;

        HW4::print(t); std::cout << "\n";

        long a = 8875824491850138409;

        HW4::print(a); std::cout << "\n";

        HW4::print("1212.1212.112"); std::cout << "\n";



        std::tuple<int, int, int, int> gf {127, 0, 0, 255};

        HW4::print(gf); std::cout << "\n";



        std::vector<std::vector<int>> ask_0 {
                                    {198, 128, 0, 1}
                                    , {255, 255, 255, 0}
                                    };

        HW4::print(ask_0); std::cout << "\n";

        std::vector<int> ask_1 {198, 128, 0, 1};

        HW4::print(ask_1); std::cout << "\n";

        std::vector<std::string> ask_2 {"asdfasdf", "asdfasdfasdf"};

        HW4::print(ask_2); std::cout << "\n";

        std::string ask_3 {"asdfasdf"};

        HW4::print(ask_3); std::cout << "\n";

    }
    catch(const std::exception& e) {

        std::cout << e.what();
    }

    return 0;
}
