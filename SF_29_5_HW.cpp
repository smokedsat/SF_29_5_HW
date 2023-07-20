#include <mutex>
#include <thread>
#include <iostream>


struct Node
{
    int value;
    Node *next;
    std::mutex * node_mutex;

    Node(int val) : value(val), next(nullptr), node_mutex(new std::mutex())
    {

    }
};

class FineGrainedQueue
{
private:
    Node * head;
    std::mutex * queue_mutex;

public:

    FineGrainedQueue() : head(nullptr), queue_mutex(new std::mutex())
    {

    }

    void insertIntoMiddle(int value, int pos)
    {
        Node * newNode = new Node(value);
        std::lock_guard<std::mutex> queueLock(*queue_mutex);

        Node * prev = nullptr;
        Node *current = head;

        int count = 0;

        // Ищем место для вставки нового узла
        if(current)
        {
            while(current && count < pos)
            {
                prev = current;
                current = current->next;
                ++count;
            }
        }

        // Вставляем новый узел в середину или в конец списка
        if(prev)
        {
            std::lock_guard<std::mutex> prevLock(*prev->node_mutex);
            prev->next = newNode;
            newNode->next = current;
        }
        else
        {
            // Если prev == nullptr, значит вставляем в начало, что не нужно
            std::cerr << "Error. INsertion at the beg is not supped";
            delete newNode;
        }
    }
};


