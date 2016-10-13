#include "stdafx.h"
#include "UIButtonArrow.h"

namespace DuiLib
{
	CButtonArrowUI::CButtonArrowUI()
	{

	}

	void CButtonArrowUI::DoEvent(DuiLib::TEventUI& event)
	{
		if( event.Type == UIEVENT_SETCURSOR ) 
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			return;
		}
		__super::DoEvent(event);
	}

	UINT CButtonArrowUI::GetButtonState() 
	{
		return m_uButtonState;
	}

	void CButtonArrowUI::SetButtonState(UINT state) 
	{
		m_uButtonState = state;
		Invalidate();
	}

		
}