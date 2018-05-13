#include "BrowserClient.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/base/cef_lock.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "../CefBrowserDll.h"

#include <string>

namespace DuiLib {
	CBrowserClient::CBrowserClient():
		browser_loaded_(false),
		life_handle_(NULL),
		load_handle_(NULL),
		display_handle_(NULL),
		filedialog_handle_(NULL),
		is_created_(false) {

	}

	CBrowserClient::~CBrowserClient(void) {
		if (handle_name_pipe_ != INVALID_HANDLE_VALUE) {
			DisconnectNamedPipe(handle_name_pipe_);
			CloseHandle(handle_name_pipe_);
		}
		function_map_.clear();
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

	CefRefPtr<CefDragHandler> CBrowserClient::GetDragHandler() {
		return this;
	}

	CefRefPtr<CefDialogHandler> CBrowserClient::GetDialogHandler() {
		return this;
	}

	CefRefPtr<CefKeyboardHandler> CBrowserClient::GetKeyboardHandler() {
		return this;
	}

	bool CBrowserClient::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
		const CefKeyEvent& event,
		CefEventHandle os_event,
		bool* is_keyboard_shortcut) {
			if (os_event) {
				if (os_event->message == WM_SYSKEYDOWN || os_event->message == WM_SYSKEYUP) {
					return true;
				}
			}
			return false;
	}

	bool CBrowserClient::OnFileDialog(CefRefPtr<CefBrowser> browser,
		FileDialogMode mode,
		const CefString& title,
		const CefString& default_file_path,
		const std::vector<CefString>& accept_filters,
		int selected_accept_filter,
		CefRefPtr<CefFileDialogCallback> callback) {
			if (filedialog_handle_) {
				filedialog_handle_->OnBeforeFileDialog(browser);
			}

			std::wstring path = OpenPictureFile();
			if (filedialog_handle_) {
				filedialog_handle_->OnAfterFileDialog(browser);
			}

			if (path.empty()) {
				callback->Cancel();
			} else {
				std::vector<CefString> filters;
				filters.push_back(path);
				callback->Continue(selected_accept_filter, filters);
			}
			return true;
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

	bool CBrowserClient::OnDragEnter(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDragData> dragData,
		DragOperationsMask mask) { 
			return true;
	}

	bool CBrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) {
			CEF_REQUIRE_UI_THREAD();
			//无用消息
			if (!message->IsValid()) {
				DWORD dwWrite;
				WriteFile(handle_name_pipe_, L"DONE", wcslen(L"DONE") * 2, &dwWrite, NULL);
				return false;
			}
			
			//执行函数
			CefString name = message->GetName();
			
			auto iter = function_map_.find(name);
			if (iter != function_map_.end()) {
				CefV8ValueList list;
				//base::AutoLock lock_scope(lock_);
				auto arguments = message->GetArgumentList();

				wchar_t buf[1024] = {0}; 
				CefRefPtr<CefValue> ret = iter->second(arguments);
				if(ret != NULL) {
					if (ret->GetType() == VTYPE_BOOL) {
						if (ret->GetBool()) {
							wsprintf(buf, L"%s", L"true");
						} else {
							wsprintf(buf, L"%s", L"false");
						}

					} else if (ret->GetType() == VTYPE_INT) {
						wsprintf(buf, L"%d", ret->GetInt());

					} else if (ret->GetType() == VTYPE_STRING) {
						wsprintf(buf, L"%s", ret->GetString());

					} else if (ret->GetType() == VTYPE_DOUBLE) {
						wsprintf(buf, L"%f", ret->GetDouble());
					}

				} else {
					wsprintf(buf, L"%s", L"DONE");
				}
				DWORD dwWrite;
				WriteFile(handle_name_pipe_, buf, wcslen(buf) * 2, &dwWrite, NULL);
				return true;

			} else {
				DWORD dwWrite;
				WriteFile(handle_name_pipe_, L"DONE", wcslen(L"DONE") * 2, &dwWrite, NULL);
				return false;
			}
	}

