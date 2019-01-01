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
};