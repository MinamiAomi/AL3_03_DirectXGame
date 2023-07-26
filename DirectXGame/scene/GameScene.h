#pragma once

#include <memory>

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "DebugCamera.h"
#include "Player.h"
#include "Skydome.h"
#include "Ground.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// リソース
	/// </summary>
	std::unique_ptr<Model> cubeModel_;
	std::unique_ptr<Model> skydomeModel_;
	std::unique_ptr<Model> groundModel_;
	
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	ViewProjection camera_;

	std::unique_ptr<DebugCamera> debugCamera_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Ground> ground_;
};
