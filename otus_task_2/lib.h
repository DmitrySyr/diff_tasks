#pragma once

#include <bits/stdc++.h>

#include "version.h"

using ip_t = std::vector<unsigned int>;

bool MyComp(const ip_t& LV, const ip_t& RV);


// конвертация
template<typename Target, typename Source>
Target convert(Source arg)
{
  std::stringstream interpreter;
  Target result;

  if(!(interpreter << arg) ||
     !(interpreter >> result) ||
     !(interpreter >> std::ws).eof())
    {
        interpreter << "Converting from " << typeid(Source).name() << " to " << typeid(Target).name() << " is impossible.\n";
        throw std::runtime_error(interpreter.str());
    }

  return result;
}

//получение версии файла
int version();


// Переопределяем потоки вывода для вектора интов и вектора векторов
template <typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T>& vec) {
    for(const auto& v : vec) {
        out << v << "\n";
    }

    return out;
}


std::ostream& operator <<(std::ostream&, const ip_t&);
std::ostream& operator <<(std::ostream&, const std::vector<std::string>&);

//========================================================================================

std::vector<std::string> split(const std::string &, char );

//========================================================================================

//  фильтрация по аргументам, -1 используется как индикатор любого числа в позиции
template <typename ... Arg>
bool filter(const ip_t& OrigVec, Arg ... arg) {

    std::vector<int> CondVec{(arg)...};

    if(OrigVec.size() < CondVec.size()) {
            std::cerr << "There is an error in a number of conditions.\n";
            return false;
        }


    for(size_t i = 0; i < CondVec.size(); ++i) {

        if((CondVec[i] != -1) && (OrigVec[i] != CondVec[i])) {
            return false;
        }
    }


    return true;
}


bool filter_any(const ip_t& , const int );


// =======================================================================================



