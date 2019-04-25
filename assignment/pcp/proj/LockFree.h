#include <iostream>
#include <atomic>
#include <cstdio>
#include <limits.h>
#include <vector>

template <typename T>
class atomwrapper
{
  public:
    std::atomic<T> _a;

    atomwrapper()
        : _a()
    {
    }

    atomwrapper(const std::atomic<T> &a)
        : _a(a.load())
    {
    }

    atomwrapper(const atomwrapper &other)
        : _a(other._a.load())
    {
    }

    atomwrapper &operator=(const atomwrapper &other)
    {
        _a.store(other._a.load());
    }
    T load()
    {
        return _a.load();
    }
    void store(T elem)
    {
        _a.store(elem);
    }
    bool compare_exchange_strong(T old, T newb)
    {
        return _a.compare_exchange_strong(old, newb);
    }
};

template <class T>
class MarkableReference
{
  private:
    uintptr_t val;
    static const uintptr_t mask = 1;

  public:
    MarkableReference(T *ref = NULL, bool mark = false)
    {
        val = ((uintptr_t)ref & ~mask) | (mark ? 1 : 0);
    }
    T *getRef(void)
    {
        return (T *)(val & ~mask);
    }
    bool getMark(void)
    {
        return (val & mask);
    }
};

template <class T>
class LockFreeSkipList
{
    static const int MAX_LEVEL=15;

    std::default_random_engine generator{time(NULL)};
    std::geometric_distribution<int> distribution{0.5};

    int randomLevel(){
        int temp = distribution(generator) + 1;
        if(temp < MAX_LEVEL)    
            return temp;
        
        return MAX_LEVEL;
    } // randomLevel

    template <class G>
    class Node
    {
        G value;

      public:
        int topLevel;
        int64_t key;
        std::vector<atomwrapper<MarkableReference<Node<G>>>> next;

        Node(int keyy)
        {
            value = 0;
            key = keyy;
            MarkableReference<Node<G>> elem(NULL, false);
            atomwrapper<MarkableReference<Node<G>>> temp(elem);
            for (int i = 0; i < MAX_LEVEL + 1; i++)
            {
                next.push_back(temp);
            }
            topLevel = MAX_LEVEL;
        }

        Node(G x, int height)
        {
            value = x;
            key = x;
            MarkableReference<Node> elem(NULL, false);
            atomwrapper<MarkableReference<Node<G>>> temp(elem);
            for (int i = 0; i < height + 1; i++)
                next.push_back(temp);
            topLevel = height;
        }
    };

    Node<T> *head = new Node<T>(INT_MIN);
    Node<T> *tail = new Node<T>(INT_MAX);

    bool find(T x, std::vector<Node<T> *> &preds, std::vector<Node<T> *> &succs)
    {
        int bottomLevel = 0;
        int key = x;
        bool snip;
        Node<T> *pred = NULL, *curr = NULL, *succ = NULL;
    retry:
        while (true)
        {
            MarkableReference<Node<T>> mark;
            pred = head;
            for (int level = MAX_LEVEL; level >= bottomLevel; level--)
            {
                mark = pred->next[level].load();
                curr = mark.getRef();

                while (true)
                {

                    mark = curr->next[level].load();
                    bool marked = mark.getMark();
                    Node<T> *succ = mark.getRef();
                    while (marked)
                    {

                        snip = pred->next[level].compare_exchange_strong(curr, succ);
                        if (!snip)
                            goto retry;

                        curr = pred->next[level].load().getRef();
                        succ = curr->next[level].load().getRef();
                        marked = curr->next[level].load().getMark();
                    }
                    if (curr->key < key)
                    {
                        pred = curr;
                        curr = succ;
                    }
                    else
                        break;
                }
                preds[level] = pred;
                succs[level] = curr;
            }
            return curr->key == key;
        }
    }

  public:
    LockFreeSkipList()
    {
        MarkableReference<Node<T>> mark(tail, false);
        atomwrapper<MarkableReference<Node<T>>> toIns(mark);
        for (int i = 0; i < head->next.size(); i++)
        {
            head->next[i] = toIns;
        }
    }

