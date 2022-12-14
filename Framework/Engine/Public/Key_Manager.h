#pragma once
#include "Base.h"

BEGIN(Engine)
enum class KEY
{
	LEFT = 0,
	RIGHT,
	UP,
	DOWN,
	NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0,
	F1, F2, F3, F4, F5, F6, F7, F8, F9,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	ALT,
	LSHIFT,
	SPACE,
	CTRL,
	ENTER,
	ESC,
	TAB,
	LBUTTON,
	RBUTTON,
	BACKSPACE,
	DELETEKEY,
	LAST,
};

enum class KEY_STATE {
	TAP, // 막 누른 시점
	HOLD,// 꾹 누르고 있음
	AWAY,// 막 뗀 시점
	NONE,// 걍 안눌린 상태
};



class CKey_Manager final : public CBase
{
	DECLARE_SINGLETON(CKey_Manager)
private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	KEY_STATE				Get_KeyState(KEY _key);

public:
	HRESULT		Initialize(const GRAPHICDESC& desc);
	void		Tick();
	const POINT&	Get_MousePos() { return m_PtMouse; }
	const POINT&	Get_MouseOffset() { return m_PtMouseOffset; }
private:
	struct tKeyInfo
	{
		KEY_STATE eState;
		bool bPrevPush;
	};
	HWND				m_hWnd = 0;
	vector<tKeyInfo>	m_vecKey;
	POINT				m_PtMouse;
	POINT				m_PtMouseOffset;
};

END