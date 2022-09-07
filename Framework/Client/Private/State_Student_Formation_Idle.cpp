#include "stdafx.h"
#include "State_Student_Idle.h"
#include "Student.h"
#include "StateBase.h"
#include "Model.h"
#include "Animation.h"
#include "State_Student_Formation_Idle.h"
#include "GameInstance.h"
#include "State_Student_Formation_Pickup.h"
CState_Student_Formation_Idle::CState_Student_Formation_Idle(CStudent * pStudent)
	:CStateBase(pStudent)
{
}

HRESULT CState_Student_Formation_Idle::Initialize()
{

	char pAnimaitonStr[MAX_PATH];

	WideCharToMultiByte(CP_ACP, 0, m_pStudent->Get_Name(), MAX_PATH, pAnimaitonStr, MAX_PATH, NULL, NULL);

	strcat_s(pAnimaitonStr, "_Original_Formation_Idle");

	m_pAnimation = m_pModel->Get_AnimationFromName(pAnimaitonStr);

	if (nullptr == m_pAnimation)
		return E_FAIL;

	m_pAnimation->Play();

	return S_OK;
}

void CState_Student_Formation_Idle::Enter()
{
}

_bool CState_Student_Formation_Idle::Loop(_float fTimeDelta)
{
	m_pAnimation->Update(fTimeDelta);
	m_pModel->Update_CombinedMatrix();
	if (KEY(Q, TAP))
	{
		return true;
	}
	return false;
}

CStateBase * CState_Student_Formation_Idle::Exit()
{
	return CState_Student_Formation_Pickup::Create(m_pStudent);
}

CState_Student_Formation_Idle * CState_Student_Formation_Idle::Create(CStudent * pStudent)
{
	CState_Student_Formation_Idle*	pInstance = new CState_Student_Formation_Idle(pStudent);

	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}