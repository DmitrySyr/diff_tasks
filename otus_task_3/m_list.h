#pragma once


/*




*/

#include <iostream>

template<typename T>
struct Node{


    T data;
    Node* next;


    Node( const T & d = T{}, Node* n = nullptr )
    :data{d}, next{n} {}


    Node( T && d, Node* n = nullptr )
    :data{std::move(d)}, next{n} {}

};

template <typename T, typename Alloc = std::allocator<Node<T>>>
class MList {

private:

    size_t size_ = 0;
    Node<T>* head = nullptr;
    Alloc alloc_;

public:
    //types

    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = ptrdiff_t;

    // итератор

    class iterator : public std::iterator<
                                            std::forward_iterator_tag
                                            , Node<T>
                                            , std::ptrdiff_t
                                            , Node<T>*
                                            , Node<T>&
                                        >
    {
        public:
            iterator();
            T& operator*();


            iterator& operator++();
            iterator operator++(int);

            bool operator==(const iterator&) const;
            bool operator!=(const iterator&) const;

        protected:
            Node<T>* current_;
            iterator( Node<T>* p );
            friend class MList<T, Alloc>;
    };

    // служебные функции


    size_t size() const;
    bool empty() const;

    void push_front(const T& val );
    void push_front(T && val );

    void pop_front();

    void clear();


    iterator begin();
    iterator end();

    const Alloc& get_allocator() const {

        return alloc_;
    }

    // конструкторы
    MList() = default;

    MList(MList& other) {


        for(auto it = other.begin(); it != other.end(); ++it) {

            push_front(*it);
        }

        size_ = other.size_;
    }

    MList<T>& operator=(const MList& other){

        this -> clear();

        for( auto it = other.begin(); it != other.end(); ++it ) {

            push_front(*it);
            ++size_;
        }

        return *this;
    }

    ~MList() {
        if(size_)
            clear();
    }

};

// =========================================================================
// ========================   iterators           ==========================
// =========================================================================

template<typename T, typename Alloc>
MList<T, Alloc>::iterator::iterator() {}

template<typename T, typename Alloc>
MList<T, Alloc>::iterator::iterator(Node<T>* p) {

    current_ = p;
}

template<typename T, typename Alloc>
T& MList<T, Alloc>::iterator::operator*()  {
	return this->current_->data;
}


template<typename T, typename Alloc>
typename MList<T, Alloc>::iterator & MList<T, Alloc>::iterator::operator++()  {
	this->current_ = this->current_->next;
	return *this;
}

template<typename T, typename Alloc>
typename MList<T, Alloc>::iterator MList<T, Alloc>::iterator::operator++(int)  {
	auto copy_ = *this;
	this->current_ = this->current_->next;
	return copy_;
}

template<typename T, typename Alloc>
bool MList<T, Alloc>::iterator::operator==( const typename MList<T, Alloc>::iterator & rhs ) const  {
	return current_ == rhs.current_;
}

template<typename T, typename Alloc>
bool MList<T, Alloc>::iterator::operator!=( const typename MList<T, Alloc>::iterator & rhs ) const  {
	return current_ != rhs.current_;
}


// =========================================================================
// ========================          functions    ==========================
// =========================================================================

template<typename T, typename Alloc>
typename MList<T, Alloc>::iterator MList<T, Alloc>::begin(){

    return MList<T, Alloc>::iterator(head);
}

template<typename T, typename Alloc>
typename MList<T, Alloc>::iterator MList<T, Alloc>::end(){

    return nullptr;
}

template<typename T, typename Alloc>
size_t MList<T, Alloc>::size() const  {
	return size_;
}

template<typename T, typename Alloc>
bool MList<T, Alloc>::empty() const  {
	return size() == 0;
}

template<typename T, typename Alloc>
void MList<T, Alloc>::clear()  {
	while( !empty() )  {
		pop_front();
	}
}


template<typename T, typename Alloc>
void MList<T, Alloc>::push_front(const T& val )  {

	auto ptr = alloc_.allocate(1);
	alloc_.construct(ptr, Node<T>());

	ptr->data = val;

	if(head) ptr->next = head;

	head = ptr;
	++size_;
}

template<typename T, typename Alloc>
void MList<T, Alloc>::push_front( T && val )  {

    auto tmp = T();
    std::swap(tmp, val);
	push_front(tmp);
}

template<typename T, typename Alloc>
void MList<T, Alloc>::pop_front()  {

	if(!head) return;

	auto ptr = head;

    head = head-> next;

    alloc_.destroy(ptr);
    alloc_.deallocate(ptr, 1);

    --size_;
}

template<typename T, typename Alloc>
std::ostream & operator<<( std::ostream & out, MList<T, Alloc> & lst )  {

	for(auto it = lst.begin(); it != lst.end(); ++it) {

        if(it != lst.begin()) out << " ";

        out << (*it);
	}

	return out;
}

