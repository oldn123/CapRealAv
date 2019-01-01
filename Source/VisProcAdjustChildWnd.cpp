#include "stdafx.h"

#include "..\Header\VisProcAdjustChildWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#else
#define _USE_MINIDUMP
#endif


CVisProcAdjustChildWnd::CVisProcAdjustChildWnd(HWND Parent, HWND MsgWnd)
{
	m_Parent = Parent;
	m_MsgWnd = MsgWnd;
}

CVisProcAdjustChildWnd::~CVisProcAdjustChildWnd(void)
{

}

void		CVisProcAdjustChildWnd :: OnUpdate(updateUIType type, CImageBaseFilter*pFilter)
{
	__super::OnUpdate(type,pFilter);
}


#if Use_FPK_adj_fx

CFxAdjPane_FIEx::CFxAdjPane_FIEx(HWND pMsgCWnd, HWND pParent)
{
	m_pWndMsg = pMsgCWnd;
	SetInstallStatus(TRUE);
	m_pImageFilterMgr = NULL;
	return;
}
void CFxAdjPane_FIEx::OnModifyFilter(CImageBaseFilter* pFilter)
{
	__super::OnModifyFilter(pFilter);
	ASSERT(m_pWndMsg);
}
void CFxAdjPane_FIEx::OnRemoveFilter(CImageBaseFilter* pFilter)
{
	__super::OnRemoveFilter(pFilter);
	ASSERT(m_pWndMsg);
}
void CFxAdjPane_FIEx::OnEnableFilter(BOOL bEnable, CImageBaseFilter* pFilter)
{
	__super::OnEnableFilter(bEnable, pFilter);
	ASSERT(m_pWndMsg);
}
void CFxAdjPane_FIEx::OnAddFilter(CImageBaseFilter* pFilter)
{
	//CdvImageInterface	*m_pImageSrc1 = pFilter->GetSrcImage();
	//ASSERT(pFilter);
	//ASSERT(pFilter->GetSrcImage());
	__super::OnAddFilter(pFilter);
	ASSERT(m_pWndMsg);
}

DuiLib::CDuiString CFxAdjPane_FIEx::GetSkinFile()
{
	return _T("BatchImgVideoProc\\fxAdjust\\ui.xml");
}

void CFxAdjPane_FIEx::OnClick(DuiLib::TNotifyUI& msg)
{
	__super::OnClick(msg);
}

CFxAdjPane_FIEx::~CFxAdjPane_FIEx()
{

}

void CFxAdjPane_FIEx::AddMethod( LPCTSTR lpszCmdName )
{
	GUID mainID = CLSID_IMAGE_MAIN_ENHANCE;
	GUID subID  = CLSID_IMAGE_SUB_BRIGHTCONTRAST;
	if (_tcscmp(lpszCmdName,L"cmd_brightline")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_ENHANCE;
		subID  = CLSID_IMAGE_SUB_BRIGHTCURVE;
	}
	else	if (_tcscmp(lpszCmdName,L"cmd_bright")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_ENHANCE;
		subID  = CLSID_IMAGE_SUB_BRIGHTCONTRAST;
	}
	else	if (_tcscmp(lpszCmdName,L"cmd_gama")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_ENHANCE;
		subID  = CLSID_IMAGE_SUB_GAMAADJ;
	}
	else	if (_tcscmp(lpszCmdName,L"cmd_histogram")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_ENHANCE;
		subID  = CLSID_IMAGE_SUB_HISTADJ;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_sharpen")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_ENHANCE;
		subID  = CLSID_IMAGE_SUB_SHARPEN;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_midfilter")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_FILTER;
		subID  = CLSID_IMAGE_SUB_MIDFILTER;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_odd")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_MULTIFRAME;
		subID  = CLSID_IMAGE_SUB_FIELD;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_removeline")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_MULTIFRAME;
		subID  = CLSID_IMAGE_SUB_REMOVELINE;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_colorinverse")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_COLOR;
		subID  = CLSID_IMAGE_SUB_COLORINVERSE;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_brightinverse")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_COLOR;
		subID  = CLSID_IMAGE_SUB_BRIGHTCONVERT;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_hueadj")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_COLOR;
		subID  = CLSID_IMAGE_SUB_HUEEQUALIZE;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_rotate")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_GEOMETRYTRANSFORM;
		subID  = CLSID_IMAGE_SUB_ROTATE;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_lense")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_GEOMETRYTRANSFORM;
		subID  = CLSID_IMAGE_SUB_LENSE;
	}   
	else if (_tcscmp(lpszCmdName,L"cmd_stab")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_MULTIFRAME;
		subID  = CLSID_IMAGE_SUB_STABILE;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_frameAvg")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_MULTIFRAME;
		subID  = CLSID_IMAGE_SUB_FRAMESAVG;
	}   
	else if (_tcscmp(lpszCmdName,L"cmd_removeFog")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_MULTIFRAME;
		subID  = CLSID_IMAGE_SUB_DEFOGMODEL;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_SmartDeblur")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_BLUR;
		subID  = CLSID_IMAGE_SUB_SMARTDEBULE;
	}   
	else if (_tcscmp(lpszCmdName,L"cmd_LDREnhance")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_MULTIFRAME;
		subID  = CLSID_IMAGE_SUB_LDREnhance;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_clrEnhance")== 0 )
	{
		mainID = CLSID_IMAGE_MAIN_COLOR;
		subID  = CLSID_IMAGE_SUB_COLORENHANCE;
	}
	else if (_tcscmp(lpszCmdName,L"cmd_contStretch")== 0)
	{
		mainID = CLSID_IMAGE_MAIN_COLOR;
		subID  = CLSID_IMAGE_SUB_CONTRASTSTRETCH;
	}
	else
	{	
		if (_tcscmp(lpszCmdName,L"btn_exten")== 0)
		{

		}
		else
		{
		}
		return;
	}
	if (!m_pImageFilterMgr)
	{
		m_pImageFilterMgr = new CImageFilterManager();
	}
	//添加去重机制
	CImageFilterManager* pImageFilterManager = GetImageFilterManager();
	if (pImageFilterManager)
	{
		CImageFilterArray* pArr = pImageFilterManager->GetFilterArray();
		for (int i = 0 ; i < pArr->size();i++)
		{
			if ((pArr->at(i)->GetMajorID() == mainID)&& (pArr->at(i)->GetSubTypeID() == subID) )
			{
				return;
			}
		}
	}
	
	AddFilter(mainID, subID);
}

int CFxAdjPane_FIEx::GetMethodCnt()
{
	return GetFxCtrl()->GetCount();
}

#endif
