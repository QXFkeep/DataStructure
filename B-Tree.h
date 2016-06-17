#pragma once

#pragma warning(disable:4018)

template<class K, class V, int M = 3>
struct BTreeNode
{
	typedef BTreeNode<K, V, M> Node;

	K _key[M];    //�ؼ�������
	V _value[M];  //��Ϣ����
	Node* _subs[M + 1];  //ָ���ӽڵ��ָ������
	size_t size;         //�ؼ��ֵĸ���
	Node* _parent;       //ֻ�븸�׽ڵ��ָ��

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
	Pair<Node*, bool> _Find(const K& key);  //����(����ֵ��һ��ָ��B-Tree�ڵ��ָ�룬���Ƿ��ҵ��ýڵ�true/false)
	bool _Insert(const K& key, const V& value, Node* sub = NULL);  //����
	void _InOrder()      //�������
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
	if (NULL == _root)   //���ڵ�Ϊ��
	{
		_root = new Node();
		_root->_key[0] = key;
		_root->_value[0] = value;
		_root->size++;
	}
	else   //���ڵ㲻Ϊ��
	{
		Pair<Node*, bool> exist = _Find(key);
		if (exist._second)  //����ùؼ����Ѿ�����
		{
			cout << "This key already exist!" << endl;
			return false;
		}
		else     //B���л�û�д˹ؼ��֣���ʱӦ�ò�����Ӧ����Ϣ
		{
			Node* cur = exist._first;
			Node* before = NULL;
			Node* tmp = sub;
			K middlekey = key;
			V middlevalue = value;
			while (1)
			{
				//��һ�η������֮��_keys[middle]��Ҫ�����׽ڵ��в���
				//���׽ڵ����Ϊ��
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
					cur->_key[i] = cur->_key[i - 1];    //���ؼ��ֺ���
					cur->_value[i] = cur->_value[i - 1]; //���ؼ��ֶ�Ӧ����Ч��Ϣ����
					cur->_subs[i + 1] = cur->_subs[i];   //ֻ�뺢�ӽڵ��ָ�����
				}

				cur->_key[index] = middlekey;            //�ƶ���֮����Ӧ�����ݸ���
				cur->_value[index] = middlevalue;
				cur->_subs[index + 1] = tmp;             //tmp�Ƿ��ѳ������½ڵ�

				if (tmp)
				{
					tmp->_parent = cur;
				}
				++cur->size;

				if (cur->size < M)    //�ؼ��ֵĸ����Ϸ�
				{
					return true;
				}
				else                 //�ؼ��ֵĸ����Ƿ�(M���ؼ���)
				{
					int middle = M / 2;
					int position = 0;
					int size = cur->size;
					Node* _tmp = new Node();

					for (int i = middle + 1; i <= cur->size; ++i)  //���Ұ�߷��ѳ�ȥ
					{
						_tmp->_key[position] = cur->_key[i];
						_tmp->_value[position] = cur->_value[i];
						_tmp->_subs[position] = cur->_subs[i];
						--cur->size;
						++_tmp->size;
					}
					_tmp->_subs[_tmp->size] = cur->_subs[size];
					middlekey = cur->_key[middle];    //���ϲ���Ĺؼ���
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

//�������
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
	bt._Insert(53, "���ݽṹ");
	bt._Insert(75, "Linux");
	bt._Insert(139, "�㷨����");
	bt._Insert(49, "��ָoffer");
	bt._Insert(145, "C++ Primer");
	bt._Insert(36, "����ϵͳ");
	bt._Insert(101, "�����ԭ��");

	bt._InOrder();
}