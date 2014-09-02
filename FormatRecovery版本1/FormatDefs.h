#include "stdafx.h"
#include <afxtempl.h>

#ifndef FORMAT_RECOVERY
#define FORMAT_RECOVERY

//���峣��
#define FORMAT_API			WINAPI
#define MAX_FILESIGLEN		0x100
#define MAX_CHILDNUM		0x100
#define FORMAT_FAILURE		(DWORD)-1
#define FORMAT_INVALIDID	(DWORD)-1
#define TREE_ADDFAILURE		FALSE
#define TREE_ADDSUCCESS		TRUE
#define MAX_EXTLEN			0x100
#define MAX_DESCIPTION		0x1000

//�ṹ������~~
#define CLASS_BEGIN_DEFINETYPE//������ṹ
#define CLASS_END_DEFINETYPE
#define CLASS_BEGIN_DEFMEMBERVAR//�������Ա����
#define CLASS_END_DEFMEMBERVAR
#define CLASS_BEGIN_DEFMEMFUNC//�������Ա����
#define CLASS_END_DEFMEMFUNC

//����һЩ����������Ե�����
typedef LPCTSTR				FORMAT_EXTENSION;	
typedef BYTE				FORMAT_DATA;				
typedef DWORD				FORMAT_SFILESIZE;//С��4G���ļ���С
typedef LONGLONG			FORMAT_FILESIZE;//����4G���ļ���С
typedef LONGLONG			FORMAT_DISKSIZE;
typedef LONGLONG			FORMAT_FILEPOSITION;
typedef HMODULE				FORMAT_DLLHANDLE;
typedef HANDLE				FORMAT_DISKHANDLE;
typedef INT					FORMAT_DATATYPE;
typedef INT					FORMAT_FILESIGLEN;
typedef BOOL				FORMAT_ISSUCCESS;
typedef INT					FORMAT_FILESIGID;//Ψһ��ӦDll��������Ψһ��Ӧһ��Sig
typedef INT					FORMAT_ERRORCODE;		

typedef BOOL (*FORMAT_DLLFUNC_GETFILESIGNATURE)(PVOID);
typedef FORMAT_ISSUCCESS (*FORMAT_GETFILESIG)(PVOID);
typedef VOID (*FORMAT_GETSUPPORTEXT)(CHAR Ext[MAX_EXTLEN]);
typedef BOOL (*FORMAT_GETCOMPLEXFILETYPE)(CONST FORMAT_DATA DiskInfo[MAX_FILESIGLEN]);

//ǩ�����ڵ�
class SigTreeNode
{
public:
	CLASS_BEGIN_DEFMEMFUNC
		SigTreeNode();
		SigTreeNode*& operator[](FORMAT_FILESIGID Id);//��ȡ�ӽڵ�
		FORMAT_FILESIGID& operator*();//��ȡId
	CLASS_END_DEFMEMFUNC

private:
	CLASS_BEGIN_DEFMEMBERVAR
		SigTreeNode* Child[MAX_CHILDNUM];
		//�ļ�ǩ��ÿһλΪ0x00-0xFF ���Childÿ��Ԫ����Ŷ�Ӧ��ǰλ��ǩ���ֽ��룬������һλǩ���ֽ���
		//����D0CF11E0  ���ڵ�ĵ�D0���ӽڵ����ڣ�D0�ڵ�ĵ�CF���ӽڵ����ڣ�CF�ڵ�ĵ�11���ӽڵ����ڣ�
		//11�ڵ�ĵ�E0���ӽڵ����ڣ���E0���Id����ΪDll��Ӧ�����

		FORMAT_FILESIGID Id;//��ǰ֦�ɶ�ӦID�������������ΪFORMAT_INVALIDID����ʾû��dll֧�ֵ�ǰ���ȵ�sig
	CLASS_END_DEFMEMBERVAR
};

