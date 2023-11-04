#pragma once
#include "ContentsLevel.h"

// ���� :
class TestLevel : public ContentsLevel
{
public:
	// constrcuter destructer
	TestLevel();
	~TestLevel();

	// delete Function
	TestLevel(const TestLevel& _Other) = delete;
	TestLevel(TestLevel&& _Other) noexcept = delete;
	TestLevel& operator=(const TestLevel& _Other) = delete;
	TestLevel& operator=(TestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
	void LevelStart(class GameEngineLevel* _NextLevel) override;
	void LevelEnd(class GameEngineLevel* _NextLevel) override;

private:
	std::shared_ptr<class Ellie> Player = nullptr;
	std::shared_ptr<class UIManager> UI = nullptr;
	std::shared_ptr<class Bush> m_bush = nullptr;

	std::shared_ptr<GameEngineActor> Map = nullptr;

};