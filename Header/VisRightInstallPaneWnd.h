#pragma once

struct sParams
{
	float	fDenosie;
	int		nScl;
	float	nNoise; //噪声方差 0.3
	bool	bUseAddNoise;
	bool	bUseBM3D;
	bool	bUseSSR;

	float fDenoiseParams[6];	// 去噪声的参数，第一个参数需要节目展示，名称：噪声强度，范围是1-100
	float fInterpParams[1];		// 超分辨率的参数

	dvSize	szZoomImgSize;
	dvSize	szOutImgSize;
	vector<std::pair<enum eAlgoType, bool>> fnAlgs;
};

class ICmdParamNotify
{
public:
	virtual void OnParamChanged(int type, bool buse, int val)	= 0;
	virtual bool OnStartRealPlay()								= 0;
};

class CFxAdjPane_FIEx;
class CVisRightInstallPaneWnd : public CVisDuiWndBase,public DuiLib::IMenuNoifyUI
{
public:
	CVisRightInstallPaneWnd(ICmdParamNotify*);
	~CVisRightInstallPaneWnd(); 
	CString  GetDesFolder();
	void		SetUIStatus(BOOL bCanStart);
	void		SetImageData(CImageData* pData);
	void		SetActiveRendWnd(CFxRenderWnd* pWnd) {m_pFxRenderWnd = pWnd;};
	CImageFilterManager*	GetImageFilterManager();
	void		setRealTimeDevInfo(int ,LPCTSTR ,int);
	void		OnRealTimeDevErrorCatch(int);

	void		GetParams(sParams &);
public:
	virtual DuiLib::CDuiString GetSkinFile();
	LPCTSTR			GetWindowClassName() const;
	void				Notify(TNotifyUI& msg);
	void				InitWindow();
	virtual void		OnClick(TNotifyUI& msg);
	virtual void		MenuNotify(CDuiString strMenuCmd, CDuiString strUserData, UINT nTag);
	virtual void		MenuNotifyForType(CDuiString strMenuCmd, CDuiString strUserData, UINT nTag, UINT uType);
	virtual HRESULT		HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

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

	ICmdParamNotify * m_pParamNotify;
};