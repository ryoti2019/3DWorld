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

	// �Ǐ]����
	Player* follow_;

	// ���̂̈ʒu
	VECTOR pos_;

	// �X�V�X�e�b�v
	float step_;

};

