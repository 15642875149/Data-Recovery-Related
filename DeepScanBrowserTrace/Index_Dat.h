#pragma pack(push, curAlignment)
#pragma pack(1)

struct FILEMAP_ENTRY
{
	char dwSig[4];	//��Ŀ��ʶ
	DWORD nBlocks;	//��Ŀռ�ö��ٸ���(128�ֽ�)
};

struct URL_LEAK:FILEMAP_ENTRY
{
    LONGLONG	LastModifiedTime;		//����޸�ʱ��
    LONGLONG	LastAccessedTime;		//������ʱ��
    DWORD		dostExpireTime;			//����ʱ��
    DWORD		dostPostCheckTime;		
    DWORD		dwFileSize;				//Ӳ�̻����е��ļ��Ĵ�С
    DWORD		dwRedirHashItemOffset;	// ask DanpoZ
    DWORD		dwGroupOffset;
    DWORD		data;
    DWORD    CopySize;					// ��������0x60
    DWORD    UrlNameOffset;				// URL����ƫ�ơ�����ַ�Ǳ�URL��Ŀ�Ŀ�ʼ��ַ
    BYTE     DirIndex;					// ���ڵ����ļ�������
    BYTE     bSyncState;				// automatic sync mode state
    BYTE     bVerCreate;				// ������ENTRY��CACHE�İ汾
    BYTE     bVerUpdate;				// ������ENTRY��CACHE�İ汾
	DWORD    InternalFileNameOffset;	//Ӳ�����ļ�����������Ŀ¼���ַ�����ƫ�ƣ�����ַ�Ǳ�URL��Ŀ�Ŀ�ʼ��ַ��
    DWORD    CacheEntryType;			//��������
    DWORD    HeaderInfoOffset;			//��WEB��������ȡ���ļ�ʱ�ķ��ص�HTTPͷ����Ϣ
    DWORD    HeaderInfoSize;			//�ʹ�С��ע��������Ļس����еģ�
    DWORD    FileExtensionOffset;		// should be WORD
    DWORD    dostLastSyncTime;       
    DWORD    NumAccessed;				// ��ȡ����������ʣ�
    DWORD    NumReferences;				// ���ô���
    DWORD    dostFileCreationTime;		// ������ULONG��
};

struct REDIR:FILEMAP_ENTRY
{
	DWORD dwItemOffset;					// offset to hash table item of destination URL
	DWORD dwHashValue;					// destination URL hash value (BUGBUG: collisions?)
};

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

#pragma pack(pop, curAlignment)