#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "GameInstance.h"
#include "LEvel_Loading.h"
#include "UI_Progress.h"
#include "UI_LevelMoveButton.h"
#include "UI_Text.h"
#include "UI_Loading.h"
#include "StrUtil.h"

//Json ���
#include "Json_Utility.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
	
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	//if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
	//	return E_FAIL;	
	
	if (FAILED(LoadUI()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOBBY))))
			return;

		Safe_Release(pGameInstance);
	}

}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	SetWindowText(g_hWnd, TEXT("���������. "));

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);
	
	CUI_Progress*	pHpBar= CUI_Progress::Create(m_pDevice, m_pContext);

	if (FAILED(pGameInstance->Add_UI(LEVEL_LOGO, pHpBar)))
	{
		MSG_BOX("UI��������");
	}

	Safe_Release(pGameInstance);

	return S_OK;
}





HRESULT CLevel_Logo::UI_Extract()
{
	Mat img = imread("../../Resources/UI/UI_original/image/ �ڸ� �̹��� .png", IMREAD_UNCHANGED);

	if (img.empty())
	{
		return E_FAIL;
	}

	json	JsonTemp;
	
	if (FAILED(CJson_Utility::Load_Json(CJson_Utility::Complete_Path(
		L"../../Resources/UI/UI_original/json/���̽�����.json ����������  ").c_str(), &JsonTemp)))
	{
		MSG_BOX("���̽� ���Ф̤�");
		return E_FAIL;
	}

	auto json_mSprites = JsonTemp["mSprites"];

	for (auto it = json_mSprites.begin(); it != json_mSprites.end(); ++it)
	{
		string _name = (*it)["name"];
		int _x		= (*it)["x"];
		int _width = (*it)["width"];
		int _y		= (*it)["y"];
		int _height= (*it)["height"];

		Mat cropped_image = img(Range(_y, (_y + _height)), Range(_x, (_x + _width)));

		string savePath = "../../Resources/UI/UI_extract/image/������ ���ϰ��/";
		
		savePath += _name;
		savePath += ".png";

		imwrite(savePath.c_str(), cropped_image);
	}
	
	return S_OK;
}

HRESULT CLevel_Logo::LoadUI()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//pGameInstace->Clear_UIVec();	UI �ִ°Ŵ�����

	json loadJson;

	ifstream fin;
	fin.open("../../Resources/Data/SaveData.json");

	if (fin.is_open())
		fin >> (loadJson);
	else
	{
		MSG_BOX("JSON_UI �ҷ����� ����");
		return E_FAIL;
	}
	fin.close();

	auto UIJsonVec = loadJson["UI"];

	for (auto it = UIJsonVec.begin(); it != UIJsonVec.end(); ++it)
	{
		string	_ClassName = (*it)["ClassName"];
		string	_TextureName = (*it)["TextureName"];
		string	_Name = (*it)["Name"];

		_uint	_Level = (*it)["Level"];
		if (_Level == LEVEL::LEVEL_LOADING_START)
			continue;
		_uint	_Type = (*it)["Type"];

		_float3 _Pos;
		_Pos.x = (*it)["Pos_x"];
		_Pos.y = (*it)["Pos_y"];
		_Pos.z = (*it)["Pos_z"];

		_float3 _Size;
		_Size.x = (*it)["Size_x"];
		_Size.y = (*it)["Size_y"];
		_Size.z = (*it)["Size_z"];

		_float2 _ThrowPos;
		_ThrowPos.x = (*it)["ThrowPos_x"];
		_ThrowPos.y = (*it)["ThrowPos_y"];

		CUI* pUI = nullptr;


		if (!strcmp(_ClassName.c_str(), "CUI_LevelMoveButton"))
		{
			pUI = CUI_LevelMoveButton::Create(m_pDevice, m_pContext);

			_uint	_MoveLevel = (*it)["MoveLevel"];
			static_cast<CUI_LevelMoveButton*>(pUI)->SetMoveLevel(_MoveLevel);
		}


		else if (!strcmp(_ClassName.c_str(), "CUI_Text"))
		{
			pUI = CUI_Text::Create(m_pDevice, m_pContext);

			string _UITextTemp = (*it)["UIText"];
			_tchar* _UIText = CStrUtil::ConvertUTF8toWC(_UITextTemp.c_str());
			static_cast<CUI_Text*>(pUI)->SetUIText(_UIText);
			Safe_Delete_Array(_UIText);

			static_cast<CUI_Text*>(pUI)->SetUIScale(((*it)["UITextScale"]));

			_float4 _UITextColor;
			_UITextColor.x = (*it)["UITextColor_x"];
			_UITextColor.y = (*it)["UITextColor_y"];
			_UITextColor.z = (*it)["UITextColor_z"];
			_UITextColor.w = (*it)["UITextColor_w"];

			static_cast<CUI_Text*>(pUI)->SetUITextColor(_UITextColor);

		}

		else if (!strcmp(_ClassName.c_str(), "CUI_Loading"))
		{
			pUI = CUI_Loading::Create(m_pDevice, m_pContext);
			
		}

		if (nullptr == pUI)
		{
			MSG_BOX("Ŭ�������� ������ �����Ұ�");
			return E_FAIL;
		}

		_tchar* pUtil_ImageTag = CStrUtil::ConvertCtoWC(_TextureName.c_str());
		_tchar* pUtil_Name = CStrUtil::ConvertCtoWC(_Name.c_str());

		pUI->LoadUIImage(pUtil_ImageTag);
		pUI->Set_UIType((UI_TYPE)_Type);
		pUI->Set_Size(_float3(_Size.x, _Size.y, _Size.z));
		pUI->Set_Pos(_float3(_Pos.x, _Pos.y, _Pos.z));
		pUI->Set_UIName(pUtil_Name);
		pUI->Set_UILevel(_Level);
		pUI->Set_ThrowPos(_ThrowPos);
		pUI->Initialization();

		if (FAILED(pGameInstance->Add_UI(_Level, pUI)))
		{
			MSG_BOX("UI���� �ҷ����� ����");
			return E_FAIL;
		}

		Safe_Delete_Array(pUtil_Name);
		Safe_Delete_Array(pUtil_ImageTag);
	}

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}


CLevel_Logo * CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo*		pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();



}

