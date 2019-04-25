/**
 * PCP Project
 * Lock-based SkipList
 * Supreet Singh, Mayank Hooda
 */
#ifndef __LZ_SKP_LST 
#define __LZ_SKP_LST

#include <mutex>			/* Recursive Mutex */
#include <atomic>			/* Emulation of java's volatile */
#include <vector>			/* Vectors instead of heap based arrays */
#include <cstdint>			/* For int64_t min, max vals*/
#include <random>			/* For random level selection! */
#include <cstdlib>			/*	time() */
#include <ctime>
#include <unordered_set>	/* Garbage Colllection */
//#include <jemalloc/jemalloc.h> /* -ljemalloc, sudo apt install libjemalloc-dev*/
using namespace std;


// Node<G> stores a G: Must be copy constructible!
template <typename T> class LazySkipList{
	static const int MAX_LEVEL = 15;
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
			for(int i=0; i<MAX_LEVEL+1;i++) next.push_back(new Node());
			topLevel = MAX_LEVEL;
		}
		Node(G x, int64_t height): item(x), key((int64_t) x){
			next.reserve(height+1);
			for(int i=0; i<height+1;i++) next.push_back(new Node());
			//cout<<"height=="<<height<<", sz: "<<next.size()<<endl;
			topLevel = height;
		}

		void lock(){
			mtx.lock();
		}
		void unlock(){
			mtx.unlock();
		}

		~Node(){
			//for(int i=0; i<next.size();i++) delete next[i];
		}

	}; // Node<T> ends

	Node<T>* head = new Node<T>(INT64_MIN);
	Node<T>* tail = new Node<T>(INT64_MAX);

	void delGarbage(std::unordered_set<Node<T>*> &garbage){
		for(auto v: garbage) delete v;
		//garbage.clear();
	} // delGarbage

public:
	LazySkipList(){
		int s = (head->topLevel)+1;
		//cout<<s<<endl;
		for (int i=0; i<s; i++) {
			delete head->next[i];
			head->next[i]=tail;
		}
	}
	
	~LazySkipList(){
		Node<T>* p = head->next[0];
		Node<T>* q;
		while(p!=tail){
			q = p->next[0];
			//cout<<"delling ~LazySkipList(): "<<p<<endl;			
			delete p;
			p = q;
		}
		delete head;
		int s = (tail->topLevel)+1;
		for (int i=0; i<s; i++) {
			delete tail->next[i];
		}
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
		std::vector<Node<T>*> preds(MAX_LEVEL+1);
		std::vector<Node<T>*> succs(MAX_LEVEL+1);
		for(int i=0; i<MAX_LEVEL+1; i++) preds[i] = new Node<T>();
		for(int i=0; i<MAX_LEVEL+1; i++) succs[i] = new Node<T>();
		// garbage collection
		std::unordered_set<Node<T>*> garbage{};
		for(int i=0; i<MAX_LEVEL+1; i++) garbage.insert(preds[i]);
		for(int i=0; i<MAX_LEVEL+1; i++) garbage.insert(succs[i]);

		while(true){
			int lFound = find(x, preds, succs);
			if(lFound!=-1){
				Node<T>* nodeFound = succs[lFound];
				if(!nodeFound->marked.load()){
					while(!nodeFound->fullyLinked.load());
					delGarbage(garbage);

					return false;
				}
				continue;
			} // if Lfound!=-1

			int highestLocked = -1;

			Node<T> * pred;
			Node<T> * succ;
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
			delGarbage(garbage);
			return true;
		}// while(true)

	} // add ends

	bool remove(T x){
		Node<T>* victim=NULL;
		bool isMarked=false;
		int topLevel=-1;

		std::vector<Node<T>*> preds(MAX_LEVEL+1);
		std::vector<Node<T>*> succs(MAX_LEVEL+1);
		for(int i=0; i<MAX_LEVEL+1; i++) preds[i] = new Node<T>();
		for(int i=0; i<MAX_LEVEL+1; i++) succs[i] = new Node<T>();

		// garbage collection!
		std::unordered_set<Node<T>*> garbage{};
		for(int i=0; i<MAX_LEVEL+1; i++) garbage.insert(preds[i]);
		for(int i=0; i<MAX_LEVEL+1; i++) garbage.insert(succs[i]);

		while(true){
			int lFound = find(x, preds, succs);
			if (lFound != -1) victim = succs[lFound];

			if(isMarked||
				 (lFound!=-1&&
				 	(victim->fullyLinked && victim->topLevel==lFound &&!victim->marked))){
				if(!isMarked){
					topLevel = victim->topLevel;
					victim->lock();
					if(victim->marked){
						victim->unlock();
						delGarbage(garbage);

						return false;
					} // victim marked
					victim->marked=true;
					isMarked=true;
				} // if !isMarked 
			
				int highestLocked=-1;
				Node<T>* pred, succ; bool valid = true;
				for (int level = 0; valid && (level <= topLevel); level++){
					pred = preds[level];
					pred->lock();
					highestLocked = level;
					valid = !pred->marked && pred->next[level]==victim;
				} // for level=0
				if(!valid) continue;
				
				for (int level = topLevel; level >= 0; level--) {
					preds[level]->next[level] = victim->next[level];
				} // for level

				victim->unlock();
				for (int i = 0; i <= highestLocked; i++) 
					preds[i]->unlock();
				
				delGarbage(garbage);

				return true;
			} // big if
			else {
				delGarbage(garbage);

				return false;
			}
		} // while true
	} // remove

	bool contains(T x){
		std::vector<Node<T>*> preds(MAX_LEVEL+1);
		std::vector<Node<T>*> succs(MAX_LEVEL+1);
		for(int i=0; i<MAX_LEVEL+1; i++) preds[i] = new Node<T>();
		for(int i=0; i<MAX_LEVEL+1; i++) succs[i] = new Node<T>();

		std::unordered_set<Node<T>*> garbage{};
		for(int i=0; i<MAX_LEVEL+1; i++) garbage.insert(preds[i]);
		for(int i=0; i<MAX_LEVEL+1; i++) garbage.insert(succs[i]);

		int lFound = find(x, preds, succs);
		delGarbage(garbage);
		return (lFound != -1 && succs[lFound]->fullyLinked &&  !succs[lFound]->marked);
	}

	void display(char delim){
		cout<<"Display\n";
		Node<T>* p = head->next[0];
		while(p!=tail){
			//cout<<"ptr: "<<p<<endl;
			cout<<p->key<<delim;
			p = p->next[0];
		}
		cout<<endl;
	}
}; // class LazySkipList<T>

#endif