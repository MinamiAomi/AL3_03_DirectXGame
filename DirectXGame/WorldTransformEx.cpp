#include "WorldTransform.h"

#include "MyMath.h"

void WorldTransform::UpdateWorldMatrix() {
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	
	if (parent_) {
		matWorld_ *= parent_->matWorld_;
	}
	if (constBuffer_) {
		TransferMatrix();
	}
}