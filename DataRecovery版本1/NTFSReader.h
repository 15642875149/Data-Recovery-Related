// NTFSReader.h: interface for the NTFSReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTFSREADER_H__18B867A1_115A_44E1_9611_B861FB5549C2__INCLUDED_)
#define AFX_NTFSREADER_H__18B867A1_115A_44E1_9611_B861FB5549C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif

#pragma pack(push, curAlignment)
#pragma pack(1)

struct NTFS//NTFS��������
{
	char		chJumpInstruction[3];//��תָ��
	char		chOemID[4];//���к����б�ʾ�����ļ�ϵͳ��������"NTFS"��
	char		chDummy[4];//�ĸ��ո�
	
	struct NTFS_BPB
	{
		WORD		wBytesPerSec;//ÿ�����ֽ���
		BYTE		uchSecPerClust;//ÿ��������
		WORD		wReservedSec;//��������
		BYTE		uchReserved[3];//����0
		WORD		wUnused1;//δʹ�� Ϊ0
		BYTE		uchMediaDescriptor;//��������
		WORD		wUnused2;//δʹ�� Ϊ0
		WORD		wSecPerTrack;//ÿ�ŵ�������Ŀ
		WORD		wNumberOfHeads;//ÿ�����ͷ��
		DWORD		dwHiddenSec;//��������
		DWORD		dwUnused3;//δʹ�� Ϊ0
		DWORD		dwUnused4;//δʹ�� Ϊ80008000
		LONGLONG	n64TotalSec;//��������
		LONGLONG	n64MFTLogicalClustNum;//$MFT����ʼ�߼��غ�
		LONGLONG	n64MFTMirrLogicalClustNum;//$MFTMirr����ʼ�߼��غ�
		int			nClustPerMFTRecord;//�ļ���¼�Ĵ�С����
		int			nClustPerIndexRecord;//��������Ĵ�С����
		LONGLONG	n64VolumeSerialNum;//�������߼����кţ��˴�Ϊ��ʽ��ʱ�������
		DWORD		dwChecksum;//У���
	} bpb;
	
	char		chBootstrapCode[426];//��������
	WORD		wSecMark;//��������������־55AA
};

struct MFT_HEAD
{//MFTͷ��Ϣ
	char		szSignature[4];		//"FILE"
	WORD		wFixupOffset;		//�������кŵ�ƫ��
	WORD		wFixupSize;			//�������кŵĴ�С������
	LONGLONG	n64LogSeqNumber;	//��־�ļ����к�
	WORD		wSequence;			//���к�
	WORD		wHardLinks;			//Ӳ������
	WORD		wAttribOffset;		//��һ�����Ե�ƫ�Ƶ�ַ
	WORD		wFlags;				//��־λ:00H�ļ���ɾ�� 01H��¼����ʹ�� 02H�ü�¼ΪĿ¼
	DWORD		dwRecLength;		//�ļ���¼��ʵ�ʳ���
	DWORD		dwAllLength;		//�ܹ��������¼�ĳ���
	LONGLONG	n64BaseMftRec;		//�����ļ���¼�е��ļ�������
	WORD		wNextAttrID;		//��һ����ID
	WORD		wFixupPattern;		//�߽�
	DWORD		dwMFTRecNumber;		//MFT��¼���
	WORD		wFixup;				//�������к�
	WORD		wFixupMatrix[3];	//������������
	//������10H-F0H����:NTFS_ATTRIBUTE
};

struct NTFS_ATTRIBUTE	//��������ͷ
{
	DWORD	dwType;					//��������			
	DWORD	dwFullLength;			//��������ͷ���ڵı����Գ���
	BYTE	uchNonResFlag;			//0Ϊ��פ ����Ϊ�ǳ�פ
	BYTE	uchNameLength;			//����������
	WORD	wNameOffset;			//�����Կ�ʼ��ƫ��
	WORD	wFlags;					//����ѹ��ϡ���־
	WORD	wID;					//����ID
	
