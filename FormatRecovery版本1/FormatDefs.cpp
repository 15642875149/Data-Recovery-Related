#include "stdafx.h"
#include <afxtempl.h>
#include "FormatDefs.h"
#include <shlwapi.h>
#include <winioctl.h>

#define IOCTL_DISK_GET_LENGTH_INFO CTL_CODE(IOCTL_DISK_BASE, 0x0017, METHOD_BUFFERED, FILE_READ_ACCESS)

// typedef struct _GET_LENGTH_INFORMATION 
// {
//     LARGE_INTEGER   Length;
// }GET_LENGTH_INFORMATION, *PGET_LENGTH_INFORMATION;

VOID CleanCListCtrl(CListCtrl& list)
{//�����ظ�����
	int total=list.GetItemCount();
	for(int i=0;i<total;i++)
	{
		StorageData* pData=(StorageData*)list.GetItemData(i);
		if(pData)
			delete pData;
	}
	list.DeleteAllItems();
}

SigTreeNode::SigTreeNode()
{
	for(int i=0;i<MAX_FILESIGLEN;i++)
	{
		Child[i]=NULL;
	}
	Id=FORMAT_INVALIDID;
}

SigTreeNode*& SigTreeNode::operator [](FORMAT_FILESIGID Id)
{//���صĽڵ�����޸�
	return Child[Id];
}

FORMAT_FILESIGID& SigTreeNode::operator*()
{
	return Id;
}

CFileSigTree::CFileSigTree()
{
	Root=new SigTreeNode;
}

CFileSigTree::~CFileSigTree()
{
	DelTree(Root);
	delete Root;
}

VOID CFileSigTree::DelTree(SigTreeNode* CurrentTrunk)
{
	if(!CurrentTrunk)
		return;
	for(int i=0;i<MAX_CHILDNUM;i++)
	{
		if((*CurrentTrunk)[i])
			DelTree((*CurrentTrunk)[i]);
	}
	if(CurrentTrunk != Root)
		delete CurrentTrunk;
}

BOOL CFileSigTree::AddTreeTrunk(CONST FORMAT_SIMPLETYPE& ObjFileSig,FORMAT_FILESIGID Id)
{
	SigTreeNode* Cur=Root;
	int i;
	for(i=0;i<ObjFileSig.FileSigLen-1;i++)//����ǰN�����ṹ
	{
		SigTreeNode*& FindNode=(*Cur)[ObjFileSig.FileSig[i]];
		if(!FindNode)
		{
			FindNode=new SigTreeNode;
		}
		Cur=FindNode;
	}
	
	//�����һ�����Id
	SigTreeNode*& FindNode=(*Cur)[ObjFileSig.FileSig[i]];
	if(!FindNode)
	{
		FindNode=new SigTreeNode;
		**FindNode=Id;
	}
	else
	{//����ظ��ͳ�ͻ
		if(**FindNode == FORMAT_INVALIDID)
		{
			**FindNode=Id;
			return TREE_ADDSUCCESS;
		}
		if(**FindNode != Id)//
		{
			return TREE_ADDFAILURE;
		}
	}

	return TREE_ADDSUCCESS;
}

FORMAT_FILESIGID CFileSigTree::SearchFileSig(CONST FORMAT_DATA DiskInfo[MAX_FILESIGLEN]) CONST
{
	SigTreeNode* Cur=Root;
	for(int i=0;i<MAX_FILESIGLEN;i++)
	{
		SigTreeNode*& FindNode=(*Cur)[DiskInfo[i]];
		if(!FindNode) //���ӽڵ㲻����˵���Ѿ��ҵ���ͷ
			return **Cur;
		Cur=FindNode;
	}
	return **Cur;
}

