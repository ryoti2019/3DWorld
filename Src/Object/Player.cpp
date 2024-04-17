#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Shot.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Draw(void)
{
	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(modelId_);

	// 当たり判定の描画
	DrawCapsule3D(VAdd(pos_, { 0.0f,150.0f,0.0f }), VAdd(pos_, { 0.0f,30.0f,0.0f }), 20, 10, 0xff0000, 0xff0000, false);
}

void Player::InitAnimation(void)
{

	// モデルの読み込み
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Walking.mv1").c_str());
	//playerWalk_ = MV1LoadModel((Application::PATH_MODEL + "Human.mv1").c_str());
	playerRun_ = MV1LoadModel((Application::PATH_MODEL + "Running.mv1").c_str());

}

std::vector<Shot*> Player::GetShots(void)
{
	return shots_;
}

void Player::SetParam(void)
{

	// 弾のモデルのロード
	shotModel_ = MV1LoadModel((Application::PATH_MODEL + "SpikeShot.mv1").c_str());

	// 爆発エフェクト読み込み
	LoadDivGraph((Application::PATH_IMAGE + "BlastSpike.png").c_str(),
		BLAST_ANIM_NUM, 4, 4, BLAST_SIZE_X, BLAST_SIZE_Y, blastImgs_, true);

	// ３Ｄモデルの位置(引数は、３Ｄ座標)
	pos_ = AsoUtility::VECTOR_ZERO;

	// モデルの角度
	rot_ = { 0.0f, 0.0f, 0.0f };
	rotLocal_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	animNo_ = 1;

	dir_.y = rot_.y;

	// 状態遷移
	ChangeState(STATE::WALK);

}

void Player::Move(void)
{

	// カメラの角度を取得
	VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();

	auto& ins = InputManager::GetInstance();
	float movePow = 3.0f;

	// 左のシフトキーで走る
	if (ins.IsNew(KEY_INPUT_SPACE))
	{
		ChangeState(STATE::RUN);
		movePow = 5.0f;
	}
	else if(!ins.IsNew(KEY_INPUT_SPACE))
	{
		ChangeState(STATE::WALK);
	}

	switch (state_)
	{
	case Player::STATE::WALK:
		SetWalkAnimation();
		break;
	case Player::STATE::RUN:
		SetRunAnimation();
		break;
	}

	// 方向(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// WASDでカメラの位置を変える
	if (ins.IsNew(KEY_INPUT_W)) { dir = { 0.0f, 0.0f, 1.0f }; }
	if (ins.IsNew(KEY_INPUT_A)) { dir = { -1.0f, 0.0f, 0.0f }; }
	if (ins.IsNew(KEY_INPUT_S)) { dir = { 0.0f, 0.0f, -1.0f }; }
	if (ins.IsNew(KEY_INPUT_D)) { dir = { 1.0f, 0.0f, 0.0f }; }

	if (!AsoUtility::EqualsVZero(dir))
	{

		// XYZの回転行列
		// XZ平面移動にする場合は、XZの回転を考慮しないようにする
		MATRIX mat = MGetIdent();
		//mat = MMult(mat, MGetRotX(angles_.x));
		mat = MMult(mat, MGetRotY(cameraAngles.y));
		//mat = MMult(mat, MGetRotZ(angles_.z));
		
		// 回転行列を使用して、ベクトルを回転させる
		VECTOR moveDir = VTransform(dir, mat);

		// 方向×スピードで移動量を作って、座標に足して移動
		pos_ = VAdd(pos_, VScale(moveDir, movePow));

		// 方向を角度に変換する(XZ平面 Y軸)
		float angle = atan2f(dir.x, dir.z);

		// カメラの角度を基準とし、方向分の角度を加える
		//rot_.y = cameraAngles.y + angle;

		// カメラの角度を基準とし、方向分の角度を加える
		LazyRotation(cameraAngles.y + angle);

	}

	// 攻撃
	Attack();

}

void Player::Attack(void)
{

	// 攻撃キーを押すと、弾を生成する
	auto& ins = InputManager::GetInstance();
	if (ins.IsNew(KEY_INPUT_Q) && stepShotDelay_ <= 0.0f)
	{

		// 有効な球を取得する
		Shot* shot = GetValidShot();

		// 座標をずらす
		VECTOR pos = pos_;
		pos.y += 70.0;
		
		// 弾を生成(弾発射の準備)

		//// XYZの回転行列
		//// XZ平面移動にする場合は、XZの回転を考慮しないようにする
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(rot_.y));

		// 回転行列を使用して、ベクトルを回転させる
		VECTOR moveDir = VTransform({0.0f,0.0f,1.0f}, mat);

		shot->CreateShot(pos, moveDir);

		// 弾の発射後の硬直時間セット
		stepShotDelay_ = SHOT_DELAY;

	}

	if (stepShotDelay_ > 0.0f)
	{
		// 弾発射後の硬直時間を減らしていく
		stepShotDelay_ -= 1.0f / SceneManager::DEFAULT_FPS;
	}

}

Shot* Player::GetValidShot(void)
{

	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		if (!shots_[i]->IsAlive())
		{
			return shots_[i];
		}
	}
	Shot* shot = nullptr;
	 shot = new Shot(shotModel_, blastImgs_, BLAST_ANIM_NUM);
	shots_.push_back(shot);

	return shot;

}

void Player::ChangeState(STATE state)
{
	// 状態の更新
	state_ = state;
}

void Player::SetWalkAnimation(void)
{

	MV1DetachAnim(modelId_, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(modelId_, animNo_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(modelId_, animAttachNo_);
	//stepAnim_ = 0.0f;

	// アニメーション速度
	speedAnim_ = 20.0f;

	// モデルに指定時間のアニメーションを設定する
	MV1SetAttachAnimTime(modelId_, animAttachNo_, stepAnim_);

}

void Player::SetRunAnimation(void)
{

	MV1DetachAnim(modelId_, animAttachNo_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(modelId_, animNo_, playerRun_);

	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(modelId_, animAttachNo_);
	//stepAnim_ = 0.0f;

	// アニメーション速度
	speedAnim_ = 20.0f;

	// モデルに指定時間のアニメーションを設定する
	MV1SetAttachAnimTime(modelId_, animAttachNo_, stepAnim_);

}
