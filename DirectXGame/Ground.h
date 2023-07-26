#pragma once

#include <memory>

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Ground {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	Model* model_;
	WorldTransform transform_;
};