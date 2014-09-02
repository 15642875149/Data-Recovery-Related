// #define REG_NONE						0//û�й̶���ʽ
// #define REG_SZ							1//�̶������ַ���
// #define REG_EXPAND_SZ					2//�ɱ䳤���ַ���
// #define REG_BINARY						3//����������
// #define REG_DWORD						4//32 λ����
// #define REG_DWORD_BIG_ENDIAN			5//32 λ����
// #define REG_LINK						6//Unicode �ַ���
// #define REG_MULTI_SZ					7//����ַ���
// #define REG_RESOURCE_LIST				8//����������
// #define REG_FULL_RESOURCE_DESCRIPTION	9//����������
// #define REG_RESOURCE_REQUIREMENTS_LIST	10//����������
// #define REG_QWORD						11//64 λ����

#pragma pack(push, curAlignment)
#pragma pack(1)

struct REGF//������
{
	char		Signature[4];	//"regf"
	DWORD		Sequence1;		//�������к�
	DWORD		Sequence2;		//�������к�
	LONGLONG	LastWriteTime;	//���һ��д��������ʱ��
	DWORD		Version1;		//���汾��
	DWORD		Version2;		//�ΰ汾��
	DWORD		Type;
	DWORD		Format;
	DWORD		RootCell;		//��һ������¼��ƫ��
	DWORD		Lenght;			//���ݿ鳤��
	DWORD		Cluster;
	char		name[64];		//�����ļ���
	DWORD		Reserved1[99];
	DWORD		CheckSum;		//У���
	DWORD		Reserved2[894];
	DWORD		BootType;
	DWORD		BootRecover;
};

struct HBIN//����
{
	char		Signature[4];	//"hbin"
	DWORD		FileOffset;		//��������Ե�һ��������ʼ��ƫ��
	DWORD		Size;			//������Ĵ�С
	DWORD		Reserved[2];
	LONGLONG	LastWreiteTime;	//���һ��д��������ʱ��
	DWORD		Spare;
};
//���ҿ������ɣ�����ֵ����ȫ��������һ���Ӽ���һ�м�ֵ
struct  Key_Cell//������
{
	DWORD		Size;				//100000000-SizeΪ��С
	char		Signature[2];		//"nk"
	WORD		Flags;				//����:0x2C 0xAC ����:0x20
	LONGLONG	LastWriteTime;		//���һ��д��������ʱ��
	DWORD		Spare;
	DWORD		Parent;				//������ƫ��
	DWORD		SubKeyCounts[2];	//[0]Ϊ�Ӽ��ĸ���
	DWORD		SubKeyList[2];		//[0]Ϊ�Ӽ��б����"�׸�"BIN��ƫ��
	DWORD		Count;				//����ֵ�ĸ���
	DWORD		OffsetToList;		//��ֵ�б��ƫ����
	DWORD		OffsetToSecurity;	//��ȫ��������¼��ƫ��
	DWORD		Class;				//������ƫ��
	DWORD		Reserved[5];
	WORD		NameLength;			//��������
	WORD		ClassLength;		//��������
	//�Ӽ�����
};

struct	KeyNode//ֵ����
{
	DWORD	Size;			//��С
	char	Signature[2];	//"vk"
	WORD	NameLength;		//ֵ���Ƴ���
	WORD	DataLength;		//ֵ���ݳ���
	BYTE	Reserved;
	BYTE	DataType;		//��������
	DWORD	Data;			//�������ݵ�ƫ�ƻ�ֵ����  ��DataLength��λΪ1Ϊ���ݣ�DataLength&0x7FFFFFFFΪ���ݳ���
	DWORD	Type;			//ֵ����
	WORD	Flags;			//�Ƿ������ֶ�,0x0:ֵ��¼������һ������
	WORD	Spare;
	//��ֵ���ƻ� {DWORD ��С;��С ֵ����}
};

struct	KeyChildList//�Ӽ��б���
{
	DWORD	Size;			//��С
	char	Signature[2];		//ri,li,lf,lh
	WORD	Count;			//�������
};//lf,lh���Count��InsideChild	  <==>   ri,li���Count��InsideChild.offset

struct InsideChild
{
	DWORD	offset;			//�������ƫ����
	DWORD	CheckCode;		//�������Ƶ�ǰ4���ַ�����У����
};

struct	KeySecurity//��ȫ����������
{
	DWORD	Size;
	WORD	Signature;		//"sk"
	WORD	Reserved;
	DWORD	Pre;			//��һ��"sk"��¼��ƫ��
	DWORD	Next;			//��һ��"sk"��¼��ƫ��
	DWORD	ReferenceCount;	//���ü���
	DWORD	Length;			//���ݴ�С
	SECURITY_DESCRIPTOR_RELATIVE Descriptor;//����
};

#pragma pack(pop, curAlignment)
