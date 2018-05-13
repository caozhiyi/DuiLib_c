#pragma once
#include "browser/BrowserClient.h"
#include "include/cef_base.h"
#include <vector>
#include <map>
#include "include/cef_load_handler.h"
#include <Windows.h>
namespace DuiLib
{

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
			const CefString& url) {}
		//标题改变
		virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
			const CefString& title) {}
		//页面图标改变
		virtual void OnFaviconURLChange(CefRefPtr<CefBrowser> browser,
			const std::vector<CefString>& icon_urls) {}
		//页面全屏时调用
		virtual void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
			bool fullscreen) {}
		//浏览器显示工具提示
		virtual void OnTooltip(CefRefPtr<CefBrowser> browser,
			CefString& text) {}
		//接收到状态信息时
		virtual void OnStatusMessage(CefRefPtr<CefBrowser> browser,
			const CefString& value) {}
		//调用显示控制台信息
		virtual void OnConsoleMessage(CefRefPtr<CefBrowser> browser,
			const CefString& message,
			const CefString& source,
			int line) {}
	};

	//CefLoadHandler回调类
	class ILoadHandleSlot {
	public:
		//加载出错
		virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefLoadHandler::ErrorCode errorCode,
			const CefString& errorText,
			const CefString& failedUrl) {}

		//加载状态改变
		virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
			bool isLoading,
			bool canGoBack,
			bool canGoForward) {}

		//加载开始
		virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame) {}

		//加载完成
		virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			int httpStatusCode) {}
	};

	//CefLifeSpanHandler回调类
	class ILifeSpanHandleSlot {
	public:
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) {}
		virtual void DoClose(CefRefPtr<CefBrowser> browser) {}
		virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) {}
	};

	//JS文件选择对话框生命周期回调类
	class ILifeSpanFileDialogSolt {
	public:
		virtual void OnBeforeFileDialog(CefRefPtr<CefBrowser> browser) {}
		virtual void OnAfterFileDialog(CefRefPtr<CefBrowser> browser) {}
	};

	class CEF_API CefBrowserDll{
	public:
		CefBrowserDll();
		~CefBrowserDll();

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

		//析构自身
		void Release();
		//cef入口函数
		void CreateCefWindow(HWND hWnd, const CefString& url, 
			ILifeSpanHandleSlot* life_handle = NULL, 
			ILoadHandleSlot* load_handle = NULL,
			IDisplayHandleSolt* display_handle = NULL,
			ILifeSpanFileDialogSolt* filedialog_handle = NULL);
		//关闭一个浏览器窗口
		void CloseCefWindow();
		//添加注册函数
		void SetFunction(const CefString &name, CustomFunction function);

	//静态初始化功能接口
	public:
		//初始化cef
		static bool InitCef(HINSTANCE hInstance, const CefString& cache_path = "", const CefString& log_file = "");
		//关闭消息循环
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
	};
}


