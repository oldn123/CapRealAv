#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CVisImgVideoProcToolApp:
// �йش����ʵ�֣������ VisTaskDispatchDetermineApp.cpp
//

class CVisImgVideoProcToolApp : public CVisAppEx
{
public:
	CVisImgVideoProcToolApp();
	virtual DWORD	GetSoftID() {return 0;}			//����Ӧ�ó���id
	virtual void	SetupAppUpdate(){};				//���ü�����
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	BOOL		IsHasRun();
// ʵ��
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
