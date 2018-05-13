#include "V8ExtensionHandler.h"
#include "include/cef_values.h"
namespace DuiLib
{
	CV8ExtensionHandler::CV8ExtensionHandler() {

	}

	CV8ExtensionHandler::~CV8ExtensionHandler() {
		if (handle_name_pipe_ != INVALID_HANDLE_VALUE) {
			DisconnectNamedPipe(handle_name_pipe_);
			CloseHandle(handle_name_pipe_);
		}
		function_name_.clear();
	}

	bool CV8ExtensionHandler::Execute(const CefString& name,
		CefRefPtr<CefV8Value> object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception) {

		CefRefPtr<CefProcessMessage> msg= CefProcessMessage::Create(name);
		CefRefPtr<CefListValue> args = msg->GetArgumentList();
		for (int i = 0; i <  arguments.size(); i++) {
			if (arguments[i]->IsBool()) {
				args->SetBool(i, arguments[i]->GetBoolValue());
			} else if (arguments[i]->IsInt()) {
				args->SetInt(i, arguments[i]->GetIntValue());

			} else if (arguments[i]->IsString()) {
				args->SetString(i, arguments[i]->GetStringValue());

			} else if (arguments[i]->IsDouble()) {
				args->SetDouble(i, arguments[i]->GetDoubleValue());
			}
		}
		browser_->SendProcessMessage(PID_BROWSER, msg);
		wchar_t buf[1024] = {0};
		DWORD  dwRead;
		ReadFile(handle_name_pipe_, buf, 1024, &dwRead, NULL);
		if (wcscmp(buf, L"DONE") == 0) {
			return true;
		} else {
			retval = CefV8Value::CreateString(buf);
			return true;
		}
	}

	void CV8ExtensionHandler::OnContextCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) {
			browser_ = browser;
			//Retrieve the context's window object.
			CefRefPtr<CefV8Value> object = context->GetGlobal();
			for (auto iter = function_name_.begin(); iter != function_name_.end(); ++iter) {
				//Create the "NativeLogin" function.
				CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction((*iter), this);
				//Add the "NativeLogin" function to the "window" object.
				object->SetValue((*iter), func, V8_PROPERTY_ATTRIBUTE_NONE);
			}
	}

	void CV8ExtensionHandler::SetFunction(const CefString& name) {
		function_name_.push_back(name);
	}

	void CV8ExtensionHandler::ConnectionNamePipe(const CefString& pipe_name) {
		if (WaitNamedPipe(pipe_name.ToWString().c_str(), NMPWAIT_WAIT_FOREVER)) {
			handle_name_pipe_ = CreateFile(pipe_name.ToWString().c_str(), GENERIC_READ | GENERIC_WRITE, 
				0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		}
	}

}
