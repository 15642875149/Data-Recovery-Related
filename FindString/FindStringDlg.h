// FindStringDlg.h : header file
//

#if !defined(AFX_FINDSTRINGDLG_H__5BB4ADD9_A049_41D4_B1B2_BD5851683261__INCLUDED_)
#define AFX_FINDSTRINGDLG_H__5BB4ADD9_A049_41D4_B1B2_BD5851683261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SIZE 512
#define MAX_CHAR_SIZE	256
/////////////////////////////////////////////////////////////////////////////
// CFindStringDlg dialog

class CFindStringDlg : public CDialog
{
// Construction
public:
	void Sunday(size_t patt_size,size_t text_size,char* text,char* patt,LARGE_INTEGER cum);
	CFindStringDlg(CWnd* pParent = NULL);	// standard constructor
	HANDLE m_hDevice;
	LARGE_INTEGER m_TotalSec;
	DWORD m_BytePerSec;
	BOOL m_StopScan;
// Dialog Data
	//{{AFX_DATA(CFindStringDlg)
	enum { IDD = IDD_FINDSTRING_DIALOG };
	CListCtrl	m_result;
	CListCtrl	m_keyword;
	CListCtrl	m_drive;
	CString	m_data;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindStringDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	HANDLE m_Find;
	static DWORD WINAPI FindSomething(LPVOID lpVoid);
	// Generated message map functions
	//{{AFX_MSG(CFindStringDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnScan();
	afx_msg void OnStop();
	afx_msg void OnEndlabeleditKeyword(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

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

#pragma pack(pop, curAlignment)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDSTRINGDLG_H__5BB4ADD9_A049_41D4_B1B2_BD5851683261__INCLUDED_)
