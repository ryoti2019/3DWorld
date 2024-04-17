#pragma once
#include <DxLib.h>
class Player;

class RollBall
{

public:

	RollBall(Player* follow);

	~RollBall(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;
	float GetAngle(void) const;

private:

	// 追従相手
	Player* follow_;

	// 球体の位置
	VECTOR pos_;

	// 更新ステップ
	float step_;

};

