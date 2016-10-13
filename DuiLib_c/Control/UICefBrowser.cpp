#include "StdAfx.h"
#include "UICefBrowser.h"
using namespace DuiLib;
CCefBrowserUI::CCefBrowserUI(HWND parent,
	ISetHandleSolt* set_handle):
	parent_hwnd_(parent),
	set_handle_(set_handle),
	is_created_(false) {

}

CCefBrowserUI::~CCefBrowserUI(void) {
	
}

LPCTSTR CCefBrowserUI::GetClass() const {
	return CCefBrowserUIClassName;

}

LPVOID CCefBrowserUI::GetInterface(LPCTSTR pstrName) {
	if( _tcscmp(pstrName, CCefBrowserUIItemInterface) == 0 ) 
		return static_cast<CCefBrowserUI*>(this);  
	return CControlUI::GetInterface(pstrName);  
}

void CCefBrowserUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) {
	if ( _tcscmp(pstrName, _T("name")) == 0)
		browser_name_ = pstrValue;
	else if( _tcscmp(pstrName, _T("url")) == 0 ) 
		url_ = pstrValue;  
	CControlUI::SetAttribute(pstrName, pstrValue);  
}

void CCefBrowserUI::DoInit() {
	if (set_handle_) {
		auto life_handle = std::unique_ptr<ILifeSpanHandleSlot>(set_handle_->GetILifeSpanHandleSlot(browser_name_));
		auto load_handle = std::unique_ptr<ILoadHandleSlot>(set_handle_->GetILoadHandleSlot(browser_name_));
		auto display_handle = std::unique_ptr<IDisplayHandleSolt>(set_handle_->GetIDisplayHandleSolt(browser_name_));
		browser_dll_.CreateCefWindow(parent_hwnd_, url_, std::move(life_handle), std::move(load_handle), std::move(display_handle));
		is_created_ = true;

	} else {
		browser_dll_.CreateCefWindow(parent_hwnd_, url_);
		is_created_ = true;

	}	
}

void CCefBrowserUI::InitCef(HINSTANCE hInstance, CefString cache_path, CefString log_file) {
	CefBrowserDll::InitCef(hInstance, cache_path, log_file);

}

void CCefBrowserUI::Quit() {
	CefBrowserDll::Quit();

}

void CCefBrowserUI::SetFunction(const CefString &name, CustomFunction function) {
	if (!is_created_) {
		return;
	}
	browser_dll_.SetFunction(name, function);

}

void CCefBrowserUI::StartCefMessageLoop() {
	CefBrowserDll::Run();

}


void CCefBrowserUI::LoadURL(std::wstring url) {
	if (!is_created_) {
		return;
	}
	browser_dll_.LoadURL(url);

}

bool CCefBrowserUI::CanGoForward() {
	if (!is_created_) {
		return false;
	}
	return browser_dll_.CanGoForward();

}

void CCefBrowserUI::GoForward() {
	if (!is_created_) {
		return;
	}
	browser_dll_.GoForward();

}

void CCefBrowserUI::Reload() {
	if (!is_created_) {
		return;
	}
	browser_dll_.Reload();

}

void CCefBrowserUI::ReloadIgnoreCache() {
	if (!is_created_) {
		return;
	}
	browser_dll_.ReloadIgnoreCache();

}

void CCefBrowserUI::StopLoad() {
	if (!is_created_) {
		return;
	}
	browser_dll_.StopLoad();

}

bool CCefBrowserUI::CanGoBack() {
	if (!is_created_) {
		return false;
	}
	return browser_dll_.CanGoBack();

}

void CCefBrowserUI::GoBack() {
	if (!is_created_) {
		return;
	}
	browser_dll_.GoBack();

}

CefWindowHandle CCefBrowserUI::GetHwnd() {
	if (!is_created_) {
		return NULL;
	}
	return browser_dll_.GetHwnd();

}

void CCefBrowserUI::SetZoomLevel(double zoomLevel) {
	if (!is_created_) {
		return;
	}
	browser_dll_.SetZoomLevel(zoomLevel);

}

CefString CCefBrowserUI::GetURL() {
	if (!is_created_) {
		return "";
	}
	return browser_dll_.GetURL();
	
}

void CCefBrowserUI::ExectueJS(const CefString &jscode) {
	if (!is_created_) {
		return;
	}
	browser_dll_.ExectueJS(jscode);

}

bool CCefBrowserUI::CreateCefWindow(CefString url, bool needfocus,
	std::unique_ptr<ILifeSpanHandleSlot> life_handle, 
	std::unique_ptr<ILoadHandleSlot> load_handle,
	std::unique_ptr<IDisplayHandleSolt> display_handle) {
	browser_dll_.SetNeedFocus(needfocus);
	if(browser_dll_.CreateCefWindow(parent_hwnd_, url_, std::move(life_handle), std::move(load_handle), std::move(display_handle))) {
		is_created_ = true;
		return true;

	}
	return false;
	
}

void CCefBrowserUI::CloseCefWindow() {
	if (!is_created_) {
		return;
	}
	is_created_ = false;
	browser_dll_.CloseCefWindow();

}

void CCefBrowserUI::ResizeBrowser(DuiLib::CDuiRect rect) {
	if (!is_created_) {
		return;
	}
	CefWindowHandle hwnd = browser_dll_.GetHwnd();
	if (hwnd) {
		//::UpdateWindow(parent_hwnd_);
		::MoveWindow(hwnd, rect.left, rect.top, rect.GetWidth(), rect.GetHeight(), TRUE);

	}
}

void CCefBrowserUI::SetVisible(bool bVisible) {
	CControlUI::SetVisible(bVisible);
	if (is_created_) {
		CefWindowHandle hwnd = browser_dll_.GetHwnd();
		if( hwnd != NULL) 
			::ShowWindow(hwnd, IsVisible() ? SW_SHOW : SW_HIDE);

	}
}

void CCefBrowserUI::SetInternVisible(bool bVisible) {
	CControlUI::SetInternVisible(bVisible);
	if (is_created_) {
		CefWindowHandle hwnd = browser_dll_.GetHwnd();
		if( hwnd != NULL) 
			::ShowWindow(hwnd, IsVisible() ? SW_SHOW : SW_HIDE);

	}
}