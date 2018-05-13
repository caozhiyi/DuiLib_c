#include "StdAfx.h"
#include "UICalendar.h"

#include <time.h>

namespace DuiLib {
/************************************************************************/
/* 日历窗口                                                             */
/************************************************************************/
#ifndef UICALENDAR_DLG_H
#define UICALENDAR_DLG_H

class CCalendarUI;
class CCalendarDlg : public CWindowWnd,public INotifyUI,public IMessageFilterUI
{
public:
	CCalendarDlg(CButtonUI* _pControl);
	~CCalendarDlg();
public:
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName() const;
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT MessageHandler( UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled );
	LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Init();
	
	void Init(CButtonUI *pOwner);
	void Notify(TNotifyUI& msg);
	void OnFinalMessage(HWND /*hWnd*/);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	CCalendarUI*	pCalendar;
	CButtonUI*		pCControl;
	CDuiString		ZipSkinPath;
	CPaintManagerUI pm;
};

CCalendarDlg::CCalendarDlg(CButtonUI* _pControl)
{
	ASSERT(_pControl);
	pCalendar	= NULL;
	pCControl	= _pControl;
 

	CDuiRect rc;     
	CDuiRect rcOwner = pCControl->GetPos();
	rc = rcOwner;

	rc.left  += 0;
	rc.top   += rcOwner.GetHeight(); // // CVerticalLayoutUI 默认的Inset 调整
	rc.right  = rc.left +200;
	rc.bottom = rc.top + 168;	// 计算弹出窗口高度
 
	::MapWindowRect(pCControl->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);

    MONITORINFO oMonitor = {};
    oMonitor.cbSize = sizeof(oMonitor);
    ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
    CDuiRect rcWork = oMonitor.rcWork;
	if( ( rc.bottom > rcWork.bottom ) && ( rc.right > rcWork.right )) //如果超过边界，则往左上弹出
	{
		 rc = rcOwner;

		 rc.bottom = rcOwner.top;
		 rc.top    = rc.bottom-168;
         rc.left  -= rcOwner.GetWidth();
		 rc.right  = rc.left +200;
	     ::MapWindowRect(pCControl->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);
	}
	else if( rc.bottom > rcWork.bottom ) //如果超过边界，则往上弹出
	{
		rc = rcOwner;

		rc.bottom = rcOwner.top;
		rc.top    = rc.bottom-168;
		rc.left  += 0; 
		rc.right  = rc.left +200;
	   ::MapWindowRect(pCControl->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);
    }
	else if( rc.right > rcWork.right ) //如果超过边界，则往左弹出
	{
        rc = rcOwner;
 
		rc.top   += rcOwner.GetHeight(); // // CVerticalLayoutUI 默认的Inset 调整
		rc.bottom = rc.top + 168;
        rc.left  -= rcOwner.GetWidth();
		rc.right  = rc.left +200;

	   ::MapWindowRect(pCControl->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);
    }

	
	CWindowWnd::Create(pCControl->GetManager()->GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW, rc);
	::SetWindowPos(m_hWnd, HWND_TOP, rc.left, rc.top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	::ShowWindow(m_hWnd, SW_SHOW);
}

CCalendarDlg::~CCalendarDlg()
{

}
 
LPCTSTR CCalendarDlg::GetWindowClassName() const
{ 
	return _T("CCalendarDlg");
}

LRESULT CCalendarDlg::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{ 
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left,rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	pm.Init(m_hWnd);
	pm.AddPreMessageFilter(this);

	CDialogBuilder builder;

	if(!GetSkinFile().IsEmpty())
		pm.SetResourcePath(GetSkinFile());

	CDuiString nStrRoot =_T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<Window size=\"222,170\" ><Calendar name=\"CalendarDlg\" bordersize=\"1\" bordercolor=\"#ffbac0c5\" inset=\"1,1,1,1\" bkcolor=\"#FFFFFFFF\" /></Window>");

	CControlUI* pRoot = builder.Create(nStrRoot.GetData(), (UINT)0, NULL, &pm);
	pm.AttachDialog(pRoot);
	pm.AddNotifier(this);

	Init();
	return 0;
}

void CCalendarDlg::Init(CButtonUI *pOwner)
{

}

LRESULT CCalendarDlg::MessageHandler( UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled )
{
	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			return ResponseDefaultKeyEvent(wParam);
		default:
			break;
		}
	}
	return FALSE;
}

LRESULT CCalendarDlg::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		Close();
		return TRUE;
	}

	return FALSE;
}

LRESULT CCalendarDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
		case WM_CREATE:         lRes = OnCreate(uMsg, wParam, lParam, bHandled); break; 
		case WM_NCHITTEST:      lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:           lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:     lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_KILLFOCUS:
			{
				//ShowWindow(SW_HIDE);
				PostMessage(WM_CLOSE);
				//return S_OK;
			}
		default:                
			bHandled = FALSE; break;
	}

	if (bHandled) return lRes;

	if (pm.MessageHandler(uMsg, wParam, lParam, lRes))
		return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CCalendarDlg::OnFinalMessage(HWND /*hWnd*/)
{
	pm.RemovePreMessageFilter(this);
	pm.RemoveNotifier(this);
//	pm.ReapObjects(pm.GetRoot());
}