CDiskMappingPointer::CDiskMappingPointer(FORMAT_DISKHANDLE hDisk,FORMAT_FILEPOSITION Base):m_DiskHandle(hDisk),m_Base(((Base)>>12)<<12)
{
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	DWORD dwGran=SysInfo.dwAllocationGranularity;
	m_dwBlockBytes=1000*dwGran;
	m_Data=new FORMAT_DATA[m_dwBlockBytes*2];//˫������
	if(m_Data == NULL)
	{
		MessageBox(NULL,"�ڴ治��","�ڴ治��",MB_OK);
		exit(-1);
	}
	LARGE_INTEGER li;
	li.QuadPart=m_Base;

	//��ȡָ��ͷ������
	SetFilePointer(m_DiskHandle,li.LowPart,&li.HighPart,FILE_BEGIN);
	DWORD retlen;
	ReadFile(m_DiskHandle,m_Data,m_dwBlockBytes,&retlen,NULL);//����һ�黺����
	m_IsCurrentBufBuf1=TRUE;
	//��ȡ���̴�С
	m_DiskSize=0;
	GET_LENGTH_INFORMATION getlen;
	if(DeviceIoControl(hDisk,IOCTL_DISK_GET_LENGTH_INFO,NULL,0,(LPVOID)&getlen,sizeof(getlen),&retlen,NULL))
		m_DiskSize=getlen.Length.QuadPart;
	m_CurrentPos=(Base&0xFFF);
}

CDiskMappingPointer::CDiskMappingPointer(CONST CDiskMappingPointer& ObjPtr):m_Base(ObjPtr.m_Base),m_DiskHandle(ObjPtr.m_DiskHandle)
{//�������
	m_dwBlockBytes=ObjPtr.m_dwBlockBytes;
	m_CurrentPos=ObjPtr.m_CurrentPos;
	m_DiskSize=ObjPtr.m_DiskSize;
	m_Data=new FORMAT_DATA[m_dwBlockBytes];
	memcpy(m_Data,ObjPtr.m_Data,m_dwBlockBytes);
}

CDiskMappingPointer::~CDiskMappingPointer()
{
	delete []m_Data;
}

FORMAT_DATA CDiskMappingPointer::operator [](FORMAT_SFILESIZE delta) CONST//��������Ҫ��֤�����Ϸ�
{
	return m_Data[m_CurrentPos+delta];
}

VOID CDiskMappingPointer::operator+=(FORMAT_SFILESIZE delta)//��������Ҫ��֤�����Ϸ�
{//�����޸����λ��ָ��
	SetDiskPointer(m_Base+m_CurrentPos+delta);
}

FORMAT_DATA CDiskMappingPointer::operator*() CONST
{
	return m_Data[m_CurrentPos];
}

CDiskMappingPointer::operator FORMAT_DATA*() CONST
{//ǿ��ת��ΪFORMAT_DATA*
	return (m_Data+m_CurrentPos);
}

CDiskMappingPointer::operator DWORD*() CONST
{//ǿ��ת��ΪDWORD*
	return (DWORD*)(m_Data+m_CurrentPos);
}

CDiskMappingPointer::operator int*() CONST
{//ǿ��ת��Ϊint*
	return (int*)(m_Data+m_CurrentPos);
}

CDiskMappingPointer::operator WORD*() CONST
{//ǿ��ת��ΪWORD*
	return (WORD*)(m_Data+m_CurrentPos);
}

VOID CDiskMappingPointer::SwapBuf()
{
	if(!m_IsCurrentBufBuf1)
		m_Data = m_oData;//����ǰΪ�ڶ����������ͽ�������һ��������
	else
		m_Data = m_oData+m_dwBlockBytes;//����ǰΪ��һ���������ͽ������ڶ���������
	m_Base += m_dwBlockBytes;
	DWORD retlen;
	ReadFile(m_DiskHandle,m_Data,m_dwBlockBytes,&retlen,NULL);
	m_IsCurrentBufBuf1 = !m_IsCurrentBufBuf1;
}

VOID CDiskMappingPointer::BeginBatchRead()
{
	m_IsCurrentBufBuf1=TRUE;
	m_oData=m_Data;
}

VOID CDiskMappingPointer::EndBatchRead()
{
	m_IsCurrentBufBuf1=TRUE;
	m_Data=m_oData;
}

VOID CDiskMappingPointer::BeforeSeek()
{
	m_oBase=m_Base;
	m_oCurrentPos=m_CurrentPos;
}

VOID CDiskMappingPointer::SeekFromOrigin(FORMAT_SFILESIZE delta)
{
	if(m_oCurrentPos+delta < m_dwBlockBytes/2 && m_Base == m_oBase)
	{
	}
	else//���ָ�뷢�����ƶ�
	{//�ƻ�ԭλ�ò���ȡ����
		SetDiskPointer(m_oBase+m_oCurrentPos+delta);
	}
}

