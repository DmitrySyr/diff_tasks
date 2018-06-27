#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <numeric>
#include <map>
#include <unordered_set>


int main() {

   std::unordered_set<int> prof_list;
   size_t counter = 0;
   int N = 0;
   int tmp;

   std::cin >> N;

   for(int i = 0; i != N; ++i )
   {
       std::cin >> tmp;
       prof_list.insert( tmp );
   }

   std::cin >> N;

   for(int i = 0; i != N; ++i )
   {
       std::cin >> tmp;
       if( prof_list.find( tmp ) != prof_list.end() )
       {
           ++counter;
       }
   }

   std::cout << counter;

   return 0;
}
