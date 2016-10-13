#pragma once
#include "Control/UICefBrowser/CefBrowser/CefBrowserDll.h"
#include <share.h>
#include <boost/bind.hpp>
#include "../UIlib.h"

/***********************************************************
使用说明：
	1.在DuiLib::Create函数前调用InitCef初始化cef
	2.在MessageLoop()之后Quit
	3.继承实现ISetHandleSolt回调,函数中实现指定浏览器的回调实例
	4.继承实现OnAfterCreate函数中ResizeBrowser浏览器
***********************************************************/

namespace DuiLib {
	//类名称
	const TCHAR CCefBrowserUIClassName[] = L"CefBrowserUI";		
	//控件名称
	const TCHAR CCefBrowserUIItemInterface[] = L"CefBrowser";	
	class ISetHandleSolt {
	public:
		//浏览器回调实例指针设置函数
		virtual ILoadHandleSlot* GetILoadHandleSlot(std::wstring browser_name) = 0;
		virtual ILifeSpanHandleSlot* GetILifeSpanHandleSlot(std::wstring browser_name) = 0;
		virtual IDisplayHandleSolt* GetIDisplayHandleSolt(std::wstring browser_name) = 0;
	};

	class UILIB_API CCefBrowserUI : public DuiLib::CControlUI {
	public:
		CCefBrowserUI(HWND parent, ISetHandleSolt* set_handle = NULL);
		~CCefBrowserUI(void);

	//DuiLib回调函数
	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoInit();
		//父容器调用设置显示与否
		virtual void SetInternVisible(bool bVisible = true);

	//静态功能接口
	public:
		//初始化CEF
		static void InitCef(HINSTANCE hInstance, CefString cache_path = "", CefString log_file = "");
		//退出CEF
		static void Quit();
		//开启cef消息循环
		static void StartCefMessageLoop();

	//控件cef功能
	public:
		//加载url
		void LoadURL(std::wstring url);
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
		//获取当前Url
		CefString GetURL();
		//执行JS 参数为 一段完整的js代码 或者已经存在的js函数名加参数
		void ExectueJS(const CefString &jscode);
		//创建Cef浏览器窗口
		bool CreateCefWindow(CefString url, bool needfocus,
			std::unique_ptr<ILifeSpanHandleSlot> life_handle = NULL, 
			std::unique_ptr<ILoadHandleSlot> load_handle = NULL,
			std::unique_ptr<IDisplayHandleSolt> display_handle = NULL);
		//关闭浏览器
		void CloseCefWindow();
		//添加注册函数
		void SetFunction(const CefString &name, CustomFunction function);
	

	//UI层接口
	public:
		//重绘浏览器
		void ResizeBrowser(DuiLib::CDuiRect rect);
		//设置是否显示
		virtual void SetVisible(bool bVisible = true);

	private:
		//默认地址
		std::wstring url_;
		//cef功能接口对象
		CefBrowserDll browser_dll_;
		//设置load life 回调函数回调指针
		ISetHandleSolt* set_handle_;
		//父窗体句柄
		HWND parent_hwnd_;
		//浏览器名称
		std::wstring browser_name_;
		//是否创建完成
		bool is_created_;
	};
}

