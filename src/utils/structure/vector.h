/*** 
 * Author       : Linloir
 * Date         : 2022-06-05 16:22:09
 * LastEditTime : 2022-06-07 14:41:07
 * Description  : 
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

#define _MIN_(a, b) ((a) < (b) ? (a) : (b))
#define _MAX_(a, b) ((a) < (b) ? (b) : (a))

template<typename T>
class Vec {
    private:
        unsigned int _size;
        unsigned int _capacity;
        const unsigned int _defaultCapacity = 4;
        T* _items;
        bool _setCapacity(unsigned int newCapacity);
        bool _ensureCapacity(unsigned int minCapacity);
    public:
        Vec();
        Vec(int capacity);
        Vec(const Vec<T>& other);
        ~Vec();
        void pushBack(const T& item);
        void popBack();
        void insert(unsigned int pos, const T& item);
        void erase(unsigned int pos);
        Vec<T>& operator+=(const Vec<T>& other);
        T& operator[](int index);
        T& at(int index);
        T& front();
        T& back();
        unsigned int size();
        unsigned int capacity();
        void clear();
        bool isEmpty();
};

#endif