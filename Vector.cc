// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// YOUR NAME HERE

#include <iostream> // debugging
#include "Vector.h"

// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector()
: elements(NULL), count(0), reserved(0)
{
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
: elements(NULL), count(rhs.count), reserved(rhs.reserved)
{
    if (reserved > 0) {
        
        elements = (T*)malloc(reserved * sizeof(T));
        for (size_t i = 0; i < count; ++i) {
            new (elements + i) T(rhs.elements[i]);
        }
    }
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
    if (&rhs != this) {
        
        for (size_t i = 0; i < count; ++i) {
            (elements + i)->~T();
        }
        free(elements);
        elements = NULL;
        count = rhs.count;
        reserved = rhs.reserved;
        if (reserved > 0) {
            elements = (T*)malloc(reserved * sizeof(T));
            for (size_t i = 0; i < count; ++i) {
                new (elements + i) T(rhs.elements[i]);
            }
        }
    }
    return *this;
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
: elements(NULL), count(0), reserved(nReserved)
{ // Initialize with reserved memory
    
    elements = (T*)malloc(reserved * sizeof(T));
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
: elements(NULL), count(n), reserved(n)
{ // Initialize with "n" copies of "t"
    
    elements = (T*)malloc(reserved * sizeof(T));
    for (size_t i = 0; i < count; ++i) {
        new (elements + i) T(t);
    }
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{ // Reserve extra memory
    if (n > reserved) {
        T* oldElements = elements;
        
        elements = (T*)malloc(n * sizeof(T));
        for (size_t i = 0; i < count; ++i) {
            new (elements + i) T(oldElements[i]);
            (oldElements + i)->~T();
        }
        free(oldElements);
        reserved = n;
    }
}

#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
    
    for (size_t i = 0; i < count; ++i) {
        (elements + i)->~T();
    }
    free(elements);
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
    
    if (count == reserved) {
        T* oldElements = elements;
        elements = (T*)malloc((reserved + 1) * sizeof(T));
        for (size_t i = 0; i < count; ++i) {
            new (elements + i) T(oldElements[i]);
            (oldElements + i)->~T();
        }
        free(oldElements);
        reserved++;
    }
    new (elements + count) T(rhs);
    count++;
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
    
    if (count == reserved) {
        T* oldElements = elements;
        elements = (T*)malloc((reserved + 1) * sizeof(T));
        for (size_t i = count; i > 0; --i) {
            new (elements + i) T(oldElements[i - 1]);
            (oldElements + i - 1)->~T();
        }
        free(oldElements);
        reserved++;
    }
    new (elements) T(rhs);
    count++;
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
    count--;
    
    (elements + count)->~T();
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
    count--;
    
    for (size_t i = 0; i < count; ++i) {
        (elements + i)->~T();
        new (elements + i) T(elements[i + 1]);
    }
    (elements + count)->~T();
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
    return elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
    return elements[count - 1];
}

template <typename T>
const T& Vector<T>::operator[](size_t i) const
{ // const element access
    return elements[i];
}

template <typename T>
T& Vector<T>::operator[](size_t i)
{//nonconst element access
    return elements[i];
}

template <typename T>
size_t Vector<T>::Size() const
{
    return count;
}

template <typename T>
bool Vector<T>::Empty() const
{
    return count == 0;
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
    
    for (size_t i = 0; i < count; ++i) {
        (elements + i)->~T();
    }
    count = 0;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
    return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
    return VectorIterator<T>(elements + count);
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
    count--;
    
    (it.current)->~T();
    size_t i;
    for (i = 0; (elements + i) != it.current; ++i) {
        // stop at erase index
    }
    for (i; i < count; ++i) {
        new (elements + i) T(elements[i + 1]);
        (elements + i + 1)->~T();
    }
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
    
    if (count == reserved) {
        count++;
        T* oldElements = elements;
        elements = (T*)malloc((reserved + 1) * sizeof(T));
        size_t i;
        for (i = 0; (oldElements + i) != it.current; ++i) {
            new (elements + i) T(oldElements[i]);
            (oldElements + i)->~T();
        }
        new (elements + i) T(rhs);
        for (++i; i < count; ++i) {
            new (elements + i) T(oldElements[i - 1]);
            (oldElements + i - 1)->~T();
        }
        free(oldElements);
        reserved++;
    } else {
        count++;
        size_t i;
        for (i = count; (elements + i) != it.current; --i) {
            new (elements + i) T(elements[i - 1]);
            (elements + i - 1)->~T();
        }
        new (elements + i) T(rhs);
    }
}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
: current(NULL)
{
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
: current(c)
{
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
: current(rhs.current)
{
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
    return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
    current++;
    return *this;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
    T* previous = current;
    current++;
    return VectorIterator<T>(previous);
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
    return current != rhs.current;
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
    return current == rhs.current;
}




