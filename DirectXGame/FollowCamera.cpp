#include "FollowCamera.h"

#include "MyMath.h"
#include "Input.h"

void FollowCamera::Initialize() { viewProjection_.rotation_.x += 5.0f * Math::ToRadian; }

void FollowCamera::Update() {
	auto input = Input::GetInstance();


	XINPUT_STATE state{};
	if (input->GetJoystickState(0, state)) {
		const float rotateSpeed = 3.0f * Math::ToRadian;
		viewProjection_.rotation_.y += float(state.Gamepad.sThumbRX) / float(SHORT_MAX) * rotateSpeed;
	}

	if (target_) {
		Vector3 offset = {0.0f, 3.0f, -10.0f};
		Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(viewProjection_.rotation_);
		offset = TransformNormal(offset, rotateMatrix);

		viewProjection_.translation_ = target_->translation_ + offset;
	}


	viewProjection_.UpdateViewMatrix();
	viewProjection_.UpdateProjectionMatrix();
}
