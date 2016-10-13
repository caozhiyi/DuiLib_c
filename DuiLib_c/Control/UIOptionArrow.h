
#pragma once

namespace DuiLib
{	
	//#define  DUI_CTR_OPTION_ARROW                    (_T("OptionArrow"))
	class UILIB_API COptionArrowUI : public COptionUI
	{
	public:
		COptionArrowUI();
		virtual void DoEvent(TEventUI& event);
		UINT GetButtonState();		
		void SetButtonState(UINT state);		
	};
	
} // namespace DuiLib

