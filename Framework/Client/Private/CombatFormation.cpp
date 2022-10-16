#include "stdafx.h"
#include "CombatFormation.h"
#include "..\Public\CombatFormation.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Actor.h"	
#include "Student.h"
#include "UserData.h"
#include "StateMachineBase.h"
#include "StateBase.h"
#include "State_Student_Default.h"
#include "Camera.h"
#include "Camera_Event.h"
#include "Camera_Stage.h"
#include "State_Student_Ex.h"
#include "Base.h"


CCombatFormation::CCombatFormation()
{
}

CCombatFormation::~CCombatFormation()
{
}



HRESULT CCombatFormation::Initialize(void * pArg)
{

	//���� ���� ������ ������
	m_vecFormationPos.push_back(XMVectorSet(0.f, 0.f, 0.5f, 1.f));
	m_vecFormationPos.push_back(XMVectorSet(0.5f, 0.f, 0.f, 1.f));
	m_vecFormationPos.push_back(XMVectorSet(-0.5f, 0.f, 0.f, 1.f));

	ZeroMemory(&m_eDesc, sizeof(FORMATIONDESC));

	memcpy(&m_eDesc, pArg, sizeof(FORMATIONDESC));


	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CGameObject* pStudent = nullptr;

	_tchar	szStudentPath[MAX_PATH] = L"Prototype_Student";

	//�� Ŭ�е� ��Ʃ��Ʈ�� ������ �־���.
	CGameObject::OBJ_DESC	desc;

	vector<CGameObject::OBJ_DESC> m_formationDesc = CUserData::Get_Instance()->Get_Formation();
	for (_uint i = 0; i < m_formationDesc.size(); i++)
	{
		if (FAILED(pGameInstance->Add_GameObject(m_eDesc.eLevel,m_eDesc.szLayer, szStudentPath, (void*)&m_formationDesc[i], &pStudent)))
			return E_FAIL;
		((CStudent*)pStudent)->Set_Transform(m_vecFormationPos[i]);
		m_vecStudent.push_back((CStudent*)pStudent);
	}
	Safe_Release(pGameInstance);
	
	
	m_pStageCam->Set_Formation(&m_vecStudent);

	return S_OK;
}

void CCombatFormation::Tick(_float fTimeDelta)
{
	for (auto& elem  = m_vecStudent.begin(); elem != m_vecStudent.end();)
	{
		if ((*elem)->Get_StageState()  == CActor::STAGE_STATE_DEAD)
		{
			elem = m_vecStudent.erase(elem);
		}
		else
		{
			elem++;
		}
	}
}

void CCombatFormation::LateTick(_float fTimeDelta)
{
}

HRESULT CCombatFormation::Render()
{
	return S_OK;
}

HRESULT CCombatFormation::StartGame()
{
	if (m_eDesc.eGameStartEvent)
	{
		m_vecStudent[0]->Get_StateMachine()->Add_State
		(
			CState_Student_Default::Create(m_vecStudent[0], L"_Normal_Callsign")
		);
		_float3	vOffset = { 1.5f,1.f, 1.5f };

		m_pEventCam->Ready_Event_Stage_Start(m_pStageCam, m_vecStudent[0],
			m_vecStudent[0]->Get_StateMachine()->Get_CurrentState()->Get_Animation(),
			vOffset, &m_vecStudent);
	}
	else
	{
		vector<_float4> StartPos = m_pEventCam->Get_VecStudentStartPos();
		for (_uint i = 0; i < StartPos.size();i++)
			m_vecStudent[i]->Set_Transform(XMLoadFloat4(&StartPos[i]));
	}
	
	return S_OK;
}

CCombatFormation * CCombatFormation::Create(FORMATIONDESC * pDesc, CCamera * pStageCam, CCamera * pEventCam)
{
	CCombatFormation*	pInstance = new CCombatFormation();
	
	pInstance->m_pStageCam = (CCamera_Stage*)pStageCam;
	pInstance->m_pEventCam = (CCamera_Event*)pEventCam;

	if (FAILED(pInstance->Initialize(pDesc)))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

void CCombatFormation::Free()
{
	m_vecStudent.clear();

}