	bool CBrowserClient::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		bool is_redirect) {
			return false;  
	}

	void CBrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();
		base::AutoLock lock_scope(lock_);
		if (!browser_) {
			browser_ = browser;
			is_created_ = true;
			//给Render进程发送消息

			//创建命名管道
			wchar_t name_pipe[50] = {0};
			wsprintf(name_pipe, L"\\\\.\\pipe\\cef_pipe_%d", browser->GetIdentifier());
			handle_name_pipe_ = CreateNamedPipe(name_pipe, PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED,
				0, 1, 1024, 1024, 0, NULL);

			//发送消息 创建JS回调和连接管道
			CefRefPtr<CefProcessMessage> msg= CefProcessMessage::Create(L"CreateBrowser");
			if (msg->IsValid()) {
				CefRefPtr<CefListValue> msg_param = msg->GetArgumentList();
				msg_param->SetString(0, name_pipe);
				browser_->SendProcessMessage(PID_RENDERER, msg);
			}

			//客户端连接
			if (handle_name_pipe_ != INVALID_HANDLE_VALUE) {
				HANDLE hEvent;
				hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
				if (hEvent != INVALID_HANDLE_VALUE) {
					OVERLAPPED over = {0};
					ConnectNamedPipe(handle_name_pipe_, &over);
				}
			}

			//给Render进程发送消息 设置函数名称
			if (function_map_.size() != 0) {
				CefRefPtr<CefProcessMessage> msg_fun= CefProcessMessage::Create(L"SetFunctionName");
				if (msg_fun->IsValid()) {
					CefRefPtr<CefListValue> args = msg_fun->GetArgumentList();
					int index = 0;
					for (auto iter = function_map_.begin(); iter != function_map_.end(); ++iter) {
						args->SetString(index, iter->first);
						++index;
					}
					browser_->SendProcessMessage(PID_RENDERER, msg_fun);
				}
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
		function_map_.clear();
		is_created_ = false;

		//通知render进程关闭浏览器
		CefRefPtr<CefProcessMessage> msg= CefProcessMessage::Create(L"CloseBrowser");
		browser->SendProcessMessage(PID_RENDERER, msg);

		return true;
	}

	void CBrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
		CEF_REQUIRE_UI_THREAD();
		base::AutoLock lock_scope(lock_);
		/*if (life_handle_) {
			life_handle_->OnBeforeClose(browser);
		}*/
		browser_ = NULL;
	}

	void CBrowserClient::OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) {
		CEF_REQUIRE_UI_THREAD();
		if (load_handle_) {
			load_handle_->OnLoadError(browser, frame, errorCode, errorText, failedUrl);
		}
	}

	//加载状态改变
	void CBrowserClient::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward) {
		CEF_REQUIRE_UI_THREAD();
		if (load_handle_) {
			load_handle_->OnLoadingStateChange(browser, isLoading, canGoBack, canGoForward);
		}
	}

	//加载开始
	void CBrowserClient::OnLoadStart(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame) {
		CEF_REQUIRE_UI_THREAD();
		if (load_handle_) {
			load_handle_->OnLoadStart(browser, frame);
		}
	}

	//加载完成
	void CBrowserClient::OnLoadEnd(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int httpStatusCode) {
		CEF_REQUIRE_UI_THREAD();
		browser_loaded_ = true;
		if (load_handle_) {
			load_handle_->OnLoadEnd(browser, frame, httpStatusCode);
		}
	}

	//地址改变
	void CBrowserClient::OnAddressChange(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& url) {
		CEF_REQUIRE_UI_THREAD();
		if (display_handle_) {
			display_handle_->OnAddressChange(browser, frame, url);
		}
	}

	//标题改变
	void CBrowserClient::OnTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) {
		CEF_REQUIRE_UI_THREAD();
		if (display_handle_) {
			display_handle_->OnTitleChange(browser, title);
		}
	}

	//页面图标改变
	void CBrowserClient::OnFaviconURLChange(CefRefPtr<CefBrowser> browser,
		const std::vector<CefString>& icon_urls) {
		CEF_REQUIRE_UI_THREAD();
		if (display_handle_) {
			display_handle_->OnFaviconURLChange(browser, icon_urls);
		}
	}

	//页面全屏时调用
	void CBrowserClient::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
		bool fullscreen) {
		CEF_REQUIRE_UI_THREAD();
		if (display_handle_) {
			display_handle_->OnFullscreenModeChange(browser, fullscreen);
		}
	}

	//浏览器显示工具提示
	bool CBrowserClient::OnTooltip(CefRefPtr<CefBrowser> browser,
		CefString& text) {
		CEF_REQUIRE_UI_THREAD();
		if (display_handle_) {
			display_handle_->OnTooltip(browser, text);
		}
		return true;
	}

	//接收到状态信息时
	void CBrowserClient::OnStatusMessage(CefRefPtr<CefBrowser> browser,
		const CefString& value) {
		CEF_REQUIRE_UI_THREAD();
		if (display_handle_) {
			display_handle_->OnStatusMessage(browser, value);
		}
	}

	//调用显示控制台信息
	bool CBrowserClient::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
		const CefString& message,
		const CefString& source,
		int line) {
		CEF_REQUIRE_UI_THREAD();
		if (display_handle_) {
			display_handle_->OnConsoleMessage(browser, message, source, line);
		}
		return true;
	}

	std::wstring CBrowserClient::OpenPictureFile() {
		wchar_t szFileName[MAX_PATH];
		wmemset(szFileName, 0, MAX_PATH);
		OPENFILENAME openfilename = {0};
		ZeroMemory(&openfilename, sizeof(openfilename));

		openfilename.lStructSize = sizeof(openfilename);
		openfilename.hwndOwner = GetParent(browser_->GetHost()->GetWindowHandle());
		openfilename.hInstance = NULL;
		openfilename.lpstrCustomFilter = NULL;
		openfilename.nMaxCustFilter = 0L;
		openfilename.nFilterIndex = 1L;
		openfilename.lpstrFile = szFileName;
		openfilename.nMaxFile = MAX_PATH;
		openfilename.lpstrFileTitle = NULL;
		openfilename.nMaxFileTitle = 0;
		openfilename.lpstrInitialDir = NULL;
		openfilename.lpstrFileTitle = NULL;
		openfilename.nFileOffset = 0;
		openfilename.nFileExtension = 0;
		openfilename.lpstrDefExt = L"*.*";
		openfilename.lCustData = 0;
		openfilename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_READONLY | OFN_EXPLORER;

		if (GetOpenFileName(&openfilename)) {
			return szFileName;
		}
		return L"";
	}

	void CBrowserClient::CreateCefWindow(HWND hWnd, const CefString& url, 
		ILifeSpanHandleSlot* life_handle, 
		ILoadHandleSlot* load_handle,
		IDisplayHandleSolt* display_handle,
		ILifeSpanFileDialogSolt* filedialog_handle) {
			if (!CefCurrentlyOn(TID_UI)) {
				// Execute this method on the UI thread.
				CefPostTask(TID_UI, base::Bind(&CBrowserClient::CreateCefWindow, this,
					hWnd, url, life_handle, load_handle,
					display_handle, filedialog_handle));
				return;
			}

			CEF_REQUIRE_UI_THREAD();
			CefWindowInfo info;

			//创建子窗口
			RECT rect = { 0, 0, 1, 1 };

			info.SetAsChild(hWnd, rect);
			//info.SetAsPopup(NULL,"test");

			life_handle_ = life_handle;
			load_handle_ = load_handle;
			display_handle_ = display_handle;
			filedialog_handle_ = filedialog_handle;

			CefBrowserSettings b_settings;
			CefBrowserHost::CreateBrowserSync(info, this, url, b_settings, NULL);
	}

	CefRefPtr<CefBrowser> CBrowserClient::GetBrowser() {
		if (is_created_) {
			return browser_;
		}
		return NULL;
	}

	bool CBrowserClient::IsCreated() const {
		return is_created_;
	}

	void CBrowserClient::SetFunction(const CefString &name, CustomFunction function) {
		function_map_[name] = function;
	}

	void CBrowserClient::CloseBrowser() {
		browser_->GetHost()->CloseBrowser(true);
	}
}

