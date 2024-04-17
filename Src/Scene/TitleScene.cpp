#include <cmath>
#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "TitleScene.h"
#include "../Manager/Camera.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Shot.h"
#include "../Object/Enemy.h"
#include "../Object/Grid.h"
#include "../Object/RollBall.h"

TitleScene::TitleScene(void) : SceneBase()
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{

	// グリッド線の初期化
	grid_ = new Grid();
	grid_->Init();

	// ステージの初期化
	stage_ = new Stage();
	stage_->Init();

	// プレイヤーの初期化
	player_ = new Player();
	player_->Init();

	// エネミーの初期化
	enemy_ = new Enemy(player_);
	enemy_->Init();

	// ボールの初期化
	rollBall_ = new RollBall(player_);
	rollBall_->Init();

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(player_);
	camera->ChangeMode(Camera::MODE::FOLLOW);

}

void TitleScene::Update(void)
{

	// グリッド線の更新
	grid_->Update();

	// ステージの更新
	stage_->Update();

	// プレイヤーの更新
	player_->Update();

	// ステージモデルID
	int stageModelId = stage_->GetModelId();

	// 弾の更新
	auto shots = player_->GetShots();
	for (auto shot : shots)
	{

		shot->Update();

		// 弾がSHOT状態じゃなかったら衝突判定しない
		if (!shot->IsShot())
		{
			// 爆発中や処理終了後は、以降の処理は実行しない
			continue;
		}

		// ステージモデルとの衝突判定
		auto info = MV1CollCheck_Sphere(
			stageModelId, -1, shot->GetPos(), shot->GetCollisionRadius());

		if (info.HitNum > 0)
		{
			// 衝突している
			shot->Blast();
		}

		MV1CollResultPolyDimTerminate(info);

		// 弾と敵の衝突判定
		if (AsoUtility::IsHitSphereCapsule(
			shot->GetPos(), shot->GetCollisionRadius(),
			VAdd(enemy_->GetPos(), { 0.0f,150.0f,0.0f }), VAdd(enemy_->GetPos(), {0.0f,30.0f,0.0f}),enemy_->GetCollisionRadius()))
		{
			// 敵にダメージを与える
			shot->Blast();
		}

	}
	// エネミーの更新
	enemy_->Update();

	// ボールの更新
	rollBall_->Update();

}

void TitleScene::Draw(void)
{

	// グリッド線の描画
	grid_->Draw();

	// ステージの描画
	stage_->Draw();

	// プレイヤーの描画
	player_->Draw();

	// 弾の更新
	auto shots = player_->GetShots();
	for (auto shot : shots)
	{
		if (shot->IsAlive())
		{
			shot->Draw();
		}


		//DrawSphere3D(shot->GetPos(), shot->GetCollisionRadius(),
		//	10, 0xff0000, 0xff0000, false);

	}

	// エネミーの描画
	enemy_->Draw();

	// ボールの更新
	rollBall_->Draw();

	// デバッグ描画
	DrawDebug();

}

