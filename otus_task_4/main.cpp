#include <iostream>
#include <string>
#include <vector>


#include "print_ip.h"


using namespace std;

int main()
{

    char f = -1;

    PRNT::print(f); std::cout << "\n";

    char f1 = -55;

    PRNT::print(f1); std::cout << "\n";

    short sh1 = 0;

    PRNT::print(sh1); std::cout << "\n";

    short shrt = -32640;

    PRNT::print(shrt); std::cout << "\n";

    int t = 256;

    PRNT::print(t); std::cout << "\n";

    long a = 8875824491850138409;

    PRNT::print(a); std::cout << "\n";

    std::tuple<int, int, int, int> gf {127, 0, 0, 255};

    PRNT::print(gf); std::cout << "\n";

    return 0;
}