VOID CDiskMappingPointer::AfterSeek()
{//�ָ�ָ�뼰����
	if(m_Base != m_oBase)
	{
		SetDiskPointer(m_oBase+m_oCurrentPos);
	}
	m_Base=m_oBase;
	m_CurrentPos=m_oCurrentPos;
}

int CDiskMappingPointer::GetProgress()
{
	return int(1000*(float)(m_CurrentPos+m_Base)/m_DiskSize);
}

BOOL CDiskMappingPointer::IsEndOfDisk() CONST
{
	return (m_Base+m_CurrentPos) > m_DiskSize;
}

BOOL CDiskMappingPointer::SetDiskPointer(FORMAT_FILEPOSITION pos)
{//���þ���λ��ָ��
	if(pos>m_DiskSize)
	{
		m_CurrentPos=pos-m_Base;
		return FALSE;
	}

	FORMAT_FILEPOSITION newpos=((pos>>12)<<12);//����Ϊ��λ�����ַ
	if(m_Base <= newpos && newpos <= m_Base+m_dwBlockBytes/2-0x1000 && m_CurrentPos < m_dwBlockBytes/2)
	{
		m_CurrentPos=pos-m_Base;
	}
	else
	{
		m_Base=((pos>>12)<<12);
		m_CurrentPos=(pos&0xFFF);//����ƫ�ƣ�Ҳ����ȥ���˻�ַ���ǲ���
		LARGE_INTEGER li;
		li.QuadPart=m_Base;
		SetFilePointer(m_DiskHandle,li.LowPart,&li.HighPart,FILE_BEGIN);
		DWORD retlen;
		ReadFile(m_DiskHandle,m_Data,m_dwBlockBytes,&retlen,NULL);
	}

	return TRUE;
}

FORMAT_DISKSIZE CDiskMappingPointer::GetDiskSize() CONST
{
	return m_DiskSize;
}

CFormatRecovery::CFormatRecovery(FORMAT_DISKHANDLE DiskHandle,CListCtrl& ObjListCtrl,CProgressCtrl& ObjProgressCtrl):
	m_ListCtrl(ObjListCtrl),m_DiskHandle(DiskHandle),m_ProgressCtrl(ObjProgressCtrl)
{
	ASSERT(DiskHandle != INVALID_HANDLE_VALUE);
	CleanCListCtrl(ObjListCtrl);
	m_ProgressCtrl.SetRange(0,1000);
	m_Stop=FALSE;
	m_IsSearching=FALSE;
}

CFormatRecovery::~CFormatRecovery()
{
	POSITION pos=m_FileInfo.GetHeadPosition();
	CleanCListCtrl(m_ListCtrl);
	for(int i=0;i<m_FileInfo.GetCount();i++)
	{
		FreeLibrary(m_FileInfo.GetNext(pos).DllHandle);
	}
}

CFormatRecovery* pFather=NULL;
DWORD blockbytes=0;
CString result="";
CString index="";
FORMAT_FILESIZE globalcount=0;
int globalindex=0;

DWORD WINAPI SearchThread(FORMAT_DATA* fData)
{
	DWORD curpos,offset;


	for(curpos=0;curpos<blockbytes;curpos += offset)
	{
		offset=0x1000;//�ش�С
		FORMAT_FILESIGID id=pFather->FastGetSpecFileExt(fData+curpos,result);
		if(FORMAT_INVALIDID == id)
		{//δ��ȡ��
		}
		else
		{
			index.Format("%d",globalindex);
			pFather->m_ListCtrl.InsertItem(globalindex,index);
			pFather->m_ListCtrl.SetItemText(globalindex,1,result);
			pFather->m_ListCtrl.SetItemText(globalindex,3,"δ��֤");
		
			StorageData* pData=new StorageData;
			memset(pData,0,sizeof(StorageData));
			pData->Base=globalcount;
			pData->Offset=curpos;
			pData->Id=id;
		
			FORMAT_FILETYPEINFO& info=pFather->m_FileInfo.GetAt(pFather->m_FileInfo.FindIndex(id));
			if(info.CanGetFileLenImmediately)
			{
				FORMAT_SFILESIZE filesize=info.GetFileLenFast(fData+curpos);//�����ȡʧ����Ҫ���شش�С�Ա�ָ�����
				result.Format("%d",filesize);
				pFather->m_ListCtrl.SetItemText(globalindex,2,result);
				pData->FileSize=filesize;
				pFather->m_ListCtrl.SetItemData(globalindex,(DWORD)pData);
				offset=max(0x1000,(((filesize-1)>>12)<<12));//��ʽ��ƫ�ƣ���ʵָ�����������GetFileLenFast����
			}
			else
			{
				pFather->m_ListCtrl.SetItemData(globalindex,(DWORD)pData);
			}
			globalindex++;
		}
	}
	globalcount += blockbytes;
	return 0;
}

