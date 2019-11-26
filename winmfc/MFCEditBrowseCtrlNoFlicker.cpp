#include "StdAfx.h"
#include "MFCEditBrowseCtrlNoFlicker.h"


CMFCEditBrowseCtrlNoFlicker::CMFCEditBrowseCtrlNoFlicker(void)
{
}


CMFCEditBrowseCtrlNoFlicker::~CMFCEditBrowseCtrlNoFlicker(void)
{
}

void CMFCEditBrowseCtrlNoFlicker::OnDrawBrowseButton(CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bIsButtonHot)
{
	CMemDC dc(*pDC, rect);
	__super::OnDrawBrowseButton(&dc.GetDC(), rect, bIsButtonPressed, bIsButtonHot);
}
