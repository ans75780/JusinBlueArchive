#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CMeshContainer;
class CCollider;
class CAnimation;
END

BEGIN(Client)

class CActor : public CGameObject
{
public:
	CActor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CActor(const CActor& rhs);
	virtual ~CActor() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	class CStateMachineBase*		Get_StateMachine() { return m_pStateMachine; }
	void	Set_Transform(_vector vPos);
	class CAnimation*	Get_Animation(const char* pAnimationName);
	_bool	Collision_AABB(RAYDESC& ray, _float& distance);
	CCollider*		Get_AABB() { return m_pAABBCom; }
	CModel*			Get_ModelCom(){ return m_pModelCom; }


protected:
	class CStateMachineBase* m_pStateMachine = nullptr;
	CShader*				 m_pShaderCom = nullptr;
	CRenderer*				 m_pRendererCom = nullptr;
	CModel*					 m_pModelCom = nullptr;
	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	CCollider*				m_pSphereCom = nullptr;

protected:


protected:
	_uint					m_iAnimIndex = 0;

protected:
	virtual HRESULT SetUp_Components();
	virtual HRESULT SetUp_ShaderResource();
	virtual HRESULT	SetUp_StateMachine(_uint iLevel);

public:
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END