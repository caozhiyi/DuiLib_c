#include "BrowserClient.h"
#include <string>
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/cef_browser.h"
#include "include/base/cef_lock.h"
#include "include/wrapper/cef_helpers.h"
#include "CefBrowserDll.h"

namespace DuiLib
{
	CBrowserClient::CBrowserClient(std::unique_ptr<ILifeSpanHandleSlot> life_handle,
		std::unique_ptr<ILoadHandleSlot> load_handle,
		std::unique_ptr<IDisplayHandleSolt> display_handle):
	    is_closing_(false), 
		life_handle_(std::move(life_handle)),
		load_handle_(std::move(load_handle)),
		display_handle_(std::move(display_handle)),
		is_created_(false),
		browser_(NULL) {
		extension_ = new CV8ExtensionHandler();

	}

	CBrowserClient::~CBrowserClient(void) {
		life_handle_.release();
		load_handle_.release();
		display_handle_.release();

	}

	CefRefPtr<CefRequestHandler> CBrowserClient::GetRequestHandler() {
		return this;
	}

	CefRefPtr<CefContextMenuHandler> CBrowserClient::GetContextMenuHandler() {
		return this;
	}

	CefRefPtr<CefDisplayHandler> CBrowserClient::GetDisplayHandler() {
		return this;

	}

	CefRefPtr<CefLifeSpanHandler> CBrowserClient::GetLifeSpanHandler() {
		return this;

	}

	CefRefPtr<CefLoadHandler> CBrowserClient::GetLoadHandler() {
		return this;

	}

	bool CBrowserClient::OnBeforePopup(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		CefLifeSpanHandler::WindowOpenDisposition target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access) {
			ShellExecute(NULL, L"open", target_url.ToWString().c_str(), NULL, NULL, SW_SHOWNORMAL);
			return true;
	}

	void CBrowserClient::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model) {
			if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0) {
				// Add a separator if the menu already has items.
				if (model->GetCount() > 0) {
					model->Clear();
					//model->AddSeparator();
				}
			}
	}

	void CBrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();
		base::AutoLock lock_scope(lock_);

		if (!browser_) {
			browser_ = browser;
			is_created_ = true;
			if (browser_create_solt_) {
				browser_create_solt_->OnAfterCreated(browser);

			}
		}

		if (life_handle_) {
			life_handle_->OnAfterCreated(browser);

		}
		
	}

	bool CBrowserClient::DoClose(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();

		base::AutoLock lock_scope(lock_);

		if (life_handle_) {
			life_handle_->DoClose(browser);

		}

		is_closing_ = true;
		return true;
	}

	void CBrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
	{
		CEF_REQUIRE_UI_THREAD();

		base::AutoLock lock_scope(lock_);

		/*if (life_handle_)
		{
			life_handle_->OnBeforeClose(browser);
		}*/

		browser_ = NULL;
	}

	void CBrowserClient::OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) {
		if (load_handle_) {
			load_handle_->OnLoadError(browser, frame, errorCode, errorText, failedUrl);

		}
	}

	//加载状态改变
	void CBrowserClient::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward) {
		if (load_handle_) {
			load_handle_->OnLoadingStateChange(browser, isLoading, canGoBack, canGoForward);

		}
	}

	//加载开始
	void CBrowserClient::OnLoadStart(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame) {
		if (load_handle_) {
			load_handle_->OnLoadStart(browser, frame);

		}
	}

	//加载完成
	void CBrowserClient::OnLoadEnd(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int httpStatusCode) {
		if (load_handle_) {
			load_handle_->OnLoadEnd(browser, frame, httpStatusCode);

		}
	}

	bool CBrowserClient::IsClosing() const {
		return is_closing_;

	}

	bool CBrowserClient::IsCreated() const {
		return is_created_;

	}


	//地址改变
	void CBrowserClient::OnAddressChange(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& url) {
		if (display_handle_) {

			display_handle_->OnAddressChange(browser, frame, url);
		}	
	}

	//标题改变
	void CBrowserClient::OnTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) {
		if (display_handle_) {
			display_handle_->OnTitleChange(browser, title);

		}
		
	}

	//页面图标改变
	void CBrowserClient::OnFaviconURLChange(CefRefPtr<CefBrowser> browser,
		const std::vector<CefString>& icon_urls) {
		if (display_handle_) {
			display_handle_->OnFaviconURLChange(browser, icon_urls);

		}
	}

	//页面全屏时调用
	void CBrowserClient::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
		bool fullscreen) {
		if (display_handle_) {
			display_handle_->OnFullscreenModeChange(browser, fullscreen);

		}
	}

	//浏览器显示工具提示
	bool CBrowserClient::OnTooltip(CefRefPtr<CefBrowser> browser,
		CefString& text) {
		if (display_handle_) {
			return display_handle_->OnTooltip(browser, text);

		}
		return true;
	}

	//接收到状态信息时
	void CBrowserClient::OnStatusMessage(CefRefPtr<CefBrowser> browser,
		const CefString& value) {
		if (display_handle_) {
			display_handle_->OnStatusMessage(browser, value);

		}
	}

	//调用显示控制台信息
	bool CBrowserClient::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
		const CefString& message,
		const CefString& source,
		int line) {
		if (display_handle_) {
			return display_handle_->OnConsoleMessage(browser, message, source, line);

		}
		return true;
	}

	bool CBrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) {
		return false;

	}

	void CBrowserClient::SetFunction(const CefString &name, CustomFunction function) {
		extension_->SetFunction(name, function);

	}

	void CBrowserClient::SetIOnAfterCreateSolt(IOnAfterCreateSolt* browser_crate_solt) {
		browser_create_solt_ = browser_crate_solt;

	}
}

