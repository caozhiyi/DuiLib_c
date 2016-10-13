#include "StdAfx.h"
#include "UIGif.h"

///////////////////////////////////////////////////////////////////////////////////////
DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;
typedef struct
{ 
	int index;                 // index of this file within the zip
	char name[MAX_PATH];       // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;
typedef struct
{ 
	int index;                 // index of this file within the zip
	TCHAR name[MAX_PATH];      // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRYW;
#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern HZIP OpenZipU(void *z,unsigned int len,DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);
#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif
extern ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern ZRESULT FindZipItemA(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern ZRESULT FindZipItemW(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);
///////////////////////////////////////////////////////////////////////////////////////

namespace DuiLib{

	CGifUI::CGifUI(void) :
	m_pGifImage(NULL),
	m_pPropertyItem(NULL),
	m_nFrameCount(0),	
	m_nFramePosition(0),	
	m_bIsAutoPlay(true),
	m_bIsAutoSize(false),
	m_gdiplusToken(0),
	m_bIsPlaying(false) {
		//作用是初始化GDI+函数库
		GdiplusStartup( &m_gdiplusToken, &m_gdiplusStartupInput, NULL);
	}

	CGifUI::~CGifUI(void) {
		DeleteGif();
		m_pManager->KillTimer(this, DUI_TIMERID_GIF);
		//关闭Gdi+函数库
		GdiplusShutdown(m_gdiplusToken);
	}

	LPCTSTR	CGifUI::GetClass() const {
		return L"GifUI";
	}

	LPVOID CGifUI::GetInterface(LPCTSTR pstrName) {
		if (_tcscmp(pstrName, DUI_CTR_GIF) == 0) {
			return static_cast<CGifUI*>(this);
		}
		return CControlUI::GetInterface(pstrName);
	}

	void CGifUI::DoInit() {
		InitGifImage();
	}

	void CGifUI::DoPaint(HDC hDC, const RECT& rcPaint) {
		if (!IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) {
			return;
		}
		if (m_pGifImage == NULL) {
			InitGifImage();
		}
		DrawFrame(hDC);
	}

	void CGifUI::DoEvent(TEventUI& event) {
		if (event.Type == UIEVENT_TIMER) {
			OnTimer((UINT_PTR)event.wParam);
		}	
	}

	void CGifUI::SetVisible(bool bVisible) {
		CControlUI::SetVisible(bVisible);
		if (bVisible) {
			PlayGif();
		
		} else {
			StopGif();
		}
	}

	void CGifUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)  {
		if (_tcscmp(pstrName, L"image") == 0) {
			SetBkImage(pstrValue);
		
		} else if (_tcscmp(pstrName, L"autosize") == 0) {
			SetAutoSize(_tcscmp(pstrValue, L"true") == 0);

		} else if (_tcscmp(pstrName, L"autoplay") == 0) {
			SetAutoPlay(_tcscmp(pstrValue, L"true") == 0);

		} else {
			CControlUI::SetAttribute(pstrName, pstrValue);
		}
	}

	void CGifUI::SetBkImage(LPCTSTR pStrImage) {
		if (m_sBkImage == pStrImage || pStrImage == NULL) {
			return;
		}
		m_sBkImage = pStrImage;
		StopGif();
		DeleteGif();
		Invalidate();
	}

	LPCTSTR CGifUI::GetBkImage() {
		return m_sBkImage.GetData();
	}

	void CGifUI::SetAutoPlay(bool bIsAuto) {
		m_bIsAutoPlay = bIsAuto;
	}

	bool CGifUI::IsAutoPlay() const {
		return m_bIsAutoPlay;
	}

	void CGifUI::SetAutoSize(bool bIsAuto) {
		m_bIsAutoSize = bIsAuto;
	}
	 
	bool CGifUI::IsAutoSize() const {
		return m_bIsAutoSize;
	}

	void CGifUI::PlayGif() {
		if (m_bIsPlaying || m_pGifImage == NULL) {
			return;
		}

		//获取gif帧之间的时间间隔
		long lPause = ((long*)m_pPropertyItem->value)[m_nFramePosition] * 10;
		if (lPause == 0) {
			lPause = 100;
		}
		m_pManager->SetTimer(this, DUI_TIMERID_GIF, lPause);
		m_bIsPlaying = true;
	}
	 
	void CGifUI::PauseGif() {
		if (!m_bIsPlaying || m_pGifImage == NULL) {
			return;
		}
		m_pManager->KillTimer(this, DUI_TIMERID_GIF);
		this->Invalidate();
		m_bIsPlaying = false;
	}

	void CGifUI::StopGif() {
		if (!m_bIsPlaying) {
			return;
		}

		m_pManager->KillTimer(this, DUI_TIMERID_GIF);
		m_nFrameCount = 0; 
		this->Invalidate();
		m_bIsPlaying = false;
	}

	void CGifUI::InitGifImage() {
		m_pGifImage = LoadGifFromFile(GetBkImage());
		if (m_pGifImage == NULL) {
			return;
		}
		UINT nCount = 0;
		//返回层 每层可以包含很多帧
		nCount = m_pGifImage->GetFrameDimensionsCount();
		GUID* pDimensionIDs = new GUID[nCount];
		//获取层的GUID列表
		m_pGifImage->GetFrameDimensionsList(pDimensionIDs, nCount);
		//获取帧数
		m_nFrameCount	= m_pGifImage->GetFrameCount(&pDimensionIDs[0]);
		//获取属性结构体大小
		int nSize		= m_pGifImage->GetPropertyItemSize(PropertyTagFrameDelay);
		m_pPropertyItem = (Gdiplus::PropertyItem*)malloc(nSize);
		//获取属性信息
		m_pGifImage->GetPropertyItem(PropertyTagFrameDelay, nSize, m_pPropertyItem);
		delete pDimensionIDs;
		pDimensionIDs = NULL;
		
		if (IsAutoSize()) 
		{
			SetFixedWidth(m_pGifImage->GetWidth());
			SetFixedHeight(m_pGifImage->GetHeight());
		}
		
		if (IsAutoPlay()) 
		{
			PlayGif();
		} 
	}

	void CGifUI::DeleteGif() {
		if (m_pGifImage != NULL) {
			delete m_pGifImage;
			m_pGifImage = NULL;
		}

		if (m_pPropertyItem != NULL) {
			free(m_pPropertyItem);
			m_pPropertyItem = NULL;
		}

		m_nFrameCount = 0;
		m_nFramePosition = 0;
		
	}

	void CGifUI::OnTimer( UINT_PTR idEvent ) {
		if(idEvent != DUI_TIMERID_GIF) {
			return;
		}
		m_pManager ->KillTimer(this, DUI_TIMERID_GIF);
		this->Invalidate();

		m_nFramePosition = (++m_nFramePosition) % m_nFrameCount;

		long lPause = ((long*)m_pPropertyItem->value)[m_nFramePosition] * 10;
		if (lPause == 0) {
			lPause = 100;
		}
		m_pManager->SetTimer(this, DUI_TIMERID_GIF, lPause);
	}

	void CGifUI::DrawFrame( HDC hDC ) {
		if (hDC == NULL || m_pGifImage == NULL) {
			return;
		}
		GUID pageGuid = Gdiplus::FrameDimensionTime;
		Gdiplus::Graphics graphics(hDC);
		//绘制当前帧
		graphics.DrawImage(m_pGifImage, m_rcItem.left, m_rcItem.top, m_rcItem.right-m_rcItem.left, m_rcItem.bottom-m_rcItem.top );
		//选择当前活动帧
		m_pGifImage->SelectActiveFrame(&pageGuid, m_nFramePosition);
	}

	Gdiplus::Image*	CGifUI::LoadGifFromFile(LPCTSTR pstrGifPath) {
		LPBYTE pData = NULL;
		DWORD dwSize = 0;
		do 
		{
			CDuiString sFile = CPaintManagerUI::GetResourcePath();
			if (CPaintManagerUI::GetResourceZip().IsEmpty()) {
				sFile += pstrGifPath;
				HANDLE hFile = CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
					FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE) {
					break;
				}
				dwSize = GetFileSize(hFile, NULL);
				if (dwSize == 0) {
					break;
				}

				DWORD dwRead = 0;
				pData = new BYTE[dwSize];
				memset(pData, 0, dwSize);
				ReadFile(hFile, pData, dwSize, &dwRead, NULL);
				CloseHandle(hFile);

				if (dwRead != dwSize) {
					delete[] pData;
					pData = NULL;
					break;
				}
			
			} else {
				sFile += CPaintManagerUI::GetResourceZip();
				HZIP hz = NULL;
				if (CPaintManagerUI::IsCachedResourceZip()) {
					hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
				
				} else {
					hz = OpenZip((void*)sFile.GetData(), 0, 2);
				}
				if (hz == NULL) {
					break;
				}
				ZIPENTRY ze;
				int i;
				if (FindZipItem(hz, pstrGifPath, true, &i, &ze) != 0) {
					break;
				}
				dwSize = ze.unc_size;
				if (dwSize == 0) {
					break;
				}
				pData = new BYTE[dwSize];
				int res = UnzipItem(hz, i, pData, dwSize, 3);
				if( res != 0x00000000 && res != 0x00000600) {
					delete[] pData;
					pData = NULL;
					if(!CPaintManagerUI::IsCachedResourceZip())
						CloseZip(hz);
					break;
				}
				
				if (!CPaintManagerUI::IsCachedResourceZip()) {
					CloseZip(hz);
				}
			}
		} while (0);

		while (!pData)
		{
			HANDLE hFile = CreateFile(pstrGifPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
				FILE_ATTRIBUTE_NORMAL, NULL);
			if( hFile == INVALID_HANDLE_VALUE ) break;
			dwSize = ::GetFileSize(hFile, NULL);
			if( dwSize == 0 ) break;

			DWORD dwRead = 0;
			pData = new BYTE[ dwSize ];
			::ReadFile( hFile, pData, dwSize, &dwRead, NULL );
			::CloseHandle( hFile );

			if( dwRead != dwSize ) {
				delete[] pData;
				pData = NULL;
			}
			break;
		}

		if (!pData) 
		{
			return NULL;
		} 

		Gdiplus::Image* pImage = LoadGifFromMemory(pData, dwSize);
		int n = GetLastError();
		delete pData;
		return pImage;
	} 
		 

	Gdiplus::Image* CGifUI::LoadGifFromMemory( LPVOID pBuf,size_t dwSize ) {
		HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, dwSize);
		BYTE* pMem = (BYTE*)GlobalLock(hMem);

		memcpy(pMem, pBuf, dwSize); 
		IStream* pStm = NULL; 
		CreateStreamOnHGlobal(hMem, TRUE, &pStm); 
		Gdiplus::Image * pImg = Gdiplus::Image::FromStream(pStm);
		if (!pImg || pImg->GetLastStatus() != Gdiplus::Ok) {
			pStm->Release(); 
			GlobalUnlock(hMem);
			return 0;
		}
		return pImg;	
	}
}