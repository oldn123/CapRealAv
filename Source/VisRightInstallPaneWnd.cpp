#include "stdafx.h"
#include "../Header/VisRightInstallPaneWnd.h"
#include "../struct.h"
#include "../Header/VisMessageManager.h"
#include "../Header/VisProcAdjustChildWnd.h"

#include "..\avCapSdk\CallBackVideo.h"

#define OPENFILESCOUNT 100
#define	ALGCNT		3 //最多支持3个算法


CVisRightInstallPaneWnd::CVisRightInstallPaneWnd()
{
	m_pComboArchInstall = NULL;		
	m_pBtnAddFile = NULL;		
	m_pBtnAddFile_Install = NULL;
	m_pBtnDelFile = NULL;		
	m_pBtnDelFile_Install = NULL;
	m_pBtnExecute = NULL;		
	m_pBtnExecute_Install = NULL;

	m_pBtnDesFolder = NULL;		
	m_pBtnOpenDesFolder = NULL;	
	m_pEditDesFolder = NULL;	
	m_pMethodPanel = NULL;
	m_pImgData = NULL;
	m_pFxRenderWnd = NULL;
	m_pBindWnd = NULL;
	m_pTempFxRenderWnd = new CFxRenderWnd();
}

CVisRightInstallPaneWnd::~CVisRightInstallPaneWnd()
{
	deleteWnd(m_pMethodPanel);
	deleteWnd(m_pTempFxRenderWnd);
}

LPCTSTR CVisRightInstallPaneWnd::GetWindowClassName() const
{
	return _T("VisRightInstallPaneWnd");
}

void CVisRightInstallPaneWnd::setRealTimeDevInfo(int ,LPCTSTR sText,int)
{
	if (m_PaintManager.FindControl(L"btn_connectRealtimeDev"))
	{
		m_PaintManager.FindControl(L"btn_connectRealtimeDev")->SetText(sText);
	}
}

void CVisRightInstallPaneWnd::OnRealTimeDevErrorCatch(int nErr)
{
	switch(nErr)
	{
	case CAP_EC_INVALID_PARAM:
		{
			MessageBoxW(AfxGetMainWnd()->GetSafeHwnd(), L"打开设备失败", L"提示", 0);
			m_PaintManager.FindControl(L"btn_connectRealtimeDev")->SetEnabled(true);
		}	
		break;
	}
}

void CVisRightInstallPaneWnd::Notify( TNotifyUI& msg )
{

	CString curName = msg.pSender->GetName();
	CString curType = msg.sType;
	if (_tcscmp(curType,_T("itemclick"))==0)
	{
		if (m_pMethodPanel && m_pMethodPanel->GetHWND())
		{
			if (m_pMethodPanel->GetMethodCnt() < 3)
			{
				m_pMethodPanel->AddMethod(curName);
			}
			/*else
			{
				::MessageBoxW(NULL,_T("使用算法数已达上限，请删除后再添加新算法！"),_T("提示"),MB_OK);
			}*/
		}
	}
	__super::Notify(msg);
	if (_tcscmp(curType, DUI_MSGTYPE_SELECTCHANGED)==0)
	{
		if (curName == L"opt_localVideo" ||
			curName == L"opt_realtimeVideo")
		{
			m_PaintManager.FindControl(L"localVideoMode")->SetVisible(curName == L"opt_localVideo");
			m_PaintManager.FindControl(L"realTimeVideoMode")->SetVisible(curName == L"opt_realtimeVideo");
		}	
	}

}

