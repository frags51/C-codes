#include <iostream>
#include <mutex>
#include <atomic>

using namespace std;

template<class T>
class MarkableReference
{
private:
    uintptr_t val;
    static const uintptr_t mask = 1;
public:
    MarkableReference(T* ref = NULL, bool mark = false)
    {
        val = ((uintptr_t)ref & ~mask) | (mark ? 1 : 0);
    }
    T* getRef(void)
    {
        return (T*)(val & ~mask);
    }
    bool getMark(void)
    {
        return (val & mask);
    }
};


template <typename T> class LazySkipList{
	static const int MAX_LEVEL = 5;
	
	template <typename G> class Node{
	public:
		const T* value;
		const int key;
		atomic<MarkableReference<LazySkipList::Node<G>>*> *next;

	private:
		int topLevel;
	public:
	
		Node(int k): key(k), value(NULL){
			
			next = new atomic<MarkableReference<LazySkipList::Node<G>>*>[MAX_LEVEL+1];
			
			for (int i=0; i<MAX_LEVEL+1;i++){
				MarkableReference<LazySkipList::Node<G>>* x = 
				new MarkableReference<LazySkipList::Node<G>>(NULL, false);
				next[i].store(x);
			}
			topLevel = MAX_LEVEL;
		}

	}; // Node<T> ends 
};

int main(){
	auto a = new LazySkipList<int>::Node<int>(5);
	cout<<(a->next[0].load()->getRef() == NULL ? "Null": "Not NULL");
}