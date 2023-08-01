#include "GameScene.h"

#include <cassert>

#include "AxisIndicator.h"
#include "ModelContainer.h"
#include "TextureManager.h"
#include "WinApp.h"

#include "GlobalVariables.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	modelContainer_ = ModelContainer::GetInstance();
	modelContainer_->LoadCube();
	modelContainer_->LoadModel("skydome");
	modelContainer_->LoadModel("ground");
	modelContainer_->LoadModel("head");
	modelContainer_->LoadModel("body");
	modelContainer_->LoadModel("arm_r");
	modelContainer_->LoadModel("arm_l");
	modelContainer_->LoadModel("hammer");
	modelContainer_->LoadModel("mimic_head");
	modelContainer_->LoadModel("mimic_body");

	sceneCamera_.Initialize();

	axisIndicator_ = AxisIndicator::GetInstance();
	axisIndicator_->Initialize();
	axisIndicator_->SetTargetViewProjection(&sceneCamera_);
	axisIndicator_->SetVisible(true);

	player_ = std::make_unique<Player>();
	enemy_ = std::make_unique<Enemy>();
	skydome_ = std::make_unique<Skydome>();
	ground_ = std::make_unique<Ground>();
	followCamera_ = std::make_unique<FollowCamera>();
	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);

	player_->Initialize(
	    {modelContainer_->Find("body"), modelContainer_->Find("head"),
	     modelContainer_->Find("arm_r"), modelContainer_->Find("arm_l"),
	     modelContainer_->Find("hammer")});
	player_->SetViewProjection(&followCamera_->GetViewProjection());
	enemy_->Initialize({modelContainer_->Find("mimic_head"), modelContainer_->Find("mimic_body")});
	skydome_->Initialize(modelContainer_->Find("skydome"));
	ground_->Initialize(modelContainer_->Find("ground"));
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());

}

void GameScene::Update() {

	player_->Update();
	followCamera_->Update();
	enemy_->Update();

	if (useDebugCamera) {
		debugCamera_->Update();
		sceneCamera_.matView = debugCamera_->GetViewProjection().matView;
		sceneCamera_.matProjection = debugCamera_->GetViewProjection().matProjection;
	} else {
		sceneCamera_.matView = followCamera_->GetViewProjection().matView;
		sceneCamera_.matProjection = followCamera_->GetViewProjection().matProjection;
	}
	sceneCamera_.TransferMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(sceneCamera_);
	enemy_->Draw(sceneCamera_);
	skydome_->Draw(sceneCamera_);
	ground_->Draw(sceneCamera_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

	axisIndicator_->Draw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
