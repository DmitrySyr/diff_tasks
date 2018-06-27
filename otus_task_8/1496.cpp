#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <numeric>
#include <map>
#include <set>





int main() {


   int N = 0;
   std::set<std::string> dict;
   std::set<std::string> result;

   std::cin >> N;

   for(int i = 0; i != N; ++i )
   {
       std::string input;
       std::cin >> input;
        if( dict.find( input ) == dict.end() )
        {
            dict.insert( input );
        }
        else
        {
            result.insert( input );
        }
   }

   for( const auto& res : result )
   {
       std::cout << res << "\n";
   }

   return 0;
}
