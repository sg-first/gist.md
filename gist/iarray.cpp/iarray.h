#pragma once
#include<iostream>
using namespace std;

class iarray
{
private:
    int size;
    int *ary;
    void copy(int* ary)
    {
        for(int i=0;i<this->size;i++)
            this->ary[i]=ary[i];
    }
    void init(int size)
    {
        this->insertSize=0;
        this->size=size;
        this->ary=new int[size];
    }

public:
    iarray(int size=100)
    {
        this->init(size);
    }
    iarray(const iarray& ary)
    {
        this->ary=0;
        *this=ary;
    }
    iarray& operator=(const iarray& ary)
    {
        this->init(ary.size);
        this->copy(ary.ary);
        this->insertSize=ary.insertSize;
        return *this;
    }
    iarray(int size,int ary[])
    {
        this->init(size);
        this->copy(ary);
    }
    ~iarray()
    {
        delete []this->ary;
    }

    int getSize() {return this->size;}
    int insertSize;

    void relPrint(int s)
    {
        cout<<"[";
        for(int i=0;i<s;i++)
            cout<<ary[i]<<", ";
        cout<<"]"<<endl;
    }

    void printAll() {this->relPrint(this->size);}
    void print() {this->relPrint(this->insertSize);}

    int& operator[] (int i) {return this->ary[i];}

    int find(int val)
    {
        for(int i=0;i<size;i++)
        {
            if(this->ary[i]==val)
                return i;
        }
        return -1;
    }

    void push(int val)
    {
        this->ary[insertSize]=val;
        this->insertSize++;
    }

    void delSub(int sub)
    {
        this->insertSize--;
        for(int i=sub;i<insertSize;i++)
            this->ary[i]=this->ary[i+1];
    }

    void delVal(int val)
    {
        int sub=this->find(val);
        while(sub!=-1)
        {
            this->delSub(sub);
            sub=this->find(val);
        }
    }
};