//�ļ�ǩ���ṹ��
struct FORMAT_SIMPLETYPE
{
	FORMAT_DATA FileSig[MAX_FILESIGLEN];//�ļ�ǩ������
	FORMAT_FILESIGLEN FileSigLen;//�ļ�ǩ������                             AddTreeTrunk����֮ǰ������
};

//����ǰ���ڼ��ļ�ǩ���������ļ�ǩ���������ڸ����ļ�ǩ����ȡ���ļ�ǩ����֤������ַ������ʱ�������ļ�ǩ����������������
class CFileSigTree
{
public:
	CLASS_BEGIN_DEFMEMFUNC
		CFileSigTree();
		virtual ~CFileSigTree();
		BOOL AddTreeTrunk(IN CONST FORMAT_SIMPLETYPE& ObjFileSig,IN FORMAT_FILESIGID Id);//�����µ��ļ�ǩ��
		FORMAT_FILESIGID SearchFileSig(IN CONST FORMAT_DATA DiskInfo[MAX_FILESIGLEN]) CONST;//�����ļ�ǩ�������û�ѵ�����FORMAT_INVALIDID
	CLASS_END_DEFMEMFUNC

private:
	CLASS_BEGIN_DEFMEMFUNC
		VOID DelTree(SigTreeNode*);//�ݹ�ɾ��ǩ����
	CLASS_END_DEFMEMFUNC

	CLASS_BEGIN_DEFMEMBERVAR
		SigTreeNode* Root;//ǩ�������ڵ㣬άϵ����ǩ����������
	CLASS_END_DEFMEMBERVAR
};

//�����ڴ�ӳ��ָ�룬����ʵ���������ڴ��ļ�ӳ�䣬�д��Ľ�
typedef class CDiskMappingPointer
{//��ȫ������ʷ�Χ���Ϊm_dwBlockBytes/2
public:
	CLASS_BEGIN_DEFMEMFUNC
		CDiskMappingPointer(FORMAT_DISKHANDLE hDisk,FORMAT_FILEPOSITION Base=0);//��Ҫ�ṩҪ�������豸���
		CDiskMappingPointer(CONST CDiskMappingPointer& ObjPtr);//�Զ�����=()
		virtual ~CDiskMappingPointer();
		FORMAT_FILEPOSITION GetBase(VOID) CONST;//��ȡ��ַ
		FORMAT_DISKSIZE GetDiskSize(VOID) CONST;//��ȡ���豸���̴�С
		FORMAT_FILEPOSITION GetOffset(VOID) CONST;//��ȡƫ��   ��ַ��ƫ�ƹ�ͬȷ����ǰ���ڴ�λ��
		int GetProgress(VOID);//���㵱ǰ����λ��ռ�������̵Ľ���
		BOOL IsEndOfDisk(VOID) CONST;//�Ƿ��Ǵ��̽�β
		BOOL SetDiskPointer(FORMAT_FILEPOSITION pos);//��ȡ�����ض�λ��
		VOID SwapBuf(VOID);
		FORMAT_DATA operator[](FORMAT_SFILESIZE delta) CONST;
		VOID operator+=(FORMAT_SFILESIZE delta);//�ƶ�ָ��
		FORMAT_DATA operator*(VOID) CONST;
		operator FORMAT_DATA*(VOID) CONST;
		operator DWORD*(VOID) CONST;//��ȡ��Ӧ������
		operator int*(VOID) CONST;//��ȡ��Ӧ������
		operator WORD*(VOID) CONST;//��ȡ��Ӧ������
		//���������Ա�������ڿ��ٸı��ļ�ָ���ȡ����
		VOID BeginBatchRead(VOID);
		VOID EndBatchRead(VOID);
		VOID BeforeSeek();//��¼��ǰλ���Ա㻹ԭλ��
		VOID SeekFromOrigin(FORMAT_SFILESIZE delta);//�Ӽ�¼λ��Ϊ��ʼ�ƶ��ڴ�ָ�룬��ȡ����
		VOID AfterSeek();//��ԭ����¼λ�õ�����
	CLASS_END_DEFMEMFUNC

	CLASS_BEGIN_DEFMEMBERVAR
		FORMAT_SFILESIZE m_dwBlockBytes;
	CLASS_END_DEFMEMBERVAR

private:
	CLASS_BEGIN_DEFMEMBERVAR
		FORMAT_DATA* m_Data;
		FORMAT_FILEPOSITION m_Base;
		FORMAT_FILEPOSITION m_CurrentPos;//m_CurrentPos < m_dwBlockBytes
		CONST FORMAT_DISKHANDLE m_DiskHandle;
		FORMAT_DISKSIZE m_DiskSize;
		BOOL m_IsCurrentBufBuf1;
		//����������Ա�������ڿ��ٸı��ļ�ָ���ȡ����
		FORMAT_FILEPOSITION m_oBase;
		FORMAT_FILEPOSITION m_oCurrentPos;
		FORMAT_DATA* m_oData;
	CLASS_END_DEFMEMBERVAR
}FORMAT_DATAPOINTER;

