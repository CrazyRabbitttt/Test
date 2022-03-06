#ifndef MYALLOCATOR_H_
#define MYALLOCATOR_H_
#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

namespace  MA
{
    template<typename  T>

    inline
    T * _allocate(ptrdiff_t size, T*)       //ptrdiff_t : signed
    {
        T * tmp = static_cast<T*>(::operator new(static_cast<size_t>(size * sizeof (T))));
        if (tmp == nullptr)
        {
            std::cerr << "Out of memory" << std::endl;
        }
        return tmp;
    }
}

template<typename T>
inline
void _deallocator(T * buffer)
{
    ::operator delete (buffer);         //会调用析构函数的
}

template<typename T1, typename T2>
inline
void _constructor (T1 *p, const T2 & value)
{
    //就是使用new创建对象，强转类型
    new (p) T1(value);          //在分配好的空间p上调用构造函数
                                //解释：https://blog.csdn.net/xiaorenwuzyh/article/details/44514815
}

template<typename T>
inline
void _destory(T * p)
{
    p -> ~T();      //直接调用析构函数
}

template<typename  T>
struct allocator{
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    template<typename U>
    struct rebind{
        using other = allocator<U>;
    };

    pointer allocate(size_type n, const void * hint = 0)
    {
        return MA::_allocate(static_cast<difference_type>(n), static_cast<pointer>(nullptr));
    }

    void deallocate(pointer p, size_type n)
    {
        _deallocator(p);
    }

    void construct (pointer p, const T &value)
    {
        _constructor(p, value);
    }

    void destory(pointer p)
    {
        _destory(p);
    }

    pointer address(reference x)
    {
        return static_cast<pointer> (&x);
    }

    size_type max_size() const
    {
        return static_cast<size_type>(UINT_MAX / sizeof(T));
    }

};


















#endif MYALLOCATOR_H_