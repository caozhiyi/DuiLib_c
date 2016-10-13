#include "V8ExtensionHandler.h"

namespace DuiLib
{

	CV8ExtensionHandler::CV8ExtensionHandler() {

	}

	CV8ExtensionHandler::~CV8ExtensionHandler() {
		function_map_.clear();

	}

	bool CV8ExtensionHandler::Execute(const CefString& name,
		CefRefPtr<CefV8Value> object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception) {
		auto iter = function_map_.find(name);
		if (iter != function_map_.end()) {
			retval = iter->second(arguments);
			return true;

		} else {
			return false;

		}
	}

	void CV8ExtensionHandler::OnContextCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) {

		CefRefPtr<CefV8Value> object = context->GetGlobal();

		for (auto iter = function_map_.begin(); iter != function_map_.end(); ++iter) {
			//Create the "NativeLogin" function.
			CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(iter->first, this);
			//Add the "NativeLogin" function to the "window" object.
			object->SetValue(iter->first, func, V8_PROPERTY_ATTRIBUTE_NONE);

		}
	}

	void CV8ExtensionHandler::SetFunction(const CefString &name, CustomFunction function) {
		function_map_[name] = function;

	}
}
