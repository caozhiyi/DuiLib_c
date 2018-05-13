#include "ClientApp.h"

namespace DuiLib {
	ClientApp::ClientApp() {

	}

	ClientApp::~ClientApp() {

	}

	ProcessType ClientApp::GetProcessType(CefRefPtr<CefCommandLine> command_line) {
		// The command-line flag won't be specified for the browser process.
		if (!command_line->HasSwitch("type"))
			return BrowserProcess;

		const std::string& process_type = command_line->GetSwitchValue("type");
		if (process_type == "renderer")
			return RendererProcess;

		return OtherProcess;
	}
}

