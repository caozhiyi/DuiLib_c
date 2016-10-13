#include "TestWnd.h"


CTestWnd::CTestWnd(void) : browser_(NULL), browser_pan_(NULL){

}


CTestWnd::~CTestWnd(void) {

}

LPCTSTR CTestWnd::GetWindowClassName() const {
	return L"TestWndClass";
}

CDuiString CTestWnd::GetSkinFolder() {
	return L"skin";
}

CDuiString CTestWnd::GetSkinFile() {
	return L"TestWnd.xml";
}

UILIB_RESOURCETYPE CTestWnd::GetResourceType() const {
	return UILIB_FILE;
}

void CTestWnd::InitWindow() {
	browser_ = static_cast<CCefBrowserUI*>(m_PaintManager.FindControl(_T("TestBrowser")));
	browser_pan_ = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("BrowserPan")));
}

void CTestWnd::Notify(TNotifyUI& msg) {
	__super::Notify(msg);
}

LRESULT CTestWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) {
	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}

void CTestWnd::Show() {
	Create(GetHWND(), _T("TestWnd"), UI_WNDSTYLE_FRAME&~WS_THICKFRAME&~WS_MAXIMIZEBOX, NULL);
	CenterWindow();
	ShowModal();
	SetFocus(m_hWnd);
}

CControlUI* CTestWnd::CreateControl(LPCTSTR pstrClass) {
	//创建Cef控件类******************************************************************************
	if (_tcsicmp(pstrClass, CCefBrowserUIItemInterface) == 0) {
		return  new CCefBrowserUI(m_hWnd, this);
	}
	return NULL;  
}

ILoadHandleSlot* CTestWnd::GetILoadHandleSlot(std::wstring browser_name) {
	if (browser_name == L"TestBrowser") {
		return this;
	}
	return NULL;
}

ILifeSpanHandleSlot* CTestWnd::GetILifeSpanHandleSlot(std::wstring browser_name) {
	if (browser_name == L"TestBrowser") {
		return this;
	}
	return NULL;
}

IDisplayHandleSolt* CTestWnd::GetIDisplayHandleSolt(std::wstring browser_name) {
	return NULL;
}

void CTestWnd::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	//可以在这里动态加载URL
	CDuiRect rect = browser_pan_->GetPos();
	browser_->ResizeBrowser(rect);
}

bool CTestWnd::DoClose(CefRefPtr<CefBrowser> browser) {
	return true;
}

//加载出错
void CTestWnd::OnLoadError(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefLoadHandler::ErrorCode errorCode,
	const CefString& errorText,
	const CefString& failedUrl) {

}

//加载状态改变
void CTestWnd::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
	bool isLoading,
	bool canGoBack,
	bool canGoForward) {

}

//加载开始
void CTestWnd::OnLoadStart(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame) {

}

//加载完成
void CTestWnd::OnLoadEnd(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	int httpStatusCode) {
		CDuiRect rect = browser_pan_->GetPos();
		browser_->ResizeBrowser(rect);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR command_line, int) {
	CCefBrowserUI::InitCef(instance);
	CTestWnd wnd;
	wnd.Show();
}
