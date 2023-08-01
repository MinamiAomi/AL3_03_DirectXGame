#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter {
public:
	enum Parts {
		kHead,
		kBody,

		kPartsCount
	};

	void Initialize(const std::vector<Model*>& models) override;
	void Update() override;
	void Draw(const ViewProjection& viewProjection) override;

private:
	void UpdateTransforms();
	void InitializeAnimation();
	void UpdateAnimation();

	WorldTransform hingeTransform_;
	WorldTransform partTransforms_[kPartsCount];

	float animationParameter_;
};
