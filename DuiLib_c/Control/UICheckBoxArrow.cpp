#include "stdafx.h"


namespace DuiLib
{
	CCheckBoxArrowUI::CCheckBoxArrowUI()
	{
	}

	void CCheckBoxArrowUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_SETCURSOR ) {
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
			return;
		}

		__super::DoEvent(event);
	}
}