LRESULT CCalendarDlg::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if( !::IsZoomed(*this) ) {
		RECT rcSizeBox = pm.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	RECT rcCaption = pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(pm.FindControl(pt));
			if( pControl && _tcsicmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
				_tcsicmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcsicmp(pControl->GetClass(), _T("TextUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CCalendarDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = pm.GetRoundCorner();

	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CCalendarDlg::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE)
	{
		bHandled = TRUE;
		return 1;
	}

	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

void CCalendarDlg::Init()
{
	pCalendar	= static_cast<CCalendarUI*>(pm.FindControl(_T("CalendarDlg")));

	if(pCalendar)
	{
		if(pCControl->GetCalendarStyle())
			pCalendar->AnyCalendarStyle(pCControl->GetCalendarStyle());
		if(pCControl->GetCalendarProfile())
			pCalendar->ApplyAttributeList(pCControl->GetCalendarProfile());
	}
}

void CCalendarDlg::Notify( TNotifyUI& msg )
{
	if(msg.sType == _T("OnSelcetDay") || msg.sType == _T("OnToday"))
	{
		if(!pCalendar)
			return;
		if(pCControl)
		{
			CControlUI* pCalendarDestObj = pCControl->GetManager()->FindControl(pCControl->GetCalendarValDest());
			if(pCalendarDestObj){
				pCalendarDestObj->SetText(pCalendar->GetCurSelDateTime());
			}
			pCControl->SetUserData(pCalendar->GetCurSelDateTime());
			pCControl->SetText(pCalendar->GetCurSelDateTime());
			pCControl->GetManager()->SendNotify(pCControl,_T("OnSelcetDay"));
		}
		Close(IDOK);
	}
}

CDuiString CCalendarDlg::GetSkinFile()
{
	if(pCControl)
		return pCControl->GetManager()->GetResourcePath();
	return _T(".\\");
}

#endif // UICALENDAR_DLG_H

CCalendarUI::CCalendarUI(void)
{
	//初始化当前日期
	time_t now;	
	time(&now);
	pCurDateTime = localtime(&now);
	pCurDateTime->tm_year	= pCurDateTime->tm_year-100+2000;
	pCurDateTime->tm_mon	= pCurDateTime->tm_mon+1;
	mToday					= pCurDateTime->tm_mday;

	CalDateTime(pCurDateTime->tm_year,pCurDateTime->tm_mon,mToday);

	pDateTimeBtn		= NULL;
	pToDayBtn			= NULL;
	pICalendar			= this;
	pEnabledYearSel		= true;
	pEnabledMoothSel	= true;

	//初始化缺省样式
	m_DefaultStyle.nCalenderBorderColor = _T("#FFA1ABBD");
	m_DefaultStyle.nMainTitleBkColor	= _T("#FF596980");
	m_DefaultStyle.nSubTitleBkColor		= _T("#FFFFFFFF");
	m_DefaultStyle.nWeekIntervalColorA	= _T("#FFFFFFFF");
	m_DefaultStyle.nWeekIntervalColorB	= _T("#FFFFFFFF");
	m_DefaultStyle.nWeekendColorA		= _T("#FFFFFFFF");
	m_DefaultStyle.nWeekendColorB		= _T("#FFFFFFFF"); 
	m_DefaultStyle.nDayHotColor			= _T("#FF7EB0BC");
	m_DefaultStyle.nDaySelectColor		= _T("#FF3F91CB");	// _T("#FF67AF3E");
	m_DefaultStyle.nNoCurMonthDayColor	= _T("#FFD0D0D0");
	m_DefaultStyle.nStatusBkColor		= _T("#FFEBEDF1");

	 
	mSubTitleString.nSundayStr			= _T("{c #FF0000}日{/c}");
	mSubTitleString.nMondayStr			= _T("{c #0054E3}一{/c}");
	mSubTitleString.nTuesdayStr			= _T("{c #0054E3}二{/c}");
	mSubTitleString.nWednesdayStr		= _T("{c #0054E3}三{/c}");
	mSubTitleString.nThursdayStr		= _T("{c #0054E3}四{/c}");
	mSubTitleString.nFridayStr			= _T("{c #0054E3}五{/c}");
	mSubTitleString.nSaturdayStr		= _T("{c #FF0000}六{/c}");
	mSubTitleString.nToDayString		= _T("{b}今天{/b}");
 

	AddSubTitleString(_T("Default"),mSubTitleString);

	//初始化日历主标题容器与元素
	pMainTitleHoriz	= new CHorizontalLayoutUI();
	pLastYearBtn	= new CButtonUI();
	pMoothSelectBtn	= new CButtonUI();
	pNextYearBtn	= new CButtonUI();

	pLastYearBtn->SetShowHtml(true); 
	pMoothSelectBtn->SetShowHtml(true); 
	pNextYearBtn->SetShowHtml(true); 

	pLastYearBtn->SetFixedWidth(60);
	pNextYearBtn->SetFixedWidth(60);
	pMainTitleHoriz->SetFixedHeight(22);
	pMoothSelectBtn->SetFixedWidth(80);

 	pLastYearBtn->OnNotifyUI	+= MakeDelegateUI(this,&CCalendarUI::OnLastYear,_T("click"));
 	pMoothSelectBtn->OnNotifyUI	+= MakeDelegateUI(this,&CCalendarUI::OnMoothSelect,_T("click"));
 	pNextYearBtn->OnNotifyUI	+= MakeDelegateUI(this,&CCalendarUI::OnNextYear,_T("click"));

	pMainTitleHoriz->Add(pLastYearBtn);
	pMainTitleHoriz->Add(new CControlUI());
	pMainTitleHoriz->Add(pMoothSelectBtn);
	pMainTitleHoriz->Add(new CControlUI());
	pMainTitleHoriz->Add(pNextYearBtn);

	//初始化日历副标题容器与元素
	pSubTitleHoriz	= new CHorizontalLayoutUI();

	for(int nWeekIndex = 0;nWeekIndex < 7;nWeekIndex++)
	{
		CControlUI* pWeek = pICalendar->CreateWeekPanel(pSubTitleHoriz,nWeekIndex);
		if(!pWeek)
			pWeek = this->CreateWeekPanel(pSubTitleHoriz,nWeekIndex);
		InitWeekPanel(pWeek,nWeekIndex);

		if(pWeek) 
			pSubTitleHoriz->Add(pWeek);
	}

	//初始化月份选择容器与元素
	CVerticalLayoutUI* pMoothSubPanelA = new CVerticalLayoutUI();
	CVerticalLayoutUI* pMoothSubPanelB = new CVerticalLayoutUI();
	pMoothPanelHorz	= new CHorizontalLayoutUI();
	pMoothPanelHorz->SetFloat(true);
	pMoothPanelHorz->Add(pMoothSubPanelA);
	pMoothPanelHorz->Add(pMoothSubPanelB);
	pMoothPanelHorz->SetFixedHeight(120);
	pMoothPanelHorz->SetMaxWidth(80);

	CDuiString TmpDef;
	for(int nMoothItem = 1;nMoothItem < 13;nMoothItem++)
	{
		TmpDef.SmallFormat(_T("CalendMoothGroupName_%d"),pMoothPanelHorz);

		COptionUI* pMooth = new COptionUI();
		pMooth->SetGroup(TmpDef.GetData());

		TmpDef.SmallFormat(_T("%d"),nMoothItem);
		pMooth->SetText(TmpDef.GetData());
		pMooth->SetTag(nMoothItem);
		pMooth->OnNotifyUI += MakeDelegateUI(this,&CCalendarUI::OnSelectMooth,_T("click"));

		nMoothItem % 2 ?pMoothSubPanelA->Add(pMooth):pMoothSubPanelB->Add(pMooth);
	}

	//初始化日期容器与元素
	pWeekPanelVert	= new CVerticalLayoutUI();

	TmpDef.SmallFormat(_T("CalendGroupName_%d"),this);

	for(int nWeekLine = 1;nWeekLine < 7;nWeekLine++)
	{
		CHorizontalLayoutUI* pWeekLine = new CHorizontalLayoutUI();
		for(int nDayIndex = 1;nDayIndex < 8;nDayIndex++)
		{
			CHorizontalLayoutUI* pDayParent = new CHorizontalLayoutUI();

			CControlUI* pDay = static_cast<CControlUI*>(pICalendar->CreateDayPanel(nDayIndex,TmpDef.GetData()));

			if(!pDay)
				pDay = this->CreateDayPanel(nDayIndex,TmpDef.GetData());

			pDayParent->Add(pDay);
			
			TCalendarInfo* pCalendarInfo	= new TCalendarInfo();
			pDayParent->SetTag((UINT_PTR)pCalendarInfo);
			mTCalendarInfoArray.Add(pCalendarInfo);

			pWeekLine->Add(pDayParent);
		}
		pWeekPanelVert->Add(pWeekLine);
	}

	//初始化信息栏容器与元素
	pInfoPanelHorz	= new CHorizontalLayoutUI();
	CControlUI* pInfoPanel = pICalendar->CreateInfoPanel();
	if(!pInfoPanel)
		pInfoPanel = CreateInfoPanel();
	pInfoPanelHorz->Add(pInfoPanel);
	pInfoPanelHorz->SetFixedHeight(22);
	pInfoPanelHorz->SetAttribute(_T("bkcolor"),m_DefaultStyle.nStatusBkColor);

	Add(pMainTitleHoriz);
	Add(pSubTitleHoriz);
	Add(pWeekPanelVert);
	Add(pMoothPanelHorz);
	Add(pInfoPanelHorz);

	//初始化默认样式
	SetCalendarStyle(m_DefaultStyle);

	//初始化日历数据
	InitCalendarDis(pCurDateTime->tm_year,pCurDateTime->tm_mon);

	OnCalendarChange(pCurDateTime->tm_year,pCurDateTime->tm_mon,pCurDateTime->tm_mday);
}


CCalendarUI::~CCalendarUI(void)
{
	RemoveAllSubTitleString();
}
 
LPCTSTR CCalendarUI::GetClass() const
{ 
	return _T("CalendarUI"); 
}

LPVOID CCalendarUI::GetInterface( LPCTSTR pstrName )
{ 
	if( _tcscmp(pstrName, _T("Calendar")) == 0 ) return static_cast<CCalendarUI*>(this);
	return CVerticalLayoutUI::GetInterface(pstrName);
}

void CCalendarUI::DoEvent( TEventUI& event )
{ 
	CVerticalLayoutUI::DoEvent(event);
}

CControlUI* CCalendarUI::CreateWeekPanel( CControlUI* _Panent,int _Week )
{
	CButtonUI* pWeek	= new CButtonUI();
	_Panent->SetFixedHeight(22);
	pWeek->SetMouseEnabled(false);
	return pWeek;
}

CControlUI* CCalendarUI::CreateDayPanel( int _Week,LPCTSTR _GroupName )
{ 
	COptionUI*	pDay = new COptionUI();
	int heighta = pDay->GetHeight();
	int hegithb = pDay->GetFixedHeight();
	pDay->SetGroup(_GroupName);
	pDay->SetName(_GroupName);
	pDay->SetAttribute(_T("algin"),_T("right"));
	pDay->OnNotifyUI += MakeDelegateUI(this,&CCalendarUI::OnSelcetDay,_T("click"));

	return pDay;
}

bool CCalendarUI::SetSubTitleString( LPCTSTR _Name )
{ 
	if(!_Name || !mSubTitleStringArray.Find(_Name))
		return false;

	TSubTitleString* pSubTitleString = static_cast<TSubTitleString*>(mSubTitleStringArray.Find(_Name));

	SetSubTitleString(*pSubTitleString);

	return true;
}

void CCalendarUI::SetSubTitleString( TSubTitleString& _SubTitleString )
{
	pSundayBtn->SetText(_SubTitleString.nSundayStr);
	pMondayBtn->SetText(_SubTitleString.nMondayStr);
	pTuesdayBtn->SetText(_SubTitleString.nTuesdayStr);
	pWednesdayBtn->SetText(_SubTitleString.nWednesdayStr);
	pThursdayBtn->SetText(_SubTitleString.nThursdayStr);
	pFridayBtn->SetText(_SubTitleString.nFridayStr);
	pSaturdayBtn->SetText(_SubTitleString.nSaturdayStr);
}

void CCalendarUI::SetCalendarStyle( TCalendarStyle& _TCalendarStyle )
{
	this->SetAttribute(_T("bordercolor"),_TCalendarStyle.nCalenderBorderColor);
	this->SetAttribute(_T("bkcolor"),_TCalendarStyle.nCalendarBkColor);
	pMainTitleHoriz->SetAttribute(_T("bkcolor"),_TCalendarStyle.nMainTitleBkColor);
	pLastYearBtn->SetAttribute(_T("hotbkcolor"),_TCalendarStyle.nDayHotColor);
	pMoothSelectBtn->SetAttribute(_T("hotbkcolor"),_TCalendarStyle.nDayHotColor);
	pNextYearBtn->SetAttribute(_T("hotbkcolor"),_TCalendarStyle.nDayHotColor);

	pSubTitleHoriz->SetAttribute(_T("bkcolor"),_TCalendarStyle.nSubTitleBkColor);
	
	pMoothPanelHorz->SetAttribute(_T("bkcolor"),_TCalendarStyle.nMainTitleBkColor);

	CVerticalLayoutUI* pMoothSubPanelA = static_cast<CVerticalLayoutUI*>(pMoothPanelHorz->GetItemAt(0));
	for(int nMoothItem = 0;nMoothItem < 6;nMoothItem++)
	{
		COptionUI* pMooth = static_cast<COptionUI*>(pMoothSubPanelA->GetItemAt(nMoothItem));
		pMooth->SetAttribute(_T("hotbkcolor"),_TCalendarStyle.nDayHotColor);
		pMooth->SetAttribute(_T("selectedbkcolor"),_TCalendarStyle.nDaySelectColor);
	}
	CVerticalLayoutUI* pMoothSubPanelB = static_cast<CVerticalLayoutUI*>(pMoothPanelHorz->GetItemAt(1));
	for(int nMoothItem = 0;nMoothItem < 6;nMoothItem++)
	{
		COptionUI* pMooth = static_cast<COptionUI*>(pMoothSubPanelB->GetItemAt(nMoothItem));
		pMooth->SetAttribute(_T("hotbkcolor"),_TCalendarStyle.nDayHotColor);
		pMooth->SetAttribute(_T("selectedbkcolor"),_TCalendarStyle.nDaySelectColor);
	}

	for(int nWeekIndex = 0;nWeekIndex < 6;nWeekIndex++)
	{
		CHorizontalLayoutUI* pWeekLine = static_cast<CHorizontalLayoutUI*>(pWeekPanelVert->GetItemAt(nWeekIndex));
		if(!pWeekLine)
			continue;

		pWeekLine->SetAttribute(_T("bkcolor"),!(nWeekIndex%2)?_TCalendarStyle.nWeekIntervalColorA:_TCalendarStyle.nWeekIntervalColorB);

		for(int nDayIndex = 0;nDayIndex < 7;nDayIndex++)
		{
			CVerticalLayoutUI* pDayParent = static_cast<CVerticalLayoutUI*>(pWeekLine->GetItemAt(nDayIndex));
			if(!pDayParent)
				continue;

			if(nWeekIndex%2 && (nDayIndex == 0 || nDayIndex == 6))
				pDayParent->SetAttribute(_T("bkcolor"),_TCalendarStyle.nWeekendColorA);
			else if(!(nWeekIndex%2) && (nDayIndex == 0 || nDayIndex == 6))
				pDayParent->SetAttribute(_T("bkcolor"),_TCalendarStyle.nWeekendColorB);

			COptionUI* pDay = static_cast<COptionUI*>(pDayParent->GetItemAt(0));

			if(!pDay || CDuiString(pDay->GetClass()) != CDuiString(_T("OptionUI")))
				continue;

			pDay->SetAttribute(_T("hotbkcolor"),_TCalendarStyle.nDayHotColor);
			pDay->SetAttribute(_T("selectedbkcolor"),_TCalendarStyle.nDaySelectColor);
		}
	}
}

bool CCalendarUI::AddSubTitleString( LPCTSTR _Name,TSubTitleString& _SubTitleString )
{
	if(!_Name && mSubTitleStringArray.Find(_Name) != NULL )
		return false;
	
	TSubTitleString* pSubTitleString = new TSubTitleString();
	pSubTitleString->nSundayStr		= _SubTitleString.nSundayStr;
	pSubTitleString->nMondayStr		= _SubTitleString.nMondayStr;
	pSubTitleString->nWednesdayStr	= _SubTitleString.nWednesdayStr;
	pSubTitleString->nThursdayStr	= _SubTitleString.nThursdayStr;
	pSubTitleString->nFridayStr		= _SubTitleString.nFridayStr;
	pSubTitleString->nSaturdayStr	= _SubTitleString.nSaturdayStr;

	mSubTitleStringArray.Set(_Name,pSubTitleString);

	return true;
}

bool CCalendarUI::AddSubTitleString( LPCTSTR _Name,LPCTSTR _Sunday,LPCTSTR _Monday,LPCTSTR _Tuesday,LPCTSTR _Wednesday,LPCTSTR _Thursday,LPCTSTR _Friday,LPCTSTR _Saturday )
{
	if(!_Name || !_Sunday || !_Monday || !_Tuesday || !_Wednesday || !_Thursday || !_Friday || !_Saturday)
		return false;

	TSubTitleString* pSubTitleString = new TSubTitleString();
	pSubTitleString->nSundayStr		= _Sunday;
	pSubTitleString->nMondayStr		= _Monday;
	pSubTitleString->nWednesdayStr	= _Wednesday;
	pSubTitleString->nThursdayStr	= _Thursday;
	pSubTitleString->nFridayStr		= _Friday;
	pSubTitleString->nSaturdayStr	= _Saturday;

	if(pSubTitleString != NULL)
	{
		if( mSubTitleStringArray.Find(_Name) == NULL )
			mSubTitleStringArray.Set(_Name,pSubTitleString);
		else
			delete pSubTitleString;
		pSubTitleString = NULL;
		return true;
	}
	return false;
}

TSubTitleString& CCalendarUI::GetSubTitleString( LPCTSTR _Name /*= NULL*/ )
{
	if(!_Name)
		return mSubTitleString;

	TSubTitleString* pSubTitleString = static_cast<TSubTitleString*>(mSubTitleStringArray.Find(_Name));

	return pSubTitleString?*pSubTitleString:mSubTitleString;
}

bool CCalendarUI::RemoveAtSubTitleString( LPCTSTR _Name )
{
	if(!_Name)
		return false;

	TSubTitleString* pSubTitleString = static_cast<TSubTitleString*>(mSubTitleStringArray.Find(_Name));

	if( !pSubTitleString )
		return false;

	delete pSubTitleString;
	return mSubTitleStringArray.Remove(_Name);
}

void CCalendarUI::RemoveAllSubTitleString()
{
	TSubTitleString* pSubTitleString = NULL;
	for( int i = 0; i< mSubTitleStringArray.GetSize(); i++ ) {
		if(LPCTSTR key = mSubTitleStringArray.GetAt(i)) {
			pSubTitleString = static_cast<TSubTitleString*>(mSubTitleStringArray.Find(key));
			delete pSubTitleString;
		}
	}
}

bool CCalendarUI::IsLeapYear( int _Year )
{
	return ((_Year % 4 == 0 && _Year % 100 != 0) || _Year % 400 == 0);
}

int CCalendarUI::GetNumLeapYear( int _Year )
{
	int count=0;
	for(int i=1;i<_Year;i++)
		if(IsLeapYear(i))
			count++;
	return count;
}

int CCalendarUI::DaysOfMonth( int _Mooth,int _Year /*= -1*/ )
{
	int nYear = _Year == -1?pCurDateTime->tm_year:_Year;

	switch (_Mooth)
	{
		case 0: return 0;
		case 1 :
		case 3 :
		case 5 :
		case 7 :
		case 8 :
		case 10 :
		case 12 : return 31 ;
		case 4 :
		case 6 :
		case 9 :
		case 11 : return 30 ;
		case 2 : return IsLeapYear(nYear)?29:28;
	}  
	return 0 ;
}

void CCalendarUI::InitCalendarDis( int _Year,int _Month )
{
	if(_Month > 12){
		_Month = 1;
		_Year++;
	}

	if(_Month < 1) {
		_Month = 12;
		_Year--;
	}

	pICalendar->OnCalendarUpdateStart(_Year,_Month);

	CDuiString TmpDef;

	TmpDef.SmallFormat(_T("{b}{c #FFFFFF}%.4d-%.2d{/c}{/b}"),_Year,_Month);
	pMoothSelectBtn->SetText(TmpDef.GetData());

	int nLastMaxDay		= _Month == 1 ?DaysOfMonth(12,_Year-1):DaysOfMonth(_Month-1,_Year);
	int nLastMinDay		= 0;
	int nAsMooth		= 0;
	int nDays			= DaysOfMonth(_Month,_Year);
	int nWeek			= 0;
	int nFDay			= 0;
	int nCalDay			= 0;
	for(int i=1;i<=13;i++)
	{
		nFDay += DaysOfMonth(i-1);

		int nCurFday = (nFDay+(_Year-1)*365+GetNumLeapYear(_Year))%7+1;
		if(i == _Month)
			nWeek = nCurFday;
	}

	nAsMooth	= nWeek == 0?0:_Month-1;
	nLastMinDay = nWeek == 0?0:nLastMaxDay-nWeek;
	for(int nWeekIndex = 0;nWeekIndex < 6;nWeekIndex++)
	{
		CHorizontalLayoutUI* pWeekLine = static_cast<CHorizontalLayoutUI*>(pWeekPanelVert->GetItemAt(nWeekIndex));
		
		for(int nDayIndex = 0;nDayIndex < 7;nDayIndex++)
		{
			CVerticalLayoutUI* pDayParent = static_cast<CVerticalLayoutUI*>(pWeekLine->GetItemAt(nDayIndex));
			if(!pDayParent)
				continue;

			TCalendarInfo* pTCalendarInfo = (TCalendarInfo*)pDayParent->GetTag();
			if(!pTCalendarInfo)
				continue;

			COptionUI *pDay = static_cast<COptionUI*>(pDayParent->GetItemAt(0));
			if(!pDay->IsShowHtml())
				pDay->SetShowHtml(true);

			if(!(nWeek && nDayIndex < nWeek && nCalDay <= nDays))
			{
				nCalDay++;
				nAsMooth = nCalDay <= nDays?_Month:_Month+1;
			}

			if((nLastMinDay <= nLastMaxDay && nAsMooth != _Month) || nCalDay >= nDays){
				nLastMinDay++;
				nAsMooth	= _Month+1;
			}

			if(nCalDay == nDays) {
				nLastMinDay = 0;
				nAsMooth	= _Month;
			}
			
			pTCalendarInfo->nYear		= _Year;
			pTCalendarInfo->nMooth		= _Month;
			pTCalendarInfo->nDay		= ((nWeek && nDayIndex < nWeek) || nCalDay > nDays)?nLastMinDay:nCalDay;
			pTCalendarInfo->nWeekLine	= nWeekIndex;
			pTCalendarInfo->nWeek		= nWeek;
			pTCalendarInfo->nAsMooth	= _Month;

			if(nCalDay < 1)
				pTCalendarInfo->nAsMooth= _Month -1;
			else if(nCalDay > nDays)
				pTCalendarInfo->nAsMooth= _Month +1;
			
			pICalendar->InitDayPanel(pDay,(nDayIndex == 0 || nDayIndex == 6),pTCalendarInfo->nYear,_Month,pTCalendarInfo->nDay,pTCalendarInfo->nWeek,pTCalendarInfo->nWeekLine,pTCalendarInfo->nAsMooth);

			pDay->Selected(nCalDay == mToday);

		}
		nWeek = 0;
	}
	pICalendar->OnCalendarUpdateEnd(_Year,_Month);
}

bool CCalendarUI::OnLastYear( TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam )
{
	if( pTNotifyUI->pSender && pTNotifyUI->pSender == pLastYearBtn)
	{
		pCurDateTime->tm_year--;

		if(GetEnabledYearSel())
			InitCalendarDis(pCurDateTime->tm_year,pCurDateTime->tm_mon);

		this->GetManager()->SendNotify(pLastYearBtn,_T("OnCalendarLastYear"),pCurDateTime->tm_year,pCurDateTime->tm_mon);
		pMoothPanelHorz->SetVisible(false);

		return true;
	}
	return false;
}

bool CCalendarUI::OnMoothSelect( TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam )
{
	if( pTNotifyUI->pSender && pTNotifyUI->pSender == pMoothSelectBtn && GetEnabledMoothSel())
	{
		pMoothPanelHorz->SetVisible(true);
		RECT rc = pMoothSelectBtn->GetPos();
		rc.top		= rc.bottom;
		rc.bottom	= rc.top + 132;
		pMoothPanelHorz->SetPos(rc);
		pMoothPanelHorz->SetFloat(true);
	}
	return false;
}

bool CCalendarUI::OnNextYear( TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam )
{
	if( pTNotifyUI->pSender && pTNotifyUI->pSender == pNextYearBtn)
	{
		if(GetEnabledYearSel())
			InitCalendarDis(++pCurDateTime->tm_year,pCurDateTime->tm_mon);

		this->GetManager()->SendNotify(this,_T("OnCalendarNextYear"),pCurDateTime->tm_year,pCurDateTime->tm_mon);
		pMoothPanelHorz->SetVisible(false);
	}
	return true;
}

bool CCalendarUI::OnSelectMooth( TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam )
{
	if( pTNotifyUI->pSender)
	{
		if(GetEnabledMoothSel())
			InitCalendarDis(pCurDateTime->tm_year,pTNotifyUI->pSender->GetTag());

		this->GetManager()->SendNotify(this,_T("OnSelectMonth"),pCurDateTime->tm_year,pCurDateTime->tm_mon);
		pMoothPanelHorz->SetVisible(false);
	}
	return true;
}

bool CCalendarUI::OnSelcetDay( TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam )
{
	if( pTNotifyUI->pSender)
	{	
		TCalendarInfo* pTCalendarInfo = (TCalendarInfo*)pTNotifyUI->pSender->GetParent()->GetTag();

		if(GetEnabledMoothSel() && pTCalendarInfo && pTCalendarInfo->nMooth != pTCalendarInfo->nAsMooth){
			InitCalendarDis(pTCalendarInfo->nYear,pTCalendarInfo->nAsMooth);
			return true;
		}
		OnCalendarChange(pTCalendarInfo->nYear,pTCalendarInfo->nMooth,pTCalendarInfo->nDay);

		CalDateTime(pTCalendarInfo->nYear,pTCalendarInfo->nMooth,pTCalendarInfo->nDay);
		this->GetManager()->SendNotify(this,_T("OnSelcetDay"),pTNotifyUI->pSender->GetTag(),(LPARAM)pTNotifyUI->pSender,true);
		pMoothPanelHorz->SetVisible(false);
	}
	return true;
}

void CCalendarUI::SetCallback( ICalendar* _ICalendar )
{
	if(pICalendar)
		pICalendar = _ICalendar;
}

void CCalendarUI::InitWeekPanel( CControlUI* _Control,int _Week )
{
	switch (_Week)
	{
		case 0:
			{
				_Control->SetText(mSubTitleString.nSundayStr);
				_Control->SetAttribute(_T("showhtml"),_T("true"));
			}
			break;
		case 1:
			{
				_Control->SetText(mSubTitleString.nMondayStr);
				_Control->SetAttribute(_T("showhtml"),_T("true"));
			}
			break;
		case 2:
			{
				_Control->SetText(mSubTitleString.nTuesdayStr);
				_Control->SetAttribute(_T("showhtml"),_T("true"));
			}
			break;
		case 3:
			{
				_Control->SetText(mSubTitleString.nWednesdayStr);
				_Control->SetAttribute(_T("showhtml"),_T("true"));
			}
			break;
		case 4:
			{
				_Control->SetText(mSubTitleString.nThursdayStr);
				_Control->SetAttribute(_T("showhtml"),_T("true"));
			}
			break;
		case 5:
			{
				_Control->SetText(mSubTitleString.nFridayStr);
				_Control->SetAttribute(_T("showhtml"),_T("true"));
			}
			break;
		case 6:
			{
				_Control->SetText(mSubTitleString.nSaturdayStr);
				_Control->SetAttribute(_T("showhtml"),_T("true"));
			}
			break;
		default:
			break;
	}
}

void CCalendarUI::InitDayPanel( CControlUI* _Control,bool bWeekEnd,TCalendarInfo& _CalendarInfo )
{ 
	InitDayPanel(_Control,bWeekEnd,_CalendarInfo.nYear,_CalendarInfo.nMooth,_CalendarInfo.nDay,_CalendarInfo.nWeek,_CalendarInfo.nWeekLine,_CalendarInfo.nAsMooth);
}

void CCalendarUI::InitDayPanel( CControlUI* _Control,bool bWeekEnd,int _Year,int _Mooth,int _Day,int _Week,int _WeekLine,int _AsMooth )
{
	if(_AsMooth != _Mooth)
		_Control->SetAttribute(_T("bkcolor"),m_DefaultStyle.nNoCurMonthDayColor);
	else 
		_Control->SetAttribute(_T("bkcolor"),_T(""));

	if(_Mooth != _AsMooth)
		_Control->SetMouseEnabled(GetEnabledMoothSel());
	else 
		_Control->SetMouseEnabled(true);
	
	CDuiString TmpDef;

	if(bWeekEnd)
	{
		TmpDef.SmallFormat(_T("{c #FF0000}%d{/c}"),_Day); 
	}
	else
	{
		TmpDef.SmallFormat(_T("%d"),_Day); 
	}

	_Control->SetText(TmpDef.GetData());
}

void CCalendarUI::SetEnabledYearSel( bool _Enabled /*= true*/ )
{
	pEnabledYearSel = _Enabled;
}

bool CCalendarUI::GetEnabledYearSel()
{
	return pEnabledYearSel;
}

void CCalendarUI::SetEnabledMoothSel( bool _Enabled /*= true*/ )
{
	pEnabledMoothSel = _Enabled;
}

bool CCalendarUI::GetEnabledMoothSel()
{
	return pEnabledMoothSel;
}

void CCalendarUI::OnCalendarUpdateStart( int _Year,int _Mooth)
{
	CDuiString TmpDef;

	TmpDef.SmallFormat(_T("{b}{c #FFFFFF}\r\n%.4d-%.2d{/c}{/b}"),_Year,_Mooth);
	pMoothSelectBtn->SetText(TmpDef.GetData());
	
	TmpDef.SmallFormat(_T("{b}{c #FFFFFF}%.4d<<{/c}{/b}"),_Year-1);
	pLastYearBtn->SetText(TmpDef.GetData());

	TmpDef.SmallFormat(_T("{b}{c #FFFFFF}>>%.4d{/c}{/b}"),_Year+1);
	pNextYearBtn->SetText(TmpDef.GetData());
}

void CCalendarUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	if( _tcscmp(pstrName, _T("enableyearsel")) == 0 ) SetEnabledYearSel(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("enablemoothsel")) == 0 ) SetEnabledYearSel(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("mtheight")) == 0 ) SetMainTitleHeight(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("stheight")) == 0 ) SetSubTitleHeight(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("itheight")) == 0 ) SetStatusInfoHeight(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("style")) == 0 ) AnyCalendarStyle(pstrValue);
	else if( _tcscmp(pstrName, _T("cbtargetname")) == 0 ) SetComboTargetName(pstrValue);
	else CVerticalLayoutUI::SetAttribute(pstrName,pstrValue);
}

