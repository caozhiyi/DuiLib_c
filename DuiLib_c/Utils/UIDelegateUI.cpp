#include "StdAfx.h"

namespace DuiLib
{
	
	//************************************
	// 函数名称: CDelegateUIBaseUI
	// 返回类型: 
	// 参数信息: void * pObject
	// 参数信息: void * pFn
	// 函数说明: 
	//************************************
	CDelegateBaseUI::CDelegateBaseUI( void* pObject, FunVoid pFn,LPARAM lParam /*= NULL*/,WPARAM wParam /*= NULL*/ )
	{
		m_pObject				= pObject;
		m_unionFnType.pFunVoid	= pFn;
		m_iEventType			= UIEVENT__ALL;
		m_lParam				= lParam;
		m_wParam				= wParam;
		m_sNotifyTypeName.Empty();
	}

	//************************************
	// 函数名称: CDelegateUIBaseUI
	// 返回类型: 
	// 参数信息: void * pObject
	// 参数信息: void * pFn
	// 参数信息: UINT _iEventType
	// 函数说明: 
	//************************************
	CDelegateBaseUI::CDelegateBaseUI( void* pObject, FunTEvent pFn,UINT _iEventType /*= UIEVENT__ALL*/,LPARAM lParam /*= NULL*/,WPARAM wParam /*= NULL*/ )
	{
		m_pObject					= pObject;
		m_unionFnType.pFunTEvent	= pFn;
		m_iEventType				= _iEventType;
		m_lParam					= lParam;
		m_wParam					= wParam;
		m_sNotifyTypeName.Empty();
	}

	//************************************
	// 函数名称: CDelegateUIBaseUI
	// 返回类型: 
	// 参数信息: void * pObject
	// 参数信息: void * pFn
	// 参数信息: LPCTSTR _sNotifyTypeName
	// 函数说明: 
	//************************************
	CDelegateBaseUI::CDelegateBaseUI( void* pObject, FunTNotify pFn,LPCTSTR _sNotifyTypeName /*= NULL*/,LPARAM lParam /*= NULL*/,WPARAM wParam /*= NULL*/ )
	{
		m_pObject					= pObject;
		m_unionFnType.pFunTNotify	= pFn;
		m_iEventType				= UIEVENT__ALL;
		m_lParam					= lParam;
		m_wParam					= wParam;
		if(NULL != _sNotifyTypeName)
			m_sNotifyTypeName = _sNotifyTypeName;
	}

	//************************************
	// 函数名称: CDelegateUIBaseUI
	// 返回类型: 
	// 参数信息: const CDelegateUIBaseUI & rhs
	// 函数说明：
	//************************************
	CDelegateBaseUI::CDelegateBaseUI( const CDelegateBaseUI& rhs )
	{
		m_pObject				= rhs.m_pObject;
		m_unionFnType.pFunVoid	= rhs.m_unionFnType.pFunVoid;
		m_iEventType			= rhs.m_iEventType;
		m_lParam				= rhs.m_lParam;
		m_wParam				= rhs.m_wParam;
		m_sNotifyTypeName		= rhs.m_sNotifyTypeName.GetData();
	}

	CDelegateBaseUI::~CDelegateBaseUI()
	{
		if(!m_sNotifyTypeName.IsEmpty())
			m_sNotifyTypeName.Empty();
	}

	bool CDelegateBaseUI::Equals(const CDelegateBaseUI& rhs) const 
	{
		return m_pObject == rhs.m_pObject && m_unionFnType.pFunVoid	== rhs.m_unionFnType.pFunVoid && m_iEventType == rhs.m_iEventType && m_sNotifyTypeName == rhs.m_sNotifyTypeName.GetData(); 
	}
	
	//************************************
	// 函数名称: CEventSourceUI
	// 返回类型: 
	// 函数说明: 
	//************************************
	CEventSourceUI::CEventSourceUI()
	{
		m_aDelegates.Empty();
	}

	CEventSourceUI::~CEventSourceUI()
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBaseUI* pObject = m_aDelegates.GetAt(i);
			if( pObject) delete pObject;
			pObject = NULL;
		}
	}

	CEventSourceUI::operator bool()
	{
		return m_aDelegates.GetSize() > 0;
	}

	void CEventSourceUI::operator+= (const CDelegateBaseUI& d)
	{ 
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBaseUI* pObject = m_aDelegates.GetAt(i);
			if( pObject && pObject->Equals(d) ) return;
		}

		m_aDelegates.Add(d.Copy());
	}
	
	void CEventSourceUI::operator-= (const CDelegateBaseUI& d) 
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBaseUI* pObject = m_aDelegates.GetAt(i);
			if( pObject && pObject->Equals(d) ) {
				delete pObject;
				m_aDelegates.Remove(i);
				return;
			}
		}
	}

	bool CEventSourceUI::operator() (void* param) 
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBaseUI* pObject = m_aDelegates.GetAt(i);
			if( pObject && !pObject->Invoke(param,pObject->GetLParam(),pObject->GetWParam()) ) return false;
		}
		return true;
	}

	bool CEventSourceUI::operator() (TEventUI* pTEventUI) 
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBaseUI* pObject = m_aDelegates.GetAt(i);
			if( pObject && !pObject->Invoke(pTEventUI,pObject->GetLParam(),pObject->GetWParam()) ) return false;
		}
		return true;
	}

	bool CEventSourceUI::operator() (TNotifyUI* pTNotifyUI) 
	{
		for( int i = 0; i < m_aDelegates.GetSize(); i++ ) {
			CDelegateBaseUI* pObject = m_aDelegates.GetAt(i);

			if( pObject && !pObject->Invoke(pTNotifyUI,pObject->GetLParam(),pObject->GetWParam()) ) return false;
		}
		return true;
	}


} // namespace UiLib
