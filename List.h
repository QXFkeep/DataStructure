#pragma once

#include "Iterator.h"

template<class T>
struct __ListNode
{
	__ListNode<T>* _prev;
	__ListNode<T>* _next;
	T _data;

	__ListNode(const T& x = T())
		:_data(x)
		,_next(NULL)
		,_prev(NULL)
	{
	}
};

template<class T, class Ref, class Ptr>
struct __ListIterator
{
	typedef __ListNode<T> Node;
	typedef Node* LinkType;

	typedef BidirectionalIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef Ptr Pointer; // const T*
	typedef Ref Reference; // const T&
	typedef ptrdiff_t DifferenceType;

	typedef __ListIterator<T, Ref, Ptr>  Self;

public:
	__ListIterator()
	{}

	__ListIterator(LinkType node)
		:_node(node)
	{}

	Self operator++()
	{
		_node = _node->_next;
		return *this;
	}

	Self operator++(int)
	{
		LinkType tmp(_node);

		_node = _node->_next;
		return Self(tmp);
	}

	Self operator--()
	{
		_node = _node->_prev;
		return *this;
	}

	Self operator--(int)
	{
		LinkType tmp(_node);

		_node = _node->_prev;
		return Self(tmp);
	}

	Reference operator*()
	{
		return _node->_data;
	}

	Pointer operator->()
	{
		return &(_node->_data);
	}

	bool operator==(Self& it)
	{
		return _node == it._node;
	}

	bool operator !=(Self& it)
	{
		return _node != it._node;
	}

//protected:
	LinkType _node;
};


template<class T>
class List
{
	 typedef __ListNode<T> Node;
	 typedef Node* LinkType;
public:

	typedef __ListIterator<T, T&, T*> Iterator;
	typedef __ListIterator<T, const T&, const T*> ConstIterator;

	typedef ReverseIterator<Iterator> ReverseIterator;

	List()
		:_head(new Node())
	{
		_head->_next = _head;
		_head->_prev = _head;
	}

	/*void PushBack(const T& x)
	{
		LinkType tail = _head->_prev;
		LinkType tmp = new Node(x);
		tail->_next = tmp;
		tmp->_prev = tail;
		tail = tmp;

		_head->_prev = tail;
		tail->_next = _head;
	}*/

	Iterator Begin()
	{
		return Iterator(_head->_next);
	}

	Iterator End()
	{
		return Iterator(_head);
	}

	ConstIterator Begin() const
	{
		return ConstIterator(_head->_next);
	}

	ConstIterator End() const
	{
		return ConstIterator(_head);
	}

	ReverseIterator RBegin()
	{
		return ReverseIterator(End());
	}

	ReverseIterator REnd()
	{
		return ReverseIterator(Begin());
	}


	// pos前插入一个节点
	Iterator Insert(Iterator pos, const T& x)
	{
		Node* cur = pos._node;
		Node* prev = cur->_prev;

		Node* tmp = new Node(x);
		tmp->_next = cur;
		cur->_prev = tmp;

		prev->_next = tmp;
		tmp->_prev = prev;
		
		return tmp;
	}

	Iterator Erase(Iterator pos)
	{
		assert(pos != End());

		Node* next = pos._node->_next;
		Node* prev = pos._node->_prev;

		Node* del = pos._node;

		prev->_next = next;
		next->_prev = prev;

		delete del;

		return Iterator(next);
	}

	void PushBack(const T& x)
	{
		Insert(End(), x);
	}

	void PopBack()
	{
		// assert
		Erase(--End());
	}

	void PushFront(const T& x)
	{
		Insert(Begin(), x);
	}

	void PopFront()
	{
		// assert
		Erase(Begin());
	}

	void Reverse();
	void Merge(List<T>& list);
	void Unique();

protected:
	LinkType _head;
	//Node _head;
};

void PrintList1(const List<int>& l)
{
	List<int>::ConstIterator it = l.Begin();
	while (it != l.End())
	{
		//cout<<*it<<" ";
		//*it = 10;
		cout<<*it<<" ";

		++it;
	}
	cout<<endl;
}

void PrintList2(List<int>& l)
{
	List<int>::Iterator it = l.Begin();
	while (it != l.End())
	{
		*it = 10;
		cout<<*it<<" ";

		++it;
	}
	cout<<endl;
}

void PrintList3(List<int>& l)
{
	List<int>::ReverseIterator it = l.RBegin();
	while (it != l.REnd())
	{
		cout<<*it<<" ";

		++it;
	}
	cout<<endl;
}

void TestList()
{
	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	l.PushBack(5);

	//PrintList1(l);
	//PrintList2(l);
	PrintList3(l);
}