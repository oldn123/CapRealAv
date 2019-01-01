#pragma once

class CVisMessageManager
{
public:
	CVisMessageManager();
	~CVisMessageManager();
	static CVisMessageManager *GetMsgHander();
	void SetMsgWnd(CWnd *p){m_MsgWnd = p;}
	void SendMessage(int nType,WPARAM wp,LPARAM lp);
protected:
	static CVisMessageManager* m_MsgHander;
	CWnd * m_MsgWnd;
};