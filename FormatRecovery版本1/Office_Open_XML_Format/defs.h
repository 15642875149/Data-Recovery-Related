#include <windows.h>

#pragma pack(push)
#pragma pack(1)

// OFFSET              Count TYPE   Description
// 0000h                   4 char   ID='PK',03,04
// 0004h                   1 word   Version needed to extract archive
// 0006h                   1 word   General purpose bit field (bit mapped)
// 									  0 - file is encrypted
// 									  1 - 8K/4K sliding dictionary used
// 									  2 - 3/2 Shannon-Fano trees were used
// 									3-4 - unused
// 								   5-15 - used internally by ZIP
// 								 Note:  Bits 1 and 2 are undefined if the
// 										compression method is other than
// 										type 6 (Imploding).
// 0008h                   1 word   Compression method (see table 0010)
// 000Ah                   1 dword  Original DOS file date/time (see table 0009)
// 000Eh                   1 dword  32-bit CRC of file (inverse??)
// 0012h                   1 dword  Compressed file size
// 0016h                   1 dword  Uncompressed file size
// 001Ah                   1 word   Length of filename
// 								 ="LEN"
// 001Ch                   1 word   Length of extra field
// 								 ="XLN"
// 001Eh               "LEN" char   path/filename
// 001Eh               "XLN" char   extra field
// +"LEN"


// OFFSET              Count TYPE   Description
// 0000h                   4 char   ID='PK',01,02
// 0004h                   1 byte   Version made by
// 0005h                   1 byte   Host OS (see table 0011)
// 0006h                   1 byte   Minimum version needed to extract
// 0007h                   1 byte   Target OS
// 								 see above "Host OS"
// 0008h                   1 word   General purpose bit flag
// 								 see above "General purpose bit flag"
// 000Ah                   1 word   Compression method
// 								 see above "Compression method"
// 000Ch                   1 dword  DOS date / time of file
// 0010h                   1 dword  32-bit CRC of file (see table 0009)
// 0014h                   1 dword  Compressed size of file
// 0018h                   1 dword  Uncompressed size of file
// 001Ch                   1 word   Length of filename
// 								 ="LEN"
// 001Eh                   1 word   Length of extra field
// 								 ="XLN"
// 0020h                   1 word   Length of file comment
// 								 ="CMT"
// 0022h                   1 word   Disk number ??
// 0024h                   1 word   Internal file attributes (bit mapped)
// 									0 - file is apparently an ASCII/binary file
// 								 1-15 - unused
// 0026h                   1 dword  External file attributes (OS dependent)
// 002Ah                   1 dword  Relative offset of local header from the
// 								 start of the first disk this file appears on
// 002Eh               "LEN" char   Filename / path; should not contain a drive
// 								 or device letter, all slashes should be forward
// 								 slashes '/'.
// 002Eh+              "XLN" char   Extra field
// +"LEN"
// 002Eh               "CMT" char   File comment
// +"LEN"
// +"XLN"


// OFFSET              Count TYPE   Description
// 0000h                   4 char   ID='PK',05,06
// 0004h                   1 word   Number of this disk
// 0006h                   1 word   Number of disk with start of central directory
// 0008h                   1 word   Total number of file/path entries on this disk
// 000Ah                   1 word   Total number of entries in central dir
// 000Ch                   1 dword  Size of central directory
// 0010h                   1 dword  Offset of start of central directory relative
// 								 to starting disk number
// 0014h                   1 word   Archive comment length
// 								 ="CML"
// 0016h               "CML" char   Zip file comment
struct Open_XML_File_Header
{//�ļ�ͷ���0x04034b50
	WORD ExtractVersion;//��ѹ�ļ����� pkware �汾
	WORD GeneralBit;//ȫ�ַ�ʽλ���
	WORD CompressMethod;//ѹ����ʽ
	DWORD LastModify;//����޸��ļ�����ʱ��
	DWORD CRC32Check;//CRC-32У��
	DWORD CompressedSize;//ѹ����ߴ�
	DWORD UncompressedSize;//δѹ���ߴ�
	WORD FilenameLen;//�ļ�������
	WORD ExtraFieldLen;//��չ��¼����
	char FileName[0];//�ļ���
//	char ExtraField[0];//��չ�ֶ�
};

struct Open_XML_Dir_Header
{//Ŀ¼���ļ��ļ�ͷ���0x02014b50
	BYTE ExtractVersion;//ѹ��ʹ�õ�pkware �汾
	BYTE HostOS;//����ϵͳ
	BYTE UnExtractVersion;//��ѹ�ļ����� pkware �汾
	BYTE TargetOS;//Ŀ��ϵͳ
	WORD GeneralBit;////ȫ�ַ�ʽλ���
	WORD CompressMethod;//ѹ����ʽ
	DWORD LastModify;//����޸��ļ�����ʱ��
	DWORD CRC32Check;//CRC-32У��
	DWORD CompressedSize;//ѹ����ߴ�
	DWORD UnCompressedSize;//δѹ���ߴ�
	WORD FilenameLen;//�ļ�������
	WORD ExtraFieldLen;//��չ��¼����
	WORD CommentLen;//�ļ�ע�ͳ���
	WORD DiskNumber;//���̿�ʼ��
	WORD InternalAttrib;//�ڲ��ļ�����
	DWORD ExternalAttrib;//�ⲿ�ļ�����
	DWORD RelativeOffset;//�ֲ�ͷ��ƫ����
	char FileName[0];//�ļ���
//	char ExtraField[0];//��չ�ֶ�
//	char Comment[0];//�ļ�ע��
};

