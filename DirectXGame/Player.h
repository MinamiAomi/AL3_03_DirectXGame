#pragma once

#include <memory>

#include "Model.h"
#include "WorldTransform.h"

class Player {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	Model* model_;
	WorldTransform transform_;
};
