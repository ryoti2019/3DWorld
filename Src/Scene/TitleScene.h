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

	// �O���b�h
	Grid* grid_;

	// �X�e�[�W
	Stage* stage_;

	// �v���C���[
	Player* player_;

	// �e
	Shot* shot_;

	// �v���C���[
	Enemy* enemy_;

	// �{�[��
	RollBall* rollBall_;

	// �p�x
	float deg_;

};
