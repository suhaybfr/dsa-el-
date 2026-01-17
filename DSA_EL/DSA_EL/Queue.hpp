#pragma once

template<typename T, int MAX_SIZE = 100>
class Queue
{
public:
    Queue() : front(0), rear(0), count(0) {}

    bool push(const T& value)
    {
        if (count == MAX_SIZE)
            return false; // queue full

        data[rear] = value;
        rear = (rear + 1) % MAX_SIZE;
        count++;
        return true;
    }

    bool pop(T& out)
    {
        if (count == 0)
            return false; // queue empty

        out = data[front];
        front = (front + 1) % MAX_SIZE;
        count--;
        return true;
    }

    bool empty() const
    {
        return count == 0;
    }

    bool full() const
    {
        return count == MAX_SIZE;
    }

private:
    T data[MAX_SIZE];
    int front;
    int rear;
    int count;
};
