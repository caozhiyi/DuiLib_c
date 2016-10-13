#pragma once
#include "include/cef_v8.h"
#include <map>
#include "RenderAndBrowserApp.h"
#include "CefBrowserDll.h"
namespace DuiLib {
	class CV8ExtensionHandler : public CefV8Handler,
								public IOnContextCreatedSolt {
	public:
		CV8ExtensionHandler();
		~CV8ExtensionHandler();

		//js回调函数
		virtual bool Execute(const CefString& name,
			CefRefPtr<CefV8Value> object,
			const CefV8ValueList& arguments,
			CefRefPtr<CefV8Value>& retval,
			CefString& exception) OVERRIDE;

		//OnContextCreated实际执行函数
		virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context);
		
		//注册拓展函数
		void SetFunction(const CefString &name, CustomFunction function);

	private:
		//V8js回调接口实例
		//CefRefPtr<IExecuteSolt> execute_solt_;
		std::map<CefString, CustomFunction> function_map_;
		IMPLEMENT_REFCOUNTING(CV8ExtensionHandler);
	};
}

