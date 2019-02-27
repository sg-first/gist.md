#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
using namespace std;

class binarySerialize
{
public:
	template <class T>
	static bool saveStruObject(string path, int len, T* content)
	{
		FILE *fp=fopen(path.data(), "wb");
		if(fp==NULL)
			return false;
		else
		{
			fwrite(content, sizeof(T), len, fp);
			fclose(fp);
			return true;
		}
	}

	template <class T>
	static bool loadStruObject(string path, int len, T* space)
	{
		FILE *fp=fopen(path.data(), "rb");
		if(fp==NULL)
			return false;
		else
		{
			fread(space,sizeof(T),len,fp);
			fclose(fp);
			return true;
		}
	}

	template <class T>
	static bool loadClasObject(string path, int len, T* space)
	{
		fstream infile(path,ios::in | ios::binary); //以读方式打开文件
		if (!infile)
			return false;
		else
		{
			infile.read((char*)space,sizeof(T)*len);
			infile.close();
			return true;
		}
	}

	template <class T>
	static bool saveClasObject(string path, int len, T* content)
	{
		fstream outfile(path, ios::app | ios::binary); //这里以app方式打开文件进行添加
		if (!outfile)
			return false;
		{
			outfile.write((char*)content, sizeof(T)*len);
			outfile.close();
			return true;
		}
	}
};