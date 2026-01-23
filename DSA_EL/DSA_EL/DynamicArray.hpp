#pragma once

template <typename T>
class DynamicArray
{
public:
    DynamicArray()
        : m_data(nullptr), m_size(0), m_capacity(0) {
    }

    // Copy constructor
    DynamicArray(const DynamicArray& other)
        : m_data(nullptr), m_size(0), m_capacity(0)
    {
        if (other.m_size > 0)
        {
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            m_data = new T[m_capacity];
            for (int i = 0; i < m_size; ++i)
                m_data[i] = other.m_data[i];
        }
    }

    // Copy assignment operator
    DynamicArray& operator=(const DynamicArray& other)
    {
        if (this != &other)
        {
            delete[] m_data;
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;

            if (other.m_size > 0)
            {
                m_capacity = other.m_capacity;
                m_size = other.m_size;
                m_data = new T[m_capacity];
                for (int i = 0; i < m_size; ++i)
                    m_data[i] = other.m_data[i];
            }
        }
        return *this;
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

    const T& operator[](int index) const
    {
        return m_data[index];
    }

    int size() const
    {
        return m_size;
    }

    void clear()
    {
        m_size = 0;
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
