//���ڸ���Ŀ����vc6��bug�����ò���vs2010����

//�˸�ʽ���ָ����߷ּ�����
//�ٳ�������ext�ļ����µ�dll��dll��GetSupportExtension�ṩ��dll��ʶ����ļ����ͣ���ģ�齫����֧�ֵ����͸��û���ʾ��
//�ڳ����г�����֧�ֵ�(����)�������ͣ�Ŀǰ֧�ֱ���Ӳ�̡��ƶ�Ӳ�̡�u�̡��Ժ����֧���ڴ��ļ��ָ����г����û���ʾ�Ľ����Ǹ��������ط���������Ӳ�̵��豸���ӷ��š�
//���û�ѡ����Ҫ�������ļ����ͺ�Ҫ�ָ��Ľ��ʣ����������ʼ������
//����ģ�齫�û�ѡ����ļ����ͼ������������ж��ڼ��ļ�ǩ��ֱ�Ӽ����������У����ڸ����ļ�ǩ����Ѻ�����ַ����һ������
//����Ϊ�����ļ�������
//����ģ����豸����0������ʼ��������������ǩ���������û�����ڸ����ļ�ǩ���б��������������û��ֱ�ӽ�����һ������������ҵ�����ת���ޡ�
//�޽��������ĳ�����Ϣ��ʾ���б�ؼ�������ܿ��ٻ�ȡ�ļ���С����Լ��ٱ��������򰴴ر���(ÿ�����0x1000)����¼ÿ���ҵ����ļ������Ϣ���ں�����Ƚ�����
//����Ϊ����ļ�������
//�߶����ѵ���ÿһ���ȡ��Ϣ����λ���ļ������ļ����ݽ���dll��ȡ��ϸ��Ϣ����Щ��Ϣ������ȷ��׺������Ч�ԣ����ȣ����ļ����ض���Ϣ��
//������ļ�������ɺ��Ҽ�����б�ؼ��ᵯ��2���˵���һ���ǻָ�������Ϳ�ʼ�ָ�����ѡ����ļ����ļ���Ϊ��ţ���׺Ϊʶ������ĺ�׺��Ŀ¼Ϊѡ���Ŀ¼��
//��һ���˵��ǲ鿴�ļ���Ϣ��ѡ�����Բ鿴dll���������ʱ�ṩ���ض��ļ���Ϣ

/*
Dll��������Ҫ��

//��֪��ģ���dll���ڸ���buf(����֤���ļ�ǩ��)�Ƿ����ṩ���ٷ����ļ���С����GetFileLenImmediately����ʹ�ÿ����ڳ����ļ���ʽʶ���мӿ����������ٶȡ�
BOOL	CanGetFileLenImmediately;

//�˺�����Ӧ��CanGetFileLenImmediately�����CanGetFileLenImmediately��ΪTRUE��Ӧ���ṩ�˺�����ڿ��ټ����ļ���С�����ڳ����ļ�������
FORMAT_SFILESIZE GetFileLenImmediately(VOID);

//��֪עģ���ļ�ǩ���Ƿ��(����ͨ���ж����������ַ�ȷ��)���˵���ֵ��Ҫ��GetFileSig�����Ի�ȡ�ļ�ǩ����Ϣ
BOOL	IsFileSigSimple;

//�˺������ڻ�ȡdll�е��ļ�ǩ����Ϣ������ļ�ǩ�����Լ򵥶Աȳ�����ֱ�ӽ�ǩ������Щ�ֽڿ���FORMAT_SIMPLETYPE�����������Ϣ���ɣ�
//��������ļ�ǩ��������ͨ������ͷ�ֽڶ��ձȽϳ�������Ӧ�ṩ�ļ�ǩ���жϵ���ں�����������ΪFORMAT_GETCOMPLEXFILETYPE
BOOL	GetFileSig(PVOID pData);//simplefile:pData=FORMAT_SIMPLETYPE* complexfile:pData=FORMAT_GETCOMPLEXFILETYPE*

//��������ļ���������ȡ�ļ���С����չ��Ϣ,��ȡΨһ�ļ���׺
BOOL	GetRawFileInfo(CONST FORMAT_DATAPOINTER pData,FORMAT_FILEINFO* pfileinfo);

//�ṩ��dll֧�ֵ��ļ����ͣ����Է����ļ�������Ҳ���Է������ͺ�׺����Ҫ���в�����Ϊ�ļ����ķǷ��ַ�
VOID	GetSupportExtension(CHAR Ext[MAX_EXTLEN]);
*/