BOOL CFormatRecovery::AnalysisDisk()
{
	CDiskMappingPointer Pointer(m_DiskHandle,0);//��0������ʼǳ������
	int i=0;
	m_IsSearching=TRUE;
	blockbytes=Pointer.m_dwBlockBytes;
	pFather=this;
	FORMAT_DATA* pData;
	globalcount=0;
	globalindex=0;
	try
	{
		Pointer.BeginBatchRead();
		while(!Pointer.IsEndOfDisk() && !m_Stop)
		{
			m_ProgressCtrl.SetPos(Pointer.GetProgress());
			pData=(FORMAT_DATA*)Pointer;
			HANDLE hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SearchThread,pData,0,NULL);
			Pointer.SwapBuf();//ƽ��IO����ʱ��
			WaitForSingleObject(hThread,INFINITE);
		}
		Pointer.EndBatchRead();
	}
	catch(...)
	{
		;
	}

	if(m_Stop)
		m_Stop=FALSE;

	//��ʼ�����������������
#ifdef _DEBUG
	char format[0x100];
	CFile log("log.txt",CFile::modeCreate | CFile::modeWrite);
#endif

	int listnum=m_ListCtrl.GetItemCount();
	for(i=0;i<listnum && !m_Stop;i++)
	{
		m_ProgressCtrl.SetPos(int(1000*(float)i/listnum));
		StorageData* pData=(StorageData*)m_ListCtrl.GetItemData(i);

#ifdef _DEBUG
		sprintf_s(format,"���%6d��ƫ��%0I64X\n",i,pData->Base+pData->Offset);
		log.Write(format,strlen(format));
#endif

		Pointer.SetDiskPointer(pData->Base+pData->Offset);//���¼���λ��
		FORMAT_FILEINFO fi;
		memset(&fi,0,sizeof(fi));
		fi.cbSize=sizeof(fi);
		if(!DeepAnalysisFile(Pointer,pData->Id,&fi))
			continue;
		CString str;
		if(fi.bitmap&0x01)
		{
			pData->FileSize=fi.filesize;
			str.Format("%I64d",pData->FileSize);
			m_ListCtrl.SetItemText(i,2,str);
		}
		if(fi.bitmap&0x02)
		{
			m_ListCtrl.SetItemText(i,1,fi.fileextDeepscan);
		}
		if(fi.bitmap&0x04)
		{
			pData->IsValid=fi.IsFileValid;
			if(pData->IsValid)
				m_ListCtrl.SetItemText(i,3,"��Ч");
			else
				m_ListCtrl.SetItemText(i,3,"��Ч");
		}
		if(fi.bitmap&0x08)
		{
			strncpy_s(pData->Desciption,fi.Description,MAX_DESCIPTION);
		}
	}

#ifdef _DEBUG
	log.Close();
#endif

	m_ProgressCtrl.SetPos(0);
	m_Stop=FALSE;
	m_IsSearching=FALSE;
	return TRUE;
}

BOOL CFormatRecovery::DoGetInfo(CString& result) CONST
{//��ȡdll�ṩ�������ļ���Ϣ
	POSITION itor=m_ListCtrl.GetFirstSelectedItemPosition();
	if(!itor)
		return FALSE;
	while(itor)
	{
		int index=m_ListCtrl.GetNextSelectedItem(itor);
		StorageData* pData=(StorageData*)m_ListCtrl.GetItemData(index);
		result+=m_ListCtrl.GetItemText(index,0)+_T(".")+m_ListCtrl.GetItemText(index,1)+_T(":\r\n");
		result+=pData->Desciption;
		result+="\r\n";
	}
	return TRUE;
}

