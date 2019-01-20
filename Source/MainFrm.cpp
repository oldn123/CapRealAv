
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"

#include "../Header/MainFrm.h"
#include "../Resource.h"
#include "../Header/VisMessageManager.h"
#include "../Header/VisBottomResWnd.h"
#include "../Header/VisRightInstallPaneWnd.h"
#include "../Header/VisProWnd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
#define	OPENFILESCOUNT 30

IMPLEMENT_DYNCREATE(CMainFrame, __BaseFrame)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, __BaseFrame)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(UM_SELECTITEM, OnSelectItem)
	ON_MESSAGE(UM_DBCLKITEM, OnDbclkItem)	
	ON_MESSAGE(MSG_ADDFILE,OnOpenFile)
	ON_MESSAGE(MSG_ADDFILE_DIRECTORY,OnOpenDirectory)
	ON_MESSAGE(MSG_DELFILE,OnDeleteFile)
	ON_MESSAGE(MSG_DOMETHOD,OnDoMethod)
	ON_MESSAGE(MSG_DOMETHOD_CANCLE,OnDoMethodCancel)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)	

	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CMainFrame* m_pThis;
// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	m_bTabMode = FALSE;  
	m_pBottomResWnd = NULL;
	m_pRightPane = NULL;
	m_pBottomDockPane = NULL;
	m_pRightDockPane = NULL;
	m_pMainImgWnd = NULL;
	m_pActItem = NULL;
	m_hExitEvent		= ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pThis = this;
	m_pProWnd = NULL;

	m_ImgForProc = ::GetCVImageInterface();
}

CMainFrame::~CMainFrame()
{
	m_pthWork->CancelWork();

	CCallBackVideo::Release();

	deleteWnd(m_pBottomResWnd);
	if(m_pBottomDockPane) deleteWnd(m_pBottomDockPane);
	if(m_pRightDockPane) deleteWnd(m_pRightDockPane);
	if(m_pMainImgWnd) deleteWnd(m_pMainImgWnd);
	if (m_pRightPane && m_pRightPane->GetHWND())
	{
		delete m_pRightPane;
	}
	for (int i = 0 ;i < m_vecRes.size(); i++)
	{
		SAFE_DELETE(m_vecRes.at(i));
	}
	m_vecRes.clear();
	CImageDataManager::GetInstance()->ReleaseIntance();
	m_vecdomethod.clear();
	CloseHandle(m_hExitEvent);
	if (m_pProWnd && m_pProWnd->GetHWND())
	{
		delete m_pProWnd;
	}
} 

class CMainVideoView : public CVisDuiTwoImageWnd
{

};

void CMainFrame::InitSubWnd()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	CVisMessageManager::GetMsgHander()->SetMsgWnd(this);

	m_pRightDockPane = new CVisDockPane();
	m_pRightDockPane->Create(L"RightDockPane", this, CRect(0,0,330,720),FALSE,IDR_RIGHT_PANE,WS_VISIBLE | WS_CHILD | CBRS_RIGHT);
	
	//右侧配置窗口
	m_pRightPane = new CVisRightInstallPaneWnd();
	m_pRightPane->Create(m_hWnd, _T("VisRightInstallPaneWnd"),UI_WNDSTYLE_CHILD , 0);
	m_pRightDockPane->SetSubWnd(m_pRightPane->GetCWnd());
	m_pRightDockPane->DockToFrameWindow(CBRS_LEFT);


	//主窗口
	m_pMainImgWnd = new CMainVideoView();
	if (m_pMainImgWnd)
	{
		m_pMainImgWnd->Create(this->GetSafeHwnd(),_T(""),WS_VISIBLE | WS_CHILD,0);
		m_pMainImgWnd->SetShowConvSeqControl(false);
		GetChildView()->SetSubWnd(m_pMainImgWnd->GetCWnd());
		m_pMainImgWnd->SetViewMode(DOUBLEVIEW);
		m_pMainImgWnd->GetRenderDC(0)->SetBrotherGroupName(L"abc");
		m_pMainImgWnd->GetRenderDC(1)->SetBrotherGroupName(L"abc");

		if (m_pRightPane)
		{
			m_pRightPane->SetActiveRendWnd(m_pMainImgWnd->GetRenderDC());
		}
	}	
}

