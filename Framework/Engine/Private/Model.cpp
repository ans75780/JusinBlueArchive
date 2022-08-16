#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "BoneNode.h"
CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_MeshContainers(rhs.m_MeshContainers)
	, m_iNumMeshContainers(rhs.m_iNumMeshContainers)
	, m_Materials(rhs.m_Materials)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_vecBones(rhs.m_vecBones)
{
	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pTextures)
			Safe_AddRef(pTexture);
	}

	for (auto& pMeshContainer : m_MeshContainers)
		Safe_AddRef(pMeshContainer);

	for (auto& pBoneNode : m_vecBones)
		Safe_AddRef(pBoneNode);

}
HRESULT CModel::Initialize_Prototype(MODELTYPE eType, 
	const char* pModelFilePath, const char* pModelFileName, _fmatrix TransformMatrix)
{
	m_eModelType = eType;

	XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix);

	char			szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	if (TYPE_NONANIM == eType)
		m_pAIScene = m_Importer.ReadFile(szFullPath, aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
	else
		m_pAIScene = m_Importer.ReadFile(szFullPath, aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (FAILED(Create_Materials(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Create_Bones(m_pAIScene->mRootNode, nullptr, 0)))
		return E_FAIL;

	sort(m_vecBones.begin(), m_vecBones.end(), [](CBoneNode* pSour, CBoneNode* pDest)
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	if (FAILED(Create_MeshContainers()))
		return E_FAIL;

	return S_OK;
}



HRESULT CModel::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	if (iMeshIndex >= m_iNumMeshContainers)
		return E_FAIL;

	m_MeshContainers[iMeshIndex]->Render();
	return S_OK;
}


HRESULT CModel::Bind_SRV(CShader * pShader, const char * pConstantName, _uint iMeshContainerIndex, aiTextureType eType)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	_uint		iMaterialIndex = m_MeshContainers[iMeshContainerIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex].pTextures[eType]->Set_ShaderResourceView(pShader, pConstantName);
}

CBoneNode * CModel::Find_Bone(const char * pBoneName)
{
	auto	iter = find_if(m_vecBones.begin(), m_vecBones.end(), [&](CBoneNode* pNode)
	{
		return !strcmp(pBoneName, pNode->Get_Name());
	});
	if (iter == m_vecBones.end())
		return nullptr;

	return *iter;
}

HRESULT CModel::Create_MeshContainers()
{
	m_iNumMeshContainers = m_pAIScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshContainers; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i],
			this, XMLoadFloat4x4(&m_TransformMatrix));
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_MeshContainers.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Create_Materials(const char * pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;
	
	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		//머테리얼 개수만큼 생성
		MODEL_MATERIAL	Material;
		ZeroMemory(&Material, sizeof(MODEL_MATERIAL));
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX;j++)
		{
			//텍스쳐가 받을 수 있는 모든 타입을 받음.
			char		szFullPath[MAX_PATH] = "";
			
			aiString	strPath;
		
			//필요한 텍스쳐의 경로를 반환
			if (FAILED(m_pAIScene->mMaterials[i]->GetTexture(aiTextureType(j), 0, &strPath)))
				continue;
			/*
				이 작업을 해주는 이유.
				어씸프가 모델을 로드할 때 나오는 텍스쳐 경로가
				상대 경로가 아닌 해당 모델을 로드한 곳의
				절대 경로를 반환한다.
				그렇기에 이걸 다시 우리의 경로로 만들어주는 작업임.
			*/
			char	szFileName[MAX_PATH] = "";
			char	szExt[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar		szTextureFilePath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szTextureFilePath, MAX_PATH);

			Material.pTextures[j] = CTexture::Create(m_pDevice, m_pContext, szTextureFilePath);
			if (nullptr == Material.pTextures[j])
				return E_FAIL;
		}
		m_Materials.push_back(Material);

	}

	return S_OK;
}

HRESULT CModel::Create_Bones(aiNode * pAINode, CBoneNode * pParent, _uint iDepth)
{
	CBoneNode*		pBoneNode = CBoneNode::Create(pAINode, pParent, iDepth);
	if (nullptr == pBoneNode)
		return E_FAIL;

	m_vecBones.push_back(pBoneNode);

	for (_uint i = 0; i < pAINode->mNumChildren; ++i)
		Create_Bones(pAINode->mChildren[i], pBoneNode, iDepth + 1);

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, MODELTYPE eType, 
	const char * pModelFilePath, const char * pModelFileName, _fmatrix TransformMatrix)
{
	CModel*		pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, pModelFileName, TransformMatrix)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*		pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CModel::Free()
{
	__super::Free();

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pTextures)
			Safe_Release(pTexture);
	}
	m_Materials.clear();


	for (auto& pMeshContainer : m_MeshContainers)
		Safe_Release(pMeshContainer);

	m_MeshContainers.clear();

	for (auto& pBoneNode : m_vecBones)
		Safe_Release(pBoneNode);
	
	m_vecBones.clear();


	if (false == m_isCloned)
		m_Importer.FreeScene();

}
