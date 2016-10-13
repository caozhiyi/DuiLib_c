#include "stdafx.h"
#include "UIButtonHtml.h"

namespace DuiLib
{
	CButtonHtmlUI::CButtonHtmlUI()
	{
		SetShowHtml(true);
	}

	void CButtonHtmlUI::DoEvent(DuiLib::TEventUI& event)
	{
		//鼠标进入时显示下划线
		if( event.Type == UIEVENT_MOUSEENTER ) 
		{
			CDuiString str = GetText();
			CDuiString strTemp ;
			
			strTemp.Format(_T("{u}%s{/u}"), str);
			SetText(strTemp);
		}

		//鼠标隐藏时去掉下划线
		if( event.Type == UIEVENT_MOUSELEAVE ) 
		{
			CDuiString str = GetText();
			str.Replace(_T("{u}"), _T(""));
			str.Replace(_T("{/u}"), _T(""));
			DWORD textColor = GetTextColor();
			SetText(str);
		}
		
		__super::DoEvent(event);
	}


}