	union ATTR
	{
		struct RESIDENT
		{//��פ
			DWORD	dwLength;		//�����Եĳ���
			WORD	wAttrOffset;	//�����Ե�ƫ��
			BYTE	uchIndexedTag;	//������־
			BYTE	uchPadding;		//������
		}Resident;//������(������+)��������:NTFS10H-NTFSF0H
		
		struct NONRESIDENT
		{//�ǳ�פ
			LONGLONG	n64StartVCN;		//�ʼ������غ�
			LONGLONG	n64EndVCN;			//��������غ�
			WORD		wDatarunOffset;		//����������Ϣ��ƫ�Ƶ�ַ
			WORD		wCompressionSize;	//ѹ����λ��С
			BYTE		uchPadding[4];		//������
			LONGLONG	n64AllocSize;		//���Է���Ĵ�С
			LONGLONG	n64RealSize;		//������ʵ�Ĵ�С
			LONGLONG	n64StreamSize;		//��������Դ�С
			//������(������+)�������е�����
		}NonResident;
	}Attr;
};

struct STANDARD_INFORMATION
{//10H�������� $STANDARD_INFORMATION
	LONGLONG	n64Create;		//�ļ�����ʱ��
	LONGLONG	n64Modify;		//�ļ�����޸�ʱ��
	LONGLONG	n64Modfil;		//MFT�޸�ʱ��
	LONGLONG	n64Access;		//�ļ�������ʱ��
	DWORD		dwFATAttributes;//��ͳ�ļ�����
	DWORD		dwMaxversion;	//���汾��
	DWORD		dwVersion;		//�汾��
	DWORD		dwID;			//����ID
	DWORD		dwOwnerID;		//������ID
	DWORD		dwSafeID;		//��ȫID
	LONGLONG	dwQuota;		//������
	LONGLONG	dwSerial;		//�������к�
} ;

struct FILE_NAME
{//30H�������� $FILE_NAME 
	LONGLONG	dwMftParentDir; //��Ŀ¼���ļ��ο���
	LONGLONG	n64Create;      //�ļ�����ʱ��
	LONGLONG	n64Modify;      //�ļ��޸�ʱ��
	LONGLONG	n64Modfil;      //MFT�޸�ʱ��
	LONGLONG	n64Access;      //�ļ�������ʱ��
	LONGLONG	n64Allocated;   //�ļ������С
	LONGLONG	n64RealSize;    //�ļ�ʵ�ʴ�С
	DWORD		dwFlags;		//��־λ
	DWORD		dwEAsReparsTag;	//$EA $REPARSE_POINT
	BYTE		chFileNameLength;//�ļ�������
	BYTE		chFileNameType;  //�ļ��������ռ�	
};

struct FAT32//NTFS��������
{
	char		chJumpInstruction[3];//��תָ��
	char		chOemID[8];//���к����б�ʾ�����ļ�ϵͳ��������"NTFS"��
	
	struct FAT32_BPB
	{
		WORD		wBytesPerSec;//ÿ�����ֽ���
		BYTE		uchSecPerClust;//ÿ��������
		WORD		wReservedSec;//����������
		BYTE		wFATNum;//FAT��
		WORD		wRootEntries;//��Ŀ¼����
		WORD		wSmallSector;//С������
		BYTE		uchMediaDescriptor;//��������
		WORD		FATPerSec;//ÿFAT������
		WORD		wSecPerTrack;//ÿ�ŵ�������Ŀ
		WORD		wNumberOfHeads;//ÿ�����ͷ��
		DWORD		dwHiddenSec;//��������
		DWORD		dwTotalSec;//��������
		DWORD		dwSecPerFat;//ÿFAT������
		WORD		dwExtFlag;//��չ��־
		WORD		dwFSVersion;//�ļ�ϵͳ�汾
		DWORD		dwRootClustNum;//��Ŀ¼�غ�
		WORD		dwSectorNumber;//�ļ�ϵͳ��Ϣ������
		WORD		dwBackUpSec;//������������
		BYTE		Reserved[12];
	} bpb;
	
