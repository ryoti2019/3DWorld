#pragma once
#include "SceneBase.h"

class Grid;
class Stage;
class Player;
class Shot;
class Enemy;
class RollBall;

class TitleScene : public SceneBase
{

public:

	TitleScene(void);
	~TitleScene(void) override;

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawDebug(void);
	void DrawDebugTest(void);
	void Release(void) override;

private:

	// グリッド
	Grid* grid_;

	// ステージ
	Stage* stage_;

	// プレイヤー
	Player* player_;

	// 弾
	Shot* shot_;

	// プレイヤー
	Enemy* enemy_;

	// ボール
	RollBall* rollBall_;

	// 角度
	float deg_;

};
