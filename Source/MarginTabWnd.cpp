#include "StdAfx.h"
#include "../Header/MarginTabWnd.h"


CMarginTabWnd::CMarginTabWnd(void)
{
}


CMarginTabWnd::~CMarginTabWnd(void)
{
}

void CMarginTabWnd::ScreenToClient(HWND hWnd, LPPOINT pPoint)
{
	if (!pPoint)
	{
		return;
	}
	CPoint point(*pPoint);
	CRect rcFrame;
	::GetWindowRect(hWnd, &rcFrame);
	point.Offset(-rcFrame.left, -rcFrame.top);
	pPoint->x = point.x;
	pPoint->y = point.y;
}

BOOL CMarginTabWnd::IsInCaption(CPoint& point)
{
	CPoint pt(point);
	ScreenToClient(AfxGetMainWnd()->GetSafeHwnd(), &pt);
	//AfxGetMainWnd()->ScreenToClient(&pt);
	CRect rcCaption;
	CRect rcWnd;
	GetWindowRect(&rcWnd);
	rcCaption.SetRect(0,0,rcWnd.Width(), m_rcTabMargin.top);
	if(rcCaption.PtInRect(pt))
	{
		return TRUE;
	}
	
	return FALSE;

}

void CMarginTabWnd::OnWindowNcLbtnDbClick(UINT nFlags,CPoint pt)
{
	__super::OnWindowNcLbtnDbClick( nFlags, pt);

	if (IsInCaption(pt))
	{
		AfxGetMainWnd()->SendMessage(WM_NCLBUTTONDBLCLK, nFlags, MAKELPARAM(pt.x, pt.y));
	}
}

void CMarginTabWnd::OnWindowNcLbtnDown(UINT nFlags,CPoint pt)
{
	__super::OnWindowNcLbtnDown( nFlags, pt);

	if (IsInCaption(pt))
	{
		AfxGetMainWnd()->SendMessage(WM_NCLBUTTONDOWN, nFlags, MAKELPARAM(pt.x, pt.y));
	}

}

void CMarginTabWnd::OnWindowNcLbtnUp(UINT nFlags,CPoint pt)
{
	__super::OnWindowNcLbtnUp( nFlags, pt);

	if (IsInCaption(pt))
	{
		AfxGetMainWnd()->SendMessage(WM_NCLBUTTONUP, nFlags, MAKELPARAM(pt.x, pt.y));
	}
}

void CMarginTabWnd::OnWindowNcMouseMove(UINT nHitTest,CPoint point)
{
	__super::OnWindowNcMouseMove( nHitTest, point);
	if (IsInCaption(point))
	{
		AfxGetMainWnd()->SendMessage(WM_NCMOUSEMOVE, nHitTest, MAKELPARAM(point.x, point.y));
	}
}

void CMarginTabWnd::ReCaluTabSize(CVisSorTabInfo* tabInfo, SIZE& szOne)
{
	ASSERT(tabInfo);

	tabInfo->m_TabSize = szOne;

	m_TabSize = szOne;
}


