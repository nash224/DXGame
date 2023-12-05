#include "PreCompile.h"
#include "PlayLevel.h"

// Manager
#include "UIManager.h"
#include "TimeManager.h"
#include "AlertManager.h"
#include "SkyLerp.h"

// Actor
#include "Ellie.h"
#include "FadeObject.h"
#include "NormalProp.h"

#include "OutLineEffect.h"


PlayLevel* PlayLevel::s_MainPlayLevel = nullptr;
std::unique_ptr<TimeManager> PlayLevel::s_TimeManager;
std::unique_ptr<AlertManager> PlayLevel::s_AlertManager;
PlayLevel::PlayLevel()
{
	CreateCamera(INT_MIN, ECAMERAORDER::MainPrev);
}

PlayLevel::~PlayLevel()
{

}

void PlayLevel::Start()
{
	ContentsLevel::Start();

	s_MainPlayLevel = this;

	Player = CreateActor<Ellie>(EUPDATEORDER::Player);

	UIManagerPtr = CreateActor<UIManager>(EUPDATEORDER::UIMagnaer);
	UIManagerPtr->Init();

	if (nullptr == s_TimeManager)
	{
		s_TimeManager = std::make_unique<TimeManager>();
	}

	if (nullptr == s_AlertManager)
	{
		s_AlertManager = std::make_unique<AlertManager>();
	}

	OutLinePtr = GetMainCamera()->GetCameraAllRenderTarget()->CreateEffect<OutLineEffect>();
}

void PlayLevel::Update(float _Delta)
{
	ContentsLevel::Update(_Delta);

	if (nullptr != s_TimeManager)
	{
		s_TimeManager->Update(_Delta);
	}

	if (nullptr != s_AlertManager)
	{
		s_AlertManager->Update(_Delta);
	}

	if (true == GameEngineInput::IsDown(VK_F1, this))
	{
		IsDebug = !IsDebug;
	}
}

void PlayLevel::LevelStart(GameEngineLevel* _NextLevel)
{
	ContentsLevel::LevelStart(_NextLevel);

	s_MainPlayLevel = this;
	PrevLevelName = _NextLevel->GetName();

	if (false == LocationKRName.empty())
	{
		s_AlertManager->RegisterAlert(AlertData(LocationKRName, EALERTTYPE::Enter));
	}
}


PlayLevel* PlayLevel::GetCurLevel()
{
	if (nullptr == s_MainPlayLevel)
	{
		MsgBoxAssert("메인 레벨 포인터가 존재하지 않습니다.");
		return nullptr;
	}

	return s_MainPlayLevel;
}

std::shared_ptr<class UIManager> PlayLevel::GetUIManagerPtr() const
{
	if (nullptr == UIManagerPtr)
	{
		MsgBoxAssert("UI매니저가 존재하지 않습니다.");
		return nullptr;
	}

	return UIManagerPtr;
}

std::shared_ptr<class Ellie> PlayLevel::GetPlayerPtr() const
{
	if (nullptr == Player)
	{
		MsgBoxAssert("UI매니저가 존재하지 않습니다.");
		return nullptr;
	}

	return Player;
}

std::shared_ptr<class BackDrop_PlayLevel> PlayLevel::GetBackDropPtr() const
{
	if (nullptr == Back)
	{
		return nullptr;
	}

	return Back;
}

std::shared_ptr<class OutLineEffect> PlayLevel::GetOutLinePtr() const
{
	if (nullptr == OutLinePtr)
	{
		return nullptr;
	}

	return OutLinePtr;
}

std::shared_ptr<class SkyLerp> PlayLevel::GetSkyPtr() const
{
	if (nullptr == SkyPtr)
	{
		return nullptr;
	}

	return SkyPtr;
}