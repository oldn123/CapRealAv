#pragma once

#include <vector>
using namespace std;
using namespace DuiLib;


class CVisProcAdjustChildWnd : public CVisDuiAdjustWndBase
{
public:
	CVisProcAdjustChildWnd(HWND pParent, HWND pMsgWnd);
	virtual ~CVisProcAdjustChildWnd(void);
public:

protected:
	virtual void		OnUpdate(updateUIType, CImageBaseFilter*);
private:
	HWND m_Parent;	//������ָ��
	HWND m_MsgWnd;	//������Ϣ����ָ��
public:
	
};

#define Use_FPK_adj_fx 1

#if Use_FPK_adj_fx

class CFxAdjPane_FIEx : public CFxAdjPane
{
public:
	CFxAdjPane_FIEx(HWND pMsgCWnd, HWND pParent = NULL);
	virtual ~CFxAdjPane_FIEx();
	virtual DuiLib::CDuiString GetSkinFile();
	//��ӷ���
	void		AddMethod(LPCTSTR lpszCmdName);
	int		GetMethodCnt();
	//���ȥ�ػ���
protected:
	virtual void		OnAddFilter(CImageBaseFilter*);
	virtual void		OnRemoveFilter(CImageBaseFilter*);
	virtual void		OnModifyFilter(CImageBaseFilter*);
	virtual void		OnEnableFilter(BOOL bEnable, CImageBaseFilter*);
	void				OnClick(DuiLib::TNotifyUI& msg);
private:
	HWND m_pWndMsg;
};
#endif