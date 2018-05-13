#include "BrowserClientApp.h"
namespace DuiLib {
	CefRefPtr<CefBrowserProcessHandler> ClientAppBrowser::GetBrowserProcessHandler() {
		return this;
	}

	void ClientAppBrowser::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) {
		//主进程
		if (process_type.empty()) {
			//禁用离屏渲染
			command_line->AppendSwitch("off-screen-rendering-enabled");
			command_line->AppendSwitch("disable-extensions");
			command_line->AppendSwitch("disable-pdf-extension");
			command_line->AppendSwitch("disable-surfaces");
			//禁用GPU加速
			command_line->AppendSwitch("enable-gpu");
			command_line->AppendSwitch("disable-gpu");
			command_line->AppendSwitch("disable-gpu-compositing");
			//禁用代理服务
			command_line->AppendSwitch("no-proxy-server");
			
			//设置所有的页面共用一个进程，以节省进程数
			command_line->AppendSwitchWithValue("--renderer-process-limit", "1");
		}
	}
}