// AutoLock.cpp: implementation of the CAutoLock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoLock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoLock::CAutoLock(CCriticalSection *plock)
{
	m_pLock = plock;
    m_pLock->Lock();
}

CAutoLock::~CAutoLock()
{
	m_pLock->Unlock();
}