//���뵽���Ż�������˫���������������io����ʱ��ָ�cpu���д����ڴ�����
//�������������ռ��ʱ��̫�����������б����һ��λ��Ϊ�磬����������λ����û����(��ȡ����С�ͺ�׺)��ǿ�ƽ���

#include <windows.h>
#include <winioctl.h>
#include <assert.h>
#include <process.h>

#define IOCTL_DISK_GET_LENGTH_INFO CTL_CODE(IOCTL_DISK_BASE, 0x0017, METHOD_BUFFERED, FILE_READ_ACCESS)

// typedef struct _GET_LENGTH_INFORMATION 
// {
//     LARGE_INTEGER   Length;
// }GET_LENGTH_INFORMATION, *PGET_LENGTH_INFORMATION;

#ifndef FORMAT_COMMON	
#define FORMAT_COMMON

//�������Ҷ���ģ���ϸ˵����FormatDefs.cpp��FormatDefs.h������ֻ��������2���ļ���ر��������Ͷ������ڱ�дwindows dll��
//���ں�windows��mfc�����ݣ�û��ֱ��������2���ļ�
#define FORMAT_API			WINAPI
#define MAX_FILESIGLEN		0x100
#define MAX_CHILDNUM		0x100
#define FORMAT_FAILURE		(DWORD)-1
#define FORMAT_INVALIDID	(DWORD)-1
#define TREE_ADDFAILURE		FALSE
#define TREE_ADDSUCCESS		TRUE
#define MAX_EXTLEN			0x100
#define MAX_DESCIPTION		0x1000

#define CLASS_BEGIN_DEFINETYPE//������ṹ
#define CLASS_END_DEFINETYPE
#define CLASS_BEGIN_DEFMEMBERVAR//�������Ա����
#define CLASS_END_DEFMEMBERVAR
#define CLASS_BEGIN_DEFMEMFUNC//�������Ա����
#define CLASS_END_DEFMEMFUNC

typedef LPCTSTR				FORMAT_EXTENSION;	
typedef BYTE				FORMAT_DATA;				
typedef DWORD				FORMAT_SFILESIZE;
typedef LONGLONG			FORMAT_FILESIZE;
typedef LONGLONG			FORMAT_DISKSIZE;
typedef LONGLONG			FORMAT_FILEPOSITION;
typedef HMODULE				FORMAT_DLLHANDLE;
typedef HANDLE				FORMAT_DISKHANDLE;
typedef INT					FORMAT_DATATYPE;
typedef INT					FORMAT_FILESIGLEN;
typedef BOOL				FORMAT_ISSUCCESS;
typedef INT					FORMAT_FILESIGID;//Ψһ��ӦDll��������Ψһ��Ӧһ��Sig
typedef INT					FORMAT_ERRORCODE;

struct FORMAT_FILEINFO
{
	BYTE cbSize;
	BYTE bitmap;//��ȡ����Щ������Ա����
	FORMAT_FILESIZE filesize;//0x01
	CHAR fileextDeepscan[MAX_EXTLEN];//0x02
	BOOL IsFileValid;//0x04
	CHAR Description[MAX_DESCIPTION];//0x08
	PVOID reserved;
};

struct FORMAT_SIMPLETYPE
{
	FORMAT_DATA FileSig[MAX_FILESIGLEN];
	FORMAT_FILESIGLEN FileSigLen;//AddTreeTrunk����֮ǰ������
};
typedef BOOL (*FORMAT_GETCOMPLEXFILETYPE)(CONST FORMAT_DATA DiskInfo[MAX_FILESIGLEN]);

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

#endif

