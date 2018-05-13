#pragma once
#include "../common/ClientApp.h"
namespace DuiLib {
	class ClientAppBrowser:
		public ClientApp,
		public CefBrowserProcessHandler {
	public:
		// CefApp methods:
		virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE;
		virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;

		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(ClientAppBrowser);
	};
}


