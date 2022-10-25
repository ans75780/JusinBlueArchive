#pragma once

#include "StateBase.h"

BEGIN(Client)

class CState_Student_Ex : public CStateBase
{
private:
	CState_Student_Ex(class CActor* pStudent);
	virtual ~CState_Student_Ex() = default;
public:
	// CStateBase을(를) 통해 상속됨
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	static  CState_Student_Ex * Create(class CActor * pStudent);

private:
	class CAnimation*	m_pEx= nullptr;





};

END
