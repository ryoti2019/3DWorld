#include "../Utility/AsoUtility.h"
#include "../Object/Player.h"
#include "Enemy.h"

Enemy::Enemy(Player* player)
{
	player_ = player;
}

Enemy::~Enemy(void)
{
}

void Enemy::Update(void)
{
	// 基底クラスの関数を呼ぶ
	UnitBase::Update();
	// 索敵
	Search();
}

void Enemy::SetParam(void)
{
	// 外部ファイルの３Ｄモデルをロード
	modelFileName_ = "Enemy.mv1";

	// ３Ｄモデルの位置(引数は、３Ｄ座標)
	pos_ = { 300.0f, 0.0f, 300.0f };

	// モデルの角度
	rot_ = { 0.0f, 45.0f, 0.0f };
	rotLocal_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	animNo_ = 0;

	// 検知フラグ
	isNoticeHearing_ = false;

	// 追ってくるスピード
	speed_ = 1.0f;

	// 弾の半径
	collisionRadius_ = 40.0f;

}

void Enemy::Search(void)
{

	// 検知フラグリセット
	isNoticeHearing_ = false;
	isNoticeView_ = false;

	// プレイヤーの座標を取得
	VECTOR pPos = player_->GetPos();

	// エネミーからプレイヤーまでのベクトル
	VECTOR diff = VSub(pPos, pos_);

	// XZ距離
	float distance = diff.x * diff.x + diff.z * diff.z;

	// 聴覚
	if (distance <= HEARING_RANGE * HEARING_RANGE)
	{
		isNoticeHearing_ = true;
	}

	// 視覚
	if (distance <= VIEW_RANGE * VIEW_RANGE)
	{

		// 自分から見たプレイヤーの角度を求める
		float rad = atan2(pPos.x - pos_.x, pPos.z - pos_.z);
		float viewRad = rad - rot_.y;
		float viewDeg = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(viewRad)));
		// 視野角
		if (viewDeg <= VIEW_ANGLE || viewDeg >= (360.0f - VIEW_ANGLE))
		{
			isNoticeView_ = true;
		}

	}

	// プレイヤーの追跡
	// プレイヤーに向けたベクトル
	VECTOR vec;
	vec.x = static_cast<float>(pPos.x - pos_.x);
	vec.y = static_cast<float>(pPos.y - pos_.y);
	vec.z = static_cast<float>(pPos.z - pos_.z);

	// ベクトルの大きさ
	float size = sqrtf(vec.x * vec.x + vec.z * vec.z + vec.y * vec.y);

	// 移動方向を決める
	VECTOR direction;
	direction.x = vec.x / size;
	direction.y = vec.y / size;
	direction.z = vec.z / size;

	// 移動後座標を計算する
	// 移動量
	VECTOR movePow;
	movePow.x = direction.x * speed_;
	movePow.z = direction.z * speed_;

	// 移動処理(座標+移動量)
	pos_.x += (movePow.x);
	pos_.z += (movePow.z);

	// 向きの処理
	float dir = atan2(direction.x, direction.z);
	rot_.y = dir;

}

void Enemy::Draw(void)
{

	if (isNoticeView_)
	{
		// 視覚範囲内：ディフューズカラーを赤色にする
		MV1SetMaterialDifColor(modelId_, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if(isNoticeHearing_)
	{
		// 聴覚範囲外：ディフューズカラーを灰色にする
		MV1SetMaterialDifColor(modelId_, 0, GetColorF(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		MV1SetMaterialDifColor(modelId_, 0, GetColorF(0.5f, 0.5f, 0.5f, 1.0f));
	}
	MV1DrawModel(modelId_);

	// 視野範囲の描画
	DrawDebugSearchRange();

	// 当たり判定の描画
	DrawCapsule3D(VAdd(pos_, { 0.0f,150.0f,0.0f }), VAdd(pos_, { 0.0f,30.0f,0.0f }), collisionRadius_, 10, 0xff0000, 0xff0000, false);

}

void Enemy::DrawDebugSearchRange(void)
{

	// 聴覚
	DrawCone3D(VAdd(pos_, { 0.0f,0.0f,0.0f }),
			   VAdd(pos_, { 0.0f,1.0f,0.0f }),
			   HEARING_RANGE, 10, 0xffff7f, 0xffff7f, true);

	// 視覚
	float viewRad = AsoUtility::Deg2RadF(VIEW_ANGLE);

	// 相対座標
	VECTOR localPos = { 0.0f,10.0f,VIEW_RANGE };

	// Enemyの回転
	MATRIX selfMat = MGetIdent();
	selfMat = MMult(selfMat, MGetRotY(rot_.y));

	// Enemyの前方
	VECTOR rotForwardLocalPos = VTransform(localPos, selfMat);

	// 視野角の回転
	MATRIX viewMat = MGetIdent();
	viewMat = MMult(selfMat, MGetRotY(viewRad));
	VECTOR rotViewRLocalPos = VTransform(localPos, viewMat);

	// ほかの描画と重ならないように座標を1上げる
	VECTOR pos = pos_;
	pos.y += 10.0f;
	VECTOR pos0 = pos;
	VECTOR pos1 = VAdd(pos_, rotForwardLocalPos);
	VECTOR pos2 = VAdd(pos_, rotViewRLocalPos);

	// 描画
	DrawTriangle3D(pos0, pos1, pos2 ,0xffdead, true);
	DrawLine3D(pos0, pos1, 0x000000);
	DrawLine3D(pos1, pos2, 0x000000);
	DrawLine3D(pos2, pos0, 0x000000);

	// 反対側
	viewMat = MMult(selfMat, MGetRotY(-viewRad));
	VECTOR rotViewLLocalPos = VTransform(localPos, viewMat);
	VECTOR pos3 = VAdd(pos, rotViewLLocalPos);

	// 描画
	DrawTriangle3D(pos0, pos3, pos1, 0xffdead, true);
	DrawLine3D(pos0, pos3, 0x000000);
	DrawLine3D(pos3, pos1, 0x000000);
	DrawLine3D(pos1, pos0, 0x000000);

}

float Enemy::GetCollisionRadius(void)
{
	return collisionRadius_;
}
