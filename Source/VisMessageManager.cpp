#include "StdAfx.h"
#include "../Header/VisMessageManager.h"

CVisMessageManager* CVisMessageManager::m_MsgHander = NULL;

CVisMessageManager::CVisMessageManager()
{

}

CVisMessageManager::~CVisMessageManager()
{
	if (CVisMessageManager::m_MsgHander != NULL)
	{
		delete m_MsgHander;
	}
}

CVisMessageManager * CVisMessageManager::GetMsgHander()
{
	if (CVisMessageManager::m_MsgHander == NULL)
	{
	CVisMessageManager::m_MsgHander = new CVisMessageManager;
	}
	return CVisMessageManager::m_MsgHander;
	return NULL;
}

void CVisMessageManager::SendMessage( int nType,WPARAM wp,LPARAM lp )
{
	m_MsgWnd->SendMessage(nType,wp,lp);
}
