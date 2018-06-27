#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <deque>

#define DEBUG 0


void Extraction( std::set<std::set<std::string>>& undefined
                , std::map<std::string, int>& res
                , std::deque<std::string>& line )
{
    const std::string ApprovedName = line.front();

    for( auto it = undefined.begin(); it != undefined.end(); )
    {
        if( std::any_of( it->begin()
                      , it->end()
                      ,[&ApprovedName]( std::string name )
                      {
                          return name == ApprovedName;
                      })
            )
            {
                for( auto& name : (*it) )
                {
                    if( name != ApprovedName )
                    {
                        if( !( res.find(name) != res.end()
                              && res.find( name )->second <= (res[ApprovedName] + 1 ) ) )
                        {
                            res[name] = res[ApprovedName] + 1;
                            line.push_back( name );
                        }
                    }
                }

                it = undefined.erase( it );
            }
            else
            {
                ++it;
            }
    }

    line.pop_front();
}

int main()
{
    std::set<std::set<std::string>> undefined;
    std::deque<std::string> line;

    int n;
    std::cin >> n;

    std::map<std::string, int> result;

    for( int i = 0; i != n; ++i )
    {
        std::string A, B, C;

        std::cin >> A >> B >> C;

        if( A == "Isenbaev" || B == "Isenbaev" || C == "Isenbaev" )
        {
            result["Isenbaev"] = 0;
            line.push_back( "Isenbaev" );
        }

        undefined.insert( {A, B, C} );
    }

    while( !line.empty() )
    {
        Extraction( undefined, result, line );
    }

    // добавляем персон без числа Isenbaev в std::map

    if( !undefined.empty() )
    {
        for( const auto& team : undefined )
        {
            std::for_each( team.begin()
                          , team.end()
                          ,[&result]( std::string name )
                          {
                              result[name] = -1;
                          });
        }
    }

    if( DEBUG ) std::cout << "\nres.size is " << result.size() << "\n";

    // печатаем std::map с результатом

    for(const auto& elem : result )
    {
        std::cout << elem.first << " ";

        if( elem.second == -1)
        {
            std::cout << "undefined";
        }
        else
        {
            std::cout << elem.second;
        }

        std::cout << "\n";
    }


    return 0;
}

