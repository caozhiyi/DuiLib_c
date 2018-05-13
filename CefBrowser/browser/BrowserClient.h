#pragma once
#include "include/cef_client.h"
#include "include/cef_load_handler.h"
#include "include/cef_base.h"
#include "include/base/cef_lock.h"
#include <map>
#include <functional>
#include <share.h>
#include "include/cef_v8.h"

namespace DuiLib
{
	typedef std::function<CefRefPtr<CefValue> (CefRefPtr<CefListValue>)> CustomFunction;
	class ILoadHandleSlot;
	class ILifeSpanHandleSlot;
	class IDisplayHandleSolt;
	class ILifeSpanFileDialogSolt;

	class CBrowserClient : 
		public CefClient,
		public CefContextMenuHandler,
		public CefDragHandler,
		public CefRequestHandler,
		public CefDisplayHandler,
		public CefDialogHandler,
		public CefLifeSpanHandler,
		public CefKeyboardHandler,
		public CefLoadHandler {
	public:
		CBrowserClient();
		~CBrowserClient(void);

		virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE;
		virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler();
		virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE;
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;
		virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;
		virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE;
		virtual CefRefPtr<CefDialogHandler> GetDialogHandler() OVERRIDE;
		virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE;

		//屏蔽按键响应
		virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
			const CefKeyEvent& event,
			CefEventHandle os_event,
			bool* is_keyboard_shortcut);

		//自定义文件选择对话框
		virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser,
			FileDialogMode mode,
			const CefString& title,
			const CefString& default_file_path,
			const std::vector<CefString>& accept_filters,
			int selected_accept_filter,
			CefRefPtr<CefFileDialogCallback> callback);

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

		//右键弹出菜单
		virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefContextMenuParams> params,
			CefRefPtr<CefMenuModel> model);

		//导航前的过滤
		virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			bool is_redirect); 

		//阻止导航拖拽文件
		virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefDragData> dragData,
			DragOperationsMask mask);

		//接收消息
		virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message);

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

	public:
		//打开文件选择对话框
		std::wstring OpenPictureFile();
		//cef入口函数
		void CreateCefWindow(HWND hWnd, const CefString& url, 
			ILifeSpanHandleSlot* life_handle = NULL, 
			ILoadHandleSlot* load_handle = NULL,
			IDisplayHandleSolt* display_handle = NULL,
			ILifeSpanFileDialogSolt* filedialog_handle = NULL);
		//获取浏览器对象
		CefRefPtr<CefBrowser> GetBrowser();
		//是否创建完成
		bool IsCreated() const;
		//添加函数
		void SetFunction(const CefString &name, CustomFunction function);
		//关闭浏览器
		void CloseBrowser();
		//loaded
		bool browser_loaded_;

	private:
		//存储浏览器窗口
		CefRefPtr<CefBrowser> browser_;
		//锁
		base::Lock lock_;
		//命名管道句柄
		HANDLE	handle_name_pipe_;
		//CefLoadHandler回调实例指针
		ILoadHandleSlot* load_handle_;
		//CefLifeSpanHandler回调实例指针
		ILifeSpanHandleSlot* life_handle_;
		//CefDisplayHandler回调实例指针
		IDisplayHandleSolt* display_handle_;
		//ILifeSpanFileDialogSolt回调指针实例
		ILifeSpanFileDialogSolt* filedialog_handle_;
		//函数map
		std::map<CefString, CustomFunction> function_map_;
		//是否创建完成
		bool is_created_;
		//将类添加入自动引用计数
		IMPLEMENT_REFCOUNTING(CBrowserClient);
	};
}