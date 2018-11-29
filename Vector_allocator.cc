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
        allocator<T> alloc;
        elements = alloc.allocate(reserved);
        for (size_t i = 0; i < count; ++i) {
            alloc.construct(elements + i, rhs.elements[i]);
        }
    }
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
    if (&rhs != this) {
        allocator<T> alloc;
        for (size_t i = 0; i < count; ++i) {
            alloc.destroy(elements + i);
        }
        alloc.deallocate(elements, reserved);
        elements = NULL;
        count = rhs.count;
        reserved = rhs.reserved;
        if (reserved > 0) {
            elements = alloc.allocate(reserved);
            for (size_t i = 0; i < count; ++i) {
                alloc.contruct(elements + i, rhs.elements[i]);
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
    allocator<T> alloc;
    elements = alloc.allocate(reserved);
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
: elements(NULL), count(n), reserved(n)
{ // Initialize with "n" copies of "t"
    allocator<T> alloc;
    elements = alloc.allocate(reserved);
    for (size_t i = 0; i < count; ++i) {
        alloc.construct(elements + i, t);
    }
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{ // Reserve extra memory
    if (n > reserved) {
        T* oldElements = elements;
        allocator<T> alloc;
        elements = alloc.allocate(n);
        for (size_t i = 0; i < count; ++i) {
            alloc.construct(elements + i, oldElements[i]);
            alloc.destroy(oldElements + i);
        }
        alloc.deallocate(oldElements, reserved);
        reserved = n;
    }
}

#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
    allocator<T> alloc;
    for (size_t i = 0; i < count; ++i) {
        alloc.destroy(elements + i);
    }
    alloc.deallocate(elements, reserved);
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
    allocator<T> alloc;
    if (count == reserved) {
        T* oldElements = elements;
        elements = alloc.allocate(reserved + 1);
        for (size_t i = 0; i < count; ++i) {
            alloc.construct(elements + i, oldElements[i]);
            alloc.destroy(oldElements + i);
        }
        alloc.deallocate(oldElements, reserved);
        reserved++;
    }
    alloc.construct(elements + count, rhs);
    count++;
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
    allocator<T> alloc;
    if (count == reserved) {
        T* oldElements = elements;
        elements = alloc.allocate(reserved + 1);
        for (size_t i = count; i > 0; --i) {
            alloc.construct(elements + i, oldElements[i - 1]);
            alloc.destroy(oldElements + i - 1);
        }
        alloc.deallocate(oldElements, reserved);
        reserved++;
    }
    alloc.construct(elements, rhs);
    count++;
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
    count--;
    allocator<T> alloc;
    alloc.destroy(elements + count);
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
    count--;
    allocator<T> alloc;
    for (size_t i = 0; i < count; ++i) {
        alloc.destroy(elements + i);
        alloc.construct(elements + i, elements[i + 1]);
    }
    alloc.destroy(elements + count);
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
    allocator<T> alloc;
    for (size_t i = 0; i < count; ++i) {
        alloc.destroy(elements + i);
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
    allocator<T> alloc;
    alloc.destroy(it.current);
    size_t i;
    for (i = 0; (elements + i) != it.current; ++i) {
        // stop at erase index
    }
    for (i; i < count; ++i) {
        alloc.construct(elements + i, elements[i + 1]);
        alloc.destroy(elements + i + 1);
    }
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
    allocator<T> alloc;
    if (count == reserved) {
        count++;
        T* oldElements = elements;
        elements = alloc.allocate(reserved + 1);
        size_t i;
        for (i = 0; (oldElements + i) != it.current; ++i) {
            alloc.construct(elements + i, oldElements[i]);
            alloc.destroy(oldElements + i);
        }
        alloc.construct(elements + i, rhs);
        for (++i; i < count; ++i) {
            alloc.construct(elements + i, oldElements[i - 1]);
            alloc.destroy(oldElements + i - 1);
        }
        alloc.deallocate(oldElements, reserved);
        reserved++;
    } else {
        count++;
        size_t i;
        for (i = count; (elements + i) != it.current; --i) {
            alloc.construct(elements + i, elements[i - 1]);
            alloc.destroy(elements + i - 1);
        }
        alloc.construct(elements + i, rhs);
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