void CVisRightInstallPaneWnd::InitWindow()
{
	m_pComboArchInstall = static_cast<CComboBoxUI*>(m_PaintManager.FindControl(_T("combo_arch")));		
	m_pBtnAddFile = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_addfile")));		
	m_pBtnAddFile_Install = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_addfile_install")));	
	m_pBtnDelFile = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_delfile")));		
	m_pBtnDelFile_Install = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_delfile_install")));
	m_pBtnExecute = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_archexecute")));		
	m_pBtnExecute_Install = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_archexecute_install")));		
	m_pBtnDesFolder = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_savefolder")));		
	m_pBtnOpenDesFolder = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_opentransfolder")));	
	m_pEditDesFolder = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_exportfolder")));

	//默认转码目录为"我的文档"中“vis转码视频”文件夹
	TCHAR path[255];
	SHGetSpecialFolderPath(0,path,CSIDL_COMMON_DOCUMENTS,0);
	CString strMyDoc(path);
	strMyDoc += _T("\\图像视频批量处理输出目录");
	m_strDesFolder = strMyDoc;
	m_pEditDesFolder->SetText(m_strDesFolder);
//	SetUIStatus(FALSE);


	m_pMethodPanel= new CFxAdjPane_FIEx(this->m_hWnd, this->m_hWnd);
	m_pMethodPanel->Create(GetCWnd());
	//BindHwnd绑定控件
	m_pBindWnd = (CBindHwndUI*)m_PaintManager.FindControl(_T("bind_ArchShow"));
	m_pBindWnd->SetBind(m_pMethodPanel->GetHWND());
	if (m_pImgData)
	{
		m_pMethodPanel->SetImageFilterMgr(m_pImgData->GetImageFilter());
	}

	if (m_PaintManager.FindControl(L"btn_connectRealtimeDev"))
	{
		m_PaintManager.FindControl(L"btn_connectRealtimeDev")->SetText(L"正在尝试连接设备...");
	}
}

void CVisRightInstallPaneWnd::OnConnectRealTimeDev()
{
	if (!CCallBackVideo::GetInstance()->Start())
	{
		MessageBoxW(GetSafeHwnd(), L"连接设备失败！", L"提示", 0);
	}

	m_PaintManager.FindControl(L"btn_connectRealtimeDev")->SetEnabled(false);
}

void CVisRightInstallPaneWnd::OnClick( TNotifyUI& msg )
{
	CString curName = msg.pSender->GetName();
	CString curType = msg.sType;
	if (_tcscmp(curType,_T("click"))==0)
	{
		if (_tcscmp(curName,_T("btn_addfile"))== 0)
		{
			OnBtnAddFile();
		}
		else if (_tcscmp(curName,_T("btn_addfile_install"))== 0)
		{
			OnBtnAddFile_Install();
		}
		else if (_tcscmp(curName,_T("btn_delfile"))== 0)
		{
			OnBtnDelFile();
		}
		else if (_tcscmp(curName,_T("btn_delfile_install"))== 0)
		{
			OnBtnDelFile_Install();
		}
		else if (_tcscmp(curName,_T("btn_archexecute"))== 0)
		{
			OnBtnExecuteArch();
		}
		else if (_tcscmp(curName,_T("btn_archexecute_install"))== 0)
		{
			OnBtnExecuteArch_Install();
		}
		else if (_tcscmp(curName,_T("btn_savefolder"))== 0)
		{
			OnBtnDesFolder();
		}
		else if (_tcscmp(curName,_T("btn_opentransfolder"))== 0)
		{
			OnBtnOpenDesFolder();
		}
		else if (_tcscmp(curName,_T("btn_connectRealtimeDev"))== 0)
		{
			OnConnectRealTimeDev();
		}

	}
	return __super::OnClick(msg);
}

void CVisRightInstallPaneWnd::OpenDesFolder( LPCTSTR lpszPath )
{
	CString str(lpszPath);
	if (str.GetLength()<=0)
	{
		return ;
	}
	CString strSelect;
	strSelect.Format(TEXT("/open,%s"), str) ;
	ShellExecute(NULL, TEXT("open"), TEXT("explorer.exe"), strSelect, NULL, SW_NORMAL) ;
}

void CVisRightInstallPaneWnd::OnBtnAddFile()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_ADDFILE,0,0);
}

void CVisRightInstallPaneWnd::OnBtnDesFolder()
{
	m_strDesFolder = SelTransDir();
	if (m_strDesFolder.GetLength()<=0)
	{
		return ;
	}
	m_pEditDesFolder->SetText(m_strDesFolder);
}

