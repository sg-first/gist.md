#ifndef HASH_HPP
#define HASH_HPP
#include <iostream>
#include <list>
using namespace std;

#define NONE 0 //必须设一个初始值表示空……本来应该用联合类型，不过先这样吧

class HashTable
{
public:
    HashTable()
    {
        for(int &i:this->list)
            i=NONE;
    }
    const int size=300;
    int list[300];
    int hashfun(HashTable &hashTable, int data) //哈希函数，这里是除留余数法，可以换别的
    {
        return data%hashTable.size;
    }
};

void insert(HashTable &hashTable, int data)
{
       //哈希函数：除留余数法
       int hashAddress = hashTable.hashfun(hashTable, data);
       while (hashTable.list[hashAddress]!=NONE) //冲突
       {
           hashAddress++;
           hashAddress=hashAddress%hashTable.size; //用开放定址法解决冲突
           //开放定址法：发生了冲突就去寻找下一个空的哈希地址，只要哈希表足够大，空的哈希地址总是能找到。然后将记录插入这个地址
       }
       hashTable.list[hashAddress]=data;
}

int search(HashTable &hashTable, int data)
{
    int hashAddress = hashTable.hashfun(hashTable,data);
    while (hashTable.list[hashAddress] != data)
    {
        hashAddress++;
        hashAddress=hashAddress%hashTable.size;  //用开放定址法解决冲突
        if (hashTable.list[hashAddress]==NONE || hashAddress==hashTable.hashfun(hashTable, data))
        {
            return -1; //循环回到原点，查找失败
        }
    }
    return hashAddress;
}

void display(HashTable &hashTable)
{
    for(int i:hashTable.list)
    {
        if(i!=NONE)
            cout<<i<<endl;
    }
}

#endif
