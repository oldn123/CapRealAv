#pragma once
class CVisProWnd : public CVisDuiWndBase
{
public:
	CVisProWnd();
	~CVisProWnd(); 
	//设置文件总长度 
	void		SetTotal(LONGLONG lTotal){m_lTotal = lTotal;};
public:
	virtual DuiLib::CDuiString GetSkinFile();
	LPCTSTR			GetWindowClassName() const;
	void				Notify(TNotifyUI& msg);
	void				InitWindow();
	virtual void		OnClick(TNotifyUI& msg);
	LRESULT			HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT			OnProChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);//wParam 当前进度
protected:

	void				OnBtnCancel();
protected:
	//控件

	CButtonUI*		m_pBtnCancel;		//取消
	CProgressUI*		m_pProSingle;		//单文件进度条
	CProgressUI*		m_pProTotal;		//总进度条
	CLabelUI*		m_pLabelPer;		//单文件进度
	CLabelUI*		m_pLabelTotalPer;	//总进度
	CLabelUI*		m_pLabelCurName;	//当前执行文件名

protected:
	LONGLONG			m_lCnt;		//已处理长度
	LONGLONG			m_lTotal;	//总长度
	CString			m_strName;	//设置文件名称
	BOOL				m_bTotalCut;	//是否除以1024
};