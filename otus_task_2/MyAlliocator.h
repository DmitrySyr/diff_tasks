#pragma once

#include <limits>
#include <stdexcept>
#include <iostream>
#include <new>
#include <stdlib.h>
#include <stddef.h>
#include <vector>

/*

Сейчас будут оговорки :)

- Версия совершенно непотокобезопасная.

- Ресайз не сделан, т.е. если много навыделяли блоков допольнительно,
придётся их держать до окончания работы.

- Никакой борьбы с фрагментацией.

- Структура ужасная. Зато работает.

- НЕ ПОДДЕРЖИВАЕТСЯ выделение блоков памяти в пуле (не использовать для вектора)

*/
template <typename T, size_t Lim = 1>
class MyAlliocator{

private:

    std::vector<T*> free_ptrs;     // складываем указатели на свободное местро в пуле

    std::vector<T*> basics;        // складываем указатели на выделенные блоки, чтобы освобождать в деструкторе

    size_t size_ = 0;

    // Хэлпер для выделения памяти

    T* alloc_helper(const size_t n) const {

        //std::cout << "Выделяем память под объекты в количестве: " << n
       // << " штук, всего выделено " << (sizeof(T) * n) << " байт\n";

        if (n == 0) return NULL;

        if (n > max_size()) throw std::length_error("Численное переполнение при запросе памяти.\n");

        void * const pv = malloc(n * sizeof(T));

        if (pv == NULL) throw std::bad_alloc();

        return static_cast<T*>(pv);
    }

    // хэлпер для освобождения
    void dealloc_helper(T* const p, const size_t n) const {

        //std::cout << "Освобождаем " << n * sizeof(T) << " байт.\n";

        free(p);
    }

public:

    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    using pointer         = T*;
    using const_pointer   = const T*;

    using reference       = T&;
    using const_reference = const T&;



    //===================================================================

    template<typename U>
    struct rebind{
        using other = MyAlliocator<U, Lim>;
    };

    size_t max_size() const {

         return std::numeric_limits<size_t>::max();
    }

    size_t size() const {

        return size_;
    }


    // имея состояние мы должны определить порядок (вроде бы)

    bool operator !=(const MyAlliocator& other) const {

        return Lim == 1 ? false : true;
    }

     bool operator ==(const MyAlliocator& other) const {

        return Lim != 1 ? false : true;
    }

    //==================================================================

    MyAlliocator() {
        if(Lim != 1) {

            free_ptrs.reserve(Lim);
            basics. reserve(1);
        }
    }

    MyAlliocator(const MyAlliocator& other)

    : free_ptrs{other.free_ptrs}, basics{other.basics}, size_{other.size_}
    {

    }


    template<typename U>
    MyAlliocator(const MyAlliocator<U>& other){}

    MyAlliocator& operator==(const MyAlliocator&) = delete;

    ~MyAlliocator() {
        while(!basics.empty()) {

            ::free(basics.back());

            //std::cout << "Освободили в деструкторе " << Lim * sizeof(T) << " байт.\n";

            basics.pop_back();
        }
    }

    // ======================== конструирование и деконструкция ========

    template <typename ... Arg>
    void construct(T* ptr, Arg ... arg) const {

        void* const place_ptr = static_cast<void*>(ptr);

        new(place_ptr) T(std::forward<Arg>(arg)...);
    }


    void destroy(T* const ptr) const {

        ptr->~T();

    }

    // ======================== выделение и освобождение памяти =======


     // при выделении памяти складываем указатели на хранение
    void init_block(T* ptr) {

        for(size_t i = 0; i != Lim; ++i) {

            free_ptrs.push_back(ptr + i);
        }
    }

    T* allocate(size_t n){

        T* ptr;

        if(Lim == 1) {

           ptr = alloc_helper(n);
        }
        else {

           if(basics.empty() || free_ptrs.empty()) {

            if(free_ptrs.empty()) free_ptrs.reserve(Lim);
           else free_ptrs.reserve(free_ptrs.size() + Lim);

            ptr = alloc_helper(Lim);

            init_block(ptr);

            basics.push_back(ptr);
           }

            //std::cout << "Выделяем память в пуле в размере " << sizeof(T) << " байт.\n";

            ptr = free_ptrs.back();

            free_ptrs.pop_back();

            ++size_;
        }

        return ptr;
    }



    void deallocate(T* ptr, const size_t n){

        if(Lim == 1) {

            //std::cout << "Возвращаем память в размере " << (n * sizeof(T) ) << " байт.\n";
            ::free(ptr);

        }
        else {

            //std::cout << "Возвращаем память в пул в размере " << sizeof(T) << " байт.\n";

            free_ptrs.push_back(ptr);

            --size_;

        }
    }


    // привязывание к другому блоку памяти игнорируем
    template<typename U>
    T* allocate(const size_t n, const U* other_ptr) const {

        return allocate(n);
    }



};