void TitleScene::DrawDebug(void)
{

	// カメラ情報を取得
	Camera* camera = SceneManager::GetInstance().GetCamera();
	VECTOR pos = camera->GetPos();
	VECTOR angles = camera->GetAngles();

	DrawFormatString(0, 10, 0xffffff, "カメラ座標 : (%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 30, 0xffffff, "カメラ角度 : (%.1f, %.1f, %.1f)", 180 / DX_PI * angles.x, 180 / DX_PI * angles.y, 180 / DX_PI * angles.z);
	
	Player* player = player_;
	VECTOR pPos = player->GetPos();
	angles = player->GetRot();
	DrawFormatString(0, 50, 0xffffff, "プレイヤー座標 : (%.1f, %.1f, %.1f)", pPos.x, pPos.y, pPos.z);
	DrawFormatString(0, 70, 0xffffff, "プレイヤー角度 : (%.1f, %.1f, %.1f)", 180 / DX_PI * angles.x, 180 / DX_PI * angles.y, 180 / DX_PI * angles.z);
	
	//DrawSphere3D(pos, 20, 100, 0xffffff, 0xffffff, true);
	
	DrawSphere3D(camera->GetTargetPos(), 20, 1, 0xffffff, 0xffffff, true);
	//DrawDebugTest();

	RollBall* rollBall = rollBall_;
	VECTOR bPos = rollBall->GetPos();
	DrawFormatString(0, 90, 0xffffff, "ボール座標 : (%.1f, %.1f, %.1f)", bPos.x, bPos.y, bPos.z);

	float rad = atan2f(bPos.x - pPos.x, bPos.z - pPos.z);
	float deg = AsoUtility::Rad2DegF(rad);

	DrawFormatString(0, 130, 0xffffff, "■ボール");
	DrawFormatString(0, 150, 0xffffff, "座標　　　　　　　: (%.2f, %.2f, %.2f", bPos.x, bPos.y, bPos.z);

	DrawFormatString(0, 170, 0xffffff, "対ワールド角(Atan2) : %.2f", deg);

	// プレイヤーの向きからボール角度
	VECTOR pRot = player->GetRot();

	// 球体のワールド座標から、プレイヤー角を引く
	float viewRad = rad - pRot.y;

	// ラジアンからデグリーに変換
	float viewDeg = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(viewRad)));
	DrawFormatString(0, 190, 0xffffff, "対プレイヤー角(0-360) : %.2f", viewDeg);

	// 敵の座標
	Enemy* enemy = enemy_;
	VECTOR ePos = enemy->GetPos();
	DrawFormatString(0, 210, 0xffffff, "敵座標 : (%.1f, %.1f, %.1f)", ePos.x, ePos.y, ePos.z);

}

