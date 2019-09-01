
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


#include "\visystem\VisCommonModule\Include\VisCommonModule.h"
#include "\Visystem\VisImageProcessCommonSrc\src\VisImgDB\Header\VisImgDBHeader.h"
#include "\Visystem\VisImageProcessCommonSrc\src\VisImgDBusiness\Header\VisImgDBusinessHeader.h"
#include "\Visystem\VisImageProcessCommonSrc\src\VisImgDataComm\Header\VisImgDataCommHeader.h"
#include "\Visystem\VisImageProcessCommonSrc\Src\VisImgCommUI\Header\VisImgCommUIHeader.h"
#include "\visystem\VisImageProcessCommonSrc\Src\VisImgProcCommCtrl\Header\VisImgProcCommCtrlHeader.h"

#include ".\avCapSdk\AVerCapAPI_Pro.h"
#include ".\avCapSdk\def.h"

#pragma comment(lib, "AVerCapAPI.lib")


#include <vector>
#include <list>

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "opencv.hpp"

#ifdef _DEBUG
#pragma comment(lib, "opencv_core2410d.lib")
#pragma comment(lib, "opencv_highgui2410d.lib")
#pragma comment(lib, "opencv_imgproc2410d.lib")
#else
#pragma comment(lib, "opencv_core2410.lib")
#pragma comment(lib, "opencv_highgui2410.lib")
#pragma comment(lib, "opencv_imgproc2410.lib")
#endif

#include "GpuIVProcessor.h"
#include "MilliWaveSimulator.h"

// #ifdef _DEBUG
// #pragma comment(lib, "GpuIVProcessord.lib")
// #pragma comment(lib, "MilliWaveSimulatord.lib")
// #else
// #pragma comment(lib, "GpuIVProcessor.lib")
// #pragma comment(lib, "MilliWaveSimulator.lib")
// #endif // DEBUG

#pragma comment(lib, "GpuIVProcessor.lib")
#pragma comment(lib, "MilliWaveSimulator.lib")

#include <vector>
using namespace std;
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


