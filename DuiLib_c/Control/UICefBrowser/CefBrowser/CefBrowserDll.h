#pragma once
#include "BrowserClient.h"
#include "RenderAndBrowserApp.h"
#include <vector>
#include <map>
#include "include/cef_load_handler.h"

namespace DuiLib {
#define EXPORT
#ifdef EXPORT
#define CEF_API __declspec(dllexport)  
#else
#define CEF_API __declspec(dllimport)  
#endif 	
	//IDisplayHandleSolt 回调类
	class IDisplayHandleSolt {
	public:
		//地址改变
		virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& url) = 0;
		//标题改变
		virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
			const CefString& title) = 0;
		//页面图标改变
		virtual void OnFaviconURLChange(CefRefPtr<CefBrowser> browser,
			const std::vector<CefString>& icon_urls) = 0;
		//页面全屏时调用
		virtual void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
			bool fullscreen) = 0;
		//浏览器显示工具提示
		virtual bool OnTooltip(CefRefPtr<CefBrowser> browser,
			CefString& text) = 0;
		//接收到状态信息时
		virtual void OnStatusMessage(CefRefPtr<CefBrowser> browser,
			const CefString& value) = 0;
		//调用显示控制台信息
		virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
			const CefString& message,
			const CefString& source,
			int line) = 0;
	};

	//CefLoadHandler回调类
	class ILoadHandleSlot {
	public:
		//加载出错
		virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefLoadHandler::ErrorCode errorCode,
			const CefString& errorText,
			const CefString& failedUrl) = 0;

		//加载状态改变
		virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
			bool isLoading,
			bool canGoBack,
			bool canGoForward) = 0;

		//加载开始
		virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame) = 0;

		//加载完成
		virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			int httpStatusCode) = 0;
	};

	//CefLifeSpanHandler回调类
	class ILifeSpanHandleSlot {
	public:
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) = 0;
		virtual bool DoClose(CefRefPtr<CefBrowser> browser) = 0;
		//virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) = 0;

	};

	class CEF_API CefBrowserDll : public IOnAfterCreateSolt {
	public:
		CefBrowserDll();
		~CefBrowserDll(void);

		//CefBrowser
		//是否可以向前导航
		bool CanGoForward();
		//向前导航
		void GoForward();
		//重新加载
		void Reload();
		//忽略缓存加载
		void ReloadIgnoreCache();
		//停止加载
		void StopLoad();
		//可以向后导航
		bool CanGoBack();
		//向后导航
		void GoBack();
		//获取浏览器句柄
		CefWindowHandle GetHwnd();
		//设置缩放比例
		void SetZoomLevel(double zoomLevel);
		//CefBrowserHost
		//CefFrameg
		//加载url
		void LoadURL(const CefString& url);
		//获取当前Url
		CefString GetURL();
		//获取浏览器名称
		CefString GetName(CefRefPtr<CefBrowser> browser);
		//执行JS 参数为 一段完整的js代码 或者已经存在的js函数名加参数
		void ExectueJS(const CefString &jscode);
		//是否创建完成
		bool IsCreated() const;
		//cef入口函数
		bool CreateCefWindow(HWND hWnd, CefString url, 
			std::unique_ptr<ILifeSpanHandleSlot> life_handle = NULL, 
			std::unique_ptr<ILoadHandleSlot> load_handle = NULL,
			std::unique_ptr<IDisplayHandleSolt> display_handle = NULL);
		//关闭一个浏览器窗口
		void CloseCefWindow();
		//添加注册函数
		void SetFunction(const CefString &name, CustomFunction function);
		//创建浏览器回调接口
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser);
		//设置是否需要焦点
		void SetNeedFocus(bool dynamic);
	
	//静态初始化功能接口
	public:
		//初始化cef
		static bool InitCef(HINSTANCE hInstance, CefString cache_path = "", CefString log_file = "");
		//退出Cef
		static void Quit();
		//运行消息循环
		static void Run();

	//备用接口
	private:
		//设置焦点
		void SetFocus(bool enable);
		//获取缩放比例
		double GetZoomLevel();
		
	private:
		//浏览器对象
		CefRefPtr<CBrowserClient> client_browser_;	
		//浏览器id
		int browser_id_;
		//RenderApp实例
		static CefRefPtr<CRenderAndBrowserApp> render_app_;
		//浏览器句柄
		HWND browser_hwnd_;
		//是否需要焦点
		bool need_focus_;
	};
}


