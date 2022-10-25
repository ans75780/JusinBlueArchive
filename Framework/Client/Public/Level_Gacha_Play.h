#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Engine)
class CUI;
END

BEGIN(Client)

class CLevel_Gacha_Play final : public CLevel
{
public:
	CLevel_Gacha_Play(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Gacha_Play() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Student(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Sky(const _tchar* pLayerTag);
	HRESULT Ready_Right();

private:
	CUI*	m_pUI = nullptr;



public:
	static CLevel_Gacha_Play* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END