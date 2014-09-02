// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "datarecovery.h"
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

CListCtrlEx::CListCtrlEx()
{
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
		ON_NOTIFY_REFLECT(NM_CUSTOMDRAW,OnCustomDraw)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers

void CListCtrlEx::OnCustomDraw(NMHDR* pNMHDR,LRESULT* pResult)
{
	NMLVCUSTOMDRAW* lplvcd = (NMLVCUSTOMDRAW*)(pNMHDR);
    int nRow = (int)lplvcd->nmcd.dwItemSpec;
	switch (lplvcd->nmcd.dwDrawStage)
    {
		case CDDS_PREPAINT:
			*pResult |= CDRF_NOTIFYITEMDRAW;
			break;
		case CDDS_ITEMPREPAINT: 
			if(nRow & 1)
			{   // ����
				lplvcd->clrTextBk = RGB(0xFF,0xFF,0xFF);
				lplvcd->clrText   = RGB(0x00,0x00,0x00);
				*pResult          = CDRF_NEWFONT; 
			}
			else
			{     // ˫��
				lplvcd->clrTextBk = RGB(0xBB,0xCC,0xFF);
				lplvcd->clrText   = RGB(0x44,0x33,0x00);
				*pResult          = CDRF_NEWFONT; 
			}
			if(GetCheck(nRow) && (GetExtendedStyle() & LVS_EX_CHECKBOXES))
			{ 
				lplvcd->clrTextBk = RGB(0x00,0x00,0x80);
				lplvcd->clrText   = RGB(0xFF,0xFF,0x00);
				//--------------------------------------------
				// ��������
				HFONT hFont = (HFONT)this->SendMessage(WM_GETFONT);
				LOGFONT lf;   GetObject(hFont, sizeof(lf), &lf);
				lf.lfStrikeOut = TRUE; // ɾ����
				lf.lfItalic    = TRUE; // б��
				hFont = CreateFontIndirect(&lf);
				SelectObject(lplvcd->nmcd.hdc, hFont);
				DeleteObject(hFont);
				
				*pResult          = CDRF_NEWFONT; 
			}
			if( GetItemState(nRow, CDIS_SELECTED) )
			{ // Ԥ��BUG�����ж�������ժ������, CDIS_SELECTEDǰ��ȡ���ţ��Ұ����Ϊ��
				// ʹ�ô˴���Σ�������Juncox��163.com˵��
				lplvcd->nmcd.uItemState ^= CDIS_SELECTED;// Ԥ��BUG
				lplvcd->clrTextBk =RGB(255, 0, 0); 
				lplvcd->clrText   = RGB(0, 0, 255);    
				*pResult = CDRF_NEWFONT; 
			}
			break;
			
		default:
			*pResult=CDRF_DODEFAULT;
			break;
	}
}
