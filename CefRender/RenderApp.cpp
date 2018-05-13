#include "RenderApp.h"
#include <string>
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
#include "V8ExtensionHandler.h"
#include <share.h>


namespace DuiLib
{
	CRenderApp::CRenderApp(const std::vector<CefString> &function_name) {

	}

	CRenderApp::CRenderApp() {

	}

	CRenderApp::~CRenderApp(void) {
		
	}

	CefRefPtr<CefRenderProcessHandler> CRenderApp::GetRenderProcessHandler() {
		return this;
	}


	void CRenderApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) {
		int id = browser->GetIdentifier();
		auto iter = browser_v8extension_map_.find(id);
		if (iter != browser_v8extension_map_.end()) {
			iter->second->OnContextCreated(browser, frame, context);
		}
	}

	void CRenderApp::OnWebKitInitialized() {
		
	}

	void CRenderApp::SetIOnContextCreatedSolt(int browser_id ,CefRefPtr<IOnContextCreatedSolt> context_solt) {
		browser_v8extension_map_[browser_id] = context_solt;

	}

	bool CRenderApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) {
		//处理消息
		//创建浏览器
		if (message->GetName() == L"CreateBrowser") {
			CefRefPtr<IOnContextCreatedSolt> context_solt = new CV8ExtensionHandler();
			context_solt->ConnectionNamePipe(message->GetArgumentList()->GetString(0));
			browser_v8extension_map_[browser->GetIdentifier()] = context_solt;

		//设置函数
		} else if (message->GetName() == L"SetFunctionName") {
			auto iter = browser_v8extension_map_.find(browser->GetIdentifier());
			if (iter != browser_v8extension_map_.end()) {
				auto argu = message->GetArgumentList();
				for (int i = 0; i < argu->GetSize(); ++i) {
					iter->second->SetFunction(argu->GetString(i));
				}
			}

		//关闭浏览器
		} else if (message->GetName() == L"CloseBrowser") {
			auto iter = browser_v8extension_map_.find(browser->GetIdentifier());
			if (iter != browser_v8extension_map_.end()) {
				iter->second->Release();
			}
		}
		return true;
	}
}