typedef CONST FORMAT_SFILESIZE (*FORMAT_DLLFUNC_GETFILELENIMMEDATELY)(FORMAT_DATA*);

struct FORMAT_FILEINFO
{
	BYTE cbSize;
	BYTE bitmap;//ָ����Щ��Ա������Ч����Чλ�ڳ�Ա���ʾ
	FORMAT_FILESIZE filesize;//0x01
	CHAR fileextDeepscan[MAX_EXTLEN];//0x02
	BOOL IsFileValid;//0x04
	CHAR Description[MAX_DESCIPTION];//0x08
	PVOID reserved;//������չ
};

typedef BOOL (*FORMAT_DLLFUNC_GETRAWFILEINFO)(FORMAT_DATAPOINTER&,FORMAT_FILEINFO*);
struct FORMAT_FILETYPEINFO
{//��dll�л�ȡ��Ϣ��������������common.h�����۹�
	INT cbSize;
	BOOL IsFileSigSimple;
	FORMAT_DLLHANDLE DllHandle;
	BOOL CanGetFileLenImmediately;
	FORMAT_GETSUPPORTEXT GetSupportExtension;
	FORMAT_DLLFUNC_GETRAWFILEINFO GetRawFileInfo;
	FORMAT_DLLFUNC_GETFILELENIMMEDATELY GetFileLenFast;//���뷵��С�ļ�����
	PVOID reserved;
};

struct FORMAT_COMPLEXFILESIG
{//�����ļ�ǩ��
	FORMAT_FILESIGID Id;
	FORMAT_GETCOMPLEXFILETYPE GetComplexFileType;
};

//CListCtrl��ʾ���⣺ 0:���(�ļ���) 1:�ļ����� 2:�ļ���С 3:�Ƿ���Ч
struct StorageData
{//��¼��������������������ݻָ�
	FORMAT_FILESIGID Id;
	FORMAT_FILEPOSITION Base;
	FORMAT_FILEPOSITION Offset;
	FORMAT_FILESIZE FileSize;
	CHAR Desciption[MAX_DESCIPTION];
	BOOL IsValid;
};

class CFormatRecovery
{//һ�����һ��Disk HANDLE
public:
	CLASS_BEGIN_DEFMEMFUNC
		CFormatRecovery(FORMAT_DISKHANDLE,CListCtrl&,CProgressCtrl&);
		virtual ~CFormatRecovery();
		BOOL	AnalysisDisk(VOID);//�������̣���ʼ��������
		BOOL	DoGetInfo(CString& result) CONST;//�����������ȡdll���ص��ļ���չ��Ϣ
		BOOL	DoRecovery(CONST CString& RecoverDir) CONST;//�ָ�ѡ���ļ�
		FORMAT_FILESIGID FastGetSpecFileExt(IN CONST FORMAT_DATA DiskInfo[MAX_FILESIGLEN],OUT CString&) CONST;//ͨ��ǩ�����͸���ǩ�������õ����������
		FORMAT_ERRORCODE GetErrorCode(VOID) CONST;//Ԥ�������ƽӿ�
		VOID	GetSupportFileExtension(OUT CList<CString,CString&>&) CONST;//��ȡ��ǰ���֧�ֵ���չ��ȫ��
		VOID	Initialize(IN CList<CString,CString&>&);//����Ϊ��Ҫ���ҵ���չ������
		VOID	Stop(VOID);//ֹͣ�����ļ�����
		BOOL	IsSearching(VOID);
	CLASS_END_DEFMEMFUNC

