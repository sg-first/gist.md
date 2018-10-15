#include <afxinet.h>//FTP操作

string GetProgramPath()
{
	TCHAR _szPath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, _szPath, MAX_PATH);
	(_tcsrchr(_szPath, _T('\\')))[1] = 0;
	string strPath;
	for (int n=0;_szPath[n];n++)
	{
		if (_szPath[n]!=_T('\\'))
		{strPath +=_szPath[n] ;}
		else
		{strPath += _T("\\");}
	}
	return strPath;
}

void OpenProgram(LPCSTR name,LPSTR caner)
{
	STARTUPINFO si; 
    memset(&si, 0, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
    PROCESS_INFORMATION pi;
    CreateProcess(name,caner,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
}

string ReadFile(string name)
{
    ifstream infile(name);//用来打开文件
    string word="";//全部的
    string tmp;//在读的当前行
    while(getline(infile,tmp))
    {word+=tmp+"\n";}
    word=word.substr(0,word.length()-2);//删除最后的换行
    return word;
}

void updates(CString directory,CString newfile,CString IP,CString user,CString password)
{
	string path=GetProgramPath();
	CString cspath=op.c_str();
	DeleteFile(cspath+"bb.ssg");//bb.ssg为从FTP上下载下来的临时文件，里面记录着版本号
	
	CInternetSession* m_pInetSession
	CFtpConnection* m_pFtpConnection;
	m_pInetSession = new CInternetSession( AfxGetAppdirectory(), 1,PRE_CONFIG_INTERNET_ACCESS);
	m_pFtpConnection = m_pInetSession->GetFtpConnection(IP,user,password,21);
	m_pFtpConnection->GetFile(_T("/"+directory+"/bb.ssg"), _T(cspath+"bb.ssg"));
	if (ReadFile("bb.ssg")!=ReadFile("Ver.ssg"))//Ver.ssg为本地储存版本号的文件，与FTP上的不一样就更新
	{
		MessageBox("检测到自动更新，程序即将自动关闭，更新后将会重新启动","提示");
		m_pFtpConnection->GetFile(_T("/"+directory+"/"+newfile), _T(cspath+newfile));
		//断开连接
		if(m_pFtpConnection != NULL)
		{
			m_pFtpConnection->Close();
			delete m_pFtpConnection;
		}
		delete m_pInetSession;
		OpenProgram(newfile,NULL);//下载了更新之后打开
		exit(0);
	}
	//断开连接
	if(m_pFtpConnection != NULL)
	{
		m_pFtpConnection->Close();
		delete m_pFtpConnection;
	}
	delete m_pInetSession;
}