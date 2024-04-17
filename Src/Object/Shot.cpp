#include "Shot.h"

Shot::Shot(int baseModelId, int* blastImgs, int blastAnimNum)
{
	shotModel_ = MV1DuplicateModel(baseModelId);
	blastImgs_ = blastImgs;
	blastAnimNum_ = blastAnimNum;
}

Shot::~Shot(void)
{
}

void Shot::CreateShot(VECTOR pos, VECTOR dir)
{
	
	// 弾の大きさを設定
	scl_ = { 0.8f, 0.8f, 0.8f };

	// 弾の角度を設定
	rot_ = { 0.0f, 0.0f, 0.0f };

	// 座標
	pos_ = pos;

	// 向き
	dir_ = dir;

	// 弾の半径
	collisionRadius_ = 5.0f;

	// 弾の速度
	speed_ = 8.0f;

	// 弾の生存時間の設定
	stepAlive_ = SHOT_ALIVE;

	isAlive_ = true;


	blastSpeedAnim_ = 1.0f;

	// 状態の初期化
	state_ = STATE::NONE;

	// 状態遷移
	ChangeState(STATE::SHOT);
}

void Shot::Init(void)
{
}

void Shot::Update(void)
{

	switch (state_)
	{

	case Shot::STATE::SHOT:
		UpdateShot();
		break;
	case Shot::STATE::BLAST:
		UpdateBlast();
		break;
	case Shot::STATE::END:
		UpdateEnd();
		break;

	}

}

void Shot::UpdateShot(void)
{
	// 弾を移動させる
	// 移動量の計算(方向×スピード)
	VECTOR movePow;
	movePow = VScale(dir_, speed_);

	// 移動処理(座標+移動量)
	pos_ = VAdd(pos_, movePow);

	// 大きさの設定
	MV1SetScale(shotModel_, scl_);

	// 角度の設定
	MV1SetRotationXYZ(shotModel_, rot_);

	// 位置の設定
	MV1SetPosition(shotModel_, pos_);
}

void Shot::UpdateBlast(void)
{
	// 爆発アニメーション
	blastCntAnim_++;

	// 爆発のアニメーション番号
	blastIdxAnim_ = static_cast<int>(static_cast<float>(blastCntAnim_) * blastSpeedAnim_) % blastAnimNum_;

	// 爆発アニメーションの終了判定
	if (blastIdxAnim_ + 1 >= blastAnimNum_)
	{
		ChangeState(STATE::END);
	}
}

void Shot::UpdateEnd(void)
{
}

void Shot::Draw(void)
{

	switch (state_)
	{

	case Shot::STATE::SHOT:
		DrawShot();
		break;
	case Shot::STATE::BLAST:
		DrawBlast();
		break;
	case Shot::STATE::END:
		DrawEnd();
		break;

	}

}

void Shot::DrawShot()
{
	MV1DrawModel(shotModel_);

	// 敵の円の描画
	DrawSphere3D(VAdd(pos_, { 0.0f,50.0f,0.0f }), collisionRadius_, 10, 0xff0000, 0xff0000, false);
}

void Shot::DrawBlast()
{
	// 2Dのアニメーション描画
	DrawBillboard3D(
		pos_, 0.5f, 0.5f, 200.0f, 0.0f, blastImgs_[blastIdxAnim_], true);
}

void Shot::DrawEnd()
{
}

void Shot::Release(void)
{
	MV1DeleteModel(shotModel_);
}

bool Shot::IsAlive(void)
{
	return isAlive_;
}

bool Shot::IsShot(void)
{
	//※STATEがSHOT状態だったらtrue
	return state_ == STATE::SHOT;
}

VECTOR Shot::GetPos(void)
{
	return pos_;
}

float Shot::GetCollisionRadius(void)
{
	return collisionRadius_;
}

void Shot::ChangeState(STATE state)
{
	// 状態の更新
	state_ = state;

	// 状態遷移時の初期化
	switch (state_)
	{
	case Shot::STATE::SHOT:
		break;
	case Shot::STATE::BLAST:
		blastCntAnim_ = 0;
		break;
	case Shot::STATE::END:
		break;
	}
}

void Shot::Blast(void)
{
	//※BLAST状態へ遷移させる
	ChangeState(STATE::BLAST);
}