#include "stdafx.h"
#include "../struct.h"
#include "../Header/VisProWnd.h"
#include "../Header/VisMessageManager.h"


CVisProWnd::CVisProWnd()
{
	m_pBtnCancel = NULL;		//取消
	m_pProSingle = NULL;		//单文件进度条
	m_pProTotal = NULL;		//总进度条
	m_pLabelPer = NULL;		//单文件进度
	m_pLabelTotalPer = NULL;	//总进度
	m_pLabelCurName = NULL;	//当前执行文件名

	m_lTotal = 0;
	m_lCnt = 0;
	m_bTotalCut = FALSE;
}

CVisProWnd::~CVisProWnd()
{

}

DuiLib::CDuiString CVisProWnd::GetSkinFile()
{
	return _T("\\BatchImgVideoProc\\pro_per.xml");
}

LPCTSTR CVisProWnd::GetWindowClassName() const
{
	return _T("VisProWnd");
}

void CVisProWnd::Notify( TNotifyUI& msg )
{
	__super::Notify(msg);
}

void CVisProWnd::InitWindow()
{
	m_pBtnCancel	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_cancel")));		
	m_pProSingle	= static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("pro_singlecnt")));		
	m_pProTotal		= static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("pro_totalcnt")));		
	m_pLabelPer		= static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_singleper")));		
	m_pLabelTotalPer= static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_totalper")));	
	m_pLabelCurName	= static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("lab_filename")));

	m_pProSingle->SetValue(0);
	m_pProTotal->SetValue(0);

	if (m_lTotal>> 32)
	{
		m_pProTotal->SetMaxValue(m_lTotal/1024);
		m_bTotalCut = TRUE;
	}
	else
	{
		m_pProTotal->SetMaxValue(m_lTotal);
		m_bTotalCut = FALSE;
	}
}

void CVisProWnd::OnClick( TNotifyUI& msg )
{
	CString curName = msg.pSender->GetName();
	CString curType = msg.sType;
	if (_tcscmp(curType,_T("click"))==0)
	{
		if (_tcscmp(curName,_T("btn_cancel"))== 0)
		{
			OnBtnCancel();
		}
		else if (_tcscmp(curName,_T("closebtn"))== 0)
		{
			OnBtnCancel();
		}
	}
	return __super::OnClick(msg);
}

LRESULT CVisProWnd::OnProChange( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CImageData*	pData = (CImageData*)wParam;
	ASSERT(pData);
	LONGLONG nCurCnt = (LONGLONG)lParam;
	LONGLONG nCurLength = 0;
	nCurLength = pData->Length();
	LONGLONG nSingleCnt = (LONGLONG)lParam;
	m_pLabelCurName->SetText(pData->GetResPath());
	m_pLabelCurName->SetToolTip(pData->GetResPath());
	m_pProSingle->SetMaxValue(nCurLength);
	m_pProSingle->SetValue(nSingleCnt);
	m_lCnt++;
	if (m_bTotalCut)
	{
		m_pProTotal->SetValue(m_lCnt/1024);
	}
	m_pProTotal->SetValue(m_lCnt);
	float fCur = (float)nSingleCnt/nCurLength;
	float fTotal = (float)m_lCnt/m_lTotal;
	CString strPercent;
	if (fCur > 0)
	{
		strPercent.Format(_T("%0.2f%%"),fCur*100);
		m_pLabelPer->SetText(strPercent);
	}
	if (fTotal > 0)
	{
		strPercent.Format(_T("%0.2f%%"),fTotal*100);
		m_pLabelTotalPer->SetText(strPercent);
	}
	return 0;
}

LRESULT CVisProWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case MSG_PRO_PER:		
		lRes = OnProChange(uMsg, wParam, lParam, bHandled); 
		break;
	default:
		bHandled = FALSE;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CVisProWnd::OnBtnCancel()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_DOMETHOD_CANCLE,0,0);
	Close();
}
