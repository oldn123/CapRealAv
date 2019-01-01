#include "StdAfx.h"
#include "../Header/VisBottomResWnd.h"
#include "../struct.h"
#include "../Header/VisMessageManager.h"

CVisBottomResWnd::CVisBottomResWnd( void )
{

}

CVisBottomResWnd::~CVisBottomResWnd( void )
{

}

BEGIN_MESSAGE_MAP(CVisBottomResWnd, CMulFrameWnd)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void CVisBottomResWnd::MenuNotify( CDuiString strMenuCmd, CDuiString strUserData, UINT nTag )
{
	if (strMenuCmd == _T("del_cur"))
	{
		DelCurrent();
	}
	else if (strMenuCmd == _T("del_sel"))
	{
		DelSelect();
	}
	else if (strMenuCmd == _T("del_all"))
	{
		DelAll();
	}
}

void CVisBottomResWnd::MenuNotifyForType( CDuiString strMenuCmd, CDuiString strUserData, UINT nTag, UINT uType )
{

}

void CVisBottomResWnd::OnRButtonDown( UINT nFlags, CPoint point )
{
	__super::OnRButtonDown(nFlags,point);
	//ÓÒ¼ü²Ëµ¥
	CMenuManagerUI mgr(this);
	STRINGorID s(_T("\\BatchImgVideoProc\\res_rbtnmenu.xml"));
	CMenuUI* pMenu = mgr.Load(s);
	//µ¯³ö²Ëµ¥
	CPoint pt;
	::GetCursorPos(&pt);
	mgr.Popup(pt, eMenuAlignment_Right | eMenuAlignment_Bottom);
}

void CVisBottomResWnd::DelCurrent()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_DELFILE,UM_DELFILE_CUR,0);
}

void CVisBottomResWnd::DelSelect()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_DELFILE,UM_DELFILE_SEL,0);
}

void CVisBottomResWnd::DelAll()
{
	CVisMessageManager::GetMsgHander()->SendMessage(MSG_DELFILE,UM_DELFILE_ALL,0);
}

