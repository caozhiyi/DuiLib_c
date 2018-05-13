#ifndef Calendar_h__
#define Calendar_h__

#include <map>

#pragma once

namespace DuiLib
{
	typedef struct tag_CalendarStyle
	{
		CDuiString	nCalenderBorderColor;		//日历边框颜色
		CDuiString	nCalendarBkColor;			//日历背景颜色
		CDuiString	nMainTitleBkColor;			//日历主标题背景颜色
		CDuiString	nSubTitleBkColor;			//日历副标题背景颜色
		CDuiString	nWeekendColorA;				//周末的日期隔行背景颜色
		CDuiString	nWeekendColorB;				//周末的日期隔行背景颜色
		CDuiString	nDayHotColor;				//日期获得焦点时背景颜色
		CDuiString	nDayPushedColor;			//日期被按下时背景颜色
		CDuiString	nDaySelectColor;			//日期被选中时背景颜色
		CDuiString	nDayDisabledColor;			//日期被禁用时的背景色
		CDuiString	nNoCurMonthDayColor;		//非本月日期的背景颜色
		CDuiString	nWeekIntervalColorA;		//周隔行颜色A
		CDuiString	nWeekIntervalColorB;		//周隔行颜色B
		CDuiString	nStatusBkColor;				//底部信息栏背景色
	}TCalendarStyle;
	
	typedef struct tag_SubTitleString
	{
		CDuiString	nSundayStr;
		CDuiString	nMondayStr;
		CDuiString	nTuesdayStr;
		CDuiString	nWednesdayStr;
		CDuiString	nThursdayStr;
		CDuiString	nFridayStr;
		CDuiString	nSaturdayStr;
		CDuiString	nToDayString;
	}TSubTitleString;

	typedef struct tag_CalendarInfo
	{
		int		nYear;
		int		nMooth;
		int		nDay;
		int		nAsMooth;
		int		nWeek;
		int		nWeekLine;
	}TCalendarInfo;

	class ICalendar
	{
	public:
		virtual CControlUI* CreateWeekPanel(CControlUI* _Panent,int _Week) {return NULL;};
		virtual CControlUI* CreateDayPanel(int _Week,LPCTSTR _GroupName) {return NULL;};
		virtual CControlUI* CreateInfoPanel(){return NULL;};
		virtual void InitWeekPanel(CControlUI* _Control,int _Week) {};
		virtual void InitDayPanel( CControlUI* _Control,bool bWeekEnd,TCalendarInfo& _CalendarInfo ){};
		virtual void InitDayPanel( CControlUI* _Control,bool bWeekEnd,int _Year,int _Mooth,int _Day,int _Week,int _WeekLine,int _AsMooth ){};
		virtual void OnCalendarUpdateStart(int _Year,int _Mooth) {};
		virtual void OnCalendarUpdateEnd(int _Year,int _Mooth) {};
		virtual void OnCalendarChange(int _Year,int _Mooth,int _Day){};
	};
	
	class CCalendarDlg;

	class CCalendarUI : public CVerticalLayoutUI,public ICalendar
	{
		friend class CCalendarDlg;
	public:
		CCalendarUI(void);
		~CCalendarUI(void);

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		void	DoEvent(TEventUI& event);
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		CControlUI* CreateWeekPanel(CControlUI* _Panent,int _Week);
		CControlUI* CreateDayPanel(int _Week,LPCTSTR _GroupName);
		CControlUI* CreateInfoPanel();
		void InitWeekPanel(CControlUI* _Control,int _Week);
		void InitDayPanel( CControlUI* _Control,bool bWeekEnd,TCalendarInfo& _CalendarInfo );
		void InitDayPanel( CControlUI* _Control,bool bWeekEnd,int _Year,int _Mooth,int _Day,int _Week,int _WeekLine,int _AsMooth );
		void OnCalendarUpdateStart(int _Year,int _Mooth);
		void OnCalendarChange(int _Year,int _Mooth,int _Day);
	public:
		bool IsLeapYear(int _Year);
		int  GetNumLeapYear(int _Year);
		int  DaysOfMonth(int _Mooth,int _Year = -1);
		void InitCalendarDis(int _Year,int _Month);
		void CalDateTime(TCalendarInfo* _TCalendarInfo);
		void CalDateTime(int _Year,int _Mooth,int _Day);
		CDuiString	GetCurSelDateTime();
		void		SetComboTargetName(LPCTSTR pstrName);
		CDuiString	GetComTargetName();
		CControlUI*	GetComTargetObj();
	public:
		void SetCallback(ICalendar* _ICalendar);