	CLASS_BEGIN_DEFMEMBERVAR
		CList<FORMAT_FILETYPEINFO,FORMAT_FILETYPEINFO&> m_FileInfo;
		CListCtrl& m_ListCtrl;//SetItemData��Ŵ�С��λ�����ڻָ�
	CLASS_END_DEFMEMBERVAR
private:
	CLASS_BEGIN_DEFMEMFUNC	
		BOOL DeepAnalysisFile(IN FORMAT_DATAPOINTER&,FORMAT_FILESIGID,OUT FORMAT_FILEINFO*) CONST;//ͨ��Dll GetRawFileInfo�õ���ȼ����
		VOID SetErrorCode(FORMAT_ERRORCODE);//Ԥ�������ƽӿ�
	CLASS_END_DEFMEMFUNC

	CLASS_BEGIN_DEFMEMBERVAR
		BOOL m_Stop;
		BOOL m_IsSearching;
		INT m_DllNum;
		CFileSigTree m_SigTree;
		CProgressCtrl& m_ProgressCtrl;
		FORMAT_ERRORCODE m_ErrorCode;
		FORMAT_DATATYPE m_CurrentType;
		CONST FORMAT_DISKHANDLE m_DiskHandle;
		CList<FORMAT_COMPLEXFILESIG,FORMAT_COMPLEXFILESIG&> m_ComplexFileSig;
	CLASS_END_DEFMEMBERVAR
};
#endif

//���������б�ؼ���ͨ�ú�
#define MakeCompareFunc(Class,ListCtrl) \
	namespace Class##SPACE	\
	{	\
		int CALLBACK CompareFuncUp(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort)	\
		{	\
			Class* pdlg=(Class*)lParamSort;	\
			if(pdlg->col == 0)	\
				return atoi(pdlg->ListCtrl.GetItemText(lParam1,pdlg->col)) < atoi(pdlg->ListCtrl.GetItemText(lParam2,pdlg->col));\
			else if(pdlg->col == 2)	\
				return _atoi64(pdlg->ListCtrl.GetItemText(lParam1,pdlg->col)) < _atoi64(pdlg->ListCtrl.GetItemText(lParam2,pdlg->col));\
			else	\
				return pdlg->ListCtrl.GetItemText(lParam1,pdlg->col) < pdlg->ListCtrl.GetItemText(lParam2,pdlg->col);	\
		}	\
		int CALLBACK CompareFuncDown(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort)	\
		{	\
			Class* pdlg=(Class*)lParamSort;	\
			if(pdlg->col == 0)	\
				return atoi(pdlg->ListCtrl.GetItemText(lParam1,pdlg->col)) > atoi(pdlg->ListCtrl.GetItemText(lParam2,pdlg->col));\
			else if(pdlg->col == 2)	\
				return _atoi64(pdlg->ListCtrl.GetItemText(lParam1,pdlg->col)) > _atoi64(pdlg->ListCtrl.GetItemText(lParam2,pdlg->col));\
			else	\
				return pdlg->ListCtrl.GetItemText(lParam1,pdlg->col) > pdlg->ListCtrl.GetItemText(lParam2,pdlg->col);	\
		}	\
	};\
	using namespace Class##SPACE;
