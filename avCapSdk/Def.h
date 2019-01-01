#pragma once

enum
{
	DEMO_STATE_STOP=0,
	DEMO_STATE_PREVIEW=1,
	DEMO_STATE_CAP_IMAGE=2,
	DEMO_STATE_RECORD=4,
	DEMO_STATE_CALLBACK_VIDEO=8,
	DEMO_STATE_CALLBACK_AUDIO=16,
	DEMO_STATE_CALLBACK_ESTS=32
};
enum
{

	//SD&HD
	DEVICETYPE_C727 = 1,
	DEVICETYPE_C729 = 2,
	DEVICETYPE_CD331 = 21,
	//SD&HD; �࿨
	DEVICETYPE_C129 = 11,
	//SD&HD�����ֿܷ�ʹ��,����ά����
	//DEVICETYPE_C874 = 5,//HW;No Crossbar
	//SD
	DEVICETYPE_V1A8C = 10,
	DEVICETYPE_V1A8D = 6,
	DEVICETYPE_C039P = 7,
	DEVICETYPE_C725A = 8,
	DEVICETYPE_C725B = 9,
	DEVICETYPE_CE310B = 25, //CE310B��C725B����һ��, �õ���PCIe��solution(C725B��USB�D��), ����3D Y/C(AV-INݔ��)����
	//SD;�࿨;No Crossbar
	DEVICETYPE_C968 = 12,
	DEVICETYPE_C351 = 14,
	DEVICETYPE_CM118_A = 39,
	DEVICETYPE_C351_PAL_SQ = 16,
	//HD
	DEVICETYPE_C199 = 3,   // VGA only Capture Card PCI-E (D-SUB) 
	DEVICETYPE_C199X = 4,  // VGA Capture Card PCI-E (D-SUB + HDMI)
	DEVICETYPE_CD910 = 15, //No Crossbar
	DEVICETYPE_CD110 = 19, //SDI
	DEVICETYPE_CD750 = 20, //usb
	DEVICETYPE_CU511B = 28,//base on CD750   HDMI/DVI SDI VGA S-Video  Composite
	DEVICETYPE_CE511_HN_4K = 35,//4K�ɼ���
	DEVICETYPE_CE511_MN_4K = 29,//4K�ɼ���
	DEVICETYPE_CE511_MN_HD = 32,//4K�ɼ�����ģ��HD����
	DEVICETYPE_CE511_MN_SD = 33,//4K�ɼ�����ģ��SD����
	DEVICETYPE_CE314_SN = 31,//��·HD_SDI
	DEVICETYPE_CD530 = 22,
	DEVICETYPE_CD311 = 23,//not supported analog audio source(VGA)  //CV511����CD311
	DEVICETYPE_CD311B = 24,//supported analog audio source(VGA)
	DEVICETYPE_CE314_HN = 37,//��·HD_HDMI
	DEVICETYPE_CM311_H = 40,//Mini PCIE HDMI
	DEVICETYPE_CL311_MN = 42,//HD ��·��
	DEVICETYPE_CN311_H = 44,//HD M.2
	DEVICETYPE_CL311_M1 = 46,//HD ��·��
	//HW
	DEVICETYPE_C353 = 13,//HW
	DEVICETYPE_CN331_H = 43,//HW
	DEVICETYPE_CM313B = 26,//HW
	DEVICETYPE_CE330B = 27,//HW
	DEVICETYPE_CU331_HN = 34,
	DEVICETYPE_CL334_SN = 36,
	DEVICETYPE_CL334_HN = 38,
	DEVICETYPE_CL332_HN = 41,
	//HD;�࿨
	DEVICETYPE_CD910_DUO = 17,
	DEVICETYPE_CD910_QUAD = 18,
};

typedef struct _RIFF_HEADER
{
	char		szRiffID[4];        // 'R','I','F','F'
	DWORD	dwRiffSize;
	char		szRiffFormat[4];    // 'W','A','V','E'
}RIFF_HEADER;

typedef struct _WAVE_FORMAT
{
	WORD		wFormatTag;
	WORD		wChannels;
	DWORD		dwSamplesPerSec;
	DWORD		dwAvgBytesPerSec;
	WORD		wBlockAlign;
	WORD		wBitsPerSample;
}WAVE_FORMAT;

typedef struct _FMT_BLOCK
{
	char	szFmtID[4];		// 'f','m','t',' ' please note the
	// space character at the fourth location.
	DWORD	dwFmtSize;
	WAVE_FORMAT	wavFormat;
}FMT_BLOCK;

typedef struct _DATA_BLOCK
{
	char	szDataID[4];		// 'd','a','t','a'
	DWORD	dwDataSize;
}DATA_BLOCK;