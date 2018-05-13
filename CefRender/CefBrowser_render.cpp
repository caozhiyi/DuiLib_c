#include "CefBrowser_render.h"
#include "RenderApp.h"
#include "include/cef_runnable.h"
#include <Shlwapi.h>
#include <ShlObj.h>

namespace DuiLib
{
	bool CefBrowserBender::InitCef(HINSTANCE hInstance, CefString cache_path, CefString log_file)
	{
		//高DPI支持
		CefEnableHighDPISupport();

		void* sandbox_info = NULL;
		CefMainArgs main_args(hInstance);

		CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
		command_line->InitFromString(::GetCommandLineW());
		if (!PathIsDirectory(cache_path.ToWString().c_str()))
		{
			SHCreateDirectory(NULL, cache_path.ToWString().c_str());
		}
		if (!PathIsDirectory(log_file.ToWString().c_str()))
		{
			SHCreateDirectory(NULL, log_file.ToWString().c_str());
		}
		
		//不同进程创建不同的app
		CefRefPtr<CefApp> app;
		DuiLib::ProcessType process_type = ClientApp::GetProcessType(command_line);
		if (process_type == DuiLib::RendererProcess) {
			app = new CRenderApp();
		}
		
		//创建进程
		int exit_code = CefExecuteProcess(main_args, app.get(), sandbox_info);
		if (exit_code >= 0) {
			return false;
		}
		
		return true;
	}
}