CControlUI* CCalendarUI::CreateInfoPanel()
{
	CHorizontalLayoutUI* pControl	= new CHorizontalLayoutUI();
	pDateTimeBtn					= new CButtonUI();
	pToDayBtn						= new CButtonUI();

	pToDayBtn->SetShowHtml(true);
	pDateTimeBtn->SetMouseEnabled(false);
	pDateTimeBtn->SetFixedWidth(100);
	
	RECT rcInset = { 0 }; 
	rcInset.left = 60;    
	pControl->SetInset(rcInset);

	pControl->Add(pDateTimeBtn);
	pControl->Add(new CControlUI());
	pControl->Add(pToDayBtn);
	
	pToDayBtn->OnNotifyUI			   += MakeDelegateUI(this,&CCalendarUI::OnToday,_T("click"));
	pToDayBtn->SetText(mSubTitleString.nToDayString);
	pToDayBtn->SetFixedWidth(60);
	pToDayBtn->SetAttribute(_T("hotbkcolor"),m_DefaultStyle.nDayHotColor);

	return pControl;
}

bool CCalendarUI::OnToday(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam)
{
	if( pTNotifyUI->pSender)
	{
		//初始化当前日期
		time_t now;	
		time(&now);
		pCurDateTime = localtime(&now);
		pCurDateTime->tm_year	= pCurDateTime->tm_year-100+2000;
		pCurDateTime->tm_mon	= pCurDateTime->tm_mon+1;
		mToday					= pCurDateTime->tm_mday;

		InitCalendarDis(pCurDateTime->tm_year,pCurDateTime->tm_mon);


		this->GetManager()->SendNotify(this,_T("OnToday"),pTNotifyUI->pSender->GetTag(),(LPARAM)pTNotifyUI->pSender,true);
		pMoothPanelHorz->SetVisible(false);

		return false;
	}
	return true;
}

