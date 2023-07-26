#include "Ground.h"

#include <cassert>

void Ground::Initialize(Model* model) {
	assert(model);

	model_ = model;
	transform_.Initialize();
}

void Ground::Update() {}

void Ground::Draw(const ViewProjection& viewProjection) {
	model_->Draw(transform_, viewProjection);
}
