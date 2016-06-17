#pragma once

#include"Bit Map.h"

template<class K = string, 
		class HashFunc1 = __HashFunc1<K>, 
		class HashFunc2 = __HashFunc2<K>,
		class HashFunc3 = __HashFunc3<K>,
		class HashFunc4 = __HashFunc4<K>,
		class HashFunc5 = __HashFunc5<K>>
class BloomFilter
{
public:
	bool Set(const K& key)
	{
		size_t index1 = HashFunc1()(key);
		size_t index2 = HashFunc2()(key);
		size_t index3 = HashFunc3()(key);
		size_t index4 = HashFunc4()(key);
		size_t index5 = HashFunc5()(key);

		_bitMap.Set(index1 % _capacity);
		_bitMap.Set(index2 % _capacity);
		_bitMap.Set(index3 % _capacity);
		_bitMap.Set(index4 % _capacity);
		_bitMap.Set(index5 % _capacity);
	}

	bool IsIn(const K& key)
	{
		size_t index1 = HashFunc1()(key);
		size_t index2 = HashFunc2()(key);
		size_t index3 = HashFunc3()(key);
		size_t index4 = HashFunc4()(key);
		size_t index5 = HashFunc5()(key);

		if (!_bitMap.Test(index1 % _capacity))
		{
			return false;
		}
		if (!_bitMap.Test(index2 % _capacity))
		{
			return false;
		}
		if (!_bitMap.Test(index3 % _capacity))
		{
			return false;
		}
		if (!_bitMap.Test(index4 % _capacity))
		{
			return false;
		}
		if (!_bitMap.Test(index5 % _capacity))
		{
			return false;
		}

		return true;
	}

protected:
	BitMap _bitMap;
	size_t _capacity;
};