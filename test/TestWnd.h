#pragma once
#include "UIlib.h"
#include "UICefBrowser.h"
using namespace DuiLib;
class CTestWnd :
	public WindowImplBase,
	public ISetHandleSolt,
	public ILifeSpanHandleSlot,
	public ILoadHandleSlot
{
public:
	CTestWnd(void);
	~CTestWnd(void);
	LPCTSTR GetWindowClassName() const ;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual void InitWindow();

	virtual void Notify(TNotifyUI& msg);
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	void Show();


	CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual ILoadHandleSlot* GetILoadHandleSlot(std::wstring browser_name);
	virtual ILifeSpanHandleSlot* GetILifeSpanHandleSlot(std::wstring browser_name);
	virtual IDisplayHandleSolt* GetIDisplayHandleSolt(std::wstring browser_name);

	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser);
	virtual void DoClose(CefRefPtr<CefBrowser> browser);
	//virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser);
	//加载出错
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefLoadHandler::ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl);

	//加载状态改变
	virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward);

	//加载开始
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame);

	//加载完成
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int httpStatusCode);
private:
	CCefBrowserUI* browser_;
	CHorizontalLayoutUI* browser_pan_;
};

