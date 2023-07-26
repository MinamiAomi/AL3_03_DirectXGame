#include "Player.h"

#include <cassert>

void Player::Initialize(Model* model) {
	assert(model);

	model_ = model;
	transform_.Initialize();
}

void Player::Update() {}

void Player::Draw(const ViewProjection& viewProjection) { 
	model_->Draw(transform_, viewProjection);
}
