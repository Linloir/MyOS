/*** 
 * Author       : Linloir
 * Date         : 2022-06-05 16:22:14
 * LastEditTime : 2022-06-06 20:53:22
 * Description  : 
 */

#include "vector.h"

template<typename T>
bool Vec<T>::_setCapacity(unsigned int newCapacity) {
    if(newCapacity == _capacity)  return true;
    if(newCapacity <= 0)  return false;
    
    T* newItems = (T*)malloc(sizeof(T) * newCapacity);
    if(newItems == nullptr) return false;
    
    _size = _MIN_(_size, newCapacity);
    for(int i = 0; i < _size; i++) {
        newItems[i] = _items[i];
    }
    free(_items);
    _items = newItems;

    return true;
}

template<typename T>
bool Vec<T>::_ensureCapacity(unsigned int minCapacity) {
    if(_capacity >= minCapacity)  return true;

    unsigned int newSize = _MAX_(minCapacity, _capacity * 2);
    return _setCapacity(newSize);
}

template<typename T>
Vec<T>::Vec(): _size(0) {
    _capacity = _defaultCapacity;
    _items = (T*)malloc(sizeof(T) * _capacity);
}

template<typename T>
Vec<T>::Vec(int capacity): _size(0), _capacity(capacity) {
    _items = (T*)malloc(sizeof(T) * _capacity);
    if(_items == nullptr) {
        _capacity = _defaultCapacity;
        _items = (T*)malloc(sizeof(T) * _capacity);
    }
}

template<typename T>
Vec<T>::Vec(const Vec<T>& other) {
    _size = other._size;
    _capacity = other._capacity;
    _items = (T*)malloc(sizeof(T) * _capacity);
    if(_items == nullptr) {
        //@TODO do something
    }
    for(int i = 0; i < _size; i++) {
        _items[i] = other._items[i];
    }
}

template<typename T>
Vec<T>::~Vec() {
    free(_items);
}

template<typename T>
void Vec<T>::pushBack(const T& val) {
    if(!_ensureCapacity(_size + 1))  return;
    _items[_size++] = val;
}

template<typename T>
void Vec<T>::popBack() {
    if(_size <= 0)  return;
    _size--;
}

template<typename T>
void Vec<T>::insert(unsigned int pos, const T& item) {
    if(!_ensureCapacity(_size + 1))  return;
    for(int i = _size; i > pos; i--) {
        _items[i] = _items[i - 1];
    }
    _items[pos] = item;
    _size++;
}

template<typename T>
void Vec<T>::erase(unsigned int pos) {
    if(pos < 0 || pos >= _size)  return;
    if(_size <= 0)  return;
    for(int i = pos; i < _size; i++) {
        _items[i] = _items[i + 1];
    }
    _size--;
}

template<typename T>
T& Vec<T>::operator[](int index) {
    if(index < 0 || index >= _size) {
        //@TODO do something
    }
    return _items[index];
}

template<typename T>
T& Vec<T>::at(int index) {
    if(index < 0 || index >= _size) {
        //@TODO do something
    }
    return _items[index];
}

template<typename T>
unsigned int Vec<T>::size() {
    return _size;
}

template<typename T>
unsigned int Vec<T>::capacity() {
    return _capacity;
}

template<typename T>
void Vec<T>::clear() {
    _size = 0;
    free(_items);
    _items = (T*)malloc(sizeof(T) * _defaultCapacity);
    _capacity = _defaultCapacity;
    _size = 0;
}

template<typename T>
bool Vec<T>::isEmpty() {
    return _size == 0;
}