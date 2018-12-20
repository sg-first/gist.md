#include <stack>
#include <vector>
#include <string>
using namespace std;

class staStru //保存函数维持的临时变量
{
public:
    staStru(){}
};

class retType //函数返回值
{
public:
    retType(long r):r(r){}
    long r;
};

class parType //函数参数
{
public:
    parType(long n):n(n){}
    long n;
};

//运行时栈
stack<staStru> staStack;
stack<parType> parStack;
stack<retType> retStack;
stack<string> retAddressStack;

void retReady(retType ret) //即将返回，清除本地调用状态信息
{
    staStack.pop();
    parStack.pop();
    retStack.push(ret);
    //retAddressStack.pop(); //读取到返回地址后再pop
}

void callReady(parType par,staStru sta,string retAddress)
{
    parStack.push(par);
    staStack.push(sta);
    retAddressStack.push(retAddress);
}

retType getRet()
{
    retType ret=retStack.top();
    retStack.pop();
    return ret;
}

bool isEnd()
{
    return parStack.size()==0;
}

void clear()
{
    while(!staStack.empty()) staStack.pop();
    while(!parStack.empty()) parStack.pop();
    while(!retStack.empty()) retStack.pop();
    while(!retAddressStack.empty()) retAddressStack.pop();
}

#define CALL(par1,par2,par3)\
    callReady(par1,par2,par3);\
    goto _fun;

#define RETURN(par1)\
    retReady(par1);\
    string address;\
    if(retAddressStack.top()=="callPointer1") /*fix:有其它调用点在此添加*/ \
    {retAddressStack.pop(); goto callPointer1;}\
    if(retAddressStack.top()=="_relRet") /*终止条件要特判*/ \
    {_result=&retStack.top(); goto _relRet;}

//递归例子
/*long fact(long n)
{
    if(n==0||n==1)
        return 1;
    else
        return n*fact(n-1);
}*/

//将例子转换后：
long fact(long n)
{
    //带下划线的名都不能改
    retType *_result;
    CALL(parType(n),staStru(),"_relRet");

    _fun:
    if(parStack.top().n==0||parStack.top().n==1)
    {
        RETURN(retType(1)) //函数返回
    }
    else
    {
        CALL(parType(parStack.top().n-1),staStru(),"callPointer1") //递归调用
        ///////////////接受递归求值结果///////////////
        callPointer1: //标记调用点
        retType ret=getRet(); //调用结束，得到返回值
        //------------接受递归求值结果结束------------
        RETURN(retType(parStack.top().n*ret.r)) //函数返回
    }

    _relRet:
    return _result->r;
}

#include <iostream>
int main()
{
    cout<<fact(5);
}
