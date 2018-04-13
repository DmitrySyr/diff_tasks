#include "lib.h"

int version()
{
	return PROJECT_VERSION_PATCH;
}

//============================================================================

std::ostream& operator <<(std::ostream& out, const ip_t& vec) {

    for(auto v = vec.cbegin(); v != vec.cend(); ++v) {
        if(v != vec.cbegin()) {
            out << ".";
        }

        out << *v;
    }
    //out << "\n";

    return out;
}

std::ostream& operator <<(std::ostream& out, const std::vector<std::string>& vec) {

    for(auto v = vec.cbegin(); v != vec.cend(); ++v) {
        if(v != vec.cbegin()) {
            out << ".";
        }

        out << *v;
    }
    //out << "\n";


    return out;
}

//=======================================================================================

std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}


bool filter_any(const ip_t& OrigVec, const int Cond) {


    if(Cond < 0) { return true;}

    for(const auto& vec : OrigVec) {

            if(vec == Cond) {

                return true;
            }
        }

    return false;
}



//===========================================================================================