    bool add(T x)
    {
        int topLevel = randomLevel();
        int bottomLevel = 0;
        std::vector<Node<T> *> preds;
        preds.resize(MAX_LEVEL + 1);
        std::vector<Node<T> *> succs;
        succs.resize(MAX_LEVEL + 1);
        while (1)
        {

            bool found = find(x, preds, succs);

            if (found)
            {
                return false;
            }
            else
            {

                Node<T> *newNode = new Node<T>(x, topLevel);
                for (int level = bottomLevel; level <= topLevel; level++)
                {
                    Node<T> *succ = succs[level];
                    MarkableReference<Node<T>> elem(succ, false);
                    newNode->next[level].store(elem);
                }

                Node<T> *pred = preds[bottomLevel];
                Node<T> *succ = succs[bottomLevel];

                MarkableReference<Node<T>> oldMark(succ, false);
                if (!pred->next[bottomLevel].compare_exchange_strong(oldMark, MarkableReference<Node<T>>(newNode, false)))
                {
                    continue;
                }
                for (int level = bottomLevel + 1; level <= topLevel; level++)
                {
                    while (true)
                    {
                        pred = preds[level];
                        succ = succs[level];
                        if (pred->next[level].compare_exchange_strong(
                                MarkableReference<Node<T>>(succ, false), MarkableReference<Node<T>>(newNode, false)))
                            break;
                        find(x, preds, succs);
                    }
                }
                return true;
            }
        }
    }

    bool remove(T x)
    {
        int bottomLevel = 0;
        std::vector<Node<T> *> preds;
        preds.resize(MAX_LEVEL + 1);
        std::vector<Node<T> *> succs;
        succs.resize(MAX_LEVEL + 1);
        Node<T> *succ;
        while (1)
        {
            bool found = find(x, preds, succs);
            if (!found)
            {
                return false;
            }
            else
            {
                Node<T> *nodeToRemove = succs[bottomLevel];
                for (int level = nodeToRemove->topLevel; level >= bottomLevel + 1; level--)
                {
                    bool marked = nodeToRemove->next[level].load().getMark();
                    succ = nodeToRemove->next[level].load().getRef();
                    while (!marked)
                    {
                        nodeToRemove->next[level].compare_exchange_strong(
                            MarkableReference<Node<T>>(succ, false), MarkableReference<Node<T>>(succ, true));
                        marked = nodeToRemove->next[level].load().getMark();
                        succ = nodeToRemove->next[level].load().getRef();
                    }
                }
                bool marked = false;
                marked = nodeToRemove->next[bottomLevel].load().getMark();
                succ = nodeToRemove->next[bottomLevel].load().getRef();
                while (1)
                {
                    bool iMarkedIt = nodeToRemove->next[bottomLevel].compare_exchange_strong(
                        MarkableReference<Node<T>>(succ, false), MarkableReference<Node<T>>(succ, true));
                    marked = succs[bottomLevel]->next[bottomLevel].load().getMark();
                    succ = succs[bottomLevel]->next[bottomLevel].load().getRef();
                    if (iMarkedIt)
                    {
                        find(x, preds, succs);
                        return true;
                    }
                    else if (marked)
                    {
                        return false;
                    }
                }
            }
        }
    }

    bool contains(T x)
    {
        int bottomLevel = 0;
        int v = x;
        bool marked = false;
        Node<T> *pred = head, *curr = NULL, *succ = NULL;
        for (int level = MAX_LEVEL; level >= bottomLevel; level--)
        {
            curr = pred->next[level].load().getRef();
            while (1)
            {
                marked = curr->next[level].load().getMark();
                succ = curr->next[level].load().getRef();
                while (marked)
                {
                    curr = pred->next[level].load().getRef();

                    marked = curr->next[level].load().getMark();
                    succ = curr->next[level].load().getRef();
                }
                if (curr->key < v)
                {
                    pred = curr;
                    curr = succ;
                }
                else
                    break;
            }
        }
        return curr->key == v;
    }
};
