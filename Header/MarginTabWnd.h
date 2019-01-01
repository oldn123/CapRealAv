#pragma once
class CMarginTabWnd : public CVisSorTabWnd
{
public:
	CMarginTabWnd(void);
	~CMarginTabWnd(void);

	static void ScreenToClient(HWND hWnd, LPPOINT);

protected:
	virtual BOOL IsInCaption(CPoint&);
protected:
	virtual void OnWindowNcLbtnDown(UINT nFlags,CPoint pt);
	virtual void OnWindowNcLbtnUp(UINT nFlags,CPoint pt);
	virtual void OnWindowNcMouseMove(UINT nHitTest,CPoint point);
	virtual void OnWindowNcLbtnDbClick(UINT nFlags,CPoint pt);
	virtual void ReCaluTabSize(CVisSorTabInfo* tabInfo, SIZE& szOne);
	virtual void DrawTab( CDC* pDC,CVisSorTabInfo* pTab );
	virtual void AdjustTabRect();
	virtual void OnWindowNcCalcSize( BOOL bCalcValidRects,NCCALCSIZE_PARAMS* lpncsp );
};

