#include "stdafx.h"
#include "CallBackVideo.h"

CCallBackVideo * CCallBackVideo::m_pInstance = NULL;

CCallBackVideo * CCallBackVideo::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CCallBackVideo;
	}
	return m_pInstance;
}

void CCallBackVideo::Release()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

CCallBackVideo::CCallBackVideo()
{
	m_bHasRunning = false;
	ZeroMemory(&m_VideoCaptureInfo, sizeof(VIDEO_CAPTURE_INFO));
	m_hCaptureDevice = NULL;
	m_pImageFrame = ::GetCVImageInterface();
	m_pIRealtimeNotify = NULL;
	m_hEvtForStart = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hEvtForStop = CreateEvent(NULL,FALSE,FALSE,NULL); 
}

CCallBackVideo::~CCallBackVideo()
{
	InnerStop();

	SetEvent(m_hEvtForStart);
	SetEvent(m_hEvtForStop);

	::ReleaseCVImageInterface(&m_pImageFrame);

	CloseHandle(m_hEvtForStart);
	CloseHandle(m_hEvtForStop);
}

void CCallBackVideo::OnDeviceDisc(int nIdx, LPCTSTR name, int nType)
{
	if (m_pIRealtimeNotify)
	{
		m_pIRealtimeNotify->OnDevDiscover(nIdx, name, nType);
	}
}

bool LoadCapHandle(int * pHandle)
{
	USES_CONVERSION;
	CString sFile = CVisCommonApi::GetAppPath() + L"capHandle.dat";
	FILE * fp = fopen(T2A(sFile), "rb");
	if(fp)
	{
		fread(pHandle, 1, 4, fp);
		fclose(fp);
		return true;
	}
	return false;
}

bool SaveCapHandle(int handle)
{
	USES_CONVERSION;
	CString sFile = CVisCommonApi::GetAppPath() + L"capHandle.dat";
	FILE * fp = fopen(T2A(sFile), "wb");
	if(fp)
	{
		fwrite(&handle, 1, 4, fp);
		fclose(fp);
		return true;
	}
	return false;
}


void CCallBackVideo::OnThreadProc(HWND hWnd, int * pStatus)
{
	DWORD nDevCnt = 0;	
	DWORD nCardType = -1;
	WCHAR wszDeviceName[MAX_PATH] = {0};
	int nDevIdx = 0;
	HANDLE hCap = 0;
	bool bInitDev = false;
	bool bInit = false;
	do 
	{
		do 
		{		
			if(!bInit && AVerInitialize() != CAP_EC_SUCCESS)
			{
				break;
			}
			bInit = true;

			if (nDevCnt < 1 && AVerGetDeviceNum(&nDevCnt) != CAP_EC_SUCCESS)
			{
				break;
			}

			if(nDevCnt < 1)
			{
				break;
			}

			if (wcslen(wszDeviceName) < 1 && AVerGetDeviceName(nDevIdx, wszDeviceName) != CAP_EC_SUCCESS)
			{
				break;
			}
			if (nCardType == -1)
			{	
				AVerGetDeviceType(nDevIdx, &nCardType);
				if (nCardType == -1)
				{
					break;
				}
			}
			
			if (*pStatus != 1)
			{
				break;
			}

			OnDeviceDisc(nDevIdx, wszDeviceName, nCardType);
LOOPWAIT:	
			while(true)
			{
				if(WAIT_TIMEOUT == WaitForSingleObject(m_hEvtForStart, 500))
				{
					continue;
				}
				else
				{
					break;
				}
			}

			if (*pStatus != 1)
			{
				break;
			}

			if (!hCap)
			{
				//连接设备
				LONG lRetVal = AVerCreateCaptureObjectEx(nDevIdx, DEVICETYPE_HD, /*hWnd*/NULL, &hCap);		
				if (lRetVal != 0)
				{
					if (lRetVal == CAP_EC_DEVICE_IN_USE)
					{
						LoadCapHandle((int*)&hCap);
						if (!hCap)
						{
							break;
						}
					}
					else
					{
						break;
					}
				}

				SaveCapHandle((int)hCap);
			}
		
			if(!bInitDev && !DoInit(hCap))
			{
				goto LOOPWAIT;
			}

			bInitDev = true;
			InnerStart();
			goto LOOPWAIT;
		} while (false);

		if (*pStatus == 1)
		{
			Sleep(1000);
		}
		else
		{
			break;
		}
	} while (true);

	AVerUninitialize();
}

