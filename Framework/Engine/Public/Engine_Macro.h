#pragma once


#define GET_INSTANCE(CLASSNAME)	[](){											\
	CLASSNAME*	pInstance = CLASSNAME::Get_Instance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->AddRef();}														\
	return pInstance;															\
	}();

#define RELEASE_INSTANCE(CLASSNAME)	[](){										\
	CLASSNAME*	pInstance = CLASSNAME::Get_Instance();							\
	if(nullptr == pInstance) {													\
	char	szMessage[MAX_PATH] = "";											\
	strcpy_s(szMessage, typeid(CLASSNAME).name());								\
	strcat_s(szMessage, "is nullptr");											\
	MessageBoxA(0, szMessage, nullptr, MB_OK);}									\
	else {																		\
	pInstance->Release();}														\
	}();



#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Error"), MB_OK)

//KEY : 누른 키 , State : 누른 키의 상태
#ifdef ENGINE_EXPORTS
#define KEY(key, state) CKey_Manager::Get_Instance()->Get_KeyState(KEY::key) == KEY_STATE::state
#else
#define KEY(key, state) CGameInstance::Get_Instance()->Get_KeyState(KEY::key) == KEY_STATE::state
#endif

#ifdef ENGINE_EXPORTS
#define GETMOUSEPOS CKey_Manager::Get_Instance()->Get_MousePos();
#else
#define GETMOUSEPOS CGameInstance::Get_Instance()->Get_MousePos();
#endif


#define BEGIN(NAMESPACE) namespace NAMESPACE { 
#define END }

#define NO_COPY(ClassName)								\
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete;

#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static ClassName* Get_Instance();					\
	static unsigned long Destroy_Instance();			\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;			\
ClassName* ClassName::Get_Instance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
unsigned long ClassName::Destroy_Instance()				\
{														\
	return Safe_Release(m_pInstance);					\
}
