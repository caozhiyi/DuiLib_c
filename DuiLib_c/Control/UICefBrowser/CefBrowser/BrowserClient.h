#pragma once
#include "include/cef_client.h"
#include "include/base/cef_lock.h"
#include "V8ExtensionHandler.h"

namespace DuiLib {
	class ILoadHandleSlot;
	class ILifeSpanHandleSlot;
	class IDisplayHandleSolt;

	class IOnAfterCreateSolt : public CefBase {
	public:
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) = 0;
	private:
		IMPLEMENT_REFCOUNTING(IOnAfterCreateSolt);
	};

	class CBrowserClient : 
		public CefClient,
		public CefContextMenuHandler,
		public CefRequestHandler,
		public CefDisplayHandler,
		public CefLifeSpanHandler,
		public CefLoadHandler {
	public:

		CBrowserClient(std::unique_ptr<ILifeSpanHandleSlot> life_handle = NULL,
			std::unique_ptr<ILoadHandleSlot> load_handle = NULL,
			std::unique_ptr<IDisplayHandleSolt> display_handle = NULL);

		~CBrowserClient(void);

		virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE;
		virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler();
		virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE;
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;
		virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;

		//创建新的弹出窗体的调用
		virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& target_url,
			const CefString& target_frame_name,
			CefLifeSpanHandler::WindowOpenDisposition target_disposition,
			bool user_gesture,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings,
			bool* no_javascript_access);



		virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefContextMenuParams> params,
			CefRefPtr<CefMenuModel> model);

		// CefLifeSpanHandler methods:
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
		virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

		// CefLoadHandler methods:
		//加载出错
		virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			ErrorCode errorCode,
			const CefString& errorText,
			const CefString& failedUrl);

		//加载状态改变
		virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
			bool isLoading,
			bool canGoBack,
			bool canGoForward);

		//加载开始
		virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame);

		//加载完成
		virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			int httpStatusCode);

		// CefDisplayHandler methods:
		//地址改变
		virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& url);
		//标题改变
		virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
			const CefString& title);
		//页面图标改变
		virtual void OnFaviconURLChange(CefRefPtr<CefBrowser> browser,
			const std::vector<CefString>& icon_urls);
		//页面全屏时调用
		virtual void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
			bool fullscreen);
		//浏览器显示工具提示
		virtual bool OnTooltip(CefRefPtr<CefBrowser> browser,
			CefString& text);
		//接收到状态信息时
		virtual void OnStatusMessage(CefRefPtr<CefBrowser> browser,
			const CefString& value);
		//调用显示控制台信息
		virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
			const CefString& message,
			const CefString& source,
			int line);

		virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message);

	public:
		//添加注册函数
		void SetFunction(const CefString &name, CustomFunction function);
		//设置创建浏览器回调接口实例
		void SetIOnAfterCreateSolt(IOnAfterCreateSolt* browser_crate_solt);

		//browser是否关闭
		bool IsClosing() const;
		//是否创建完成
		bool IsCreated() const;

	public:
		//存储浏览器窗口
		CefRefPtr<CefBrowser> browser_;
		//锁
		base::Lock lock_;
		//V8拓展类
		CefRefPtr<CV8ExtensionHandler> extension_;
	private:
		//CefLoadHandler回调实例指针
		std::unique_ptr<ILoadHandleSlot> load_handle_;
		//CefLifeSpanHandler回调实例指针
		std::unique_ptr<ILifeSpanHandleSlot> life_handle_;
		//CefDisplayHandler回调实例指针
		std::unique_ptr<IDisplayHandleSolt> display_handle_;
		//创建浏览器回调接口
		IOnAfterCreateSolt* browser_create_solt_;

		//是否关闭标识
		bool is_closing_; 
		//是否创建完成
		bool is_created_;
		//将类添加入自动引用计数
		IMPLEMENT_REFCOUNTING(CBrowserClient);
	};
}

