#pragma once
#include "BaseCharacter.h"

#include <memory>
#include <optional>

#include "Model.h"
#include "WorldTransform.h"

class Player : public BaseCharacter {
public:
	enum Parts {
		kBody,
		kHead,
		kArm_R,
		kArm_L,
		kHammer,

		kPartsCount
	};

	enum class Behavior {
		Root,
		Attack
	};

	void Initialize(const std::vector<Model*>& models) override;
	void Update() override;
	void Draw(const ViewProjection& viewProjection) override;

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	void UpdateTransforms();
	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();
	void InitializeBehaviorRoot();
	void InitializeBehaviorAttack();
	void UpdateBehaviorRoot();
	void UpdateBehaviorAttack();

	void RegisterGlobalVariables();
	void ApplyGlobalVariables();

	std::array<WorldTransform, kPartsCount> partTransforms_;
	WorldTransform weaponTransform_;

	
	Behavior behavior_ = Behavior::Root;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	float floatingParameter_;
	int32_t attackParameter_;
	float attackDirection_;

	const ViewProjection* viewProjection_;
	
	float floatingCycle_ = 120;
	float floatingAmplitude_ = 0.3f;
	float floatingOffset_ = 0.5f;

	int32_t attackTime_ = 20;
	int32_t postAttackTime_ = 10;

};
