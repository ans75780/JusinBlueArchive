#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CUI;
END

BEGIN(Client)

class CUI_TestDialogButton final : public CUI
{
public:
	CUI_TestDialogButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUI_TestDialogButton() = default;


public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	OnLButtonDown();
	virtual void	OnLButtonUp();
	virtual void	OnLButtonClicked();

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ShaderResource();

public:
	static CUI_TestDialogButton * Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END