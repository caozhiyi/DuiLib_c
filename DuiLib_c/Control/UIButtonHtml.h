#ifndef __UIBUTTONHTML_H__
#define __UIBUTTONHTML_H__

#pragma once

namespace DuiLib
{
	//#define  DUI_CTR_BUTTON_HTML                   (_T("ButtonHtml"))	  
	class UILIB_API CButtonHtmlUI : public CButtonUI
	{
	public:
		CButtonHtmlUI();

		virtual void DoEvent(DuiLib::TEventUI& event);

	};

}	// namespace DuiLib

#endif


