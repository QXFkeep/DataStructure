#pragma once

#include <memory.h>
#include<vector>
#include<iostream>
using namespace std;
//定义每个Byte中有8个Bit位

#define BYTESIZE 8

void SetBit(char *p, int posi)
{
	for (int i = 0; i <(posi / BYTESIZE); i++)
	{
		p++;
	}

	*p = *p | (0x01 << (posi%BYTESIZE));//将该Bit位赋值1  按位取或，相同为0，不同为1
	return;
}

void BitMapSortDemo()
{
	//为了简单起见，我们不考虑负数
	int num[] = { 3, 5, 2, 10, 6, 12, 8, 14, 9 };
	int size = sizeof(num) / sizeof(int);    //数组大小
	cout << "原数组为:" << endl;
	for (int i = 0; i < size; ++i)
	{
		cout << num[i] << " ";
	}
	cout << endl;
	//BufferLen这个值是根据待排序的数据中最大值确定的待排序中的最大值是14，因此只需要2个Bytes(16个Bit)就可以了。
	const int BufferLen = 2;
	char *pBuffer = new char[BufferLen];

	//要将所有的Bit位置为0，否则结果不可预知。
	memset(pBuffer, 0, BufferLen);
	for (int i = 0; i<size; i++)
	{
		//首先将相应Bit位上置为1
		SetBit(pBuffer, num[i]);
	}

	//输出排序结果
	cout << "BitMap排序后的数组为:" << endl;
	for (int i = 0; i<BufferLen; i++)    //每次处理一个字节(Byte)
	{
		for (int j = 0; j<BYTESIZE; j++) //处理该字节中的每个Bit位
		{
			//判断该位上是否是1，进行输出，这里的判断比较笨。
			//首先得到该第j位的掩码（0x01<<j），将内存区中的位和此掩码作与操作。最后判断掩码是否和处理后的结果相同
			if ((*pBuffer&(0x01 << j)) == (0x01 << j))  //将1移位后，按位进行与运算，相同位1，不同位0
			{
				cout << i*BYTESIZE + j << " ";  //计算打印出比特位上对应的值
			}
		}
		pBuffer++;
	}
	cout << endl;
}


//实例
//1)已知某个文件内包含一些电话号码，每个号码为8位数字，统计不同号码的个数。 
//    8位最多99 999 999，大概需要99m个bit，大概10几m字节的内存即可。 （可以理解为从0 - 99 999 999的数字，每个数字对应一个Bit位，所以只需要99M个Bit == 1.2MBytes，这样，就用了小小的1.2M左右的内存表示了所有的8位数的电话）

//2)2.5亿个整数中找出不重复的整数的个数，内存空间不足以容纳这2.5亿个整数。

//    将bit_map扩展一下，用2bit表示一个数即可，0表示未出现，1表示出现一次，2表示出现2次及以上，在遍历这些数的时候，如果对应位置的值是0，则将其置为1；如果是1，将其置为2；如果是2，则保持不变。或者我们不用2bit来进行表示，我们用两个bit_map即可模拟实现这个2bit_map，都是一样的道理。




class BitMap
{
public:
	BitMap()
		:_size(0)
	{}
	BitMap(size_t size)
		:_size(0)
	{
		_arrays.resize((size >> 5) + 1);
	}

	bool Set(size_t num)
	{
		size_t index = num >> 5;  //
		size_t n = num % 32;      //32个bit位

		if (_arrays[index] & (0x01 << n))
		{
			return false;
		}

		_arrays[index] |= (0x01 << n);
		++_size;
		return true;
	}

	/////////////////////////////////////
	void ReSize(size_t size)
	{
		_size = ((size >> 5) + 1);
	}
	/////////////////////////////////////

	bool ReSet(size_t num)
	{
		size_t index = num >> 5;
		size_t n = num % 32;

		if (_arrays[index] & (0x01 << n))
		{
			_arrays[index] &= (~(0x01 << n));
			--_size;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Test(size_t num)
	{
		size_t index = num >> 5;
		size_t n = num % 32;

		return _arrays[index] & (0x01 << n);
	}

	void Clear()
	{
		_arrays.assign(_arrays.size(), 0);
	}

private:
	vector<size_t> _arrays;
	size_t _size;
};

void BitMapTest()
{
	BitMap bm(65);
	bm.Set(1);
	bm.Set(4);
	bm.Set(33);

	cout << "1?" << bm.Test(1) << endl;
	cout << "2?" << bm.Test(2) << endl;
	cout << "4?" << bm.Test(4) << endl;
	cout << "33?" << bm.Test(33) << endl;

	bm.ReSet(33);
	bm.ReSet(4);

	cout << "1?" << bm.Test(1) << endl;
	cout << "2?" << bm.Test(2) << endl;
	cout << "4?" << bm.Test(4) << endl;
	cout << "33?" << bm.Test(33) << endl;
}

void BitMapTest2()
{
	BitMap bm(-1);
	bm.Set(10000000);
}