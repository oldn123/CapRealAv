#pragma once
class CVisProWnd : public CVisDuiWndBase
{
public:
	CVisProWnd();
	~CVisProWnd(); 
	//�����ļ��ܳ��� 
	void		SetTotal(LONGLONG lTotal){m_lTotal = lTotal;};
public:
	virtual DuiLib::CDuiString GetSkinFile();
	LPCTSTR			GetWindowClassName() const;
	void				Notify(TNotifyUI& msg);
	void				InitWindow();
	virtual void		OnClick(TNotifyUI& msg);
	LRESULT			HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT			OnProChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);//wParam ��ǰ����
protected:

	void				OnBtnCancel();
protected:
	//�ؼ�

	CButtonUI*		m_pBtnCancel;		//ȡ��
	CProgressUI*		m_pProSingle;		//���ļ�������
	CProgressUI*		m_pProTotal;		//�ܽ�����
	CLabelUI*		m_pLabelPer;		//���ļ�����
	CLabelUI*		m_pLabelTotalPer;	//�ܽ���
	CLabelUI*		m_pLabelCurName;	//��ǰִ���ļ���

protected:
	LONGLONG			m_lCnt;		//�Ѵ�����
	LONGLONG			m_lTotal;	//�ܳ���
	CString			m_strName;	//�����ļ�����
	BOOL				m_bTotalCut;	//�Ƿ����1024
};