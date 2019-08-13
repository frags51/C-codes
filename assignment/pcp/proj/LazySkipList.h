/**
 * PCP Project
 * Fine Grained Lock-based SkipList - Set (Unique Elements Only)
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


/*
 * The SkipListClass
 * The G in Node<G> may or may not be copy constructible, we are 'new' for that. 
 */

template <typename T> class LazySkipList{
	/**
	 * The max- height of the skip-list. 
	 * Can be made non const member variable.
	 * Optimally should be ~log(N) where N is the total elements in the list.
	 */
	static const int MAX_LEVEL = 15;

	/**
	 * These are required for selecting a random level to add to the list.
	 * Chosen according to a geometric distribution as mentioned in the book.
	 */
	std::default_random_engine generator{time(NULL)};
	std::geometric_distribution<int> distribution{0.5};

	/**
	 * Choose a random level for adding a new object, acc to geometric distribution
	 * @return The level where new element should be added.
	 */
	int randomLevel(){
		int temp = distribution(generator) + 1;
		if(temp < MAX_LEVEL) 	
			return temp;
		
		return MAX_LEVEL;
	} // randomLevel

	/**
	 * The Node Class
	 */
	template <typename G> class Node{
	private:
		/**
		 * Re-entrant lock.
		 */
		std::recursive_mutex mtx;
	
	public:
		/**
		 * Member vars: Actual stored element, the hash value etc.
		 */
		const G item;
		/**
		 * Could use a better hash fxn for this key, but it doesn't matter for 
		 * demonstration purposes.
		 */
		const int64_t key;
		
		/**
		 * Per Level next nodes.
		 */
		std::vector<Node<G>*> next;
		std::atomic<bool> marked {false};
		std::atomic<bool> fullyLinked {false};

	public:
		int topLevel;
	public:
		/**
		 * Constructors
		 */
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

		/**
		 * Lock functions
		 */
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

	/**
	 * Sentinel Nodes
	 */
	Node<T>* head = new Node<T>(INT64_MIN);
	Node<T>* tail = new Node<T>(INT64_MAX);

	/**
	 * For garbage collection
	 * @param garbage An unordered set that stores pointers to be deleted.
	 */
	void delGarbage(std::unordered_set<Node<T>*> &garbage){
		for(auto v: garbage) delete v;
	} // delGarbage

public:
	/**
	 * Constructor
	 */
	LazySkipList(){
		int s = (head->topLevel)+1;
		//cout<<s<<endl;
		for (int i=0; i<s; i++) {
			delete head->next[i];
			head->next[i]=tail;
		}
	}
	/**
	 * Destructor, garbage collection
	 */
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

	/**
	 * Find function
	 * @param  x     Object to look for
	 * @param  preds A vector to store the preds per level
	 * @param  succs A vector to store the succs per level
	 * @return       The level at which this object was found
	 */
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

	/**
	 * Add an element to the skiplist.
	 * @param  x The object to add.
	 * @return   True if the object was added successfuly, false else.
	 */
	bool add(T x){
		/**
		 * Initialize the vectors, get the level to insert at.
		 */

		int topLevel = randomLevel();
		std::vector<Node<T>*> preds(MAX_LEVEL+1);
		std::vector<Node<T>*> succs(MAX_LEVEL+1);
		for(int i=0; i<MAX_LEVEL+1; i++) preds[i] = new Node<T>();
		for(int i=0; i<MAX_LEVEL+1; i++) succs[i] = new Node<T>();
		
		// garbage collection
		std::unordered_set<Node<T>*> garbage{};
		for(int i=0; i<MAX_LEVEL+1; i++) garbage.insert(preds[i]);
		for(int i=0; i<MAX_LEVEL+1; i++) garbage.insert(succs[i]);

		/**
		 * Try to insert
		 */
		while(true){
			int lFound = find(x, preds, succs);
			/**
			 * In Case object already found
			 */
			if(lFound!=-1){
				Node<T>* nodeFound = succs[lFound];
				/**
				 * Already there, don't add again.
				 */
				if(!nodeFound->marked.load()){
					while(!nodeFound->fullyLinked.load());
					delGarbage(garbage);
					
					return false;
				}
				/**
				 * Was there but was marked, try again.
				 */
				continue;
			} // if Lfound!=-1

			/**
			 * Not found, try to add.
			 */
			int highestLocked = -1;

			Node<T> * pred;
			Node<T> * succ;
			bool valid = true;

			/**
			 * Acquire lock for each level upto the level we have to add.
			 */
			for(int level=0; valid && (level<=topLevel); level++){
				pred = preds[level];
				succ = succs[level];
				pred->lock();
				highestLocked  = level;
				valid = !pred->marked && !succ->marked && pred->next[level]==succ;
			} // for
			if(!valid) {
				for (int level = 0; level <= highestLocked; level++) 
					preds[level]->unlock();
				continue;
			}

			/**
			 * Physical Insertion
			 */
			Node<T>* newNode = new Node<T>(x, topLevel);
			for (int level=0; level<=topLevel; level++) newNode->next[level] = succs[level];
			for (int level=0; level<=topLevel; level++) preds[level]->next[level]=newNode;
			
			newNode->fullyLinked = true; // successful add linearization point

			/**
			 * Do unlock!
			 */
			for (int level = 0; level <= highestLocked; level++) 
				preds[level]->unlock();
			delGarbage(garbage);
			return true;
		}// while(true)

	} // add ends

	/**
	 * Removes the element from the list if it exists.
	 * @param  x The element to remove.
	 * @return   True if succesfully removed, False else.
	 */
	bool remove(T x){
		/**
		 * Initialize.
		 */
		Node<T>* victim=NULL;
		bool isMarked=false;
		int topLevel=-1;

		std::vector<Node<T>*> preds(MAX_LEVEL+1);
		std::vector<Node<T>*> succs(MAX_LEVEL+1);
		for(int i=0; i<MAX_LEVEL+1; i++) preds[i] = new Node<T>();
		for(int i=0; i<MAX_LEVEL+1; i++) succs[i] = new Node<T>();

		/**
		 * Garbage collection
		 */
		std::unordered_set<Node<T>*> garbage{};
		for(int i=0; i<MAX_LEVEL+1; i++) garbage.insert(preds[i]);
		for(int i=0; i<MAX_LEVEL+1; i++) garbage.insert(succs[i]);

		/**
		 * Try to remove.
		 */
		while(true){
			int lFound = find(x, preds, succs);
			if (lFound != -1) victim = succs[lFound];
			/**
			 * If present
			 */
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
				
				/**
				 * Try to acquire locks upto required level.
				 */
				int highestLocked=-1;
				Node<T>* pred, succ; bool valid = true;
				for (int level = 0; valid && (level <= topLevel); level++){
					pred = preds[level];
					pred->lock();
					highestLocked = level;
					valid = !pred->marked && pred->next[level]==victim;
				} // for level=0

				if(!valid) {
					for (int level = 0; level <= highestLocked; level++) 
						preds[level]->unlock();
					continue;
				}
				/**
				 * Physical unlinking.
				 */
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

	/**
	 * Check if the list contains an element.
	 * @param  x Element to look for.
	 * @return   True if it exists, false else.
	 */
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

	/**
	 * Display the whole list.
	 * @param delim Separator for list elements.
	 */
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
