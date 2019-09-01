#include "stdafx.h"
#include "../Header/VisRightInstallPaneWnd.h"
#include "../struct.h"
#include "../Header/VisMessageManager.h"
#include "../Header/VisProcAdjustChildWnd.h"

#include "..\avCapSdk\CallBackVideo.h"

#define OPENFILESCOUNT 100
#define	ALGCNT		3 //最多支持3个算法


CVisRightInstallPaneWnd::CVisRightInstallPaneWnd(ICmdParamNotify * p) : m_pParamNotify(p)
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

HRESULT CVisRightInstallPaneWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_USER + 128)
	{
		if (m_pParamNotify)
		{
			m_pParamNotify->OnParamChanged(LOWORD(wParam), HIWORD(wParam), lParam);
		}
	}

	return __super::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
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
		if (curName == L"use_addnoise")
		{
			bool baddnoise = ((CCheckBoxUI*)msg.pSender)->GetCheck();
			m_PaintManager.FindControl(L"sliderNoisely")->SetVisible(baddnoise);	

			::PostMessage(GetSafeHwnd(), WM_USER + 128, MAKELONG(0, baddnoise), ((CSliderUI *)m_PaintManager.FindControl(L"sliderNoise"))->GetValue());
		}
		else if (curName == L"use_denoise")
		{
			bool bdenoise = ((CCheckBoxUI*)msg.pSender)->GetCheck();
			m_PaintManager.FindControl(L"sliderdeNoisely")->SetVisible(bdenoise);	

			::PostMessage(GetSafeHwnd(), WM_USER + 128, MAKELONG(1, bdenoise), ((CSliderUI *)m_PaintManager.FindControl(L"sliderdeNoise"))->GetValue());
		}
		else
		if (curName == L"opt_localVideo" ||
			curName == L"opt_realtimeVideo")
		{
			m_PaintManager.FindControl(L"localVideoMode")->SetVisible(curName == L"opt_localVideo");
			m_PaintManager.FindControl(L"realTimeVideoMode")->SetVisible(curName == L"opt_realtimeVideo");
		}
		else if (curName == L"use_zoom1")
		{
			::PostMessage(GetSafeHwnd(), WM_USER + 128, MAKELONG(2,1), 1);
		}
		else if (curName == L"use_zoom2")
		{
			::PostMessage(GetSafeHwnd(), WM_USER + 128, MAKELONG(2, 1), 2);
		}
		else if (curName == L"use_zoom3")
		{
			::PostMessage(GetSafeHwnd(), WM_USER + 128, MAKELONG(2,1), 3);
		}
	}
	else if (_tcscmp(curType, DUI_MSGTYPE_VALUECHANGED)==0)
	{
		if (curName == L"sliderNoise")
		{
			CSliderUI * pslder = (CSliderUI *)msg.pSender;
			::PostMessage(GetSafeHwnd(), WM_USER + 128, MAKELONG(0, pslder->IsVisible()), pslder->GetValue());
		}
		else if (curName == L"sliderdeNoise")
		{
			CSliderUI * pslder = (CSliderUI *)msg.pSender;
			::PostMessage(GetSafeHwnd(), WM_USER + 128, MAKELONG(1, pslder->IsVisible()), pslder->GetValue());
		}
	}
}

void CVisRightInstallPaneWnd::GetParams(sParams & param)
{
	param.bUseAddNoise	= ((CCheckBoxUI*)m_PaintManager.FindControl(L"use_addnoise"))->GetCheck();
	param.bUseBM3D		= ((CCheckBoxUI*)m_PaintManager.FindControl(L"use_denoise"))->GetCheck();
	param.bUseSSR		= ((COptionUI*)m_PaintManager.FindControl(L"use_zoom1"))->IsSelected() ? false : true;
	param.nNoise		= ((CSliderUI*)m_PaintManager.FindControl(L"sliderNoise"))->GetValue();
	param.fDenosie		= ((CSliderUI*)m_PaintManager.FindControl(L"sliderdeNoise"))->GetValue();
	param.nScl			= ((COptionUI*)m_PaintManager.FindControl(L"use_zoom1"))->IsSelected() ? 1 : ((COptionUI*)m_PaintManager.FindControl(L"use_zoom2"))->IsSelected() ? 2 : 3;
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
	strMyDoc += _T("\\");
	m_strDesFolder = strMyDoc;
	m_pEditDesFolder->SetText(m_strDesFolder);
//	SetUIStatus(FALSE);



	//BindHwnd绑定控件
	m_pBindWnd = (CBindHwndUI*)m_PaintManager.FindControl(_T("bind_ArchShow"));
	if (m_pBindWnd)
	{
		m_pMethodPanel= new CFxAdjPane_FIEx(this->m_hWnd, this->m_hWnd);
		m_pMethodPanel->Create(GetCWnd());
		m_pMethodPanel->ShowWindow(false);
		m_pBindWnd->SetBind(m_pMethodPanel->GetHWND());	
		if (m_pImgData)
		{
			m_pMethodPanel->SetImageFilterMgr(m_pImgData->GetImageFilter());
		}
	}

	if (m_PaintManager.FindControl(L"btn_connectRealtimeDev"))
	{
		m_PaintManager.FindControl(L"btn_connectRealtimeDev")->SetText(L"正在尝试连接设备...");
	}

	m_PaintManager.FindControl(L"sliderNoisely")->SetVisible(((CCheckBoxUI*)m_PaintManager.FindControl(L"use_addnoise"))->GetCheck());	

	m_PaintManager.FindControl(L"sliderdeNoisely")->SetVisible(((CCheckBoxUI*)m_PaintManager.FindControl(L"use_denoise"))->GetCheck());	


	if (m_pParamNotify)
	{
		m_pParamNotify->OnParamChanged(-1, 0,0);
	}
}

void CVisRightInstallPaneWnd::OnConnectRealTimeDev()
{
	if (m_pParamNotify)
	{
		if (m_pParamNotify->OnStartRealPlay())
		{
			m_PaintManager.FindControl(L"btn_connectRealtimeDev")->SetEnabled(false);
		}
	}
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
	TCHAR sFileBuf[MAX_PATH] = {0};
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_ADDFILE, (WPARAM)&sFileBuf[0],0);

	if (_tcslen(sFileBuf) > 0)
	{
		if (m_PaintManager.FindControl(L"edit_inputFile"))
		{
			m_PaintManager.FindControl(L"edit_inputFile")->SetText(sFileBuf);
		}
	}

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
	m_PaintManager.FindControl(L"btn_connectRealtimeDev")->SetEnabled(bCanStart);
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