void TitleScene::DrawDebugTest(void)
{
	const int CENTER_Y = Application::SCREEN_SIZE_Y / 2;
	// 線分の長さ
	const float LENGTH = 100.0f;
	// 矢印の長さ
	const float ARROW_LENGTH = 20.0f;
	const float ARROW_RAD = DX_PI_F / 180.0f * 150.0f;

	VECTOR direction;
	VECTOR rotationVec;
	Vector2 rotationPos;

	// 一旦塗りつぶし
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x0000ff, true);

	// 問題サポート
	const Vector2 Q01_POS = { 200, CENTER_Y };
	DrawCircle(Q01_POS.x, Q01_POS.y, static_cast<int>(LENGTH), 0xffffff, true);
	const Vector2 Q02_POS = { 500, CENTER_Y };
	DrawCircle(Q02_POS.x, Q02_POS.y, static_cast<int>(LENGTH), 0xffffff, true);
	const Vector2 Q03_POS = { 800, CENTER_Y };
	DrawCircle(Q03_POS.x, Q03_POS.y, static_cast<int>(LENGTH), 0xffffff, true);

	//--------------------------------------------------
	// ①ベクトル０度

	// 0度の方向を求める
	direction.x = cosf(AsoUtility::Deg2RadF(30.0f));
	direction.y = sinf(AsoUtility::Deg2RadF(30.0f));
	// 移動量＝方向×スカラー（力、長さ、スピード）
	rotationVec.x = direction.x * LENGTH;
	rotationVec.y = direction.y * LENGTH;
	// 座標＋移動量で、移動処理→０度の方向に線分を引く
	rotationPos.x = Q01_POS.x + static_cast<int>(rotationVec.x);
	rotationPos.y = Q01_POS.y + static_cast<int>(rotationVec.y);
	DrawLine(Q01_POS.x, Q01_POS.y, rotationPos.x, rotationPos.y, 0x000000);

	// ０度の方向を求める
	direction.x = cosf(AsoUtility::Deg2RadF(60.0f));
	direction.y = sinf(60.0f* DX_PI_F / 180.0f);
	// 移動量＝方向×スカラー（力、長さ、スピード）
	rotationVec.x = direction.x * LENGTH;
	rotationVec.y = direction.y * LENGTH;
	// 座標＋移動量で、移動処理→０度の方向に線分を引く
	rotationPos.x = Q01_POS.x + static_cast<int>(rotationVec.x);
	rotationPos.y = Q01_POS.y + static_cast<int>(rotationVec.y);
	DrawLine(Q01_POS.x, Q01_POS.y, rotationPos.x, rotationPos.y, 0xff0000);


	//----------------------------------------------------

	deg_ += 1.0f;
	direction.x = cosf(AsoUtility::Deg2RadF(deg_));
	direction.y = sinf(AsoUtility::Deg2RadF(deg_));

	rotationVec.x = direction.x * LENGTH;
	rotationVec.y = direction.y * LENGTH;

	rotationPos.x = Q02_POS.x + static_cast<int>(rotationVec.x);
	rotationPos.y = Q02_POS.y + static_cast<int>(rotationVec.y);
	DrawLine(Q02_POS.x, Q02_POS.y, rotationPos.x, rotationPos.y, 0xff0000);

	if (deg_ >= 360.0f)
	{
		deg_ = 0.0f;
	}

	//----------------------------------------------------
	
	// 110度の黒い線(任意ベクトル=回転させたいベクトル)
	direction.x = cosf(AsoUtility::Deg2RadF(110.0f));
	direction.y = sinf(AsoUtility::Deg2RadF(110.0f));

	rotationVec.x = direction.x * LENGTH;
	rotationVec.y = direction.y * LENGTH;

	rotationPos.x = Q03_POS.x + static_cast<int>(rotationVec.x);
	rotationPos.y = Q03_POS.y + static_cast<int>(rotationVec.y);
	DrawLine(Q03_POS.x, Q03_POS.y, rotationPos.x, rotationPos.y, 0x000000);

	// 矢印ベクトル①の方向
	VECTOR arrow1_Dir;
	arrow1_Dir.x = cosf(ARROW_RAD) * direction.x - (sinf(ARROW_RAD) * direction.y);
	arrow1_Dir.y = sinf(ARROW_RAD) * direction.x + (cosf(ARROW_RAD) * direction.y);

	// 矢印ベクトル①の座標 = 座標(黒線の終点)+移動量(方向×長さ)
	VECTOR arrow1_Pos;
	arrow1_Pos.x = rotationPos.x + (arrow1_Dir.x * ARROW_LENGTH);
	arrow1_Pos.y = rotationPos.y + (arrow1_Dir.y * ARROW_LENGTH);
	DrawLine(rotationPos.x, rotationPos.y,
		static_cast<int>(arrow1_Pos.x), static_cast<int>(arrow1_Pos.y), 0xff0000);

	// 矢印ベクトル②の方向
	VECTOR arrow2_Dir;
	arrow2_Dir.x = cosf(-ARROW_RAD) * direction.x - (sinf(-ARROW_RAD) * direction.y);
	arrow2_Dir.y = sinf(-ARROW_RAD) * direction.x + (cosf(-ARROW_RAD) * direction.y);

	// 矢印ベクトル②の座標 = 座標(黒線の終点)+移動量(方向×長さ)
	VECTOR arrow2_Pos;
	arrow2_Pos.x = rotationPos.x + (arrow2_Dir.x * ARROW_LENGTH);
	arrow2_Pos.y = rotationPos.y + (arrow2_Dir.y * ARROW_LENGTH);
	DrawLine(rotationPos.x, rotationPos.y,static_cast<int>(arrow2_Pos.x), static_cast<int>(arrow2_Pos.y),0xff0000);

}

void TitleScene::Release(void)
{

	// グリッド線の開放
	grid_->Release();
	delete grid_;

	// ステージの開放
	stage_->Release();
	delete stage_;

	// プレイヤーの開放
	player_->Release();
	delete player_;

	// 弾の更新
	auto shots = player_->GetShots();
	for (auto shot : shots)
	{
		shot->Release();
		delete shot;
	}

	// ボールの開放
	rollBall_->Release();
	delete rollBall_;

}
