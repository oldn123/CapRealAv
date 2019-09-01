#pragma once

struct sParams
{
	float	fDenosie;
	int		nScl;
	float	nNoise; //�������� 0.3
	bool	bUseAddNoise;
	bool	bUseBM3D;
	bool	bUseSSR;

	float fDenoiseParams[6];	// ȥ�����Ĳ�������һ��������Ҫ��Ŀչʾ�����ƣ�����ǿ�ȣ���Χ��1-100
	float fInterpParams[1];		// ���ֱ��ʵĲ���

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

	void				OpenFile();//����ļ�
	void				OpenDirectory();//���Ŀ¼

	void				DelFileCur();
	void				DelFileSel();
	void				DelFileAll();

	void				DoMethodCur();
	void				DoMethodSel();
	void				DoMethodAll();

	CString			SelTransDir();		//�������Ŀ¼


	void				OnConnectRealTimeDev();

protected:
	//�ؼ�
	CComboBoxUI*		m_pComboArchInstall;		//�㷨����
	CButtonUI*		m_pBtnAddFile;		//����ļ�
	CButtonUI*		m_pBtnAddFile_Install;//����ļ�ѡ��
	CButtonUI*		m_pBtnDelFile;		//ɾ����Ŀ
	CButtonUI*		m_pBtnDelFile_Install;//ɾ����Ŀѡ��
	CButtonUI*		m_pBtnExecute;		//ִ���㷨
	CButtonUI*		m_pBtnExecute_Install;//ִ���㷨ѡ��

	CButtonUI*		m_pBtnDesFolder;		//Ŀ���ļ���
	CButtonUI*		m_pBtnOpenDesFolder;	//��Ŀ��Ŀ¼		
	CEditUI*			m_pEditDesFolder;		//���Ŀ¼�ı���

	CBindHwndUI*		m_pBindWnd;

	CFxAdjPane_FIEx*	m_pMethodPanel;//�㷨���
protected:
	CString			m_strDesFolder;
	CImageData*		m_pImgData;
	CFxRenderWnd*		m_pFxRenderWnd;
	CFxRenderWnd*		m_pTempFxRenderWnd;

	ICmdParamNotify * m_pParamNotify;
};