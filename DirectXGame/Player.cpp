#include "Player.h"

#include <cassert>

#include "Input.h"
#include "MyMath.h"

#include "GlobalVariables.h"
#include "ImGuiManager.h"
#include "ModelContainer.h"

void Player::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	for (auto& transform : partTransforms_) {
		transform.Initialize();
	}
	weaponTransform_.Initialize();

	transform_.translation_ = {0.0f, 0.0f, 0.0f};
	transform_.scale_ = {0.5f, 0.5f, 0.5f};

	partTransforms_[kBody].translation_ = {0.0f, 0.0f, 0.0f};
	partTransforms_[kBody].parent_ = &transform_;
	partTransforms_[kHead].translation_ = {0.0f, 2.8f, 0.0f};
	partTransforms_[kHead].parent_ = &partTransforms_[kBody];
	partTransforms_[kArm_R].translation_ = {0.9f, 2.5f, 0.0f};
	partTransforms_[kArm_R].parent_ = &partTransforms_[kBody];
	partTransforms_[kArm_L].translation_ = {-0.9f, 2.5f, 0.0f};
	partTransforms_[kArm_L].parent_ = &partTransforms_[kBody];
	weaponTransform_.translation_ = {0.0f, 3.0f, 0.0f};
	weaponTransform_.scale_ = {1.5f, 1.5f, 1.5f};
	weaponTransform_.parent_ = &partTransforms_[kBody];
	UpdateTransforms();

	behaviorRequest_ = Behavior::Root;

	RegisterGlobalVariables();
}

void Player::Update() {
	UpdateTransforms();
	ApplyGlobalVariables();

	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		default:
		case Player::Behavior::Root:
			InitializeBehaviorRoot();
			break;
		case Player::Behavior::Attack:
			InitializeBehaviorAttack();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	default:
	case Player::Behavior::Root:
		UpdateBehaviorRoot();
		break;
	case Player::Behavior::Attack:
		UpdateBehaviorAttack();
		break;
	}

	UpdateTransforms();
}

void Player::Draw(const ViewProjection& viewProjection) {
	models_[kBody]->Draw(partTransforms_[kBody], viewProjection);
	models_[kHead]->Draw(partTransforms_[kHead], viewProjection);
	models_[kArm_R]->Draw(partTransforms_[kArm_R], viewProjection);
	models_[kArm_L]->Draw(partTransforms_[kArm_L], viewProjection);

	models_[kHammer]->Draw(weaponTransform_, viewProjection);
}

void Player::UpdateTransforms() {
	transform_.UpdateWorldMatrix();
	for (auto& transform : partTransforms_) {
		transform.UpdateWorldMatrix();
	}
	weaponTransform_.UpdateWorldMatrix();
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	// 1フレーム変化量
	const float delta = Math::TwoPi / floatingCycle_;

	floatingParameter_ += delta;
	floatingParameter_ = std::fmod(floatingParameter_, Math::TwoPi);

	// 浮遊の振れ幅
	float sin = std::sin(floatingParameter_);
	partTransforms_[kBody].translation_.y = sin * floatingAmplitude_ + floatingOffset_;

	float armRotate =
	    Math::Lerp(-45.0f * Math::ToRadian, 45.0f * Math::ToRadian, sin * 0.5f + 0.5f);
	partTransforms_[kArm_L].rotation_.x = armRotate;
	partTransforms_[kArm_R].rotation_.x = armRotate;
}

void Player::InitializeBehaviorRoot() {
	InitializeFloatingGimmick();
	weaponTransform_.translation_ = {5.0f, 7.0f, -2.0f};
	weaponTransform_.rotation_ = {0.0f, Math::HalfPi, 130.0f * Math::ToRadian};
}

void Player::InitializeBehaviorAttack() {
	attackDirection_ = transform_.rotation_.y;
	attackParameter_ = 0;
	weaponTransform_.translation_ = {0.0f, 3.0f, 0.0f};
	weaponTransform_.rotation_ = {-20.0f * Math::ToRadian, 0.0f, 0.0f};
	partTransforms_[kArm_R].rotation_.x = Math::Pi;
	partTransforms_[kArm_L].rotation_.x = Math::Pi;
}

