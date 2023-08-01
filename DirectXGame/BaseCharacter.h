#pragma once

#include <vector>

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "MyMath.h"

class BaseCharacter {
public:
	virtual ~BaseCharacter() {}
	virtual void Initialize(const std::vector<Model*>& models);
	virtual void Update() {}
	virtual void Draw(const ViewProjection& viewProjection) { viewProjection; }

	const WorldTransform& GetWorldTransform() const { return transform_; }
	Vector3 GetWorldPosition() const { return GetTranslate(transform_.matWorld_); }

protected:
	std::vector<Model*> models_;
	WorldTransform transform_;
};