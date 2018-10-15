//参数1为搜索的路径，参数2为文件名
void CheckAllFile(string sPath,string name)
{
	CFileFind ff; 
	if(sPath.Right(1) != "\\") //保证目录是以\结尾的 
	{sPath += "\\"; }
	string panding;
	panding=sPath;
	panding+=name;
	sPath += "*.*";
	BOOL bFound = ff.FindFile(sPath); 
	while(bFound)
	{ 
		bFound = ff.FindNextFile(); 
		string sFilePath = ff.GetFilePath();
		if (sFilePath==panding)
		{
			//找到文件执行的操作（panding为文件路径）
		}
		sFilePath = ff.GetFilePath(); 
		if(ff.IsDirectory())//IsDirectory()为检测此路径是否指向目录
		{ 
			if(!ff.IsDots())//IsDots()检测此路径是否为缺省目录
			CheckAllFile(sFilePath,name);
		}
		//else是函数原来的找到文件执行的操作，但似乎用不上，待调试
	} 
	ff.Close();
}