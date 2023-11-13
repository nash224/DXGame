#include "PreCompile.h"
#include "FlowerBird.h"

FlowerBird::FlowerBird() 
{
	if (nullptr == GameEngineSound::FindSound("SFX_FlowerBirdBloom_01.wav"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExistsChild("Resources");
		Dir.MoveChild("Resources\\Sound\\Actor\\FlowerBird");
		std::vector<GameEngineFile> Files = Dir.GetAllFile();
		for (GameEngineFile& pfile : Files)
		{
			GameEngineSound::SoundLoad(pfile.GetStringPath());
		}
	}
}

FlowerBird::~FlowerBird() 
{
}


// 꽃새 행동 
// 기본적으로 서있고(Idle), 움직이지 않음(Position 값이 바뀌지 않음)
// 몸 방향을 바꿈 (Turn)
// Turn할때 연속으로 1,2,4 번 바꿀때가 있음
// 바닥을 쫄땐 2번씩 2번

// 몸을 자주바꾸고 바닥을 쪼는 행동과 꽃을 피우는 행동은 낮음
// 행동을 바꾸는건 Idle에서 진행되고 비율은 1:1:1:1:1 비율로함

// 꽃을 피울때, 꽃이 빨리 시드는 행동과 꽃이 잠깐동안 완전히 핀 행동이 있음
// 빨리 시들땐, 봉우리까지 잠깐 텀이 있다가 꽃을 피운다음 시듬
// 꽃이 시들때 0.5~2s 까지 대기

// 낮은 확률(1/3 정도?)로 꽃이 완전히 피게되는데 => 수정 동일한 비율로 변경
// 꽃이 완전히 필땐, 1초동안 피고 다시 다른 행동


// 포션과 상호작용해서 꽃을 여러장 떨어뜨리는 행동이 있음
// 추후


void FlowerBird::Start()
{
	DynamicEntity::Start();

	SetInteractionOption(EINTERACTION_BUTTONTYPE::Gathering, EINTERACTION_TYPE::Far, ECOLLECTION_METHOD::None, ETOOLTYPE::Gloves);
	CreateAndSetCollision(ECOLLISION::Entity, { 300.0f }, float4::ZERO, ColType::SPHERE2D);
}

void FlowerBird::Update(float _Delta)
{
	DynamicEntity::Update(_Delta);

	UpdateState(_Delta);
}

void FlowerBird::Release()
{
	DynamicEntity::Release();

	m_Shadow = nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


void FlowerBird::Init()
{
	ApplyDepth(Transform.GetLocalPosition());
	AnimationSetting();
	DirectionSetting();
	ChangeState(EFLOWERBIRDSTATE::Idle);
}

void FlowerBird::AnimationSetting()
{
	if (nullptr == GameEngineSprite::Find("FlowerBird_Bloom.png"))
	{
		// FlowerBird
		GameEngineSprite::CreateCut("FlowerBird_Bloom.png", 4, 3);
		GameEngineSprite::CreateCut("FlowerBird_BloomB.png", 5, 5);
		GameEngineSprite::CreateCut("FlowerBird_BloomC.png", 4, 3);
		GameEngineSprite::CreateCut("FlowerBird_Fly.png", 3, 2);
		GameEngineSprite::CreateCut("FlowerBird_FlyB.png", 3, 2);
		GameEngineSprite::CreateCut("FlowerBird_IdleA.png", 3, 2);
		GameEngineSprite::CreateCut("FlowerBird_IdleB.png", 4, 3);
		GameEngineSprite::CreateCut("FlowerBird_IdleC.png", 3, 2);
		GameEngineSprite::CreateCut("FlowerBird_Standing.png", 2, 2);
	}

	static constexpr const int RenderOrder = 0;

	m_Body = CreateComponent<GameEngineSpriteRenderer>(RenderOrder);
	m_Body->CreateAnimation("Idle", "FlowerBird_Standing.png", 5.0f, 2, 2, false);
	m_Body->CreateAnimation("Turn", "FlowerBird_IdleA.png", 0.03f, 3, 5, false);
	m_Body->CreateAnimation("Pick", "FlowerBird_IdleC.png", 0.08f, 4, 5, false);

	m_Body->CreateAnimation("Bloom", "FlowerBird_Bloom.png", FlowerBirdBloomInter, 2, 10, false);
	m_Body->FindAnimation("Bloom")->Inter[2] = FlowerBirdPeaksInter;
	m_Body->FindAnimation("Bloom")->Inter[8] = FlowerBirdBlossomInter;

	m_Body->CreateAnimation("BloomFake", "FlowerBird_BloomC.png", FlowerBirdBloomInter, 2, 10, false);
	m_Body->FindAnimation("BloomFake")->Inter[2] = FlowerBirdPeaksInter;

	m_Body->CreateAnimation("Fly", "FlowerBird_Fly.png", 0.06f, 1, 4, true);
	m_Body->CreateAnimation("Fly_Bloom", "FlowerBird_FlyB.png", 0.06f, 1, 4, true);
	m_Body->Transform.AddLocalPosition({ 0.0f,30.0f });
	m_Body->AutoSpriteSizeOn();

	m_Shadow = CreateComponent<GameEngineSpriteRenderer>(RenderOrder);
	m_Shadow->SetSprite("FlowerBird_Standing.png", 1);
	m_Shadow->Transform.AddLocalPosition({ 0.0f,30.0f });



	m_Body->SetStartEvent("Turn", [&](GameEngineSpriteRenderer* _Renderer)
		{
			PlaySFX(RandomBirdCrySoundName());
		});

	m_Body->SetFrameEvent("Pick", 4, [&](GameEngineSpriteRenderer* _Renderer)
		{
			m_Shadow->SetSprite("FlowerBird_IdleC.png", 2);
		});

	m_Body->SetFrameEvent("Bloom", 6, [&](GameEngineSpriteRenderer* _Renderer)
		{
			PlaySFX("SFX_FlowerBirdBloom_01.wav");
		});

	m_Body->SetEndEvent("Pick", [&](GameEngineSpriteRenderer* _Renderer)
		{
			m_Shadow->SetSprite("FlowerBird_Standing.png", 1);
		});


	m_Body->SetStartEvent("Fly", [&](GameEngineSpriteRenderer* _Renderer)
		{
			PlaySFX("SFX_BirdFly_01.wav");

			m_Shadow->Off();
		});

	m_Body->SetStartEvent("Fly_Bloom", [&](GameEngineSpriteRenderer* _Renderer)
		{
			m_Shadow->Off();
		});
}


void FlowerBird::DirectionSetting()
{
	GameEngineRandom RandomClass;
	RandomClass.SetSeed(reinterpret_cast<__int64>(this) + GlobalValue::GetSeedValue());
	int Value = RandomClass.RandomInt(0, 1);

	switch (Value)
	{
	case 0:
		m_Dir = EDIRECTION::LEFT;
		break;
	case 1:
		m_Dir = EDIRECTION::RIGHT;
		break;
	default:
		break;
	}
}

void FlowerBird::Leave()
{
	ChangeState(EFLOWERBIRDSTATE::Fly);
}

void FlowerBird::UpdateState(float _Delta)
{
	switch (m_State)
	{
	case EFLOWERBIRDSTATE::Idle:						UpdateIdle(_Delta); 									break;
	case EFLOWERBIRDSTATE::Turn:						UpdateTurn(_Delta); 									break;
	case EFLOWERBIRDSTATE::Pick:						UpdatePick(_Delta); 									break;
	case EFLOWERBIRDSTATE::Bloom:						UpdateBloom(_Delta); 									break;
	case EFLOWERBIRDSTATE::BloomFake:					UpdateBloomFake(_Delta); 								break;
	case EFLOWERBIRDSTATE::BloomFlowers:				UpdateBloomFlowers(_Delta); 							break;
	case EFLOWERBIRDSTATE::Fly:							UpdateFly(_Delta); 										break;
	case EFLOWERBIRDSTATE::None:
		break;
	default:
		break;
	}
}

void FlowerBird::ChangeState(EFLOWERBIRDSTATE _State)
{
	if (_State != m_State)
	{
		switch (m_State)
		{
		case EFLOWERBIRDSTATE::Idle:					EndIdle(); 												break;
		case EFLOWERBIRDSTATE::Turn:					EndTurn(); 												break;
		case EFLOWERBIRDSTATE::Pick:					EndPick();												break;
		case EFLOWERBIRDSTATE::Bloom:					EndBloom(); 											break;
		case EFLOWERBIRDSTATE::BloomFake:				EndBloomFake(); 										break;
		case EFLOWERBIRDSTATE::BloomFlowers:			EndBloomFlowers(); 										break;
		case EFLOWERBIRDSTATE::Fly:							 													break;
		case EFLOWERBIRDSTATE::None:
			break;
		default:
			break;
		}

		switch (_State)
		{
		case EFLOWERBIRDSTATE::Idle:					StartIdle(); 											break;
		case EFLOWERBIRDSTATE::Turn:					StartTurn(); 											break;
		case EFLOWERBIRDSTATE::Pick:					StartPick(); 											break;
		case EFLOWERBIRDSTATE::Bloom:					StartBloom(); 											break;
		case EFLOWERBIRDSTATE::BloomFake:				StartBloomFake(); 										break;
		case EFLOWERBIRDSTATE::BloomFlowers:			StartBloomFlowers(); 									break;
		case EFLOWERBIRDSTATE::Fly:						StartFly(); 											break;
		case EFLOWERBIRDSTATE::None:
		{
			MsgBoxAssert("해당 행동으로 변환할 수 없습니다.");
			return;
		}
			break;
		default:
			break;
		}

		m_State = _State;
	}
}

void FlowerBird::ChangeFlowerBirdAnimation(std::string_view _AnimationName)
{
	if (nullptr == m_Body)
	{
		MsgBoxAssert("렌더러가 존재하지 않습니다.");
		return;
	}

	if (nullptr == m_Shadow)
	{
		MsgBoxAssert("렌더러가 존재하지 않습니다.");
		return;
	}

	if (EDIRECTION::LEFT == m_Dir)
	{
		m_Shadow->LeftFlip();
		m_Body->LeftFlip();
	}

	if (EDIRECTION::RIGHT == m_Dir)
	{
		m_Shadow->RightFlip();
		m_Body->RightFlip();
	}

	m_Body->ChangeAnimation(_AnimationName);
}

std::string FlowerBird::RandomBirdCrySoundName()
{
	GameEngineRandom RandomClass;
	int SelectValue = RandomClass.RandomInt(2, 4);

	std::string_view FileName;

	switch (SelectValue)
	{
	case 2:
		FileName = "SFX_BirdCrying_02.wav";
		break;
	case 3:
		FileName = "SFX_BirdCrying_03.wav";
		break;
	case 4:
		FileName = "SFX_BirdCrying_04.wav";
		break;
	default:
		break;
	}

	return FileName.data();
}