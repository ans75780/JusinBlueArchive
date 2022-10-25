#include "stdafx.h"
#include "..\Public\Camera_Stage.h"
#include "GameInstance.h"
#include "Student.h"
#include "CFrustum.h"


CCamera_Stage::CCamera_Stage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Stage::CCamera_Stage(const CCamera_Stage & rhs)
	: CCamera(rhs)
{
	
}

HRESULT CCamera_Stage::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Stage::Initialize(void * pArg)
{
	/* Ʈ������ �����͈p�� �߰��Ѵ�. */
	/* ���� ���ϴ� ī�޶��� �ʱ���¸� ����ȭ�ϳ�. */
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	lstrcpy(m_desc.sz_Name, TEXT("Camera_Stage"));

	m_fSpeed = 3.f;
	
	m_vOriginOffset = { 3.f,4.5f, 0.f };
	m_vOffset = m_vOriginOffset;
	m_bInCam = true;
	m_fOriginFov = m_CameraDesc.fFovy;
	m_fZoomOutFov = m_fOriginFov + 30.f;
	
	m_iCamFocusIndex = 0;
	
	return S_OK;
}

void CCamera_Stage::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!IsMainCam())
		return;

	if (nullptr == m_pVecStudents)
		return;
	if (m_pVecStudents->size() < 1)
		return;
	_vector vTargetPos = XMVectorSet(0,.0,0,1.f);

	_float	fZ = -50.f;
	_uint	iFocus = 0;
	for (int i = 0; i < m_pVecStudents->size(); i++)
	{

		//���� �ָ��ִ� ĳ���� ã��
		if (XMVectorGetZ((*m_pVecStudents)[i]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)) > fZ)
		{
			iFocus = i;
			fZ = XMVectorGetZ((*m_pVecStudents)[i]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		}
		vTargetPos += (*m_pVecStudents)[i]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	}
	vTargetPos = XMVectorSet(XMVectorGetX(vTargetPos) / 3.f, XMVectorGetY(vTargetPos) / 3.f, fZ, 1.f);

	m_pTransformCom->LookAt(vTargetPos);
	

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	m_bInCam = true;
	

	RELEASE_INSTANCE(CGameInstance);
	_vector		vCamPos = vTargetPos + XMLoadFloat3(&m_vOffset);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vCamPos);
	if (FAILED(Bind_PipeLine()))
		return;
	/*
	for (_uint i = 0; i < m_pVecStudents->size(); i++)
	{
		if (!pGameInstance->IsIn_Frustum_InWorldSpace((*m_pVecStudents)[i]->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION), 1.f))
		{
			m_bInCam = false;
			(*m_pVecStudents)[i]->Get_Transform()->Set_Speed(15.f);
			break;
		}
	}
	if (!m_bInCam)
	{
		m_vOffset.y += 1.f * fTimeDelta;
	}
	else if (m_bInCam && m_vOffset.y != m_vOriginOffset.y)
	{
		m_vOffset.y -= 1.f * fTimeDelta;

		if (m_vOriginOffset.y >= m_vOffset.y)
			m_vOffset.y = m_vOriginOffset.y;

		for (_uint i = 0; i < m_pVecStudents->size(); i++)
		{
			(*m_pVecStudents)[i]->Get_Transform()->Set_Speed(3.f);
		}
	}
	*/
	if (KEY(J, HOLD))
	{
		m_vOffset.y -= 1.f * fTimeDelta;
	}
	if (KEY(K, HOLD))
	{
		m_vOffset.y += 1.f * fTimeDelta;
	}

}

void CCamera_Stage::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Stage::Render()
{
	return S_OK;
}

CCamera_Stage * CCamera_Stage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Stage*		pInstance = new CCamera_Stage(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera_Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Stage::Clone(void * pArg)
{
	CCamera_Stage*		pInstance = new CCamera_Stage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera_Stage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Stage::Set_Formation(vector<class CStudent*>* pVecSturdents)
{
	m_pVecStudents = pVecSturdents;
}

void CCamera_Stage::Free()
{
	__super::Free();
}
