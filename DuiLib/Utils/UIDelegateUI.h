#pragma once

namespace DuiLib 
{
	typedef struct tagTEventUI TEventUI;
	typedef struct tagTNotifyUI TNotifyUI;

	typedef bool (*FunVoid)(void* pParam,LPARAM lParam,WPARAM wParam);
	typedef bool (*FunTEvent)(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
	typedef bool (*FunTNotify)(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam);

	class CDelegateBaseUI;
	
	class UILIB_API IDelegateUI
	{
	public:
		typedef union {
			FunVoid pFunVoid;
			FunTEvent pFunTEvent;
			FunTNotify pFunTNotify;
		}FnType;

	public:
		virtual FunVoid GetFunVoid() const = 0;
		virtual FunTEvent GetFunTEvent() const = 0;
		virtual FunTNotify GetFunTNotify() const = 0;
		virtual LPARAM GetLParam() const = 0;
		virtual WPARAM GetWParam() const = 0;
	public:
		virtual CDelegateBaseUI* Copy() const = 0;
		virtual bool Invoke(void* pParam,LPARAM lParam = NULL,WPARAM wParam = NULL) = 0;
		virtual bool Invoke(TEventUI* pTEventUI,LPARAM lParam = NULL,WPARAM wParam = NULL) = 0;
		virtual bool Invoke(TNotifyUI* pTNotifyUI,LPARAM lParam = NULL,WPARAM wParam = NULL) = 0;
	};

	class UILIB_API CDelegateBaseUI : public IDelegateUI
	{
	public:
		CDelegateBaseUI(void* pObject, FunVoid pFn,LPARAM lParam = NULL,WPARAM wParam = NULL);
		CDelegateBaseUI(void* pObject, FunTEvent pFn,UINT _iEventType,LPARAM lParam = NULL,WPARAM wParam = NULL);
		CDelegateBaseUI(void* pObject, FunTNotify pFn,LPCTSTR _sNotifyTypeName,LPARAM lParam = NULL,WPARAM wParam = NULL);
		CDelegateBaseUI(const CDelegateBaseUI& rhs);

		virtual ~CDelegateBaseUI();
		FunVoid GetFunVoid() const{return m_unionFnType.pFunVoid;};
		FunTEvent GetFunTEvent() const{return m_unionFnType.pFunTEvent;};
		FunTNotify GetFunTNotify() const{return m_unionFnType.pFunTNotify;};
		LPARAM GetLParam() const { return m_lParam;};
		WPARAM GetWParam() const { return m_wParam;};
		bool Equals(const CDelegateBaseUI& rhs) const;
		bool operator() (void* param,LPARAM lParam = NULL,WPARAM wParam = NULL){return Invoke(param,lParam,wParam);};
		bool operator() (TEventUI* pTEventUI,UINT _iEventType,LPARAM lParam = NULL,WPARAM wParam = NULL){return Invoke(pTEventUI,lParam,wParam);};
		bool operator() (TNotifyUI* pTNotifyUI,LPCTSTR _sNotifyTypeName,LPARAM lParam = NULL,WPARAM wParam = NULL){return Invoke(pTNotifyUI,lParam,wParam);};

		void* GetObj() const {return m_pObject;};
	protected:
		void SetEventType(UINT _iEventType){m_iEventType = _iEventType;};
		void SetNotifyTypeName(CDuiString& _sNotifyTypeName){m_sNotifyTypeName = _sNotifyTypeName.GetData();};
		UINT GetEventType(){return m_iEventType;};
		CDuiString GetNotifyTypeName(){return m_sNotifyTypeName.GetData();};

	private:
		void* m_pObject;
		FnType m_unionFnType;
		LPARAM m_lParam;
		WPARAM m_wParam;
	protected:
		UINT m_iEventType;
		CDuiString m_sNotifyTypeName;
	};

	class UILIB_API CDelegateStaticUI: public CDelegateBaseUI
	{
	public:
		CDelegateStaticUI(FunVoid pFunVoid,LPARAM lParam = NULL,WPARAM wParam = NULL) : CDelegateBaseUI(NULL, pFunVoid,lParam,wParam) { } 
		CDelegateStaticUI(FunTEvent pFunTEvent,UINT _iEventType = 0,LPARAM lParam = NULL,WPARAM wParam = NULL) : CDelegateBaseUI(NULL, pFunTEvent,_iEventType,lParam,wParam) { } 
		CDelegateStaticUI(FunTNotify pFunTNotify,LPCTSTR _sNotifyTypeName = NULL,LPARAM lParam = NULL,WPARAM wParam = NULL) : CDelegateBaseUI(NULL, pFunTNotify,_sNotifyTypeName,lParam,wParam) { } 
		CDelegateStaticUI(const CDelegateStaticUI& rhs) : CDelegateBaseUI(rhs) { } 
		virtual CDelegateBaseUI* Copy() const { return new CDelegateStaticUI(*this); }

	protected:
		virtual bool Invoke(void* param,LPARAM lParam = NULL,WPARAM wParam = NULL)
		{
			FunVoid pFunVoid = GetFunVoid();
			return pFunVoid(param,lParam,wParam); 
		}

		virtual bool Invoke(TEventUI* pTEventUI,LPARAM lParam = NULL,WPARAM wParam = NULL)
		{
			FunTEvent pFunTEvent = GetFunTEvent();
			return !pFunTEvent(pTEventUI,lParam,wParam); 
		};

		virtual bool Invoke(TNotifyUI* pTNotifyUI,LPARAM lParam = NULL,WPARAM wParam = NULL)
		{
			FunTNotify pFunTNotify = GetFunTNotify();
			return pFunTNotify(pTNotifyUI,lParam,wParam);
		};
	};

	template <class O, class T,class P = LPARAM>
	class UILIB_API CDelegateUI : public CDelegateBaseUI
	{
		typedef bool (T::*CMFunVoid)(void* pParam,P lParam,WPARAM wParam);
		typedef bool (T::*CMFunTEvent)(TEventUI* pTEventUI,P lParam,WPARAM wParam);
		typedef bool (T::*CMFunTNotify)(TNotifyUI* pTNotifyUI,P lParam,WPARAM wParam);
	public:
		CDelegateUI(O* pObj, CMFunVoid pCMFunVoid,P lParam = NULL,WPARAM wParam = NULL) : CDelegateBaseUI(pObj, *(FunVoid*)&pCMFunVoid,(LPARAM)lParam,wParam), m_pCMFunVoid(pCMFunVoid),m_pCMFunTEvent(NULL),m_pCMFunTNotify(NULL){}
		CDelegateUI(O* pObj, CMFunTEvent pCMFunTEvent,UINT _iEventType = 0,P lParam = NULL,WPARAM wParam = NULL) : CDelegateBaseUI(pObj, *(FunTEvent*)&pCMFunTEvent,_iEventType,(LPARAM)lParam,wParam), m_pCMFunVoid(NULL),m_pCMFunTEvent(pCMFunTEvent),m_pCMFunTNotify(NULL) { }
		CDelegateUI(O* pObj, CMFunTNotify pCMFunTNotify,LPCTSTR _sNotifyTypeName = NULL,P lParam = NULL,WPARAM wParam = NULL) : CDelegateBaseUI(pObj, *(FunTNotify*)&pCMFunTNotify,_sNotifyTypeName,(LPARAM)lParam,wParam), m_pCMFunVoid(NULL),m_pCMFunTEvent(NULL),m_pCMFunTNotify(pCMFunTNotify) { }
		CDelegateUI(const CDelegateUI& rhs) : CDelegateBaseUI(rhs) { m_pCMFunVoid = rhs.m_pCMFunVoid;m_pCMFunTEvent = rhs.m_pCMFunTEvent;m_pCMFunTNotify = rhs.m_pCMFunTNotify;} 
		virtual CDelegateBaseUI* Copy() const { return new CDelegateUI(*this); }

	protected:
		virtual bool Invoke(void* param,LPARAM lParam = NULL,WPARAM wParam = NULL)
		{
			O* pObject = (O*) GetObj();
			if(pObject && m_pCMFunVoid)
				return (pObject->*m_pCMFunVoid)(param,(P)GetLParam(),GetWParam());
			else if(pObject && m_pCMFunTEvent)
				return Invoke((TEventUI*)param,GetLParam(),GetWParam());
			else if(pObject && m_pCMFunTNotify)
				return Invoke((TNotifyUI*)param,GetLParam(),GetWParam());

			return true;
		} 

		virtual bool Invoke(TEventUI* pTEventUI,LPARAM lParam = NULL,WPARAM wParam = NULL)
		{
			O* pObject = (O*) GetObj();
			if(pObject && pTEventUI && GetEventType() == 0)
				return (pObject->*m_pCMFunTEvent)(pTEventUI,(P)GetLParam(),GetWParam()); 
			else if(pObject && pTEventUI && pTEventUI->Type == GetEventType())
				return (pObject->*m_pCMFunTEvent)(pTEventUI,(P)GetLParam(),GetWParam());

			return true;
		};

		virtual bool Invoke(TNotifyUI* pTNotifyUI,LPARAM lParam = NULL,WPARAM wParam = NULL)
		{
			O* pObject = (O*) GetObj();
			if(pObject && GetNotifyTypeName().IsEmpty())
				return (pObject->*m_pCMFunTNotify)(pTNotifyUI,(P)GetLParam(),GetWParam());
			else if(pObject && pTNotifyUI && pTNotifyUI->sType == GetNotifyTypeName())
				return (pObject->*m_pCMFunTNotify)(pTNotifyUI,(P)GetLParam(),GetWParam());

			return true;
		};

	private:
		CMFunVoid m_pCMFunVoid;
		CMFunTEvent m_pCMFunTEvent;
		CMFunTNotify m_pCMFunTNotify;
	};

	template <class O, class T,class P>
	CDelegateUI<O, T, P> MakeDelegateUI(O* pObject, bool (T::* pFn)(void* pParam,LPARAM lParam,WPARAM wParam),P lParam = NULL,WPARAM wParam = NULL)
	{
		return CDelegateUI<O, T, P>(pObject, pFn,lParam,wParam);
	}

	template <class O, class T,class P>
	CDelegateUI<O, T, P> MakeDelegateUI(O* pObject, bool (T::* pFn)(TEventUI*,P lParam,WPARAM wParam),UINT _iEventType,P lParam = NULL,WPARAM wParam = NULL)
	{
		return CDelegateUI<O, T, P>(pObject, pFn,_iEventType,lParam,wParam);
	}

	template <class O, class T,class P>
	CDelegateUI<O, T, P> MakeDelegateUI(O* pObject, bool (T::* pFn)(TNotifyUI*,P lParam,WPARAM wParam),LPCTSTR _sNotifyTypeName,P lParam = NULL,WPARAM wParam = NULL)
	{
		return CDelegateUI<O, T, P>(pObject, pFn,(LPCTSTR)_sNotifyTypeName,lParam,wParam);
	}

	inline CDelegateStaticUI MakeDelegateUI(FunVoid pFunVoid,LPARAM lParam = NULL,WPARAM wParam = NULL)
	{
		return CDelegateStaticUI(pFunVoid,lParam,wParam);
	}

	inline CDelegateStaticUI MakeDelegateUI(FunTEvent pFunTEvent,UINT _iEventType,LPARAM lParam = NULL,WPARAM wParam = NULL)
	{
		return CDelegateStaticUI(pFunTEvent,_iEventType,lParam,wParam); 
	}

	inline CDelegateStaticUI MakeDelegateUI(FunTNotify pFunTNotify,LPCTSTR _sNotifyTypeName,LPARAM lParam = NULL,WPARAM wParam = NULL)
	{
		return CDelegateStaticUI(pFunTNotify,_sNotifyTypeName,lParam,wParam); 
	}

	class UILIB_API CEventSourceUI
	{
	public:
		CEventSourceUI();
		~CEventSourceUI();
		operator bool();
		void operator+= (const CDelegateBaseUI& d); // add const for gcc
		void operator+= (FunVoid pFunVoid){(*this) += MakeDelegateUI(pFunVoid);};
		void operator-= (const CDelegateBaseUI& d);
		void operator-= (FunVoid pFunVoid){(*this) -= MakeDelegateUI(pFunVoid);};
		bool operator() (void* param);
		bool operator() (TEventUI* pTEventUI);
		bool operator() (TNotifyUI* pTNotifyUI);

	protected:
		TStdPtrArray<CDelegateBaseUI*> m_aDelegates;
	};

} // namespace UiLib
