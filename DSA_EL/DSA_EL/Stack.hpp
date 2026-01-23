#pragma once

template<typename T, int MAX_SIZE = 100>
class Stack
{
public:
    // Initialize data array to zeros/default
    Stack() : top(-1), data() {}

    bool push(const T& value)
    {
        if (top >= MAX_SIZE - 1)
            return false;   // stack overflow

        data[++top] = value;
        return true;
    }

    bool pop(T& out)
    {
        if (top < 0)
            return false;   // stack underflow

        out = data[top--];
        return true;
    }

    bool empty() const
    {
        return top < 0;
    }

private:
    T data[MAX_SIZE];
    int top;
};
