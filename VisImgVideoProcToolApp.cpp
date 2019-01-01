
// VisTaskDispatchDetermineApp.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "VisImgVideoProcToolApp.h"
#include "Header/MainFrm.h"
#include "Header/VisProWnd.h"
#ifndef _DEBUG
#define _USE_MINIDUMP
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVisImgVideoProcToolApp

BEGIN_MESSAGE_MAP(CVisImgVideoProcToolApp, CVisAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CVisImgVideoProcToolApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CVisImgVideoProcToolApp ����

CVisImgVideoProcToolApp::CVisImgVideoProcToolApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
	m_hMutex = NULL;
// #ifndef _DEBUG
// 	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
// #endif
	SetAppID(_T("VisTaskDispatchDetermineApp.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CVisImgVideoProcToolApp ����

CVisImgVideoProcToolApp theApp;


BOOL CVisImgVideoProcToolApp::InitInstance()
{
	LONG lRet = AVerInitialize();
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CVisAppEx::InitInstance();

	CVisCommonApi::InitGDIPlus();


	CSki::SetSki(SKI_STYLE_DARKBLUE);
	LPCTSTR strPath = CSki::GetDirectResPath();
	CString strXmlPath = CString(strPath) + CString(L"img_config.xml");
	if(PathFileExists(strXmlPath))
		CSki::SetSkiFile(CString(L"DarkBlue\\img_config.xml"));

#ifndef _DEBUG
	long nBufSize = 2000*2000*3*4; // ����2000*2000��ͼ���С
	if (!CVisCommonApi::IsBufferCanAlloc(nBufSize, 6.f))
	{
		AfxMessageBox(_T("ϵͳ�ڴ治��,�޷�����!"));
		return FALSE;
	}
#endif

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();


	// �����ʼ��
	if (!CVisVASoftDeCodec::InitVisDecoder())
	{
		AfxMessageBox(_T("��������ʼ��ʧ�ܣ�"));
		return FALSE;
	}


 	CMainFrame* pFrame = new CMainFrame;
 	if (!pFrame)
 		return FALSE;
 	m_pMainWnd = pFrame;
 	pFrame->LoadFrame(IDR_MAINFRAME,
 		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
 		NULL);
 	pFrame->ShowWindow(SW_SHOW);
 	pFrame->UpdateWindow();

	/*CVisProWnd *pFrameWnd = new CVisProWnd;	
	pFrameWnd->Create(NULL, _T("CVisProWnd"),UI_WNDSTYLE_FRAME , WS_EX_WINDOWEDGE);
	pFrameWnd->CenterWindow();
	pFrameWnd->ShowModal();*/

	/*CVisRightInstallPaneWnd *pFrameWnd = new CVisRightInstallPaneWnd;	
	pFrameWnd->Create(NULL, _T("VisRightInstallPaneWnd"),UI_WNDSTYLE_FRAME , WS_EX_WINDOWEDGE);
	pFrameWnd->CenterWindow();
	pFrameWnd->ShowModal();*/

	return TRUE;
}

int CVisImgVideoProcToolApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	AVerUninitialize();

	CVisCommonApi::UnInitGDIPlus();

	return CVisAppEx::ExitInstance();
}

// CVisImgVideoProcToolApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CVisImgVideoProcToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CVisImgVideoProcToolApp �Զ������/���淽��

void CVisImgVideoProcToolApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
}

void CVisImgVideoProcToolApp::LoadCustomState()
{
}

void CVisImgVideoProcToolApp::SaveCustomState()
{
}

// CVisImgVideoProcToolApp ��Ϣ�������



