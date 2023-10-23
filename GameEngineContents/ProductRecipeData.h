#pragma once
#include "ContentsResource.h"


// 설명 : 재료가공 레시피 데이터입니다.
class ProductRecipeData final : public ContentsResource<ProductRecipeData>
{
public:
	// constrcuter destructer
	ProductRecipeData();
	~ProductRecipeData();


	ProductRecipeData(
		std::string _ProductName = "",
		std::string _KoreanName = "",
		EBREWING_DIFFICULTY _Star = EBREWING_DIFFICULTY::Hard,
		EBREWING_DIRECTION _Ladle = EBREWING_DIRECTION::StirNone,
		EBREWING_FIRE _Fire = EBREWING_FIRE::Three,
		std::string _Material1 = "",
		unsigned int _Material1Count = 0,
		std::string _Material2 = "",
		unsigned int _Material2Count = 0,
		std::string _Material3 = "",
		unsigned int _Material3Count = 0
	)
		:
		ProductName(_ProductName),
		KoreanName(_KoreanName),
		Star(_Star),
		Ladle(_Ladle),
		Fire(_Fire),
		Material1(_Material1),
		Material1Count(_Material1Count),
		Material2(_Material2),
		Material2Count(_Material2Count),
		Material3(_Material3),
		Material3Count(_Material3Count)
	{

	}


	// 레시피가 오른쪽에 와야합니다.
	bool operator==(const ProductRecipeData* _Other) const
	{
		if (_Other->Ladle != Ladle)
		{
			return false;
		}

		if (_Other->Fire != Fire)
		{
			return false;
		}

		if (_Other->Material1 != Material1)
		{
			return false;
		}

		if (_Other->Material1Count > Material1Count)
		{
			return false;
		}

		if (_Other->Material2 != Material2)
		{
			return false;
		}

		if (_Other->Material2Count > Material2Count)
		{
			return false;
		}

		if (_Other->Material3 != Material3)
		{
			return false;
		}

		if (_Other->Material3Count > Material3Count)
		{
			return false;
		}

		return true;
	}

	// delete Function
	//ProductRecipeData(const ProductRecipeData& _Other) = delete;
	//ProductRecipeData(ProductRecipeData&& _Other) noexcept = delete;
	//ProductRecipeData& operator=(const ProductRecipeData& _Other) = delete;
	//ProductRecipeData& operator=(ProductRecipeData&& _Other) noexcept = delete;

protected:

public:
	std::string ProductName;
	std::string KoreanName;
	EBREWING_DIFFICULTY Star = EBREWING_DIFFICULTY::Easy;
	EBREWING_DIRECTION Ladle = EBREWING_DIRECTION::StirNone;
	EBREWING_FIRE Fire = EBREWING_FIRE::Three;
	std::string Material1;
	unsigned int Material1Count;
	std::string Material2;
	unsigned int Material2Count;
	std::string Material3;
	unsigned int Material3Count;
};

