#pragma once
class CFxAdjPane_FIEx;
class CVisRightInstallPaneWnd : public CVisDuiWndBase,public DuiLib::IMenuNoifyUI
{
public:
	CVisRightInstallPaneWnd();
	~CVisRightInstallPaneWnd(); 
	CString  GetDesFolder();
	void		SetUIStatus(BOOL bCanStart);
	void		SetImageData(CImageData* pData);
	void		SetActiveRendWnd(CFxRenderWnd* pWnd) {m_pFxRenderWnd = pWnd;};
	CImageFilterManager*	GetImageFilterManager();
	void		setRealTimeDevInfo(int ,LPCTSTR ,int);
	void		OnRealTimeDevErrorCatch(int);
public:
	virtual DuiLib::CDuiString GetSkinFile();
	LPCTSTR			GetWindowClassName() const;
	void				Notify(TNotifyUI& msg);
	void				InitWindow();
	virtual void		OnClick(TNotifyUI& msg);
	virtual void		MenuNotify(CDuiString strMenuCmd, CDuiString strUserData, UINT nTag);
	virtual void		MenuNotifyForType(CDuiString strMenuCmd, CDuiString strUserData, UINT nTag, UINT uType);
protected:
	void				OnBtnAddFile();
	void				OnBtnAddFile_Install();
	void				OnBtnDelFile();
	void				OnBtnDelFile_Install();
	void				OnBtnExecuteArch();
	void				OnBtnExecuteArch_Install();
	void				OnBtnSelArch();
	void				OnBtnDesFolder();
	void				OnBtnOpenDesFolder();
	void				OpenDesFolder(LPCTSTR lpszPath);

	void				OpenFile();//添加文件
	void				OpenDirectory();//添加目录

	void				DelFileCur();
	void				DelFileSel();
	void				DelFileAll();

	void				DoMethodCur();
	void				DoMethodSel();
	void				DoMethodAll();

	CString			SelTransDir();		//设置输出目录


	void				OnConnectRealTimeDev();

protected:
	//控件
	CComboBoxUI*		m_pComboArchInstall;		//算法配置
	CButtonUI*		m_pBtnAddFile;		//添加文件
	CButtonUI*		m_pBtnAddFile_Install;//添加文件选择
	CButtonUI*		m_pBtnDelFile;		//删除条目
	CButtonUI*		m_pBtnDelFile_Install;//删除条目选择
	CButtonUI*		m_pBtnExecute;		//执行算法
	CButtonUI*		m_pBtnExecute_Install;//执行算法选择

	CButtonUI*		m_pBtnDesFolder;		//目标文件夹
	CButtonUI*		m_pBtnOpenDesFolder;	//打开目标目录		
	CEditUI*			m_pEditDesFolder;		//输出目录文本框

	CBindHwndUI*		m_pBindWnd;

	CFxAdjPane_FIEx*	m_pMethodPanel;//算法面板
protected:
	CString			m_strDesFolder;
	CImageData*		m_pImgData;
	CFxRenderWnd*		m_pFxRenderWnd;
	CFxRenderWnd*		m_pTempFxRenderWnd;
};