#pragma once

namespace DuiLib
{	
	class UILIB_API CCheckBoxArrowUI : public CCheckBoxUI
	{
	public:
		CCheckBoxArrowUI();
		virtual void DoEvent(TEventUI& event);
	};

} // namespace DuiLib