void Player::UpdateBehaviorRoot() {
	Input* input = Input::GetInstance();
	const float speed = 0.3f;
	Vector3 move{};

	XINPUT_STATE state{};
	if (input->GetJoystickState(0, state)) {
		const float margin = 0.8f;
		move = {float(state.Gamepad.sThumbLX), 0.0f, float(state.Gamepad.sThumbLY)};
		if (Length(move) < margin) {
			move = Vector3Zero;
		}
	}
	if (input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_W) ||
	    input->PushKey(DIK_S)) {
		move = Vector3Zero;
		move.x += input->PushKey(DIK_D) ? 1.0f : 0.0f;
		move.x += input->PushKey(DIK_A) ? -1.0f : 0.0f;
		move.z += input->PushKey(DIK_W) ? 1.0f : 0.0f;
		move.z += input->PushKey(DIK_S) ? -1.0f : 0.0f;
	};

	if (move != Vector3Zero) {
		move = Normalize(move) * speed;
		const Matrix4x4 cameraRotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		move = TransformNormal(move, cameraRotateMatrix);
		transform_.translation_ += move;

		transform_.rotation_.y =
		    Math::LerpShortAngle(transform_.rotation_.y, std::atan2(move.x, move.z), 0.1f);
	}

	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		behaviorRequest_ = Behavior::Attack;
	}

	UpdateFloatingGimmick();
}

void Player::UpdateBehaviorAttack() {
	const float speed = 0.3f;
	const float swingAngle = 130.0f * Math::ToRadian;

	if (attackParameter_++ >= attackTime_ + postAttackTime_) {
		behaviorRequest_ = Behavior::Root;
		return;
	}

	if (attackParameter_ <= attackTime_) {
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;
		float t = attackParameter_ / float(attackTime_);
		t = c3 * t * t * t - c1 * t * t;

		weaponTransform_.rotation_.x = Math::Lerp(0.0f, swingAngle, t) - 40.0f * Math::ToRadian;
		partTransforms_[kArm_L].rotation_.x =
		    Math::Lerp(0.0f, swingAngle, t) - 210.0f * Math::ToRadian;
		partTransforms_[kArm_R].rotation_.x =
		    Math::Lerp(0.0f, swingAngle, t) - 210.0f * Math::ToRadian;

		const Matrix4x4 cameraRotateMatrix = MakeRotateYMatrix(attackDirection_);
		Vector3 forward = TransformNormal(Vector3UnitZ, cameraRotateMatrix);
		transform_.translation_ += forward * speed;
	}
}

constexpr char kGroupName[] = "Player";

void Player::RegisterGlobalVariables() {
	GlobalVariables& v = *GlobalVariables::GetInstance();
	if (!v.HasGroup(kGroupName)) {
		auto& group = v[kGroupName];
		group["Head Translation"] = partTransforms_[kHead].translation_;
		group["ArmR Translation"] = partTransforms_[kArm_R].translation_;
		group["ArmL Translation"] = partTransforms_[kArm_L].translation_;
		group["Floating Cycle"] = floatingCycle_;
		group["Floating Amplitude"] = floatingAmplitude_;
		group["Floating Offset"] = floatingOffset_;
		group["Attack Time"] = attackTime_;
		group["Post Attack Time"] = postAttackTime_;
	}
}

void Player::ApplyGlobalVariables() {
	GlobalVariables& v = *GlobalVariables::GetInstance();
	auto& group = v[kGroupName];
	partTransforms_[kHead].translation_ = group["Head Translation"].Get<Vector3>();
	partTransforms_[kArm_R].translation_ = group["ArmR Translation"].Get<Vector3>();
	partTransforms_[kArm_L].translation_ = group["ArmL Translation"].Get<Vector3>();
	floatingCycle_ = group["Floating Cycle"].Get<float>();
	floatingAmplitude_ = group["Floating Amplitude"].Get<float>();
	floatingOffset_ = group["Floating Offset"].Get<float>();
	attackTime_ = group["Attack Time"].Get<int32_t>();
	postAttackTime_ = group["Post Attack Time"].Get<int32_t>();
}
