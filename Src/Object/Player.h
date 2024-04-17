#pragma once
#include <DxLib.h>
#include "../Object/UnitBase.h"

class Shot;

class Player : public UnitBase
{
public:
	// アニメーションの再生速度
	static constexpr float SPEED_ANIM = 20.0f;

	// 回転にかかる時間
	static constexpr float TIME_ROT = 1.0f;

	// 回転量(deg)
	static constexpr float SPEED_ROT = 8.0f;

	// 回転量(rad)
	static constexpr float SPEED_ROT_RAD = SPEED_ROT * (DX_PI_F / 180.0f);

	// 弾発射後の硬直時間
	static constexpr float SHOT_DELAY = 0.3f;

	// 爆発のサイズ
	static constexpr int BLAST_SIZE_X = 256;
	static constexpr int BLAST_SIZE_Y = 256;

	// 爆発のアニメーション数
	static constexpr int BLAST_ANIM_NUM = 16;

	// プレイヤーの状態
	enum class STATE
	{
		WALK,
		RUN
	};

	Player(void);

	~Player(void);

	void Draw(void) override;

	// アニメーションの初期設定
	void InitAnimation(void) override;

	// 弾の取得
	std::vector<Shot*> GetShots(void);

protected:

	// 弾
	std::vector<Shot*> shots_;

	// プレイヤーの歩くモデル
	int playerWalk_;

	// プレイヤーの走るモデル
	int playerRun_;

	// 弾のモデル
	int shotModel_;

	// プレイヤーの状態
	STATE state_;

	// 向き
	VECTOR dir_;

	// 弾発射後の硬直時間計算用
	float stepShotDelay_;

	// パラメータ設定
	void SetParam(void) override;

	// 移動
	void Move(void) override;

	// 弾を発射する
	void Attack(void);

	// 弾の生成
	Shot* GetValidShot(void);

	// 爆発の画像(本来は外部リソース用の管理クラスを作るべき。弾モデルも。)
	int blastImgs_[BLAST_ANIM_NUM];

	// 状態遷移
	void ChangeState(STATE state);

	// アニメーションの初期設定
	void SetWalkAnimation(void);
	void SetRunAnimation(void);
};