void CMarginTabWnd::DrawTab( CDC* pDC,CVisSorTabInfo* pTab )
{
	ASSERT(pTab);
	if (pTab->m_pngFile != _T(""))
	{
		CVisPicCache * pDrawPng = CVisPicCache::GetPicCache();
		int iIdx = 0;
		if (pTab->m_bActive)
		{
			iIdx = 2;
		}
		else if (pTab->m_bHover)
		{
			iIdx = 1;
		}	

		CPoint ptDraw = pTab->m_rcTab.TopLeft();

		pDrawPng->DrawPic(pTab->m_pngFile, iIdx, pDC, ptDraw, CSize(pTab->m_TabSize));
	}
	else
	{
		if(pTab->m_iImg>=0)
		{
			CMFCToolBarImages* tImg =  m_imgs[pTab->m_iImg];
			CAfxDrawState ds;
			tImg->PrepareDrawImage(ds);
			int iIdx = 0;
			if (pTab->m_bActive)
			{
				iIdx = 2;
			}
			else if (pTab->m_bHover)
			{
				iIdx = 1;
			}	 
			tImg->Draw(pDC,pTab->m_rcTab.left ,pTab->m_rcTab.top,iIdx);
			tImg->EndDrawImage(ds);
		}
		else
		{
			CRect rcTitle(pTab->m_rcTab);	
			//rcTitle.DeflateRect(m_rcTabMargin.left,m_rcTabMargin.top,m_rcTabMargin.right,m_rcTabMargin.bottom);
			HGDIOBJ oldFont = pDC->GetCurrentFont();
			if (m_TabPos == SORTABPOS_LEFT)
			{ 
				oldFont= pDC->SelectObject(afxGlobalData.fontVertCaption);
			}
			else if(m_TabPos == SORTABPOS_TOP)
			{
				oldFont = pDC->SelectObject(afxGlobalData.fontBold);
			}
			CRect rc;
			if (pTab->m_bActive)
			{ 
				CBrush brActive(RGB(80,80,80));
				pDC->FillRect(rcTitle,&brActive);
			}
			else if (pTab->m_bHover)
			{
				CBrush brHover(RGB(200,200,200));
				pDC->FrameRect(rcTitle,&brHover);
			}
			else 
			{
				CBrush brNormal(RGB(100,100,100));				 
				pDC->FrameRect(rcTitle,&brNormal);
			}
			pDC->DrawText(pTab->m_strTabTitle,rc,DT_CALCRECT);
			if(m_TabPos == SORTABPOS_LEFT)
				pDC->TextOut(rcTitle.left+(rcTitle.Width()-rc.Height())/2,rcTitle.bottom- (rcTitle.Height()-rc.Width())/2,pTab->m_strTabTitle);
			else
				pDC->DrawText(pTab->m_strTabTitle,rcTitle,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			pDC->SelectObject(oldFont);
		}
	}

}

void CMarginTabWnd::OnWindowNcCalcSize( BOOL bCalcValidRects,NCCALCSIZE_PARAMS* lpncsp )
{
	if (m_TabPos == SORTABPOS_LEFT) 
	{
		lpncsp->rgrc[0].left += (m_rcTabMargin.left + m_TabSize.cx);		 
	}
	else if (m_TabPos == SORTABPOS_TOP)
	{
		lpncsp->rgrc[0].top += (m_rcTabMargin.top + m_TabSize.cy);
	}
}


void CMarginTabWnd::AdjustTabRect()
{
	int l = 0; int w = 0;

	int szItem = m_TabCmds.size();
	CPoint ptOffsetLeftTop(2,2);
	CRect rcWnd;
	GetWindowRect(rcWnd);
	CPoint ptOffsetBottom(2,rcWnd.Height()-2);
	CPoint ptOffsetRight(rcWnd.Width()-2,2);

	for (int i=0;i<szItem;i++)
	{
		if (m_TabCmds[i]->m_bLeftTop) // 在左或上
		{
			m_TabCmds.at(i)->SetTabRect(CRect(ptOffsetLeftTop,m_TabCmds[i]->m_TabSize));
			if (m_TabPos == SORTABPOS_LEFT)
			{
				ptOffsetLeftTop.Offset(0,m_TabCmds[i]->m_TabSize.cy);
			}
			else if (m_TabPos == SORTABPOS_TOP)
			{
				ptOffsetLeftTop.Offset(m_TabCmds[i]->m_TabSize.cx,0);
			}
		}
		else   // 在右或下面
		{

			if (m_TabPos == SORTABPOS_LEFT)
			{
				ptOffsetRight.Offset(-m_TabCmds[i]->m_TabSize.cx - 3,1);
				m_TabCmds.at(i)->SetTabRect(CRect(ptOffsetRight,m_TabCmds[i]->m_TabSize));
				//ptOffsetBottom.Offset(0,-m_TabCmds[i]->m_TabSize.cy);
				//m_TabCmds.at(i)->SetTabRect(CRect(ptOffsetBottom,m_TabCmds[i]->m_TabSize));
			}
			else if (m_TabPos == SORTABPOS_TOP)
			{
				ptOffsetRight.Offset(-m_TabCmds[i]->m_TabSize.cx - 3,1);
				m_TabCmds.at(i)->SetTabRect(CRect(ptOffsetRight,m_TabCmds[i]->m_TabSize));
			}
		}		 
	}

	CPoint ptTab(0 + m_rcTabMargin.left, m_nMargin + m_rcTabMargin.top);
	szItem = m_Tabs.size();
	for (int i=0;i<szItem;i++)
	{
		m_Tabs.at(i)->SetTabRect(CRect(ptTab,m_Tabs[i]->m_TabSize));
		if (m_TabPos == SORTABPOS_LEFT)
		{
			ptTab.Offset(0,m_Tabs[i]->m_TabSize.cy);
		}
		else if (m_TabPos == SORTABPOS_TOP)
		{
			ptTab.Offset(m_Tabs[i]->m_TabSize.cx,0);
		}
	}
	if (m_TabPos == SORTABPOS_LEFT)
	{
		ptTab.Offset(0,3);
		int szItem = m_TabCmds.size();		 
		for (int i=0;i<szItem;i++)
		{
			m_TabCmds.at(i)->SetTabRect(CRect(ptTab,m_TabCmds[i]->m_TabSize));
			ptTab.Offset(0,m_TabCmds[i]->m_TabSize.cy);
		}
	}


	SendMessage(WM_NCPAINT,0,0);
}