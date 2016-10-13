#include "stdafx.h"


namespace DuiLib
{
	COptionArrowUI::COptionArrowUI()
	{
	}

	void COptionArrowUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_SETCURSOR ) {
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			return;
		}

		__super::DoEvent(event);
	}

	UINT COptionArrowUI::GetButtonState() 
	{
		return m_uButtonState;
	}

	void COptionArrowUI::SetButtonState(UINT state) 
	{
		m_uButtonState = state;
		Invalidate();
	}
}