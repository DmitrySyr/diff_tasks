#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <numeric>
#include <map>





int main() {


   int N = 0;
   std::map<char, std::vector<char>> dict;

   std::cin >> N;

   for(int i = 0; i != N; ++i )
   {
        std::string input;
        std::cin >> input;
        dict[input[0]].push_back( input[1] );
   }

   char seeking_symbol;

   std::cin >> seeking_symbol;

   while( !dict[seeking_symbol].empty() )
   {
       for( const auto& s : dict[seeking_symbol] )
       {
           std::cout << seeking_symbol << s << "\n";
       }
   }

   return 0;
}