struct Open_XML_Dir_End
{//Ŀ¼�������0x06054b50
	WORD CurDiskNumber;//��ǰ���̱��
	WORD DirBeginDiskNumber;//Ŀ¼����ʼ���̱��
	WORD TotalRecOnDisk;//�������ϼ�¼����
	WORD TotalRecInDir;//Ŀ¼���м�¼����
	DWORD DirFieldSize;//Ŀ¼���ߴ��С
	DWORD DirOffsetToFirstDisk;//Ŀ¼���Ե�һ�Ŵ��̵�ƫ����
	WORD CommentLen;//ZIP �ļ�ע�ͳ���
	char Comment[0];//ZIP �ļ�ע��
};

struct PKZip_Open_XML
{
	DWORD Sig;
	union
	{
		Open_XML_File_Header PKZipFileHeader;
		Open_XML_Dir_Header PKZipCentralDirectoryHeader;
		Open_XML_Dir_End PKZipEndCentralDirectory;
	}Header;
};



#pragma pack(pop)

// һ�� ZIP �ļ�����ͨ��ʽ
// ����----------------------
// ���� һ�� ZIP �ļ�������������ɣ�
// ���� 
// ���� ѹ��Դ�ļ�������+ѹ��Դ�ļ�Ŀ¼��+ѹ��Դ�ļ�Ŀ¼������־
// ����
// ���� 1��ѹ��Դ�ļ�������
// ���� �������������ÿһ��ѹ����Դ�ļ�/Ŀ¼����һ����¼����¼�ĸ�ʽ���£�
// ���� 
// ���� [�ļ�ͷ+ �ļ����� + ����������]
// ���� a���ļ�ͷ�ṹ
// ���� ��� ����
// ���� �ļ�ͷ��� 4 bytes (0x04034b50)
// ���� ��ѹ�ļ����� pkware �汾 2 bytes
// ���� ȫ�ַ�ʽλ��� 2 bytes
// ���� ѹ����ʽ 2 bytes
// ���� ����޸��ļ�ʱ�� 2 bytes
// ���� ����޸��ļ����� 2 bytes
// ���� CRC-32У�� 4 bytes
// ���� ѹ����ߴ� 4 bytes
// ���� δѹ���ߴ� 4 bytes
// ���� �ļ������� 2 bytes
// ���� ��չ��¼���� 2 bytes
// ���� �ļ��� ���������ȣ�
// ���� ��չ�ֶ� ���������ȣ�
// ���� 
// ���� b���ļ�����
// ���� 
// ���� c������������
// ������� ����
// ���� CRC-32У�� 4 bytes
// ���� ѹ����ߴ� 4 bytes
// ���� δѹ���ߴ� 4 bytes
// ���� �������������ֻ��ȫ�ַ�ʽλ��ǵĵڣ�λ��Ϊ��ʱ�Ŵ��ڣ�������⣩��������ѹ�����ݵ����һ���ֽں��������������ֻ���ڲ��ܶ������ ZIP �ļ����м���ʱʹ�á����磺��һ�����ܼ��������������磺�Ŵ����ϣ��ϵ� ZIP �ļ��С�����Ǵ����ϵ�ZIP�ļ�һ��û�����������������
// ����
// ���� 2��ѹ��Դ�ļ�Ŀ¼��
// ���� �������������ÿһ����¼��Ӧ��ѹ��Դ�ļ��������е�һ������
// ���� ��� ����
// ���� Ŀ¼���ļ��ļ�ͷ��� 4 bytes (0x02014b50)
// ���� ѹ��ʹ�õ�pkware �汾 2 bytes
// ���� ��ѹ�ļ����� pkware �汾 2 bytes
// ���� ȫ�ַ�ʽλ��� 2 bytes
// ���� ѹ����ʽ 2 bytes
// ���� ����޸��ļ�ʱ�� 2 bytes
// ���� ����޸��ļ����� 2 bytes
// ���� �ãңã�����У�� 4 bytes
// ���� ѹ����ߴ� 4 bytes
// ���� δѹ���ߴ� 4 bytes
// ���� �ļ������� 2 bytes
// ���� ��չ�ֶγ��� 2 bytes
// ���� �ļ�ע�ͳ��� 2 bytes
// ���� ���̿�ʼ�� 2 bytes
// ���� �ڲ��ļ����� 2 bytes
// ���� �ⲿ�ļ����� 4 bytes
// ���� �ֲ�ͷ��ƫ���� 4 bytes
// ���� �ļ��� ���������ȣ�
// ���� ��չ�ֶ� ���������ȣ�
// ���� �ļ�ע�� ���������ȣ�
// ����
// ���� 3��ѹ��Դ�ļ�Ŀ¼������־
// ���� ��� ����
// ���� Ŀ¼������� 4 bytes (0x02014b50)
// ���� ��ǰ���̱�� 2 bytes
// ���� Ŀ¼����ʼ���̱�� 2 bytes
// ���� �������ϼ�¼���� 2 bytes
// ���� Ŀ¼���м�¼���� 2 bytes
// ���� Ŀ¼���ߴ��С 4 bytes
// ���� Ŀ¼���Ե�һ�Ŵ��̵�ƫ���� 4 bytes
// ���� ZIP �ļ�ע�ͳ��� 2 bytes
// ���� ZIP �ļ�ע�� ���������ȣ�
