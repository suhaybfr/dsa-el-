#pragma once

template <typename T>
class DynamicArray
{
public:
    DynamicArray()
        : m_data(nullptr), m_size(0), m_capacity(0) {
    }

    ~DynamicArray()
    {
        delete[] m_data;
    }

    void push_back(const T& value)
    {
        if (m_size >= m_capacity)
            resize();

        m_data[m_size++] = value;
    }

    T& operator[](int index)
    {
        return m_data[index];
    }

    int size() const
    {
        return m_size;
    }

private:
    void resize()
    {
        int newCapacity = (m_capacity == 0) ? 2 : m_capacity * 2;
        T* newData = new T[newCapacity];

        for (int i = 0; i < m_size; ++i)
            newData[i] = m_data[i];

        delete[] m_data;
        m_data = newData;
        m_capacity = newCapacity;
    }

private:
    T* m_data;
    int m_size;
    int m_capacity;
};
