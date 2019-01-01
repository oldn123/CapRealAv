
// VisTaskDispatchDetermineApp.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CVisImgVideoProcToolApp 构造

CVisImgVideoProcToolApp::CVisImgVideoProcToolApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
	m_hMutex = NULL;
// #ifndef _DEBUG
// 	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
// #endif
	SetAppID(_T("VisTaskDispatchDetermineApp.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CVisImgVideoProcToolApp 对象

CVisImgVideoProcToolApp theApp;


BOOL CVisImgVideoProcToolApp::InitInstance()
{
	LONG lRet = AVerInitialize();
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
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
	long nBufSize = 2000*2000*3*4; // 单个2000*2000的图像大小
	if (!CVisCommonApi::IsBufferCanAlloc(nBufSize, 6.f))
	{
		AfxMessageBox(_T("系统内存不足,无法启动!"));
		return FALSE;
	}
#endif

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();


	// 解码初始化
	if (!CVisVASoftDeCodec::InitVisDecoder())
	{
		AfxMessageBox(_T("解码器初始化失败！"));
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
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	AVerUninitialize();

	CVisCommonApi::UnInitGDIPlus();

	return CVisAppEx::ExitInstance();
}

// CVisImgVideoProcToolApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CVisImgVideoProcToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CVisImgVideoProcToolApp 自定义加载/保存方法

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

// CVisImgVideoProcToolApp 消息处理程序



