#include "stdafx.h"
#include "..\Public\UI_Fade_White.h"
#include "Level_Loading.h"
#include <random>
#include "ImguiMgr.h"

CUI_Fade_White::CUI_Fade_White(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext)
{
}

HRESULT CUI_Fade_White::Initialize(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;

	ZeroMemory(m_szUIName, sizeof(_tchar) * MAX_PATH);
	ZeroMemory(m_szUITextureTag, sizeof(_tchar) * MAX_PATH);
	ZeroMemory(m_szUIClass, sizeof(_tchar) * MAX_PATH);

	if (pArg == nullptr)
	{
		TransformDesc.fSpeedPerSec = 0.f;
		TransformDesc.fRotationPerSec = XMConvertToRadians(0.0f);
			
		m_Components.emplace(m_pTransformTag, m_pTransformCom);

		ZeroMemory(&m_desc, sizeof(OBJ_DESC));

		Safe_AddRef(m_pTransformCom);

	}

	if (FAILED(this->SetUp_Component()))
		return E_FAIL;

	m_eUIType = UI_POST;
	lstrcpy(m_szUIClass, TEXT("CUI_Fade_White"));

	m_fAlpha = 0.f;
	m_bAlpha = false;
	return S_OK;
}

HRESULT CUI_Fade_White::Initialization()
{
	__super::Initialization();

	return S_OK;
}

void CUI_Fade_White::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAlpha)
	{
		if (1.f > m_fAlpha)
			m_fAlpha += fTimeDelta;
		else
			m_bFullWhite = true;
	}
	else
	{
		if (0.f < m_fAlpha)
			m_fAlpha -= fTimeDelta;
		else
			m_bFullWhite = false;
	}

}

void CUI_Fade_White::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, (CGameObject*)this);
}

HRESULT CUI_Fade_White::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_Fade_White::OnLButtonDown()
{
}

void CUI_Fade_White::OnLButtonUp()
{
}

void CUI_Fade_White::OnLButtonClicked()
{
#if _DEBUG
	CImguiMgr* pImgui = GET_INSTANCE(CImguiMgr);

	pImgui->Set_SelectUI(this);

	RELEASE_INSTANCE(CImguiMgr);
#endif
}


HRESULT CUI_Fade_White::SetUp_ShaderResource()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &CUI::g_UIMatProj, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_AlphaValue", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Fade_White::SetUp_Component()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Shader_VtxTex_Test"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

CUI_Fade_White * CUI_Fade_White::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Fade_White*		pInstance = new CUI_Fade_White(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_Fade_White");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Fade_White::Free()
{
	__super::Free();
}
