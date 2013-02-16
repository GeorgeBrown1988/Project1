#pragma once
class CConfig
{
public:
	CConfig(void);
	~CConfig(void);
};

class CIniConfig
{
public:
	CIniConfig(void);
	~CIniConfig(void);
	//��������
public:
	void GetConfigNum(CString szFileName,CString szDomain,CString szKey,int &value);
	void GetConfigString(CString szFileName,CString szDomain,CString szKey,CString &value);
	void GetConfigString(CString szFileName,CString szDomain,CString szKey,TCHAR *pszValue,int Size);
	//
public:
	void ReadConfigFile(CString szFileName);
	void UpdateControl();
	void LoadConfigFile();
private:
	bool m_Status;
};