BOOL CFormatRecovery::DoRecovery(CONST CString& RecoverDir) CONST
{
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	DWORD dwGran=SysInfo.dwAllocationGranularity;
	DWORD WriteOnce=0x200*dwGran;

	POSITION itor=m_ListCtrl.GetFirstSelectedItemPosition();
	if(!itor)
		return FALSE;
	while(itor)
	{
		int index=m_ListCtrl.GetNextSelectedItem(itor);
		StorageData* pData=(StorageData*)m_ListCtrl.GetItemData(index);
		if(pData->FileSize <= 0 || !pData->IsValid)
			continue;
		//�����ļ�����·��+���+.+��׺
		HANDLE hFile=CreateFile(RecoverDir+_T("\\")+m_ListCtrl.GetItemText(index,0)+_T(".")+m_ListCtrl.GetItemText(index,1),GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hFile == INVALID_HANDLE_VALUE)
			continue;

		CDiskMappingPointer Pointer(m_DiskHandle,pData->Base+pData->Offset);//�ؼ���λ��
		FORMAT_FILESIZE i;
		DWORD writenum;
		for(i=WriteOnce;i<pData->FileSize;i+=WriteOnce)
		{
			WriteFile(hFile,(LPVOID)(FORMAT_DATA*)Pointer,WriteOnce,&writenum,NULL);
			Pointer+=WriteOnce;
		}
		WriteFile(hFile,(LPVOID)(FORMAT_DATA*)Pointer,DWORD(pData->FileSize+WriteOnce-i),&writenum,NULL);
		CloseHandle(hFile);
	}
	return TRUE;
}

FORMAT_ERRORCODE CFormatRecovery::GetErrorCode() CONST
{
	return m_ErrorCode;
}

VOID CFormatRecovery::GetSupportFileExtension(CList<CString,CString&>& strlist) CONST
{//������֧�ֵ��ļ����û���ʾ
	strlist.RemoveAll();
	CHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,dir);
	int pos=_tcslen(dir)-1;
	if(dir[pos] == _T('\\'))
		dir[pos]=_T('\0');
	StrCat(dir,_T("\\ext\\*.dll"));
	CFileFind finder;
	BOOL bResult=finder.FindFile(dir);
	
	while(bResult)
	{
		bResult=finder.FindNextFile();
		FORMAT_FILETYPEINFO fileinfo;
		memset(&fileinfo,0,sizeof(fileinfo));
		fileinfo.cbSize=sizeof(fileinfo);
		fileinfo.DllHandle=LoadLibrary(finder.GetFilePath());
		if(!fileinfo.DllHandle)
			continue;
		
		fileinfo.GetSupportExtension=(FORMAT_GETSUPPORTEXT)GetProcAddress(fileinfo.DllHandle,"GetSupportExtension");
		if(!fileinfo.GetSupportExtension)
			continue;
		CHAR Ext[MAX_EXTLEN];
		memset(Ext,0,MAX_EXTLEN);
		fileinfo.GetSupportExtension(Ext);
		strlist.AddTail(CString(Ext));
	}
	finder.Close();
}