	public:
		bool SetSubTitleString(LPCTSTR _Name);
		void SetSubTitleString(TSubTitleString& _SubTitleString);
		void SetCalendarStyle(TCalendarStyle& _TCalendarStyle);
		bool AddSubTitleString(LPCTSTR _Name,TSubTitleString& _SubTitleString);
		bool AddSubTitleString(LPCTSTR _Name,LPCTSTR _Sunday,LPCTSTR _Monday,LPCTSTR _Tuesday,LPCTSTR _Wednesday,LPCTSTR _Thursday,LPCTSTR _Friday,LPCTSTR _Saturday);
		TSubTitleString& GetSubTitleString(LPCTSTR _Name = NULL);
		bool RemoveAtSubTitleString(LPCTSTR _Name);
		void RemoveAllSubTitleString();

	public:
		bool OnLastYear(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
		bool OnMoothSelect(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
		bool OnNextYear(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
		bool OnSelectMooth(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
		bool OnSelcetDay(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);
		bool OnToday(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);

	public:
		void SetEnabledMoothSel(bool _Enabled = true);
		bool GetEnabledMoothSel();
		void SetEnabledYearSel(bool _Enabled = true);
		bool GetEnabledYearSel();
		void SetMainTitleHeight(int _Height);
		int GetMainTitleHeight();
		void SetSubTitleHeight(int _Height);
		int GetSubTitleHeight();
		void SetStatusInfoHeight(int _Height);
		int GetStatusInfoHeight();

		void AnyCalendarStyle(LPCTSTR _StyleStr,TCalendarStyle* _TCalendarStyle = NULL);

		static void ShowCalendarDlg( CButtonUI* _pControl );
	private:
		CDuiString				m_sComboTargetName;
		CDuiString				mDateTime;
		CHorizontalLayoutUI*	pMainTitleHoriz;
		CButtonUI*				pLastYearBtn;
		CButtonUI*				pMoothSelectBtn;
		CButtonUI*				pNextYearBtn;

		CHorizontalLayoutUI*	pSubTitleHoriz;
		CButtonUI*				pSundayBtn;
		CButtonUI*				pMondayBtn;
		CButtonUI*				pTuesdayBtn;
		CButtonUI*				pWednesdayBtn;
		CButtonUI*				pThursdayBtn;
		CButtonUI*				pFridayBtn;
		CButtonUI*				pSaturdayBtn;

		CButtonUI*				pDateTimeBtn;
		CButtonUI*				pToDayBtn;

		CHorizontalLayoutUI*	pMoothPanelHorz;
		CVerticalLayoutUI*		pWeekPanelVert;
		CHorizontalLayoutUI*	pInfoPanelHorz;

		CDuiString				mLastYearBtnString;
		CDuiString				mMoothSelectBtnStrign;
		CDuiString				mNextYearBtnString;

		int						mToday;
		bool					pEnabledYearSel;
		bool					pEnabledMoothSel;
		struct tm*				pCurDateTime;

		ICalendar*				pICalendar;

		TCalendarStyle			m_DefaultStyle;
		TSubTitleString			mSubTitleString;
		CStdPtrArray			mTCalendarInfoArray;
		CStdStringPtrMap		mSubTitleStringArray;
	};
}

#endif // Calendar_h__