void CMainFrame::OnThreadWork(LPARAM, int * pStatus)
{
	CCallBackVideo::GetInstance()->OnThreadProc(GetSafeHwnd(), pStatus);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__BaseFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CVisVisualManager));

//	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (GetChildView())
	{
		GetChildView()->ModifyStyle(WS_BORDER, 0);
		GetChildView()->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	}

	if (m_bTabMode)
	{
		ModifyStyle(WS_OVERLAPPEDWINDOW,WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX );

		HideCaption();
	}
	else
	{
		ModifyStyle(0, WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
		//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		//SetIcon(m_hIcon);
	}

	EnableDocking(CBRS_ALIGN_ANY);
	CDockingManager::SetDockingMode(DT_SMART);
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	InitSubWnd();

	//if (m_wndMenuBar.GetSafeHwnd())
	//{
	//	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);

	//	DockPane(&m_wndMenuBar);
	//}

	if (!m_bTabMode)
	{
		InitStatusBar();
	}

	CVisVisualManager::GetInstance()->AdjustFrames();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);


	CCallBackVideo::GetInstance()->SetCallbackNotify(this);
	DoThreadWork(0, FALSE);

	return 0;
}


void CMainFrame::OnRealtimeVideoCallback(int nDevIdx, CdvImageInterface * pImage)
{
	m_pMainImgWnd->GetRenderDC(0)->SetImagePtr(pImage);
	m_ImgForProc->Copy(*pImage);
	OnSetImage(m_ImgForProc);
}

void CMainFrame::OnErrorCatch(int nErr)
{
	if (m_pRightPane)
	{
		m_pRightPane->OnRealTimeDevErrorCatch(nErr);
	}
}

void CMainFrame::OnDevDiscover(int nIdx, LPCTSTR name, int nType)
{
	if (m_pRightPane)
	{
		m_pRightPane->setRealTimeDevInfo(nIdx, name, nType);
	}
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
//	if (m_bTabMode)
	{
		cs.hMenu = NULL;
	}
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	if( !__BaseFrame::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}



void CMainFrame::OnDestroy()
{
	//CImportVideoMgr::DeleteImportList();
	WriteLog(_T("CMainFrame::OnDestroy..."));

	GetChildView()->SetSubWnd((CWnd*)NULL);
	__BaseFrame::OnDestroy();

}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType,cx,cy);
}
// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	__BaseFrame::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	__BaseFrame::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = __BaseFrame::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}
	return lres;
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基类将执行真正的工作
	if (!__BaseFrame::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}
	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	__BaseFrame::OnSettingChange(uFlags, lpszSection);

}
BOOL CMainFrame::IsFrameWndActive(CInvestFrameWnd * )
{
	return TRUE;
}

void CMainFrame::InitStatusBar()
{

}

LRESULT CMainFrame::OnDbclkItem( WPARAM wp, LPARAM lp )
{
	CItemInterface * pData = m_pBottomResWnd->GetFrameOpr()->GetLastSelectItem();
	if (pData)
	{
		m_pActItem = (Res_Info*)pData->GetData();
		//执行双击打开
	}	
	return 0;
}


LRESULT	CMainFrame::OnSelectItem(WPARAM wp, LPARAM lp)
{
	int nPos = (int)lp;
	CItemInterface* pData = m_pBottomResWnd->GetFrameOpr()->GetItem(nPos);
	if (!pData)
	{
		m_pActItem = NULL;
		return 0;
	}
	m_pActItem = (Res_Info*)pData->GetData();
	m_pRightPane->SetImageData(m_pActItem->pData);
	SetImageToShowWnd(m_pActItem->pData);
	
	return 0;
}

bool CMainFrame::OpenFile(LPCTSTR strFile)
{
	return InsertNewInfo(strFile);
}

