#ifndef __UIBUTTONARROW_H__
#define __UIBUTTONARROW_H__

#pragma once

namespace DuiLib
{
	//#define  DUI_CTR_BUTTON_ARROW                    (_T("ButtonArrow"))
	class UILIB_API CButtonArrowUI : public CButtonUI
	{
	public:
		CButtonArrowUI();

		virtual void DoEvent(DuiLib::TEventUI& event);
		UINT GetButtonState();
		void SetButtonState(UINT state);
		
	};

}	// namespace DuiLib

#endif