VOID CFormatRecovery::Initialize(CList<CString,CString&>& ExtToFind)
{
	m_DllNum=0;
	CHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,dir);
	int pos=_tcslen(dir)-1;
	if(dir[pos] == _T('\\'))
		dir[pos]=_T('\0');
	StrCat(dir,_T("\\ext\\*.dll"));
	CFileFind finder;
	BOOL bResult=finder.FindFile(dir);
	while(bResult)
	{
		bResult=finder.FindNextFile();
		FORMAT_FILETYPEINFO fileinfo;
		memset(&fileinfo,0,sizeof(fileinfo));
		fileinfo.cbSize=sizeof(fileinfo);
		fileinfo.DllHandle=LoadLibrary(finder.GetFilePath());
		if(!fileinfo.DllHandle)
			continue;
		
		fileinfo.GetSupportExtension=(FORMAT_GETSUPPORTEXT)GetProcAddress(fileinfo.DllHandle,"GetSupportExtension");
		if(!fileinfo.GetSupportExtension)
			continue;
		
		CHAR Ext[MAX_EXTLEN];
		memset(Ext,0,MAX_EXTLEN);
		fileinfo.GetSupportExtension(Ext);
		if(!ExtToFind.Find(CString(Ext)))//�����ں�ѡ�б���ɾ��
			continue;

		BOOL* bool1=(BOOL*)GetProcAddress(fileinfo.DllHandle,"IsFileSigSimple");
		if(!bool1)
			continue;
		fileinfo.IsFileSigSimple=*bool1;
		
		bool1=(BOOL*)GetProcAddress(fileinfo.DllHandle,"CanGetFileLenImmediately");
		if(!bool1)
			continue;
		if(*bool1)
		{
			fileinfo.GetFileLenFast=(FORMAT_DLLFUNC_GETFILELENIMMEDATELY)GetProcAddress(fileinfo.DllHandle,"GetFileLenImmediately");
			if(!fileinfo.GetFileLenFast)
				continue;
		}
		
		fileinfo.CanGetFileLenImmediately=*bool1;
		
		FORMAT_DLLFUNC_GETFILESIGNATURE fgetsig=(FORMAT_DLLFUNC_GETFILESIGNATURE)GetProcAddress(fileinfo.DllHandle,"GetFileSig");
		if(!fgetsig)
			continue;
		
		fileinfo.GetRawFileInfo=(FORMAT_DLLFUNC_GETRAWFILEINFO)GetProcAddress(fileinfo.DllHandle,"GetRawFileInfo");
		if(!fileinfo.GetRawFileInfo)
			continue;
		
		if(fileinfo.IsFileSigSimple)
		{
			FORMAT_SIMPLETYPE simpleinfo;
			if(fgetsig((PVOID)&simpleinfo) && simpleinfo.FileSigLen<MAX_FILESIGLEN)
				m_SigTree.AddTreeTrunk(simpleinfo,m_DllNum);
		}
		else
		{
			FORMAT_COMPLEXFILESIG complexsig;
			memset(&complexsig,0,sizeof(complexsig));
			if(fgetsig((PVOID)&complexsig.GetComplexFileType) && complexsig.GetComplexFileType)
			{
				complexsig.Id=m_DllNum;
				m_ComplexFileSig.AddTail(complexsig);
			}
		}
		
		m_FileInfo.AddTail(fileinfo);	
		m_DllNum++;
	}
	finder.Close();
}

FORMAT_FILESIGID CFormatRecovery::FastGetSpecFileExt(CONST FORMAT_DATA DiskInfo[MAX_FILESIGLEN],CString& Output) CONST
{
	POSITION pos;
	FORMAT_FILESIGID id=m_SigTree.SearchFileSig(DiskInfo);
	CHAR Ext[MAX_EXTLEN];
	memset(Ext,0,MAX_EXTLEN);

	//ͨ��ǩ������ȡ
	if(id != FORMAT_INVALIDID && (pos=m_FileInfo.FindIndex(id)))
	{
		m_FileInfo.GetAt(pos).GetSupportExtension(Ext);
		Output=Ext;
		return id;
	}

	//ͨ�����Ӻ���ǩ����ȡ
	pos=m_ComplexFileSig.GetHeadPosition();
	for(int i=0;i<m_ComplexFileSig.GetCount();i++)
	{
		CHAR Ext[MAX_EXTLEN];
		memset(Ext,0,MAX_EXTLEN);
		CONST FORMAT_COMPLEXFILESIG* cursig=&m_ComplexFileSig.GetNext(pos);
		if(cursig->GetComplexFileType(DiskInfo))
		{
			POSITION pos2=m_FileInfo.FindIndex(cursig->Id);
			if(pos2)
			{
				id=cursig->Id;
				m_FileInfo.GetAt(pos2).GetSupportExtension(Ext);
				Output=Ext;
				return id;
			}
		}
	}

	return id;
}

BOOL CFormatRecovery::DeepAnalysisFile(FORMAT_DATAPOINTER& DiskInfo,FORMAT_FILESIGID Id,FORMAT_FILEINFO* Output) CONST
{//��ȷ����ļ�
	POSITION pos=m_FileInfo.FindIndex(Id);
	if(pos)
	{
		m_FileInfo.GetNext(pos).GetRawFileInfo(DiskInfo,Output);
		return TRUE;
	}
	return FALSE;
}

VOID CFormatRecovery::SetErrorCode(FORMAT_ERRORCODE ErrorCode)
{
	m_ErrorCode=ErrorCode;
}

VOID CFormatRecovery::Stop()
{
	m_Stop=TRUE;
}

BOOL CFormatRecovery::IsSearching()
{
	return m_IsSearching;
}