LRESULT CMainFrame::OnOpenFile(WPARAM wParam, LPARAM lParam)
{
	//打开文件类型
	wstring path;
	CVisVASoftDeCodec::GetSupportVideoTypesForOpenFileDialog(path);
	CString strPath;
	strPath.Format(L"%s",path.c_str());
	strPath.Replace(L"|",L";");
	TCHAR tchFilter[512];
	swprintf_s(tchFilter,512,L"视频|*.mp4|其它|*.*||",strPath);

	CFileDialog dlgFileOpen(TRUE, 
		NULL, 
		NULL, 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_EXTENSIONDIFFERENT|OFN_ENABLESIZING | OFN_ALLOWMULTISELECT, 
		tchFilter,
		NULL
		);
	dlgFileOpen.m_ofn.nMaxFile = OPENFILESCOUNT * MAX_PATH;
	dlgFileOpen.m_ofn.lpstrFile = new TCHAR[dlgFileOpen.m_ofn.nMaxFile]; 
	ZeroMemory(dlgFileOpen.m_ofn.lpstrFile, sizeof(TCHAR) * dlgFileOpen.m_ofn.nMaxFile);
	int retval = dlgFileOpen.DoModal();
	if(retval==IDCANCEL)	return 0;
	POSITION pos_file;
	pos_file = dlgFileOpen.GetStartPosition();
	vector<CString> ary_filename;
	while(pos_file != NULL)
		ary_filename.push_back(dlgFileOpen.GetNextPathName(pos_file));
	CWaitCursor waitcur;


	if (wParam && ary_filename.size() > 0)
	{
		if (OpenFile((LPCTSTR)ary_filename.at(0)))
		{
			_tcscpy((LPTSTR)wParam, (LPCTSTR)ary_filename.at(0));
		}
		else
		{
			MessageBox(NULL, L"打开文件失败", L"提示", 0);
		}
	}

	return 0;
}

LRESULT CMainFrame::OnOpenDirectory(WPARAM wParam, LPARAM lParam)
{
	CDuiString path =_T("");
	BROWSEINFO   browseInfo;
	LPITEMIDLIST lpItemlist;
	TCHAR TempFinderBuffer[MAX_PATH];
	memset(&browseInfo, 0, sizeof(browseInfo));
	browseInfo.hwndOwner		= m_hWnd;
	browseInfo.pidlRoot		= NULL; 
	browseInfo.pszDisplayName	= TempFinderBuffer;
	browseInfo.lpszTitle		= _T("请选择文件夹或文件");
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
	CWaitCursor waitcur;
	AddDirectory(path);	
	return 0;
}

void WINAPI funImageProcessCallback(void* pParam,CdvImageInterface* pImage,int nPos)
{

}

void WINAPI funImagePreProcessCallback(void* pParam,CdvImageInterface* pImage,CdvImageInterface* pDestImage,int nPos)
{
	CMainFrame * pThis = (CMainFrame *)pParam;
	pDestImage->Copy(*pImage);
	pThis->m_ImgForProc->Copy(*pDestImage);
	CVisCommonApi::ExchangeHeight(pThis->m_ImgForProc->GetData(), pThis->m_ImgForProc->GetStep(), pThis->m_ImgForProc->Height());
	pThis->OnSetImage(pDestImage);
}

void CMainFrame::OnSetImage(CdvImageInterface * pImg)
{
	m_pMainImgWnd->GetRenderDC(1)->SetImage(m_ImgForProc, false);
}

BOOL CMainFrame::InsertNewInfo( LPCTSTR lpszPath )
{
	CString strPath(lpszPath);
	if (strPath != L"")
	{
		//添加文件
		CImageData* pData = CImageDataManager::GetInstance()->AddImageData(strPath);
		if (!pData)
		{
			::MessageBox(m_hWnd,_T("文件打开错误！"),_T("提示"),MB_OK);
			return FALSE;
		}
		if (pData->GetType() == DATATYPE::MODEL_3D)
		{
			::MessageBox(m_hWnd,_T("不支持三维文件！"),_T("提示"),MB_OK);
			return FALSE;
		}
		if (m_pBottomResWnd)
		{
			Res_Info* pInfo = new Res_Info;
		
			pInfo->strPath = strPath;
			pInfo->pData = pData;
			m_vecRes.push_back(pInfo);
			//界面添加
			int nIndex = m_pBottomResWnd->AddItem(pInfo->strPath,pInfo->pData->GetPreviewData(),(LPVOID)pInfo);
			CItemInterface * pItem = m_pBottomResWnd->GetFrameOpr()->GetItem(nIndex);
			pItem->SetFontColor(false, RGB(169,169,169));
			pItem->SetFontColor(true, RGB(255,255,255));
			m_pBottomResWnd->GetFrameOpr()->SelectItem(0,0,nIndex);
			m_pBottomResWnd->Invalidate();
		}
		else
		{
			m_pMainImgWnd->SetData(pData);
			m_pMainImgWnd->GetImageDataPlay()->SetImageProcessCallBack(funImageProcessCallback, this);
			m_pMainImgWnd->GetImageDataPlay()->SetImagePreProcessCallBack(funImagePreProcessCallback, this);
		}
		return TRUE;
	}
	return FALSE;
}

