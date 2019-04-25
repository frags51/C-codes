#include <iostream>
#include <mutex>
#include <atomic>			/* Emulation of java's volatile */
#include <vector>			/* Vectors instead of heap based arrays */
#include <cstdint>			/* For int64_t min, max vals*/
#include <random>			/* For random level selection! */
#include <cstdlib>			/*	time() */
#include <ctime>
using namespace std;


// Node<G> stores a G: Must be copy constructible!
template <typename T> class LazySkipList{
	static const int MAX_LEVEL = 8;

	// required for level selection!
	std::default_random_engine generator{time(NULL)};
	std::geometric_distribution<int> distribution{0.5};

	int randomLevel(){
		int temp = distribution(generator) + 1;
		if(temp < MAX_LEVEL) 	
			return temp;
		
		return MAX_LEVEL;
	} // randomLevel

	// The Node Class
	template <typename G> class Node{
	public:
		const G item;
		const int64_t key;
		
		std::recursive_mutex mtx;
		std::vector<Node<G>*> next;
		std::atomic<bool> marked {false};
		std::atomic<bool> fullyLinked {false};
	public:
		int topLevel;
	public:
		Node(): key(-10), item(0) {;}
		Node(int64_t k): key(k), item(0){
			
			next.reserve(MAX_LEVEL+1);
			for(int i=0; i<MAX_LEVEL+1;i++) next[i] = new Node();
			topLevel = MAX_LEVEL;
		}
		Node(G x, int64_t height): item(x), key((int64_t) x){
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

	int find(T x, std::vector<Node<T>*> &preds, std::vector<Node<T>*> &succs){
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

	

	bool add(T x){
		int topLevel = randomLevel();
		std::vector<Node<T>*> preds;
		std::vector<Node<T>*> succs;
		for(int i=0; i<MAX_LEVEL+1; i++) preds[i] = new Node<T>();
		for(int i=0; i<MAX_LEVEL+1; i++) succs[i] = new Node<T>();

		while(true){
			int lFound = find(x, preds, succs);
			if(lFound!=-1){
				Node<T>* nodeFound = succs[lFound];
				if(!nodeFound->marked.load()){
					while(!nodeFound->fullyLinked.load());
					return false;
				}
				continue;
			} // if Lfound!=-1

			int highestLocked = -1;

			Node<T> * pred, succ;
			bool valid = true;
			for(int level=0; valid && (level<=topLevel); level++){
				pred = preds[level];
				succ = succs[level];
				pred->lock();
				highestLocked  = level;
				valid = !pred->marked && !succ->marked && pred->next[level]==succ;
			} // for
			if(!valid) continue;

			Node<T>* newNode = new Node<T>(x, topLevel);
			for (int level=0; level<=topLevel; level++) newNode->next[level] = succs[level];
			for (int level=0; level<=topLevel; level++) preds[level]->next[level]=newNode;
			
			newNode->fullyLinked = true; // successful add linearization point

			// unlock!
			for (int level = 0; level <= highestLocked; level++) 
				preds[level]->unlock();

			return true;
		}// while(true)

	} // add ends
};



int main(){
	auto b = new LazySkipList<int>();
	delete b;
	//auto a = new LazySkipList<int>::Node<int>(5);
	cout<<INT64_MIN<<"\n";
}