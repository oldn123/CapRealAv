#pragma once

class IRealtimeDevCallback
{
public:
	virtual void OnRealtimeVideoCallback(int nDevIdx, CdvImageInterface * pImage) = 0;
	virtual void OnDevDiscover(int nIdx, LPCTSTR name, int nType) = 0;
	virtual void OnErrorCatch(int) = 0;
};

class CCallBackVideo 
{
public:
	virtual ~CCallBackVideo();

protected:
	CCallBackVideo();   
	HANDLE m_hCaptureDevice;
	VIDEO_CAPTURE_INFO m_VideoCaptureInfo;
	CdvImageInterface * m_pImageFrame;

public:
	static  void	Release();
	static  CCallBackVideo * GetInstance();

	void	OnThreadProc(HWND, int *);
	void	SetCallbackNotify(IRealtimeDevCallback * p){m_pIRealtimeNotify = p;}
	bool	Start();
	bool	Stop();

protected:
	bool	InnerStart();
	bool	InnerStop();
	BOOL	DoInit(HANDLE hCaptureDevice);
	void	OnDeviceDisc(int nIdx, LPCTSTR name, int nType);
	static BOOL WINAPI CallbackRaw( VIDEO_SAMPLE_INFO VideoInfo,  BYTE *pbData,  LONG lLength, __int64 tRefTime,  LONGPTR  lUserData);
	void	onOneFrame(const VIDEO_SAMPLE_INFO & VideoInfo, BYTE *pbData);

protected:
	static  CCallBackVideo *	m_pInstance;
	IRealtimeDevCallback *		m_pIRealtimeNotify;
	HANDLE						m_hEvtForStart;
	HANDLE						m_hEvtForStop;
};
