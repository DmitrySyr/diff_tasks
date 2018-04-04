#include "lib.h"


int main(int argc, char const *argv[])
{
    try
    {
        std::vector<ip_t> ip_pool;
        ip_t vec;
        vec.reserve(4);

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            std::vector<std::string> t = split(v.at(0), '.');



            for(const auto& str : t) {
                vec.push_back(convert<unsigned int>(str));
            }
            ip_pool.push_back(vec);

            vec.clear();
        }


        // TODO reverse lexicographically sort

        std::sort(ip_pool.rbegin(), ip_pool.rend());

        std::cout << ip_pool;


        for(const auto& vec : ip_pool) {

            if(filter(vec, 1)) {
                std::cout << vec << "\n";
            }
        }

        //=========================

        for(const auto& vec : ip_pool) {

            if(filter(vec, 46, 70)) {
                std::cout << vec << "\n";
            }
        }

        //=========================

        for(const auto& vec : ip_pool) {

            if(filter_any(vec, 46)) {
                std::cout << vec << "\n";
            }
        }

        //=========================




    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
