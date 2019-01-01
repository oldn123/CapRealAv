// AutoLock.h: interface for the CAutoLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOLOCK_H__35FBC2A2_A913_4396_882E_483BE68193FB__INCLUDED_)
#define AFX_AUTOLOCK_H__35FBC2A2_A913_4396_882E_483BE68193FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <AFXMT.h>

class CAutoLock  
{
public:
	CAutoLock(CCriticalSection *plock);
	virtual ~CAutoLock();

private:
	CCriticalSection * m_pLock;
};

#endif // !defined(AFX_AUTOLOCK_H__35FBC2A2_A913_4396_882E_483BE68193FB__INCLUDED_)