void CCalendarUI::OnCalendarChange( int _Year,int _Mooth,int _Day )
{
	CDuiString TmpDef;
	TmpDef.SmallFormat(_T("%.4d-%.2d-%.2d"),_Year,_Mooth,_Day);

	if(pDateTimeBtn)
		pDateTimeBtn->SetText(TmpDef.GetData());
}

void CCalendarUI::SetMainTitleHeight( int _Height )
{
	pMainTitleHoriz->SetFixedHeight(_Height);
}

int CCalendarUI::GetMainTitleHeight()
{
	return pMainTitleHoriz->GetFixedHeight();
}

void CCalendarUI::SetSubTitleHeight( int _Height )
{
	pSubTitleHoriz->SetFixedHeight(_Height);
}

int CCalendarUI::GetSubTitleHeight()
{
	return pSubTitleHoriz->GetFixedHeight();
}

void CCalendarUI::SetStatusInfoHeight( int _Height )
{
	pInfoPanelHorz->SetFixedHeight(_Height);
}

int CCalendarUI::GetStatusInfoHeight()
{
	return pInfoPanelHorz->GetFixedHeight();
}

void CCalendarUI::AnyCalendarStyle( LPCTSTR _StyleStr,TCalendarStyle* _TCalendarStyle /*= NULL*/ )
{
	CDuiString sItem;
	CDuiString sValue;
	LPTSTR pstr = NULL;

	TCalendarStyle* pCalendarInfo = _TCalendarStyle?_TCalendarStyle:&m_DefaultStyle;

	while( *_StyleStr != _T('\0') ) {
		sItem.Empty();
		sValue.Empty();
		while( *_StyleStr > _T('\0') && *_StyleStr <= _T(' ') ) _StyleStr = ::CharNext(_StyleStr);
		while( *_StyleStr != _T('\0') && *_StyleStr != _T('=') && *_StyleStr > _T(' ') ) {
			LPTSTR pstrTemp = ::CharNext(_StyleStr);
			while( _StyleStr < pstrTemp) {
				sItem += *_StyleStr++;
			}
		}
		while( *_StyleStr > _T('\0') && *_StyleStr <= _T(' ') ) _StyleStr = ::CharNext(_StyleStr);
		if( *_StyleStr++ != _T('=') ) break;
		while( *_StyleStr > _T('\0') && *_StyleStr <= _T(' ') ) _StyleStr = ::CharNext(_StyleStr);
		if( *_StyleStr++ != _T('\'') ) break;
		while( *_StyleStr != _T('\0') && *_StyleStr != _T('\'') ) {
			LPTSTR pstrTemp = ::CharNext(_StyleStr);
			while( _StyleStr < pstrTemp) {
				sValue += *_StyleStr++;
			}
		}
		if( *_StyleStr++ != _T('\'') ) break;
		if( !sValue.IsEmpty() ) {
			if( sItem == _T("bbcolor") ) pCalendarInfo->nCalenderBorderColor = sValue;
			else if( sItem == _T("bkcolor") ) pCalendarInfo->nCalendarBkColor = sValue;
			else if( sItem == _T("tbkcolor") ) pCalendarInfo->nMainTitleBkColor = sValue;
			else if( sItem == _T("sbkcolor") ) pCalendarInfo->nSubTitleBkColor = sValue;
			else if( sItem == _T("webkcolora") ) pCalendarInfo->nWeekendColorA = sValue;
			else if( sItem == _T("webkcolorb") ) pCalendarInfo->nWeekendColorB = sValue;
			else if( sItem == _T("dayhotcolor") ) pCalendarInfo->nDayHotColor = sValue;
			else if( sItem == _T("daypushedcolor") ) pCalendarInfo->nDayPushedColor = sValue;
			else if( sItem == _T("dayselcolor") ) pCalendarInfo->nDaySelectColor = sValue;
			else if( sItem == _T("daydiscolor") ) pCalendarInfo->nDayDisabledColor = sValue;
			else if( sItem == _T("nocurmonthdaycolor") ) pCalendarInfo->nNoCurMonthDayColor = sValue;
			else if( sItem == _T("weekicolora") ) pCalendarInfo->nWeekIntervalColorA = sValue;
			else if( sItem == _T("weekicolorb") ) pCalendarInfo->nWeekIntervalColorB = sValue;
			else if( sItem == _T("ibkcolor") ) pCalendarInfo->nStatusBkColor = sValue;
		}
		if( *_StyleStr++ != _T(' ') ) break;
	}
	SetCalendarStyle(*pCalendarInfo);
}

void CCalendarUI::CalDateTime( TCalendarInfo* _TCalendarInfo )
{
	if(_TCalendarInfo)
		CalDateTime(_TCalendarInfo->nYear,_TCalendarInfo->nMooth,_TCalendarInfo->nDay);
}

void CCalendarUI::CalDateTime( int _Year,int _Mooth,int _Day )
{
	mDateTime.SmallFormat(_T("%.04d-%.02d-%.02d"),_Year,_Mooth,_Day);
}

CDuiString CCalendarUI::GetCurSelDateTime()
{
	return mDateTime;
}

void CCalendarUI::SetComboTargetName( LPCTSTR pstrName )
{
	m_sComboTargetName = mDateTime;
}

CDuiString CCalendarUI::GetComTargetName()
{
	return m_sComboTargetName;
}

CControlUI* CCalendarUI::GetComTargetObj()
{
	if(GetComTargetName().IsEmpty())
		return NULL;
	CControlUI* pControl = static_cast<CControlUI*>(GetManager()->FindControl(m_sComboTargetName));
	return pControl;
}

void CCalendarUI::ShowCalendarDlg( CButtonUI* _pControl )
{
	CCalendarDlg *pCalendarDlg=new CCalendarDlg(_pControl); 
}

}