void CVisRightInstallPaneWnd::OnBtnOpenDesFolder()
{
	m_strDesFolder = m_pEditDesFolder->GetText();
	if (!PathIsDirectory(m_strDesFolder))
	{
		CVisCommonApi::CreateDeepDirectory(m_strDesFolder);
	}
	OpenDesFolder(m_strDesFolder);
}

void CVisRightInstallPaneWnd::OnBtnExecuteArch()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_DOMETHOD,UM_DO_METHOD_CUR,0);
}

void CVisRightInstallPaneWnd::MenuNotify( CDuiString strMenuCmd, CDuiString strUserData, UINT nTag )
{
	if (strMenuCmd == _T("addfile_only"))
	{
		OpenFile();
	}
	else if (strMenuCmd == _T("addfile_directory"))
	{
		OpenDirectory();
	}
	else if (strMenuCmd == _T("del_cur"))
	{
		DelFileCur();
	}
	else if (strMenuCmd == _T("del_sel"))
	{
		DelFileSel();
	}
	else if (strMenuCmd == _T("del_all"))
	{
		DelFileAll();
	}
	else if (strMenuCmd == _T("archexecute_cur"))
	{
		DoMethodCur();
	}
	else if (strMenuCmd == _T("archexecute_sel"))
	{
		DoMethodSel();
	}
	else if (strMenuCmd == _T("archexecute_all"))
	{
		DoMethodAll();
	}
}

void CVisRightInstallPaneWnd::MenuNotifyForType( CDuiString strMenuCmd, CDuiString strUserData, UINT nTag, UINT uType )
{

}

void CVisRightInstallPaneWnd::OnBtnAddFile_Install()
{
	//右键菜单
	CMenuManagerUI mgr(this);
	STRINGorID s(_T("\\BatchImgVideoProc\\addfile_menu.xml"));
	CMenuUI* pMenu = mgr.Load(s);
	//弹出菜单
	CPoint pt;
	::GetCursorPos(&pt);
	mgr.Popup(pt, eMenuAlignment_Right | eMenuAlignment_Bottom);
}

void CVisRightInstallPaneWnd::OnBtnDelFile()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_DELFILE,UM_DELFILE_CUR,0);
}

void CVisRightInstallPaneWnd::OnBtnDelFile_Install()
{
	//右键菜单
	CMenuManagerUI mgr(this);
	STRINGorID s(_T("\\BatchImgVideoProc\\delfile_menu.xml"));
	CMenuUI* pMenu = mgr.Load(s);
	//弹出菜单
	CPoint pt;
	::GetCursorPos(&pt);
	mgr.Popup(pt, eMenuAlignment_Right | eMenuAlignment_Bottom);
}

void CVisRightInstallPaneWnd::OnBtnExecuteArch_Install()
{
	//右键菜单
	CMenuManagerUI mgr(this);
	STRINGorID s(_T("\\BatchImgVideoProc\\arch_execute_install.xml"));
	CMenuUI* pMenu = mgr.Load(s);
	//弹出菜单
	CPoint pt;
	::GetCursorPos(&pt);
	mgr.Popup(pt, eMenuAlignment_Right | eMenuAlignment_Bottom);
}

void CVisRightInstallPaneWnd::OpenFile()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_ADDFILE,0,0);
}

void CVisRightInstallPaneWnd::OpenDirectory()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_ADDFILE_DIRECTORY,0,0);
}

void CVisRightInstallPaneWnd::DelFileCur()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_DELFILE,UM_DELFILE_CUR,0);
}

void CVisRightInstallPaneWnd::DelFileSel()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_DELFILE,UM_DELFILE_SEL,0);
}

void CVisRightInstallPaneWnd::DelFileAll()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_DELFILE,UM_DELFILE_ALL,0);
}

void CVisRightInstallPaneWnd::DoMethodCur()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_DOMETHOD,UM_DO_METHOD_CUR,0);
}

void CVisRightInstallPaneWnd::DoMethodSel()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_DOMETHOD,UM_DO_METHOD_SEL,0);
}

void CVisRightInstallPaneWnd::DoMethodAll()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_DOMETHOD,UM_DO_METHOD_ALL,0);
}

