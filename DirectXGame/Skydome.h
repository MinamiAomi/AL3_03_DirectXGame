#pragma once

#include <memory>

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Skydome {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	Model* model_ = nullptr;
	WorldTransform transform_;
};