void CMainFrame::GetBorderSize(int & nLeft, int & nRight, int & nBottom)
{
	return __super::GetBorderSize(nLeft, nRight, nBottom);
	nLeft = 20;
	nRight = 20;
	nBottom= 20;
}

void CMainFrame::AddDirectory( LPCTSTR lpszFolder )
{
	CDuiString strFolderPath(lpszFolder);
	CFileFind tempFind;
	CDuiString tempSrcPath;
	BOOL bFileExit = tempFind.FindFile(strFolderPath + "\\*.*");
	while(bFileExit)
	{
		bFileExit = tempFind.FindNextFile();
		tempSrcPath = tempFind.GetFilePath();
		if(tempFind.IsDots()) continue;	//如果找到的是返回上层的目录 则结束本次查找
		if(tempFind.IsDirectory())			//当查找的是文件夹时，创建文件夹递归
		{
			AddDirectory(tempSrcPath);
		}
		else 
		{
			if (!CVisCommonApi::IsFileExist(tempSrcPath)) continue;
			InsertNewInfo(tempSrcPath);
		}
	}
}

LRESULT CMainFrame::OnDeleteFile( WPARAM wParam, LPARAM lParam )
{
	int nFlag = (int)wParam;
	CArray<CItemInterface*> itemArr;
	CArray<CItemInterface*> *ptr = &itemArr;
	switch(nFlag)
	{
	case UM_DELFILE_CUR:
		{
			DeleteFile(m_pActItem);
		}
		break;
	case UM_DELFILE_SEL:
		{
			ptr = m_pBottomResWnd->GetFrameOpr()->GetSeletList();
			
			for (int i = 0; i < ptr->GetCount();i++ )
			{
				DeleteFile(ptr->GetAt(i));
			}
			m_pBottomResWnd->GetFrameOpr()->DeleteSelectItem();
		}
		break;
	case UM_DELFILE_ALL:
		{
			ptr = (CArray<CItemInterface*> *)m_pBottomResWnd->GetFrameOpr()->GetList();
			for (int i = 0; i < ptr->GetCount();i++ )
			{
				DeleteFile(ptr->GetAt(i));
			}
			m_pBottomResWnd->GetFrameOpr()->DeleteAll();
			m_pActItem = NULL;
		}
		break;
	default:
		break;
	}
	//切换页面
	int nCnt = m_pBottomResWnd->GetFrameOpr()->GetFrameNum();
	if (nCnt > 0)
	{
		m_pBottomResWnd->GetFrameOpr()->SelectItem(0,0,nCnt - 1);
	}
	m_pBottomResWnd->Invalidate();
	return 0;
}

void CMainFrame::DeleteFile( Res_Info* pActItem )
{
	if (pActItem)
	{
		//删除界面上的数据---并关闭界面
		CItemInterface* pImgActItem = m_pBottomResWnd->GetFrameOpr()->ItemFromData((LPVOID)pActItem);
		DeleteFile(pImgActItem);
		m_pBottomResWnd->GetFrameOpr()->DeleteItem(pImgActItem->GetPos());
	}	
}

void CMainFrame::DeleteFile( CItemInterface* pActItem )
{
	//删除下方资源，清空播放窗口数据
//	m_pBottomResWnd->GetFrameOpr()->DeleteItem(pActItem->GetPos());
	m_pMainImgWnd-> GetRenderDC()->SetImage(NULL);
	//删除vec中的数据
	for (int i = 0; i < m_vecRes.size();i++)
	{
		if (m_vecRes.at(i) == (Res_Info*)pActItem->GetData())
		{
			m_vecRes.erase(m_vecRes.begin()+i);
			break;
		}
	}
}

