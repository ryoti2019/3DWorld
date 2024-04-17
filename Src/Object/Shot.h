#pragma once
#include <DxLib.h>
class Player;

class Shot
{

public:

	// 弾の生存時間
	static constexpr float SHOT_ALIVE = 3.0f;

	// 弾の状態
	enum class STATE
	{
		NONE,
		SHOT,
		BLAST,
		END
	};

	Shot(int baseModelId, int* blastImgs, int blastAnimNum);

	~Shot(void);

	// 弾の生成
	void CreateShot(VECTOR pos, VECTOR dir);

	void Init(void);
	void Update(void);
	void UpdateShot(void);
	void UpdateBlast(void);
	void UpdateEnd(void);
	void Draw(void);
	void DrawShot();
	void DrawBlast();
	void DrawEnd();
	void Release(void);

	// 生存判定の取得
	bool IsAlive(void);

	// 生存判定の設定
	void SetAlive(bool alive);

	// 弾判定
	bool IsShot(void);

	// 座標の取得
	VECTOR GetPos(void);

	// 衝突用の球体半径の取得
	float GetCollisionRadius(void);

	// 爆発
	void Blast(void);

private:

	// 弾の状態
	STATE state_;

	// 座標
	VECTOR pos_;

	// 弾の大きさ
	VECTOR scl_;

	// 弾の角度
	VECTOR rot_;

	// 向き
	VECTOR dir_;

	// 弾のスピード
	float speed_;

	// 生存時間
	float stepAlive_;

	// 弾のモデル
	int shotModel_;

	// 衝突判定用の球体半径
	float collisionRadius_;

	// 生存判定
	bool isAlive_;

	// 状態遷移
	void ChangeState(STATE state);

	// 爆発のアニメーション用カウンタ
	int blastCntAnim_;

	// 爆発アニメーション数
	int blastAnimNum_;

	// 爆発のアニメーション速度
	float blastSpeedAnim_;

	// 爆発のアニメーション番号
	int blastIdxAnim_;

	// 爆発アニメーション画像配列のポインタ
	int* blastImgs_;

};

