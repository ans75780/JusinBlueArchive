#pragma once

#include "StateBase.h"

BEGIN(Client)

class CState_Student_Idle : public CStateBase
{
private:
	CState_Student_Idle(class CStudent* pStudent);
	virtual ~CState_Student_Idle() = default;
public:
	// CStateBase을(를) 통해 상속됨
	virtual HRESULT	Initialize() override;
	virtual void Enter() override;
	virtual _bool Loop(_float fTimeDelta) override;
	virtual CStateBase * Exit() override;
	static  CStateBase * Create(class CStudent * pStudent);

};

END