LRESULT CMainFrame::OnDoMethod( WPARAM wParam, LPARAM lParam )
{
	m_vecdomethod.clear();
	int nFlag = (int)wParam;
	CArray<CItemInterface*> itemArr;
	CArray<CItemInterface*> *ptr = &itemArr;
	switch(nFlag)
	{
	case UM_DO_METHOD_CUR:
		{
			int nPos = m_pBottomResWnd ->GetFrameOpr()->GetLastClickId();
			if (nPos > -1)
			{
				CImageData*	pImgData = ((Res_Info*)m_pBottomResWnd ->GetFrameOpr()->GetItem(nPos)->GetData())->pData;
				m_vecdomethod.push_back(pImgData);
			}	
		}
		break;
	case UM_DO_METHOD_SEL:
		{
			ptr = m_pBottomResWnd->GetFrameOpr()->GetSeletList();
			CImageData*	pImgData = NULL;
			for (int i = 0; i < ptr->GetCount();i++ )
			{
				pImgData = NULL;
				pImgData = ((Res_Info*)(ptr->GetAt(i)->GetData()))->pData;
				if (pImgData)
				{
					m_vecdomethod.push_back(pImgData);
				}	
			}
		}
		break;
	case UM_DO_METHOD_ALL:
		{
			ptr = (CArray<CItemInterface*> *)m_pBottomResWnd->GetFrameOpr()->GetList();
			CImageData*	pImgData = NULL;
			for (int i = 0; i < ptr->GetCount();i++ )
			{
				pImgData = NULL;
				pImgData =  ((Res_Info*)(ptr->GetAt(i)->GetData()))->pData;
				if (pImgData)
				{
					m_vecdomethod.push_back(pImgData);
				}	
			}
		}
		break;
	default:
		break;
	}
	//执行相关算法
	if (m_vecdomethod.size())
	{
		DoMethod();
	}
	return 0;
}

void CMainFrame::SetImageToShowWnd(CImageData* pImgData)
{
	if (pImgData == NULL)
	{
		m_pMainImgWnd->GetRenderDC()->SetImage(NULL);
		m_pMainImgWnd->GetRenderDC()->SetImage(NULL);
	}
	else
	{
		if (pImgData->GetType() == DATATYPE::IMAGE_DATA)
		{
			m_pMainImgWnd->SetData(pImgData);
		}
		if (pImgData->GetType() == DATATYPE::VIDEO_DATA)
		{
			m_pMainImgWnd->SetData(pImgData);
			m_pMainImgWnd->GetVideoDataPlay()->Seek(0);
		}
		if (pImgData->GetType() == DATATYPE::IMAGE_SEQUNCE_DATA)
		{
			m_pMainImgWnd->SetData(pImgData);
			m_pMainImgWnd->GetVideoDataPlay()->Seek(0);
		}
	}
}

CString CMainFrame::GetDesPath( const CString& srcPath,const CString& desFolder )
{
	CString desPath;
	desPath.Format(_T("%s\\%s"),desFolder,CVisCommonApi::GetFileNameEx(srcPath));
	IsDesFileExist(desPath);
	return desPath;
}

CString CMainFrame::GetDesPath( const CString& srcPath )
{
	CString desPath;
	desPath.Format(_T("%s\\%s"),GetDesFolder(),CVisCommonApi::GetFileNameEx(srcPath));
	IsDesFileExist(desPath);
	return desPath;
}

void CMainFrame::SaveImgDesFile()
{

}

void CMainFrame::SaveVideoDesFile()
{

}

