#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Object/Player.h"
#include "RollBall.h"

RollBall::RollBall(Player* follow)
{
	follow_ = follow;
	pos_ = AsoUtility::VECTOR_ZERO;
	step_ = 0.0f;
}

RollBall::~RollBall(void)
{
}

void RollBall::Init(void)
{
	pos_ = AsoUtility::VECTOR_ZERO;
	step_ = 0.0f;
}

void RollBall::Update(void)
{

	// 時間進行
	step_ += SceneManager::GetInstance().GetDeltaTime();

	// 追従相手の座標を取得
	VECTOR pos = follow_->GetPos();

	// 高さ
	pos_.y = 50.0f;

	// 半径
	//float dis = 100.0f;
	//pos_.x = pos.x + (dis * sinf(step_));
	//pos_.z = pos.z + (dis * cosf(step_));

	// 回転行列
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotY(step_));

	// 相対座標
	VECTOR localPos = { 0.0f, 50.0f, 100.0f };

	// 回転させた相対座標
	VECTOR localRotPos = VTransform(localPos, mat);

	// 相対座標と基点になる
	pos_ = VAdd(pos, localRotPos);

}

void RollBall::Draw(void)
{

	// 球体描画
	DrawSphere3D(pos_, 10.0f, 50, 0xff0000, 0xff0000, true);

	// 縦線描画
	VECTOR toLineY = pos_;
	toLineY.y = 0.0f;
	DrawLine3D(pos_, toLineY, 0xffffff);

}

void RollBall::Release(void)
{
}

VECTOR RollBall::GetPos(void) const
{
	return pos_;
}

float RollBall::GetAngle(void) const
{
	return atan2f(pos_.x, pos_.z);
}