	struct FAT32_EXTBPB
	{
		BYTE		DriveNum;//������������
		BYTE		Reserved;//����
		BYTE		ExtBootSign;//��չ������ǩ
		DWORD		VolSerialNum;//�������
		char		VolLabel[11];//���
		char		SysID[8];//ϵͳID
	} extbpb;
	char		chBootstrapCode[420];//��������
	WORD		wSecMark;//��������������־55AA
};

union FAT32_DIR
{
	struct SHORTDIR//���ļ���Ŀ¼
	{
		char	dwFileName[8];//�ļ����ļ���
		char	dwFileExt[3];//�ļ�����չ��
		BYTE	dwFileAttrib;//�ļ�����
		WORD	Reserved;//����δ��
		WORD	dwCreateTime;//�ļ�����ʱ��
		WORD	dwLastAccessDate;//�ļ�����������
		WORD	dwCreateDate;//�ļ���������
		WORD	dwHigh16;//�ļ���ʼ�غŵĸ�16λ
		WORD	dwSysLastModifyTime;//ϵͳ����޸�ʱ��
		WORD	dwSysLastModifyDate;//ϵͳ����޸ĵ�����
		WORD	dwLow16;//�ļ���ʼ�غŵĵ�16λ
		DWORD	dwFileSize;//�ļ��ĳ���(Byte)
	} shortdir;

	struct LONGDIR//���ļ���Ŀ¼
	{
		BYTE	dwFlag;//��־λ
		char	namepart1[10];//�ļ�����һ����
		WORD	flag1;//���ļ���ΪOFH
		BYTE	Serial;//Ŀ¼����
		char	namepart2[12];//�ļ����ڶ�����
		WORD	Flag;//��Ϊ0
		char	namepart3[4];//�ļ����ĵ�������
	} longdir;
};

struct PartitionTable
{
	BYTE	BootIndicator;
	BYTE	StartingHead;
	WORD	StartData;
	BYTE	PartitionID;
	BYTE	EndingHead;
	WORD	EndData;
	DWORD	RelativeSectors;
	DWORD	TotalSectors;
};

#pragma pack(pop, curAlignment)

struct TableList
{
	PartitionTable	Table;
	TableList*		Next;
	BOOL			IsPartValid;
	TableList()
	{
		Next=NULL;
	}
};

struct FILESTRUCT
{
	CString		name;
	CString		DATA[14];
	int			MFT;
	int			FATHERMFT;
	HTREEITEM	treeitem;

	FILESTRUCT()
	{
		treeitem=NULL;
	}
	virtual ~FILESTRUCT()
	{
		if(DATA[13]!="")
		{
			int RunNum;
			sscanf(DATA[13],"%d",&RunNum);
			delete (CString*)RunNum;
		}
	}
};

struct DirTreeNode
{
	CString			name;
	DirTreeNode*	nextchild;//��һ���ֵܽڵ�
	HTREEITEM		m_TreeItem;
	int				selfMFT;
	int				serial;

	DirTreeNode() 
	{
		nextchild=NULL;
		name="";
		m_TreeItem=NULL;
		selfMFT=-1;
		serial=-1;
	}
};

struct LostFile
{
	CString		name;
	int			FatherMFT;
	int			SelfMFT;
	LostFile*	next;
	int			serial;

	LostFile()
	{
		name="";
		FatherMFT=-1;
		SelfMFT=-1;
		next=NULL;
		serial=-1;
	}
};

class DirTree
{//ɢ����
	private:
		DirTreeNode*	root;
		LostFile*		stillnotfind;//�������ļ�MFT����ɢ������Χ��������
		CTreeCtrl*		m_TreeCtrl;
		CListCtrl*		m_ListCtrl;
		int				TotalNum;
	public:
		DirTree(int num);
		virtual ~DirTree();
		void AddFileToTree(int self,int father,int serial,CString& name);
		void AddToCtrl(CTreeCtrl* Tree,CListCtrl* List);//����ǰ��������CTreeCtrl
	private:
		void AddToCtrlEx(HTREEITEM cur,int serial);
};

class NTFSReader
{
	NTFSReader();
	virtual ~NTFSReader();
};
