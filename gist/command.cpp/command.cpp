#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef void(*pfun)(vector<string>);

struct command
{
    string cmdName;
    pfun cmdfun;
};

//需要设置的参数
string separator;//命令与参数间的分隔符
string prompt;//进入程序时的信息
vector<command> allcmd;
void setpar(string Separator,string Prompt,vector<string>allcmdname,vector<pfun>allcmdfun)
{
    for(unsigned int i=0;i<allcmdname.size();i++)//allcmdname和allcmdfun元素数必然相同
    {
        command cmd{allcmdname.at(i),allcmdfun.at(i)};
        allcmd.push_back(cmd);
    }
    separator=Separator;
    prompt=Prompt;
}

vector<string> Split(string o_str,string fenge)//这个还是好使的
{
    vector<string> str_list;
    int comma_n = 0;
    while(1)
    {
        string tmp_s = "";//整一个缓存的字符串
        const char *ccfengge=fenge.c_str();
        comma_n = o_str.find(ccfengge);//查找分隔符第一次出现的位置
        if(comma_n==-1)//要是没找到的话
        {
            str_list.push_back(o_str);//把剩下的全整进去
            break;
        }
        tmp_s = o_str.substr( 0, comma_n );//将这一块截断
        o_str.erase(0,comma_n+fenge.length());//将这一块抹去
        str_list.push_back(tmp_s);//将截断出来的字符加入容器
    }
    return str_list;
}

template<typename T>
void deleteAt(vector<T>&vec,int sub)
{
    //std::vector<T>::iterator it = vec.begin()+sub;
    typename std::vector<T>::const_iterator it = vec.begin()+sub;
    vec.erase(it);
}

void judgefun(string input)
{
    vector<string> vect=Split(input,separator);//分割input到vect
    string fun=vect.at(0);
    deleteAt(vect,0);//把命令名删掉
    for(auto i : allcmd)
    {
        if(fun==i.cmdName)
        {i.cmdfun(vect);}
    }
}

int main(int argc, char *argv[])
{
    //setpar();//在这里设置参数
    //先执行命令行参数
    if(argc!=0)
    {
        for(auto i : allcmd)
        {
            if(argv[1]==i.cmdName)
            {
                vector<string> vect;
                for(int i=2;i<argc;i++)//把这些参数连在一起
                {vect.push_back(argv[i]);}
                i.cmdfun(vect);
            }
        }
    }
    cout<<prompt<<endl;
    string input="";
    while(1)
    {
        cin>>input;
        judgefun(input);
    }
}