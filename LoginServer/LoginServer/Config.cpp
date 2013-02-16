#include "StdAfx.h"
#include "Config.h"


CIniConfig::CIniConfig(void)
{
	m_Status = false;
}


CIniConfig::~CIniConfig(void)
{
	m_Status = false;
}
void CIniConfig::GetConfigNum(CString szFileName,CString szDomain,CString szKey,int &value)
{
	value = GetPrivateProfileInt(szDomain,szKey,0,szFileName);
}
void CIniConfig::GetConfigString(CString szFileName,CString szDomain,CString szKey,CString &value)
{
	GetPrivateProfileString(szDomain,szKey,"0",value.GetBuffer(MAX_PATH),MAX_PATH,szFileName);
}
void CIniConfig::GetConfigString(CString szFileName,CString szDomain,CString szKey,TCHAR *pszValue,int Size)
{
	GetPrivateProfileString(szDomain,szKey,"0",pszValue,Size,szFileName);
}