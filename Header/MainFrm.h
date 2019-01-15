
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "MarginTabWnd.h"
#include "..\struct.h"
#include "VisProWnd.h"
#include "..\avCapSdk\CallBackVideo.h"

class CMainFrame;
class CInvestBaseFrame;
class CInvestFrameWnd;
class CMyTabCtrl : public CMarginTabWnd
{
protected:
	BOOL IsInCaption(CPoint& pt)
	{
		if (HitTest(pt) == HTERROR)
		{
			return __super::IsInCaption(pt);
		}
		return FALSE;
	}

	BOOL IsZoomed()
	{
		RECT rt;
		SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)&rt,0);
		int width = rt.right - rt.left; 
		int height= rt.bottom - rt.top; 
		CRect rcFrame;
		AfxGetMainWnd()->GetWindowRect(&rcFrame);
		if (rcFrame.Width() >= width &&
			rcFrame.Height() >= height)
		{
			return TRUE;
		}
		return FALSE;
	}

	UINT HitTest(CPoint& point)
	{
		CPoint pt(point);
		CMarginTabWnd::ScreenToClient(GetSafeHwnd(), &pt);
		if (m_TabCmds[0]->GetTabRect().PtInRect(pt))
		{
			return HTCLOSE;
		}
		else
			if (m_TabCmds[1]->GetTabRect().PtInRect(pt))
			{
				return HTMAXBUTTON;
			}
			else
				if (m_TabCmds[2]->GetTabRect().PtInRect(pt))
				{
					return HTMINBUTTON;
				}

				return HTERROR;
	}

	virtual int  OnWindowCreate(LPCREATESTRUCT lpCreateStruct)
	{
		int nret = __super::OnWindowCreate(lpCreateStruct);
		SetTabPos(CVisSorTabWnd::SORTABPOS_TOP);
		CString sBasePath = CSki::GetDirectResPath();

		AddCmdBtn(100, sBasePath + _T("BatchImgVideoProc\\img_sys_close_btn.png"), &CSize(39,18), FALSE);
		AddCmdBtn(101, sBasePath + _T("BatchImgVideoProc\\img_sys_max_btn.PNG"), &CSize(28,20), FALSE);
		AddCmdBtn(102, sBasePath + _T("BatchImgVideoProc\\img_sys_min_btn.PNG"), &CSize(28,20), FALSE);
		return nret;
	}

	virtual void OnWindowNcLbtnUp(UINT nFlags,CPoint pt)
	{
		__super::OnWindowNcLbtnUp(nFlags, pt);
		UINT u = HitTest(pt);
		switch(u)
		{
		case HTMAXBUTTON:
			{
				if (IsZoomed())
				{
					AfxGetMainWnd()->PostMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
				}
				else
				{
					AfxGetMainWnd()->PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
				}			
			}

			break;
		case HTMINBUTTON:
			AfxGetMainWnd()->PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			break;	
		case HTCLOSE:
			AfxGetMainWnd()->PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
			break;
		}
	}

	virtual LRESULT OnWindowNcHitTest(CPoint point)
	{
		return __super::OnWindowNcHitTest(point);
	}

	virtual void OnCurTabChange(CVisSorTabInfo* pTo,CVisSorTabInfo* pFrom)
	{
		__super::OnCurTabChange(pTo, pFrom);
		ASSERT(pTo);
		if (pFrom)
		{
			((CVisDockFrame*)pFrom->GetTabWnd())->SetActive(FALSE,
				((CVisDockFrame*)pTo->GetTabWnd()));
		}

		((CVisDockFrame*)pTo->GetTabWnd())->SetActive(TRUE,
			pFrom ? ((CVisDockFrame*)pFrom->GetTabWnd()) : NULL);
		Invalidate();
	}

public:
	virtual void OnClickedCmdBtn(int nCmdId)
	{
		if (nCmdId == 1)
		{
			//m_pMainFrm->SwitchWorkGroupShow();
		}
	}
	void SetMainFrm(CMainFrame* pFrm){m_pMainFrm = pFrm;}
protected:
	CMainFrame* m_pMainFrm;
};

class CVisDockPane_locksize : public CVisDockPane
{
public:
	virtual BOOL CanBeResized() const { return FALSE; }
	virtual BOOL IsResizable() const { return FALSE; }
	virtual BOOL LoadState(LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT) -1){return FALSE;}
	virtual BOOL SaveState(LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT) -1){return FALSE;}
};


#define USE_MAP
#define __BaseFrame	CVisImgDockFrame
class CVisBottomResWnd;
class CVisRightInstallPaneWnd;
class CVisProWnd;
class CMainFrame : public __BaseFrame, public CWorkThreadLoading, public IRealtimeDevCallback
{
	
protected: // 仅从序列化创建
	virtual void OnRealtimeVideoCallback(int nDevIdx, CdvImageInterface * pImage);
	virtual void OnDevDiscover(int nIdx, LPCTSTR name, int nType);
	virtual void OnErrorCatch(int);
// 特性
public:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
// 操作
public:
	BOOL		IsFrameWndActive(CInvestFrameWnd * );
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	void		 OnSetImage(CdvImageInterface *);
	virtual void	OnThreadWork(LPARAM, int *);
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	void			InitSubWnd();
	void			InitStatusBar();
	virtual void	GetBorderSize(int & nLeft, int & nRight, int & nBottom);
	bool			OpenFile(LPCTSTR);
// 生成的消息映射函数
protected:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT	OnOpenFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnOpenDirectory(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnDeleteFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnDoMethod(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnDoMethodCancel(WPARAM wParam, LPARAM lParam);
	//资源列表
	LRESULT			OnSelectItem(WPARAM wp, LPARAM lp);
	LRESULT			OnDbclkItem(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
protected:

	void		AddDirectory(LPCTSTR lpszFolder);
	void 		DeleteFile(Res_Info*	pActItem);
	void 		DeleteFile(CItemInterface*	 pActItem);
	void		DoMethod();
	void		SetImageToShowWnd(CImageData* pImgData);
	BOOL		InsertNewInfo(LPCTSTR lpszPath);
	CString		GetDesFolder();
	CString		GetDesPath(const CString&	srcPath,const CString& desFolder);
	CString		GetDesPath(const CString& srcPath);
	void		SaveImgDesFile();
	void		SaveVideoDesFile();
	static void WINAPI ApplyMethodCallBack(int * pWorkMode, LPVOID pData);
	void		IsDesFileExist(CString& strDesPath);

	///////////////////
protected:
	BOOL					m_bTabMode;
	CVisDockPane*			m_pBottomDockPane;		//底部DockPane
	CVisDockPane*			m_pRightDockPane;		//右侧DockPane
	CVisBottomResWnd*			m_pBottomResWnd;
	CVisRightInstallPaneWnd*	m_pRightPane;
	CVisDuiMultiImageWnd*		m_pMainImgWnd;		//主视图


	vector<Res_Info*>		m_vecRes;
	Res_Info*			m_pActItem;
	vector<CImageData*>   m_vecdomethod;
	CWorkThread			m_DoMethodThread;
	HANDLE				m_hExitEvent;

	CdvImageInterface * m_ImgForProc;
	CVisProWnd*			m_pProWnd;
};