CString CVisRightInstallPaneWnd::SelTransDir()
{
	CString path =_T("");
	BROWSEINFO   browseInfo;
	LPITEMIDLIST lpItemlist;
	TCHAR TempFinderBuffer[MAX_PATH];
	memset(&browseInfo, 0, sizeof(browseInfo));
	browseInfo.hwndOwner		= m_hWnd;
	browseInfo.pidlRoot		= NULL; 
	browseInfo.pszDisplayName	= TempFinderBuffer;
	browseInfo.lpszTitle		= _T("设置转码目录");
	browseInfo.ulFlags		= BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS| BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	browseInfo.lpfn			= NULL;
	browseInfo.lParam			= 0;
	browseInfo.iImage			= 0;  
	lpItemlist = SHBrowseForFolder(&browseInfo);
	if (lpItemlist != NULL)
	{
		SHGetPathFromIDList(lpItemlist, TempFinderBuffer);//获取打开文件路径，并保存在TempFinderBuffer里（绝对路径）
		path = TempFinderBuffer;
	}
	//释放为LPITEMIDLIST lpItemlist分配的内存
	IMalloc *imalloc = 0;
	if (SUCCEEDED(SHGetMalloc(&imalloc)))
	{
		imalloc->Free(lpItemlist);
		imalloc->Release();
	}	
	return path;
}

void CVisRightInstallPaneWnd::SetUIStatus( BOOL bCanStart )
{
	if (bCanStart)
	{
		m_pComboArchInstall->SetEnabled(FALSE);	
		m_pBtnAddFile->SetEnabled(FALSE);		
		m_pBtnAddFile_Install->SetEnabled(FALSE);
		m_pBtnDelFile->SetEnabled(FALSE);		
		m_pBtnDelFile_Install->SetEnabled(FALSE);
		m_pBtnExecute->SetEnabled(TRUE);		
		m_pBtnExecute_Install->SetEnabled(TRUE);

		m_pBtnDesFolder->SetEnabled(FALSE);		
		m_pBtnOpenDesFolder->SetEnabled(FALSE);	
		m_pEditDesFolder->SetEnabled(FALSE);	
	}
	else
	{
		m_pComboArchInstall->SetEnabled(TRUE);	
		m_pBtnAddFile->SetEnabled(TRUE);		
		m_pBtnAddFile_Install->SetEnabled(TRUE);
		m_pBtnDelFile->SetEnabled(TRUE);		
		m_pBtnDelFile_Install->SetEnabled(TRUE);
		m_pBtnExecute->SetEnabled(FALSE);		
		m_pBtnExecute_Install->SetEnabled(FALSE);

		m_pBtnDesFolder->SetEnabled(TRUE);		
		m_pBtnOpenDesFolder->SetEnabled(TRUE);	
		m_pEditDesFolder->SetEnabled(TRUE);	
	}
}

DuiLib::CDuiString CVisRightInstallPaneWnd::GetSkinFile()
{
	return _T("\\ViProc\\RightInstallPane.xml");
}

CString CVisRightInstallPaneWnd::GetDesFolder()
{
	CString strDesFolder = m_pEditDesFolder->GetText();
	return strDesFolder;
}

void CVisRightInstallPaneWnd::SetImageData( CImageData* pData )
{
	m_pImgData = pData;
	if (m_pMethodPanel)
	{
		CImageFilterManager* pImageFilterManager = GetImageFilterManager();
		if (pImageFilterManager)
		{
			m_pImgData->SetImageFilter(pImageFilterManager);
		}
		else
		{
			m_pMethodPanel->SetImageFilterMgr(m_pImgData->GetImageFilter());
		}
		if (m_pFxRenderWnd)
		{
			m_pMethodPanel->SetFxViewInterface(m_pFxRenderWnd);
		}
	}
}

CImageFilterManager* CVisRightInstallPaneWnd::GetImageFilterManager()
{
	if (m_pMethodPanel)
	{
		return m_pMethodPanel->GetImageFilterManager();
	}
	return NULL;
}

