#pragma once

#include <QSharedPointer>

template <typename T>
class SingletonSharedPtrHolder
{
public:
    SingletonSharedPtrHolder(const SingletonSharedPtrHolder&) = delete;
    SingletonSharedPtrHolder &operator=(const SingletonSharedPtrHolder&) = delete;

    static void setData(T *p)
    {
        return instance().set_ptr(p);
    }

    static T *data()
    {
        return instance().ptr();
    }

    static bool isNull()
    {
        return instance().is_null();
    }

private:
    SingletonSharedPtrHolder() = default;

    static SingletonSharedPtrHolder<T> &instance()
    {
        static SingletonSharedPtrHolder<T> holder;
        return holder;
    }

    void set_ptr(T *p)
    {
        m_smartPointer.reset(p);
    }

    T *ptr() const
    {
        return m_smartPointer.data();
    }

    bool is_null() const
    {
        return m_smartPointer.isNull();
    }

    QSharedPointer<T> m_smartPointer;
};