bool CCallBackVideo::Start()
{
	SetEvent(m_hEvtForStart);
	m_bHasRunning = true;
	return true;
}

bool CCallBackVideo::Stop()
{
	SetEvent(m_hEvtForStop);
	ResetEvent(m_hEvtForStart);
	{
		CVisAutoLock lk(&m_lkFrame);
		m_bHasRunning = false;
	}
	return true;
}


BOOL WINAPI NotifyEventCallback(DWORD dwEventCode, LPVOID lpEventData, LPVOID lpUserData)
{
	switch (dwEventCode)
	{
	case EVENT_CAPTUREIMAGE:
		{
			if (!lpUserData || !lpEventData)
				return FALSE;

		}
		break;
	case EVENT_CHECKCOPP:
		{
			LONG *plErrorID = (LONG*)lpEventData;
			CStringA strErrorID;
			switch (*plErrorID)
			{
			case COPP_ERR_UNKNOWN:
				strErrorID = "COPP_ERR_UNKNOWN";
				break;
			case COPP_ERR_NO_COPP_HW:
				strErrorID = "COPP_ERR_NO_COPP_HW";
				break;
			case COPP_ERR_NO_MONITORS_CORRESPOND_TO_DISPLAY_DEVICE:
				strErrorID = "COPP_ERR_NO_MONITORS_CORRESPOND_TO_DISPLAY_DEVICE";
				break;
			case COPP_ERR_CERTIFICATE_CHAIN_FAILED:
				strErrorID = "COPP_ERR_CERTIFICATE_CHAIN_FAILED";
				break;
			case COPP_ERR_STATUS_LINK_LOST:
				strErrorID = "COPP_ERR_STATUS_LINK_LOST";
				break;
			case COPP_ERR_NO_HDCP_PROTECTION_TYPE:
				strErrorID = "COPP_ERR_NO_HDCP_PROTECTION_TYPE";
				break;
			case COPP_ERR_HDCP_REPEATER:
				strErrorID = "COPP_ERR_HDCP_REPEATER";
				break;
			case COPP_ERR_HDCP_PROTECTED_CONTENT:
				strErrorID = "COPP_ERR_HDCP_PROTECTED_CONTENT";
				break;
			case COPP_ERR_GET_CRL_FAILED:
				strErrorID = "COPP_ERR_GET_CRL_FAILED";
				break;
			}
			::MessageBoxA(NULL, strErrorID.GetBuffer(0), "AVerCapSDKDemoAP", MB_OK);
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

bool m_bHadSetVideoRenderer = false;
BOOL CCallBackVideo::DoInit(HANDLE hCaptureDevice)
{
	m_hCaptureDevice = hCaptureDevice;
	if (!m_hCaptureDevice)
	{
		return false;
	}

	LONG lRet = 0;
	do 
	{
		if (!m_bHadSetVideoRenderer)
		{
			lRet = AVerSetVideoRenderer(m_hCaptureDevice, VIDEORENDERER_EVR);
			if (lRet != CAP_EC_SUCCESS)
			{
				break;
			}
		}	
		lRet = AVerSetEventCallback(m_hCaptureDevice, NotifyEventCallback, 0, this);
		if (lRet != CAP_EC_SUCCESS)
		{
			break;
		}

		lRet = AVerStartStreaming(m_hCaptureDevice);
		if (lRet != CAP_EC_SUCCESS)
		{
			break;
		}	

		DWORD dwVideoSource = 0;
		lRet = AVerGetVideoSource(m_hCaptureDevice, &dwVideoSource);
		if (dwVideoSource==VIDEOSOURCE_ASI)
		{
			break;
		}
	} while (false);

	if (lRet != CAP_EC_SUCCESS)
	{
		if (m_pIRealtimeNotify)
		{
			m_pIRealtimeNotify->OnErrorCatch(lRet);
		}	
		return false;
	}	

	return TRUE;
}



void CCallBackVideo::onOneFrame(const VIDEO_SAMPLE_INFO & VideoInfo, BYTE *pbData)
{
	if (!m_bHasRunning)
	{
		return;
	}
	CVisAutoLock lk(&m_lkFrame);
	if (!m_pImageFrame->IsValid())
	{
		if (VideoInfo.dwWidth > 0 && VideoInfo.dwHeight > 0)
		{
			dvSize szImg;
			szImg.nWidth = VideoInfo.dwWidth;
			szImg.nHeight= VideoInfo.dwHeight;
			m_pImageFrame->Create(szImg, 8, 3);
		}
		else
		{
			return;
		}
	}

	int nStep = 0;
	if (m_VideoCaptureInfo.dwSaveType == ST_CALLBACK_RGB24)
	{
		nStep = (VideoInfo.dwWidth*3 + 3)&~3;
	}
	else
	{
		ASSERT(FALSE);
		nStep = (VideoInfo.dwWidth*4 + 3)&~3;
	}

	for (UINT i = 0 ; i < VideoInfo.dwHeight; ++i)
	{
		memcpy(m_pImageFrame->GetData()+ (VideoInfo.dwHeight - 1 - i)*nStep, pbData+(VideoInfo.dwHeight-i-1)*VideoInfo.dwStride, nStep);
	}

	if (m_pIRealtimeNotify)
	{
		m_pIRealtimeNotify->OnRealtimeVideoCallback(0, m_pImageFrame);
	}
}

BOOL WINAPI CCallBackVideo::CallbackRaw( VIDEO_SAMPLE_INFO VideoInfo,  BYTE *pbData,  LONG lLength, __int64 tRefTime,  LONGPTR  lUserData)
{
	CCallBackVideo * pThis = (CCallBackVideo * )lUserData;
	pThis->onOneFrame(VideoInfo, pbData);
	return TRUE;
}


bool CCallBackVideo::InnerStart()
{
	m_VideoCaptureInfo.dwCaptureType = CT_SEQUENCE_FRAME;
	m_VideoCaptureInfo.bOverlayMix = TRUE;

	int iSel= 1;
	switch (iSel)
	{
	case 0:m_VideoCaptureInfo.dwSaveType=ST_CALLBACK;
		break;
	case 1:m_VideoCaptureInfo.dwSaveType = ST_CALLBACK_RGB24;
		break;
	case 2:m_VideoCaptureInfo.dwSaveType = ST_CALLBACK_ARGB;
		break;
	}


	m_VideoCaptureInfo.lpCallback = CCallBackVideo::CallbackRaw;
	m_VideoCaptureInfo.lCallbackUserData= (LONGPTR)this;

	LONG lRetVal = AVerCaptureVideoSequenceStart(m_hCaptureDevice, m_VideoCaptureInfo);
	if (lRetVal == CAP_EC_HDCP_PROTECTED_CONTENT)
	{
		return false;
	}

	while(true)
	{
		if (WAIT_TIMEOUT == WaitForSingleObject(m_hEvtForStop, 500))
		{
			continue;
		}
		else
		{
			break;
		}
	}

	return true;
}

bool CCallBackVideo::InnerStop()
{
	AVerCaptureVideoSequenceStop(m_hCaptureDevice);

	SetEvent(m_hEvtForStop);

	return true;
}
