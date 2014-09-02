// Office_Open_XML_Format.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#ifndef OFFICE_OPEN_XML_FORMAT_EXPORTS
#define OFFICE_OPEN_XML_FORMAT_EXPORTS
#endif

#include "Office_Open_XML_Format.h"
#include "defs.h"

//�����ļ���ʽ������������Winhex�����������ʽ�ٿ��ҵĴ���
DWORD dwBlockBytes=0;
BOOL APIENTRY DllMain( HANDLE hModule,  DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			{
				SYSTEM_INFO SysInfo;
				GetSystemInfo(&SysInfo);
				DWORD dwGran=SysInfo.dwAllocationGranularity;
				dwBlockBytes=1000*dwGran;
			}
			break;
    }
    return TRUE;
}


OFFICE_OPEN_XML_FORMAT_API BOOL CanGetFileLenImmediately=TRUE;//���ָ�ʽ�ڽ���ʱ���Ժܿ���ǰ�ƽ�ָ����˿����ṩ�˺���
OFFICE_OPEN_XML_FORMAT_API BOOL IsFileSigSimple=TRUE;

OFFICE_OPEN_XML_FORMAT_API FORMAT_SFILESIZE GetFileLenImmediately(FORMAT_DATA* pData)
{//pData�����ʷ��ʷ�ΧΪdwBlockBytes
	//����FORMAT_DATAPOINTER&���ǰ�ȫ��
	PKZip_Open_XML*  header;
	FORMAT_SFILESIZE filesize=0;
	FORMAT_SFILESIZE size;
	BOOL FIND=FALSE;
// 	try
// 	{
		while(filesize < dwBlockBytes)
		{
			header=(PKZip_Open_XML*)(FORMAT_DATA*)pData;
			if(header->Sig == 0x04034b50)
			{
				size=sizeof(Open_XML_File_Header)+4+header->Header.PKZipFileHeader.FilenameLen+header->Header.PKZipFileHeader.ExtraFieldLen+
					header->Header.PKZipFileHeader.CompressedSize;
				if(size > 0x7FFFFFFF)
				{
					filesize=0x1000;//�����������ֱ�Ӱ��ؼ�������
					break;
				}
				filesize += size;
			}
			else if(header->Sig == 0x02014b50)
			{
				size=sizeof(Open_XML_Dir_Header)+4+header->Header.PKZipCentralDirectoryHeader.FilenameLen+
					header->Header.PKZipCentralDirectoryHeader.ExtraFieldLen+header->Header.PKZipCentralDirectoryHeader.CommentLen;
				if(size > 0x7FFFFFFF)
				{
					filesize=0x1000;//�����������ֱ�Ӱ��ؼ�������
					break;
				}
				filesize += size;
			}
			else if(header->Sig == 0x06054b50)
			{
				size = sizeof(Open_XML_Dir_End)+4+header->Header.PKZipEndCentralDirectory.CommentLen;
				if(size > 0x7FFFFFFF)
				{
					filesize=0x1000;//�����������ֱ�Ӱ��ؼ�������
					break;
				}
				filesize += size;
				FIND=TRUE;
				goto END;
			}
			else
			{//����
				filesize=0x1000;
				break;
			}
			pData += size;
		}
// 	}
// 	catch(...)
// 	{
// 		;
// 	}

END:
	if(!FIND)
		filesize=0x1000;
	return filesize;
}

OFFICE_OPEN_XML_FORMAT_API BOOL GetFileSig(PVOID pData)
{
	FORMAT_DATA sig[]={0x50,0x4B,0x03,0x04,0x14,0x00,0x06,0x00};
	FORMAT_SIMPLETYPE* data=(FORMAT_SIMPLETYPE*)pData;
	memcpy(data->FileSig,sig,sizeof(sig));
	data->FileSigLen=sizeof(sig);
	return TRUE;
}

OFFICE_OPEN_XML_FORMAT_API BOOL GetRawFileInfo(FORMAT_DATAPOINTER& pData,FORMAT_FILEINFO* pfileinfo)
{
	//����FORMAT_DATAPOINTER&���ǰ�ȫ��
	pData.BeforeSeek();//��ײ�ִ�����ô˺�������ָ��
	PKZip_Open_XML*  header;
	FORMAT_SFILESIZE filesize=0;
	BOOL NotFindType=TRUE;
	while(!pData.IsEndOfDisk())
	{
		header=(PKZip_Open_XML*)(FORMAT_DATA*)pData;
		if(header->Sig == 0x04034b50)
		{
			FORMAT_SFILESIZE size=sizeof(Open_XML_File_Header)+4+header->Header.PKZipFileHeader.FilenameLen+header->Header.PKZipFileHeader.ExtraFieldLen+
				header->Header.PKZipFileHeader.CompressedSize;
			if(size > 0x7FFFFFFF)
			{
				filesize=0;//�����������ֱ�Ӱ��ؼ�������
				break;
			}
			filesize += size;
		}
		else if(header->Sig == 0x02014b50)
		{
			FORMAT_SFILESIZE size=sizeof(Open_XML_Dir_Header)+4+header->Header.PKZipCentralDirectoryHeader.FilenameLen+
				header->Header.PKZipCentralDirectoryHeader.ExtraFieldLen+header->Header.PKZipCentralDirectoryHeader.CommentLen;
			if(NotFindType)
			{
				if(!strncmp(header->Header.PKZipCentralDirectoryHeader.FileName,"word/",5))
				{
					pfileinfo->bitmap |= 0x02;
					strcpy_s(pfileinfo->fileextDeepscan,"docx");
					NotFindType=FALSE;
				}
				else if(!strncmp(header->Header.PKZipCentralDirectoryHeader.FileName,"ppt/",4))
				{
					pfileinfo->bitmap |= 0x02;
					strcpy_s(pfileinfo->fileextDeepscan,"pptx");
					NotFindType=FALSE;
				}
				else if(!strncmp(header->Header.PKZipCentralDirectoryHeader.FileName,"xl/",3))
				{
					pfileinfo->bitmap |= 0x02;
					strcpy_s(pfileinfo->fileextDeepscan,"xlsx");
					NotFindType=FALSE;
				}
			}

			if(size > 0x7FFFFFFF)
			{
				filesize=0;//�����������ֱ�Ӱ��ؼ�������
				break;
			}
			filesize += size;
		}
		else if(header->Sig == 0x06054b50)
		{
			FORMAT_SFILESIZE size = sizeof(Open_XML_Dir_End)+4+header->Header.PKZipEndCentralDirectory.CommentLen;
			if(size > 0x7FFFFFFF)
			{
				filesize=0;//�����������ֱ�Ӱ��ؼ�������
				break;
			}
			filesize += size;
			goto END;
		}
		else
		{//����
			filesize=0;
			break;
		}
		pData += filesize;
	}

END:
	pData.AfterSeek();//��ײ�ִ�����ô˺����ָ�ָ��

	if(filesize)
	{
		pfileinfo->bitmap |= 0x01;
		pfileinfo->filesize=filesize;		
	}


	pfileinfo->bitmap |= 0x04;
	pfileinfo->IsFileValid=TRUE;

	return TRUE;
}

OFFICE_OPEN_XML_FORMAT_API VOID GetSupportExtension(CHAR Ext[MAX_EXTLEN])
{
	strcpy_s(Ext,MAX_EXTLEN,"docx or pptx or xlsx");
}