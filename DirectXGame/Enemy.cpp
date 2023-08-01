#include "Enemy.h"

void Enemy::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	for (auto& transform : partTransforms_) {
		transform.Initialize();
	}
	transform_.translation_ = {0.0f, 0.0f, 15.0f};
	transform_.scale_ = {1.0f, 1.0f, 1.0f};

	hingeTransform_.translation_ = {0.0f, 1.0f, 0.875f};
	hingeTransform_.parent_ = &transform_;

	partTransforms_[kBody].translation_ = {0.0f, 0.0f, 0.0f};
	partTransforms_[kBody].parent_ = &hingeTransform_;
	partTransforms_[kHead].translation_ = {0.0f, 0.0f, 0.0f};
	partTransforms_[kHead].parent_ = &hingeTransform_;

	InitializeAnimation();
	UpdateTransforms();
}

void Enemy::Update() {
	UpdateTransforms();

	transform_.rotation_.y += 1.0f * Math::ToRadian;
	transform_.rotation_.y = std::fmod(transform_.rotation_.y, Math::TwoPi);

	Vector3 move = {0.0f, 0.0f, 1.0f};
	move = TransformNormal(move, MakeRotateYMatrix(transform_.rotation_.y));
	transform_.translation_ += move * 0.1f;

	UpdateAnimation();

	UpdateTransforms();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	for (uint32_t i = 0; i < kPartsCount; ++i) {
		models_[i]->Draw(partTransforms_[i], viewProjection);
	}
}

void Enemy::UpdateTransforms() {
	transform_.UpdateWorldMatrix();
	hingeTransform_.UpdateWorldMatrix();
	for (auto& transform : partTransforms_) {
		transform.UpdateWorldMatrix();
	}
}

void Enemy::InitializeAnimation() { animationParameter_ = 0.0f; }

void Enemy::UpdateAnimation() {
	// 開けて閉じる周期
	const uint16_t cycle = 240;
	// 1フレーム変化量
	const float delta = Math::TwoPi / cycle;

	animationParameter_ += delta;
	animationParameter_ = std::fmod(animationParameter_, Math::TwoPi);

	// 開閉最大角
	const float openLimitAngle = -45.0f * Math::ToRadian;

	animationParameter_ += delta;
	float triangleWave = std::abs(std::sin(animationParameter_));	
	partTransforms_[kHead].rotation_.x = Math::Lerp(0.0f, openLimitAngle, triangleWave);
}
