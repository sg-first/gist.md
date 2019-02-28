#pragma once
#define maxlen 100

template <class T> //要求T支持空构造函数和拷贝构造函数
class iarray
{
private:
	T ary[maxlen];
	void copy(const T ary[], int insertSize)
	{
		for (int i = 0; i < this->insertSize; i++)
			this->ary[i] = ary[i];
	}

public:
	iarray(bool isValType=true) : isValType(isValType) {}
	iarray(const iarray& ary) { *this = ary; }
	iarray& operator=(const iarray& ary) //用于重赋值？
	{
		if(!this->isValType)
			delete []this->ary; //当前的全部释放
		this->isValType = ary.isValType;
		this->insertSize = ary.insertSize;
		this->copy(ary.ary, ary.insertSize);
		return *this;
	}
	iarray(T ary[], int insertSize)
	{
		this->copy(ary, insertSize);
	}
	~iarray()
	{
		if(!this->isValType)
			delete []this->ary;
	}

	bool isValType;
	int insertSize = 0;

	T& operator[] (int i) { return this->ary[i]; }

	/*int find(T val)
	{
		for (int i = 0; i < size; i++)
		{
			if (this->ary[i] == val)
				return i;
		}
		return -1;
	}*/

	void push(T val)
	{
		this->ary[insertSize] = val;
		this->insertSize++;
	}

	void delSub(int sub)
	{
		this->insertSize--;
		for (int i = sub; i < insertSize; i++)
			this->ary[i] = this->ary[i + 1];
	}

	/*void delVal(int val)
	{
		int sub = this->find(val);
		while (sub != -1)
		{
			this->delSub(sub);
			sub = this->find(val);
		}
	}*/
};