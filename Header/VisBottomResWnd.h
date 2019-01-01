#pragma once

class  CVisBottomResWnd: public CMulFrameWnd,public DuiLib::IMenuNoifyUI
{
public:
	CVisBottomResWnd(void);
	virtual ~CVisBottomResWnd(void);	
	virtual void		MenuNotify(CDuiString strMenuCmd, CDuiString strUserData, UINT nTag);
	virtual void		MenuNotifyForType(CDuiString strMenuCmd, CDuiString strUserData, UINT nTag, UINT uType);
protected:
	afx_msg void		OnRButtonDown	(UINT nFlags, CPoint point);
	
protected:
	void		DelCurrent();
	void		DelSelect();
	void		DelAll();
DECLARE_MESSAGE_MAP()
};

