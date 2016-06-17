#pragma once

#include <memory.h>
#include<vector>
#include<iostream>
using namespace std;
//����ÿ��Byte����8��Bitλ

#define BYTESIZE 8

void SetBit(char *p, int posi)
{
	for (int i = 0; i <(posi / BYTESIZE); i++)
	{
		p++;
	}

	*p = *p | (0x01 << (posi%BYTESIZE));//����Bitλ��ֵ1  ��λȡ����ͬΪ0����ͬΪ1
	return;
}

void BitMapSortDemo()
{
	//Ϊ�˼���������ǲ����Ǹ���
	int num[] = { 3, 5, 2, 10, 6, 12, 8, 14, 9 };
	int size = sizeof(num) / sizeof(int);    //�����С
	cout << "ԭ����Ϊ:" << endl;
	for (int i = 0; i < size; ++i)
	{
		cout << num[i] << " ";
	}
	cout << endl;
	//BufferLen���ֵ�Ǹ��ݴ���������������ֵȷ���Ĵ������е����ֵ��14�����ֻ��Ҫ2��Bytes(16��Bit)�Ϳ����ˡ�
	const int BufferLen = 2;
	char *pBuffer = new char[BufferLen];

	//Ҫ�����е�Bitλ��Ϊ0������������Ԥ֪��
	memset(pBuffer, 0, BufferLen);
	for (int i = 0; i<size; i++)
	{
		//���Ƚ���ӦBitλ����Ϊ1
		SetBit(pBuffer, num[i]);
	}

	//���������
	cout << "BitMap����������Ϊ:" << endl;
	for (int i = 0; i<BufferLen; i++)    //ÿ�δ���һ���ֽ�(Byte)
	{
		for (int j = 0; j<BYTESIZE; j++) //������ֽ��е�ÿ��Bitλ
		{
			//�жϸ�λ���Ƿ���1�����������������жϱȽϱ���
			//���ȵõ��õ�jλ�����루0x01<<j�������ڴ����е�λ�ʹ������������������ж������Ƿ�ʹ����Ľ����ͬ
			if ((*pBuffer&(0x01 << j)) == (0x01 << j))  //��1��λ�󣬰�λ���������㣬��ͬλ1����ͬλ0
			{
				cout << i*BYTESIZE + j << " ";  //�����ӡ������λ�϶�Ӧ��ֵ
			}
		}
		pBuffer++;
	}
	cout << endl;
}


//ʵ��
//1)��֪ĳ���ļ��ڰ���һЩ�绰���룬ÿ������Ϊ8λ���֣�ͳ�Ʋ�ͬ����ĸ����� 
//    8λ���99 999 999�������Ҫ99m��bit�����10��m�ֽڵ��ڴ漴�ɡ� ���������Ϊ��0 - 99 999 999�����֣�ÿ�����ֶ�Ӧһ��Bitλ������ֻ��Ҫ99M��Bit == 1.2MBytes��������������СС��1.2M���ҵ��ڴ��ʾ�����е�8λ���ĵ绰��

//2)2.5�ڸ��������ҳ����ظ��������ĸ������ڴ�ռ䲻����������2.5�ڸ�������

//    ��bit_map��չһ�£���2bit��ʾһ�������ɣ�0��ʾδ���֣�1��ʾ����һ�Σ�2��ʾ����2�μ����ϣ��ڱ�����Щ����ʱ�������Ӧλ�õ�ֵ��0��������Ϊ1�������1��������Ϊ2�������2���򱣳ֲ��䡣�������ǲ���2bit�����б�ʾ������������bit_map����ģ��ʵ�����2bit_map������һ���ĵ���




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
		size_t n = num % 32;      //32��bitλ

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