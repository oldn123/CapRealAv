#include "StdAfx.h" 

#define MSG_DELFILE		WM_USER+10084 //ɾ���ļ�
#define UM_DELFILE_CUR	WM_USER+10085 //ɾ����ǰ
#define UM_DELFILE_SEL	WM_USER+10086 //ɾ��ѡ��
#define UM_DELFILE_ALL	WM_USER+10087 //ɾ��ȫ��
#define MSG_ADDFILE		WM_USER+10090	 //����ļ�
#define MSG_ADDFILE_DIRECTORY	WM_USER+10091 //���Ŀ¼
#define MSG_DOMETHOD		WM_USER + 10094 //ִ��
#define UM_DO_METHOD_CUR	WM_USER+10089 //�Ե�ǰִ���㷨
#define UM_DO_METHOD_SEL	WM_USER+10092 //��ѡ��ִ���㷨
#define UM_DO_METHOD_ALL WM_USER+10093 //��ȫ��ִ���㷨

#define MSG_PRO_PER		WM_USER+10094		//������

#define MSG_DOMETHOD_CANCLE	WM_USER+10095
//struct Res_Info
//{
//	DATATYPE		dataType;//��������
//	CString		strPath;//�ļ�·��
//	CdvImageInterface*	pImg;//�ļ�����
//	struct Res_Info()
//	{
//		dataType = IMAGE_DATA;
//		strPath = _T(""); 
//		pImg = NULL;
//	}
//};

struct Res_Info
{
	CString		strPath;//�ļ�·��
	CImageData*	pData;//�ļ�����
	struct Res_Info()
	{
		strPath = _T(""); 
		pData = NULL;
	}
};


#define UM_CHANGE_FRAME	WM_USER+10085 //�������ڷ�����Ϣ
#define UM_ADDFILE					WM_USER+10086
#define UM_REMOVEFILE			WM_USER+10087
#define UM_LOAD_METHOD	WM_USER+10088
#define UM_DO_METHOD			WM_USER+10089

#define UM_HANDLESTATE		WM_USER+10090
#define UM_CANCELHANDLE	WM_USER+10091
#define UM_SET							WM_USER+10092
#define GRID_WIDTH		5
#define GRID_HEIGHT		5