void CMainFrame::DoMethod()
{
//	m_pRightPane->SetUIStatus(TRUE);
	LONGLONG nTotal = 0;
	for (int i = 0 ; i < m_vecdomethod.size();i++)
	{
		nTotal += m_vecdomethod.at(i)->Length();
	}
	if (m_pProWnd && m_pProWnd->GetHWND())
	{
		delete m_pProWnd;
	}
	ResetEvent(m_hExitEvent);
	m_pProWnd = new CVisProWnd();
	m_pProWnd->SetTotal(nTotal);
	m_pProWnd->Create(this->GetSafeHwnd(), _T("进度提示"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 600, 200);
	m_pProWnd->CenterWindow();
	m_DoMethodThread.SetWorkFunction(ApplyMethodCallBack,this);
	m_DoMethodThread.DoWork();
	m_pProWnd->ShowModal();
}

void WINAPI CMainFrame::ApplyMethodCallBack(int * pWorkMode, LPVOID pData)
{
	((CVideoDataPlay*)(m_pThis->m_pMainImgWnd->GetVideoDataPlay()))->SetRenderDC(NULL);
	USES_CONVERSION;
	for (int i = 0; i < m_pThis->m_vecdomethod.size();i++)
	{
		DWORD dwRes = WaitForSingleObject(m_pThis->m_hExitEvent,0);
		if (dwRes == WAIT_OBJECT_0)  //停止按钮
		{
			break;
		}
		CImageData *pImgData = m_pThis->m_vecdomethod.at(i);
		if (!pImgData)
		{
			continue;
		}
		if (pImgData->GetType() == DATATYPE::VIDEO_DATA)//视频
		{
			CVideoData*	pVideoData = (CVideoData*)pImgData;
			ASSERT(pVideoData);
			CVisVASoftDeCodec* pDecodec = pVideoData->GetDeCode();
			pDecodec->SetSinkFile(pVideoData->GetResPath());
			long nFrameCnt = 0;
			SVisStreamMediaInfo MediaInfo;
			pDecodec->GetMediaInfo(&MediaInfo);
			LONGLONG nVideoLength;
			pDecodec->GetDuration(&nVideoLength);
			CVisVASoftEnCodec* pVideoEncoder = new CVisVASoftEnCodec();
			pVideoEncoder->SetSinkFile(m_pThis->GetDesPath(pVideoData->GetResPath()),MediaInfo.sVideoMediaInfo.wWidth,MediaInfo.sVideoMediaInfo.wHeight,MediaInfo.sVideoMediaInfo.dwStandardRate);
			while(nFrameCnt != nVideoLength)
			{
				dwRes = WaitForSingleObject(m_pThis->m_hExitEvent,0);
				if (dwRes == WAIT_OBJECT_0)  //停止按钮
				{
					pDecodec->Pause();
					pDecodec->SetVideoDataCallBack(NULL,NULL);
					Sleep(1000);
					if (pVideoEncoder)
					{
						pVideoEncoder->Release();
						SAFE_DELETE(pVideoEncoder);
					}
					return;
				}
				pVideoData->Seek(nFrameCnt); 
				pVideoData->GetImageFilter()->Render(pVideoData->GetData()->GetData());
				CVisCommonApi::ExchangeHeight(pVideoData->GetImageFilter()->GetDst8UImage()->GetData(), pVideoData->GetImageFilter()->GetDst8UImage()->GetStep(), pVideoData->GetImageFilter()->GetDst8UImage()->Height());
				pVideoEncoder->PutVideoFrame(pVideoData->GetImageFilter()->GetDst8UImage()->GetData());
				if (m_pThis->m_pProWnd->GetHWND())
				{
					::SendMessage(m_pThis->m_pProWnd->GetHWND(),MSG_PRO_PER,(WPARAM)pVideoData,nFrameCnt+1);
				}
				nFrameCnt++;
			}
			pVideoEncoder->Release();
		}
		if (pImgData->GetType() == DATATYPE::IMAGE_SEQUNCE_DATA)//序列
		{
			
			CSeqImageData*	pSeqData = (CSeqImageData*)pImgData;
			ASSERT(pSeqData);
			CTempFileForMidDoc* pDecodec = pSeqData->GetSeqData();
			long nFrameCnt = 0;
			LONGLONG nVideoLength = (LONGLONG)pSeqData->Length();
			CTempFileForMidDoc pEnCodec;
			
			pEnCodec.Open(m_pThis->GetDesPath(pSeqData->GetResPath()));
			CdvImageInterface* pEnCodecImg = NULL;
			while(nFrameCnt != nVideoLength)
			{
				dwRes = WaitForSingleObject(m_pThis->m_hExitEvent,0);
				if (dwRes == WAIT_OBJECT_0)  //停止按钮
				{
					return;
				}
				pEnCodecImg = pSeqData->GetImageData(nFrameCnt);
				pSeqData->GetImageFilter()->Render(pEnCodecImg->GetData());
				pEnCodec.SaveImageToFile(nFrameCnt,pSeqData->GetImageFilter()->GetDst8UImage());
				//添加进度条提示
				if (m_pThis->m_pProWnd->GetHWND())
				{
					::SendMessage(m_pThis->m_pProWnd->GetHWND(),MSG_PRO_PER,(WPARAM)pSeqData,nFrameCnt+1);
				}
				
				nFrameCnt++;
			}
		}
		else if(pImgData->GetType() == DATATYPE::IMAGE_DATA)//图像
		{
			CdvImageInterface* pImg = pImgData->GetData();
			pImgData->GetImageFilter()->DestroyImageInterface();
			pImgData->GetImageFilter()->CreateImageInterface(pImg->Width(),pImg->Height(),pImg->GetChannels()*8);
			pImgData->GetImageFilter()->Render(pImg->GetData());
			pImgData->GetImageFilter()->GetDst8UImage()->SaveToFile(T2A(m_pThis->GetDesPath(pImgData->GetResPath())));
			//添加进度条提示
			if (m_pThis->m_pProWnd->GetHWND())
			{
				::SendMessage(m_pThis->m_pProWnd->GetHWND(),MSG_PRO_PER,(WPARAM)pImgData,1);
			}
		}
	} 
}

CString CMainFrame::GetDesFolder()
{
	return m_pRightPane->GetDesFolder();
}

LRESULT CMainFrame::OnDoMethodCancel( WPARAM wParam, LPARAM lParam )
{
	SetEvent(m_hExitEvent);
	return 0;
}

void CMainFrame::IsDesFileExist( CString& strDesPath )
{
	CString strText;
	strText.Format(_T("文件 %s 已存在，是否替换该文件？"),strDesPath);
	if (CVisCommonApi::IsFileExist(strDesPath))//目标文件已存在
	{
		if (IDOK == ::MessageBoxW(m_hWnd,strText,_T("替换文件"),MB_OKCANCEL))//替换文件
		{
			return;
		}
		else
		{

			//不替换，加时间结尾
			SYSTEMTIME systime;
			GetSystemTime(&systime);
			CString strSysTime;//年月日，时分秒
			strSysTime.Format(_T("%04d%02d%02d%02d%02d%02d"),systime.wYear,systime.wMonth,systime.wDay,\
							systime.wHour,systime.wMinute,systime.wSecond);

			strDesPath.Format(_T("%s\\%s_%s%s"),CVisCommonApi::GetPath(strDesPath),CVisCommonApi::GetFileName(strDesPath),strSysTime,CVisCommonApi::GetFileExtName(strDesPath));
			
			//加结尾判断
			if (CVisCommonApi::IsFileExist(strDesPath))//仍旧存在
			{
				int nIndex = 1;
				CString strName = CVisCommonApi::GetFileName(strDesPath);
				do 
				{
					strDesPath.Format(_T("%s\\%s(%d)%s"),CVisCommonApi::GetPath(strDesPath),strName,nIndex,CVisCommonApi::GetFileExtName(strDesPath));
					++nIndex;
				} 
				while (CVisCommonApi::IsFileExist(strDesPath));
			}	
		}

	}
}

//void WINAPI CMainFrame::DataComeCallback( double dblSampleTime, BYTE *pBuffer, long lBufferSize, PVOID lpParam )
//{
//	CMainFrame *pDlg = (CMainFrame*)lpParam;
//	ASSERT(pDlg);
////	if (m_pThis->m_pEnCodec)
//	{
//		CVisCommonApi::ExchangeHeight(pBuffer, pDlg->m_pDesImg->GetData(), pDlg->m_pDesImg->GetStep(), pDlg->m_pDesImg->Height());
//		pDlg->m_pEnCodec.PutVideoFrame(pDlg->m_pDesImg->GetData(),FALSE);
//	}
//}
//

