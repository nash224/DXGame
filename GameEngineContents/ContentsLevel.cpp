#include "PreCompile.h"
#include "ContentsLevel.h"

#include "GlobalValue.h"
#include "CameraControler.h"

#include "OutLineEffect.h"


bool ContentsLevel::PixelDebugMode = false;
ContentsLevel::ContentsLevel() 
{
	GetMainCamera()->Transform.SetLocalPosition({ 0.0f, 0.0f, -500.0f });
	GetMainCamera()->SetProjectionType(EPROJECTIONTYPE::Orthographic);
	GetMainCamera()->SetZSort(0);
	GetCamera(static_cast<int>(ECAMERAORDER::UI))->Transform.SetLocalPosition({ 0.0f, 0.0f, -500.0f });

	GameEngineLevel::IsDebug = false;
}

ContentsLevel::~ContentsLevel() 
{
}

void ContentsLevel::Start()
{
	// 입력 등록함
	GameEngineInput::AddInputObject(this);

	// 카메라 컨트롤러 생성
	m_LevelCameraControler = CreateActor<CameraControler>(EUPDATEORDER::CameraControler);

	GetMainCamera()->GetCameraAllRenderTarget()->CreateEffect<OutLineEffect>();
}

void ContentsLevel::Update(float _Delta)
{
	if (true == GameEngineInput::IsDown(VK_F1, this))
	{
		IsDebug = !IsDebug;
	}

	ChangeDebugMode();
}

void ContentsLevel::LevelStart(class GameEngineLevel* _NextLevel)
{
	if (nullptr != m_LevelCameraControler)
	{
		m_LevelCameraControler->m_MainCamera = this->GetMainCamera().get();
		GlobalValue::g_CameraControler = m_LevelCameraControler;
	}
}


// 디버그 모드 전환
void ContentsLevel::ChangeDebugMode()
{
	if (true == IsDebug)
	{
		if (true == GameEngineInput::IsDown(VK_F2, this))
		{
			PixelDebugMode = !PixelDebugMode;
		}
	}
	else
	{
		if (true == PixelDebugMode)
		{
			PixelDebugMode = false;
		}
	}
}