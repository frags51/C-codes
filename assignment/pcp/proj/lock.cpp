#include <iostream>
#include <mutex>
#include <atomic>			/* Emulation of java's volatile */
#include <vector>			/* Vectors instead of heap based arrays */
#include <cstdint>			/* For int64_t min, max vals*/
using namespace std;

/*
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
*/

// Node<G> stores a G* 
template <typename T> class LazySkipList{
	static const int MAX_LEVEL = 8;

	template <typename G> class Node{
	public:
		const G* item;
		const int64_t key;
		
		std::recursive_mutex mtx;
		std::vector<Node<G>*> next;
		std::atomic<bool> marked {false};
		std::atomic<bool> fullyLinked {false};
	public:
		int topLevel;
	public:
		Node(): key(-10), item(NULL) {;}
		Node(int64_t k): key(k), item(NULL){
			
			next.reserve(MAX_LEVEL+1);
			for(int i=0; i<MAX_LEVEL+1;i++) next[i] = new Node();
			topLevel = MAX_LEVEL;
		}
		Node(G* x, int64_t height): item(x), key((int64_t) x){
			next.reserve(height+1);
			for(int i=0; i<height+1;i++) next[i] = new Node();
			topLevel = height;
		}

		void lock(){
			mtx.lock();
		}
		void unlock(){
			mtx.unlock();
		}

		~Node(){
			for(int i=0; i<next.size();i++) delete next[i];
		}

	}; // Node<T> ends

	Node<T>* head = new Node<T>(INT64_MIN);
	Node<T>* tail = new Node<T>(INT64_MAX);

public:
	LazySkipList(){
		int s = (head->topLevel)+1;
		cout<<s<<endl;
		// TODO : Add deletion for the pointers that were alreay om head->next[]
		for (int i=0; i<s; i++) head->next[i]=tail;
	}
	
	~LazySkipList(){
		delete head;
		delete tail;
	}

	int find(T* x, std::vector<Node<T>*> preds, std::vector<Node<T>*> succs){
		int64_t key  = (int64_t) x;
		int lFound = -1;
		
		Node<T>* pred = head;
		for(int level=MAX_LEVEL;level>=0; level--){
			Node<T>* curr = pred->next[level];
			while(key>curr->key){
				pred = curr; curr = pred->next[level];
			}
			if (lFound == -1 && key == curr->key) lFound = level;

			preds[level] = pred;
			succs[level] = curr;
		}
		return lFound;
	} // find

	bool add(T* x){
		
	}
};



int main(){
	auto b = new LazySkipList<int>();
	delete b;
	//auto a = new LazySkipList<int>::Node<int>(5);
	cout<<INT64_MIN<<"\n";
}