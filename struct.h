#include "StdAfx.h" 

#define MSG_DELFILE		WM_USER+10084 //删除文件
#define UM_DELFILE_CUR	WM_USER+10085 //删除当前
#define UM_DELFILE_SEL	WM_USER+10086 //删除选择
#define UM_DELFILE_ALL	WM_USER+10087 //删除全部
#define MSG_ADDFILE		WM_USER+10090	 //添加文件
#define MSG_ADDFILE_DIRECTORY	WM_USER+10091 //添加目录
#define MSG_DOMETHOD		WM_USER + 10094 //执行
#define UM_DO_METHOD_CUR	WM_USER+10089 //对当前执行算法
#define UM_DO_METHOD_SEL	WM_USER+10092 //对选择执行算法
#define UM_DO_METHOD_ALL WM_USER+10093 //对全部执行算法

#define MSG_PRO_PER		WM_USER+10094		//进度条

#define MSG_DOMETHOD_CANCLE	WM_USER+10095
//struct Res_Info
//{
//	DATATYPE		dataType;//数据类型
//	CString		strPath;//文件路径
//	CdvImageInterface*	pImg;//文件类型
//	struct Res_Info()
//	{
//		dataType = IMAGE_DATA;
//		strPath = _T(""); 
//		pImg = NULL;
//	}
//};

struct Res_Info
{
	CString		strPath;//文件路径
	CImageData*	pData;//文件数据
	struct Res_Info()
	{
		strPath = _T(""); 
		pData = NULL;
	}
};


#define UM_CHANGE_FRAME	WM_USER+10085 //向主窗口发送消息
#define UM_ADDFILE					WM_USER+10086
#define UM_REMOVEFILE			WM_USER+10087
#define UM_LOAD_METHOD	WM_USER+10088
#define UM_DO_METHOD			WM_USER+10089

#define UM_HANDLESTATE		WM_USER+10090
#define UM_CANCELHANDLE	WM_USER+10091
#define UM_SET							WM_USER+10092
#define GRID_WIDTH		5
#define GRID_HEIGHT		5
