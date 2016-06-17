#pragma once

#pragma warning(disable:4018)

template<class K, class V, int M = 3>
struct BTreeNode
{
	typedef BTreeNode<K, V, M> Node;

	K _key[M];    //关键字数组
	V _value[M];  //信息数组
	Node* _subs[M + 1];  //指向孩子节点的指针数组
	size_t size;         //关键字的个数
	Node* _parent;       //只想父亲节点的指针

	BTreeNode()
		:size(0)
		, _parent(NULL)
	{
		for (int i = 0; i < M + 1; ++i)
		{
			_subs[i] = NULL;
		}
	}
};

template<class K, class V>
struct Pair
{
	K _first;
	V _second;

	Pair(const K& key = K(), const V& value = V())
		:_first(key)
		, _second(value)
	{}
};


template<class K, class V, int M = 3>
class BTree
{
	typedef BTreeNode<K, V, M> Node;

public:
	BTree()
		:_root(NULL)
	{}

public:
	Pair<Node*, bool> _Find(const K& key);  //查找(返回值是一个指向B-Tree节点的指针，和是否找到该节点true/false)
	bool _Insert(const K& key, const V& value, Node* sub = NULL);  //插入
	void _InOrder()      //中序遍历
	{
		InOrder(_root);
	}
	void InOrder(Node* root);
protected:
	Node* _root;
};

template<class K, class V, int M = 3>
Pair<BTreeNode<K, V, M>*, bool> BTree<K, V, M>::_Find(const K& key)
{
	Node* parent = NULL;
	Node* cur = _root;
	if (NULL == cur)
	{
		return Pair<Node*, bool>(cur, false);
	}
	else
	{
		while (cur)
		{
			int i = 0;
			for (; (key > cur->_key[i]) && (i < cur->size); ++i);
			if (key == cur->_key[i])
			{
				return Pair<Node*, bool>(cur, true);
			}
			else
			{
				parent = cur;
				cur = cur->_subs[i];
			}	
		}
		return Pair<Node*, bool>(parent, false);
	}
}

template<class K, class V, int M = 3>
bool BTree<K, V, M>::_Insert(const K& key, const V& value, BTreeNode<K, V, M>* sub = NULL)
{
	if (NULL == _root)   //根节点为空
	{
		_root = new Node();
		_root->_key[0] = key;
		_root->_value[0] = value;
		_root->size++;
	}
	else   //根节点不为空
	{
		Pair<Node*, bool> exist = _Find(key);
		if (exist._second)  //如果该关键字已经存在
		{
			cout << "This key already exist!" << endl;
			return false;
		}
		else     //B树中还没有此关键字，此时应该插入相应的信息
		{
			Node* cur = exist._first;
			Node* before = NULL;
			Node* tmp = sub;
			K middlekey = key;
			V middlevalue = value;
			while (1)
			{
				//第一次分裂完成之后_keys[middle]，要往父亲节点中插入
				//父亲节点可能为空
				if (NULL == cur)
				{
					Node* parent = new Node();
					parent->_key[0] = middlekey;
					parent->_value[0] = middlevalue;
					++parent->size;
					parent->_subs[0] = before;
					before->_parent = parent;
					parent->_subs[1] = tmp;
					tmp->_parent = parent;
					_root = parent;
					return true;
				}

				int index = _BinarySearch<K>(cur->_key, cur->size, key);
				for (int i = cur->size; i > index; --i)
				{
					cur->_key[i] = cur->_key[i - 1];    //将关键字后移
					cur->_value[i] = cur->_value[i - 1]; //将关键字对应的有效信息后移
					cur->_subs[i + 1] = cur->_subs[i];   //只想孩子节点的指针后移
				}

				cur->_key[index] = middlekey;            //移动好之后相应的数据更新
				cur->_value[index] = middlevalue;
				cur->_subs[index + 1] = tmp;             //tmp是分裂出来的新节点

				if (tmp)
				{
					tmp->_parent = cur;
				}
				++cur->size;

				if (cur->size < M)    //关键字的个数合法
				{
					return true;
				}
				else                 //关键字的个数非法(M个关键字)
				{
					int middle = M / 2;
					int position = 0;
					int size = cur->size;
					Node* _tmp = new Node();

					for (int i = middle + 1; i <= cur->size; ++i)  //将右半边分裂出去
					{
						_tmp->_key[position] = cur->_key[i];
						_tmp->_value[position] = cur->_value[i];
						_tmp->_subs[position] = cur->_subs[i];
						--cur->size;
						++_tmp->size;
					}
					_tmp->_subs[_tmp->size] = cur->_subs[size];
					middlekey = cur->_key[middle];    //往上插入的关键字
					middlevalue = cur->_value[middle];
					--cur->size;

					before = cur;
					cur = cur->_parent;
					tmp = _tmp;
				}

			}
		}
	}
}


template<class K>
int _BinarySearch(const K* keys, int size, const K& key)            //???????????
{
	assert(keys);
	int low = 0;
	int high = size - 1;
	while (low < high)
	{
		int middle = (high - low) / 2 + low;
		key > keys[middle] ? (low = middle + 1) : (high = middle - 1);
	}
	return (key > keys[low] ? low + 1 : low);
}

//中序遍历
template<class K, class V, int M = 3>
void BTree<K, V, M>::InOrder(Node* root)
{
	if (root == NULL)
	{
		return;
	}
	else
	{
		for (int i = 0; i < root->size; ++i)
		{
			InOrder(root->_subs[i]);
			cout << "[" << root->_key[i] << "]" << ":"<<root->_value[i] << endl;
		}
		InOrder(root->_subs[root->size]);
	}
}

void BTreeTest()
{
	BTree<int, string> bt;
	bt._Insert(53, "数据结构");
	bt._Insert(75, "Linux");
	bt._Insert(139, "算法导论");
	bt._Insert(49, "剑指offer");
	bt._Insert(145, "C++ Primer");
	bt._Insert(36, "操作系统");
	bt._Insert(101, "计算机原理");

	bt._InOrder();
}