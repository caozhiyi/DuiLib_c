#pragma once
namespace DuiLib {
	class UILIB_API CGifUI : public CControlUI
	{
	public:
		CGifUI(void);
		~CGifUI(void);

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		void	DoInit() override;
		void	DoPaint(HDC hDC, const RECT& rcPaint) override;
		void	DoEvent(TEventUI& event) override;
		void	SetVisible(bool bVisible = true ) override;
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		void	SetBkImage(LPCTSTR pStrImage);
		LPCTSTR GetBkImage();

		void	SetAutoPlay(bool bIsAuto = true );
		bool	IsAutoPlay() const;
		void	SetAutoSize(bool bIsAuto = true );
		bool	IsAutoSize() const;
		//播放gif图片
		void	PlayGif();
		//暂停不重置
		void	PauseGif();
		//暂停重置
		void	StopGif();

	private:
		//初始化gif图片
		void	InitGifImage();
		//析构gif图片
		void	DeleteGif();
		//响应计时器
		void    OnTimer( UINT_PTR idEvent );
		//绘制gif每帧
		void	DrawFrame( HDC hDC );
		//从文件中读取gif文件到内存
		Gdiplus::Image*	LoadGifFromFile(LPCTSTR pstrGifPath);
		//从内存中获取到gif文件对象
		Gdiplus::Image* LoadGifFromMemory( LPVOID pBuf,size_t dwSize );
	private:
		Gdiplus::Image	*m_pGifImage;				// gif图片存储
		UINT			m_nFrameCount;				// gif图片总帧数
		UINT			m_nFramePosition;			// 当前放到第几帧
		Gdiplus::PropertyItem*	m_pPropertyItem;	// 帧与帧之间间隔时间

		CDuiString		m_sBkImage;					// 图片路径
		bool			m_bIsAutoPlay;				// 是否自动播放gif
		bool			m_bIsAutoSize;				// 是否自动根据图片设置大小
		bool			m_bIsPlaying;				// 标记图片是否在放映
		ULONG_PTR		m_gdiplusToken;				// 初始化gdi+函数库暂存句柄
		GdiplusStartupInput		m_gdiplusStartupInput;//gdi+函数库信息
	};
}

