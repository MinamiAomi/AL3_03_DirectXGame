#include "GameScene.h"

#include <cassert>

#include "TextureManager.h"
#include "WinApp.h"


GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	cubeModel_.reset(Model::Create());
	skydomeModel_.reset(Model::CreateFromOBJ("skydome"));
	groundModel_.reset(Model::CreateFromOBJ("ground"));

	camera_.Initialize();

	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);

	player_ = std::make_unique<Player>();
	player_->Initialize(cubeModel_.get());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());
}

void GameScene::Update() {
	debugCamera_->Update();
	camera_.matView = debugCamera_->GetViewProjection().matView;
	camera_.matProjection = debugCamera_->GetViewProjection().matProjection;
	camera_.TransferMatrix();
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
	player_->Draw(camera_);
	skydome_->Draw(camera_);
	ground_->Draw(camera_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

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
