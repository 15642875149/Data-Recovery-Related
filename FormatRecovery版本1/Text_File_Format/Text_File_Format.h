#include "..\common.h"

#ifndef TEXT_FILE_FORMAT
#define TEXT_FILE_FORMAT

// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� TEXT_FILE_FORMAT_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// TEXT_FILE_FORMAT_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef TEXT_FILE_FORMAT_EXPORTS
#define TEXT_FILE_FORMAT_API __declspec(dllexport)
#else
#define TEXT_FILE_FORMAT_API __declspec(dllimport)
#endif

extern "C"
{
	extern TEXT_FILE_FORMAT_API BOOL CanGetFileLenImmediately;
	extern TEXT_FILE_FORMAT_API BOOL IsFileSigSimple;

	TEXT_FILE_FORMAT_API FORMAT_SFILESIZE GetFileLenImmediately(FORMAT_DATA* pData);
	TEXT_FILE_FORMAT_API BOOL GetFileSig(PVOID pData);
	TEXT_FILE_FORMAT_API BOOL GetRawFileInfo(FORMAT_DATAPOINTER& pData,FORMAT_FILEINFO* pfileinfo);
	TEXT_FILE_FORMAT_API VOID GetSupportExtension(CHAR Ext[MAX_EXTLEN]);
};

#endif
