#include "RenderAndBrowserApp.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"


namespace DuiLib
{
	CRenderAndBrowserApp::CRenderAndBrowserApp() {

	}

	CRenderAndBrowserApp::~CRenderAndBrowserApp(void) {

		
	}

	CefRefPtr<CefRenderProcessHandler> CRenderAndBrowserApp::GetRenderProcessHandler() {
		return this;

	}


	void CRenderAndBrowserApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) {

		int id = browser->GetIdentifier();
		auto iter = context_solt_map_.find(id);
		if (iter != context_solt_map_.end()) {
			iter->second->OnContextCreated(browser, frame, context);

		}
	}

	void CRenderAndBrowserApp::OnWebKitInitialized()
	{
		//扩展V8 js自定义对象
		/*std::string app_code =
		"var app;"
		"if (!app)"
		"    app = {};"
		"(function() {"
		"    app.GetId = function() {"
		"        native function GetId();"
		"        return GetId();"
		"    };"
		"})();";

		CefRegisterExtension("v8/app", app_code, v8_extension_);*/
	}

	bool CRenderAndBrowserApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) {
		return true;

	}

	void CRenderAndBrowserApp::OnContextInitialized(){

	}

	CefRefPtr<CefBrowserProcessHandler> CRenderAndBrowserApp::GetBrowserProcessHandler() {
		return this;

	}

	void CRenderAndBrowserApp::OnBeforeChildProcessLaunch(
		CefRefPtr<CefCommandLine> command_line) {

	}

	void CRenderAndBrowserApp::OnRenderProcessThreadCreated(
		CefRefPtr<CefListValue> extra_info) {

	}

	void CRenderAndBrowserApp::SetIOnContextCreatedSolt(int browser_id ,CefRefPtr<IOnContextCreatedSolt> context_solt) {
		context_solt_map_[browser_id] = context_solt;

	}

	void CRenderAndBrowserApp::RemoveIOnContextCreatedSolt(int browser_id) {
		auto iter = context_solt_map_.find(browser_id);
		if (iter != context_solt_map_.end()) {
			context_solt_map_.erase(iter);

		}
	}
}

