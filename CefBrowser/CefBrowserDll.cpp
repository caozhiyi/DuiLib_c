#include "CefBrowserDll.h"
#include "browser/BrowserClient.h"
#include "browser/BrowserClientApp.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include <Shlwapi.h>
#include <ShlObj.h>

namespace DuiLib {
	CefBrowserDll::CefBrowserDll(){

	}

	CefBrowserDll::~CefBrowserDll() {
	
	}

	//是否可以向前导航
	bool CefBrowserDll::CanGoForward() {
		if (IsCreated()) {
			return client_browser_->GetBrowser()->CanGoForward();
		}
		return false;
	}
	//向前导航
	void CefBrowserDll::GoForward() {
		if (IsCreated()) {
			client_browser_->GetBrowser()->GoForward();
		}
	}
	//重新加载
	void CefBrowserDll::Reload() {
		if (IsCreated()) {
			client_browser_->GetBrowser()->Reload();
		}
	}
	//忽略缓存加载
	void CefBrowserDll::ReloadIgnoreCache() {
		if (IsCreated()) {
			client_browser_->GetBrowser()->ReloadIgnoreCache();
		}	
	}
	//停止加载
	void CefBrowserDll::StopLoad() {
		if (IsCreated()) {
			client_browser_->GetBrowser()->StopLoad();
		}
	}
	//可以向后导航
	bool CefBrowserDll::CanGoBack() {
		if (IsCreated()) {
			return client_browser_->GetBrowser()->CanGoBack();
		}
		return false;
	}
	//向后导航
	void CefBrowserDll::GoBack() {
		if (IsCreated()) {
			client_browser_->GetBrowser()->GoBack();
		}
	}

	CefWindowHandle CefBrowserDll::GetHwnd() {
		if (client_browser_ == NULL) {
			return NULL;
		}
		if (IsCreated()) {
			return client_browser_->GetBrowser()->GetHost()->GetWindowHandle();
		}
		return NULL;
	}

	//CefBrowserDllHost
	//设置焦点
	void CefBrowserDll::SetFocus(bool enable) {
		if (IsCreated()) {
			client_browser_->GetBrowser()->GetHost()->SetFocus(enable);
		}
	}
	//获取缩放比例
	double CefBrowserDll::GetZoomLevel() {
		if (IsCreated()) {
			return client_browser_->GetBrowser()->GetHost()->GetZoomLevel();
		}
		return -1;
	}
	//设置缩放比例
	void CefBrowserDll::SetZoomLevel(double zoomLevel) {
		if (IsCreated()) {
			client_browser_->GetBrowser()->GetHost()->SetZoomLevel(zoomLevel);
		}
	}

	//CefFrame
	//加载url
	void CefBrowserDll::LoadURL(const CefString& url) {
		if (IsCreated()) {
			client_browser_->GetBrowser()->GetMainFrame()->LoadURL(url);
		}
	}
	//获取当前Url
	CefString CefBrowserDll::GetURL() {
		if (IsCreated()) {
			return client_browser_->GetBrowser()->GetMainFrame()->GetURL();
		}
		return "";
	}

	bool CefBrowserDll::InitCef(HINSTANCE hInstance, const CefString& cache_path, const CefString& log_file) {
		//高DPI支持
		CefEnableHighDPISupport();

		void* sandbox_info = NULL;
		CefMainArgs main_args(hInstance);

		CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
		command_line->InitFromString(::GetCommandLineW());
		if (!PathIsDirectory(cache_path.ToWString().c_str())) {
			SHCreateDirectory(NULL, cache_path.ToWString().c_str());
		}
		if (!PathIsDirectory(log_file.ToWString().c_str())) {
			SHCreateDirectory(NULL, log_file.ToWString().c_str());
		}

		//不同进程创建不同的app
		CefRefPtr<CefApp> app;
		DuiLib::ProcessType process_type = ClientApp::GetProcessType(command_line);
		if (process_type == DuiLib::BrowserProcess) {
			app = new ClientAppBrowser();
		} else {
			return false;
		}

		CefSettings settings; 
		CefSettingsTraits::init(&settings);
		//settings.single_process = true;
		settings.no_sandbox = true;
		CefString(&settings.browser_subprocess_path).FromWString( L"Cef_render.exe");
		settings.multi_threaded_message_loop = true;
		CefString(&settings.locale).FromWString(L"zh-CN");

		//缓存数据路径
		CefString(&settings.cache_path).FromWString(cache_path);
		//debug日志路径
		CefString(&settings.log_file).FromWString(log_file);
		//初始化cef
		CefInitialize(main_args, settings, app.get(), sandbox_info);
		return true;
	}

	void CefBrowserDll::CreateCefWindow(HWND hWnd, const CefString& url, 
		ILifeSpanHandleSlot* life_handle, 
		ILoadHandleSlot* load_handle,
		IDisplayHandleSolt* display_handle,
		ILifeSpanFileDialogSolt* filedialog_handle) {
			client_browser_ = new CBrowserClient();
			client_browser_->CreateCefWindow(hWnd, url, life_handle, load_handle, display_handle, filedialog_handle);
	}

	void CefBrowserDll::CloseCefWindow() {
		if (client_browser_) {
			client_browser_->CloseBrowser();
		}
	}

	void CefBrowserDll::SetFunction(const CefString &name, CustomFunction function) {
		if (client_browser_) {
			client_browser_->SetFunction(name, function);
		}
	}

	void CefBrowserDll::Quit() {
		CefShutdown();
	}

	void CefBrowserDll::ExectueJS(const CefString &jscode) {
		if (IsCreated()) {
			client_browser_->GetBrowser()->GetMainFrame()->ExecuteJavaScript(jscode,
				"", 0);
		}
	}

	bool CefBrowserDll::IsCreated() const {
		return client_browser_->IsCreated();
	}

	void CefBrowserDll::Run() {
		CefRunMessageLoop();
	}
}


