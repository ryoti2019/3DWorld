#pragma once
#include "../Object/UnitBase.h"
class Player;

class Enemy : public UnitBase
{

public:

	// 視野の広さ
	static constexpr float VIEW_RANGE = 300.0f;

	// 視野角
	static constexpr float VIEW_ANGLE = 30.0f;

	// 聴覚の広さ
	static constexpr float HEARING_RANGE = 400.0f;

	Enemy(Player* player);
	~Enemy(void);

	void Update(void)override;
	void Draw(void) override;
	void DrawDebugSearchRange(void);

	// 衝突用の球体半径の取得
	float GetCollisionRadius(void);

protected:

	Player* player_;

	// 検知フラグ(視覚)
	bool isNoticeView_;

	// 検知フラグ(聴覚)
	bool isNoticeHearing_;

	// パラメータ設定
	void SetParam(void) override;

	// 索敵
	void Search(void);

	float speed_;

	// 衝突判定用の球体半径
	float collisionRadius_;
};

