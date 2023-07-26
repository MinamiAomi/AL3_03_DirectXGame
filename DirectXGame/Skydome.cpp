#include "Skydome.h"

#include <cassert>

void Skydome::Initialize(Model* model) {
	assert(model);

	model_ = model;
	transform_.Initialize();
}

void Skydome::Update() {}

void Skydome::Draw(const ViewProjection& viewProjection) {
	model_->Draw(transform_, viewProjection);
}
