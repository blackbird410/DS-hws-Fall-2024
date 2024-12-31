using namespace std;

template <class T> class List;

template <class T>
class ListNode {
    friend class List <T>;
    private:
    T data;
    ListNode <T> *link;
};

template <class T>
class List {
    public:
	List() {first = last = 0;}
	void Attach(T k);
    private:
	ListNode<T> *first;
	ListNode<T> *last;
};


template <class Type>
void List<Type>::Attach(Type k)
{
    ListNode<Type> *newnode = 
	new ListNode<Type>(k);
    if (first == 0) 
	first = last = newnode;
    else {
	last->link = newnode;
	last = newnode;
    }
};
