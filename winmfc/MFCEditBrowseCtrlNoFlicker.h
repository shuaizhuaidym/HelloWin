#pragma once
#include "afxeditbrowsectrl.h"
class CMFCEditBrowseCtrlNoFlicker : public CMFCEditBrowseCtrl
{
public:
	CMFCEditBrowseCtrlNoFlicker(void);
	~CMFCEditBrowseCtrlNoFlicker(void);
public:
	void OnDrawBrowseButton(CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bIsButtonHot) override;
};

