# DuiLib_c
<p align="left">
    <a href="https://opensource.org/licenses/BSD-3-Clause"><img src="https://img.shields.io/badge/license-bsd-orange.svg" alt="Licenses"></a>
</p> 

## 简介
DuiLib_c封装了cef3作为Duilib的一个控件，以帮助Windows桌面软件以非常低的成本内嵌一个chromium浏览器。   
1.默认将渲染进程和主进程分离为两个独立的进程，以加快浏览器界面的渲染速度；    
2.支持C++和JaveScript之间相互的函数调用，只需在窗体初始化时将C++函数注册到V8内核中；   
3.支持动态的创建和销毁浏览器窗口，支持同客户端多浏览器窗口；    

## 使用

### cef初始化和析构   
主进程的入口处初始化cef：   
```c++
CCefBrowserUI::InitCef(instance);  
```
退出进程时调用析构cef资源。   
```c++
CCefBrowserUI::Quit();
```
   
### 创建控件
在窗体的xml文件中定义名为CefBrowser的控件：   
```c++
 <CefBrowser name="income_web"/>
```
支持属性可以在CCefBrowserUI::SetAttribute接口中更改。   
之后在Duilib的CreateControl回调中创建控件：      
```c++
if (_tcsicmp(pstrClass, CCefBrowserUIItemInterface) == 0) {
	return  new CCefBrowserUI(m_hWnd, this);
}
```
在OnAfterCreated回调接口中设置浏览器窗口的大小和位置。    
   
### C++和JaveScript交互    
C++调用JavaScript，直接调用控件的ExectueJS接口即可。    
JavaScript调用C++, 需要在Duilib的InitWindow接口中将C++可被调用的函数注册到V8内核中，传递的函数对象是std::function，所以可以设置任意函数作为JavaScript调用接口。    
需要注意的是，每个控件注册的函数并不共享，所以一个函数如何想被多个浏览器窗口调用，需要多次注册到每个浏览器控件中。  
   
### 浏览器基本操作和通知   
浏览器的导航，后退，前进等基本操作，直接调用[CCefBrowserUI](/test/UICefBrowser.h)控件的接口即可。   
浏览器状态改变的通知需要注册回调接口获取，首先窗体类需要继承ISetHandleSolt类，在继承的函数中通过返回this指针，来检测不同的回调事件，其中：  
ILifeSpanHandleSlot是关于浏览器窗体声明周期相关的回调;   
ILifeSpanFileDialogSolt是关于文件选择对话框的相关回调;    
ILoadHandleSlot是关于浏览器url加载状态相关的回调;   
   
### 其他
浏览器属性的修改，可以在ClientAppBrowser::OnBeforeCommandLineProcessing接口中设置启动参数修改，包括gpu进程加速，渲染进程个数等，其支持的命令参数和chrome大体类似。   
   
## 目录结构
bin: cef的语言资源和动态库。   
CefBrowser：cef控件主进程，和Duilib是同一个进程。   
CefRender：cef渲染进程，JaveScript实际运行在这个进程里。   
DuiLib: DuiLib库文件。   
include: cef导出头文件。   
libcef_dll：cef上的一层C++封装。    
test: 演示窗体和DuiLib控件实现。  
   
## 编译
git上携带的工程文件是VS2010的工程文件，可以由任何大于2010的VS编译。    
由于Duilib依赖boost解析xml文件，所以编译前需要配置下boost库目录。   

## 协议

DuiLib_c 使用BSD 3-Clause使用条款，详情请看[https://opensource.org/licenses/BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause)。  

如果觉得对你有帮助，请点个star吧(●'◡'●)   
Blog address：https://blog.csdn.net/u012778714/article/details/74178097   
