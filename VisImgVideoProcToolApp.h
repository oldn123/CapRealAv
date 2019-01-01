#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CVisImgVideoProcToolApp:
// 有关此类的实现，请参阅 VisTaskDispatchDetermineApp.cpp
//

class CVisImgVideoProcToolApp : public CVisAppEx
{
public:
	CVisImgVideoProcToolApp();
	virtual DWORD	GetSoftID() {return 0;}			//返回应用程序id
	virtual void	SetupAppUpdate(){};				//启用检查更新
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	BOOL		IsHasRun();
// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	HANDLE m_hMutex;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVisImgVideoProcToolApp theApp;
