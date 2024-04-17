#pragma once
#include "../Object/UnitBase.h"
class Player;

class Enemy : public UnitBase
{

public:

	// ����̍L��
	static constexpr float VIEW_RANGE = 300.0f;

	// ����p
	static constexpr float VIEW_ANGLE = 30.0f;

	// ���o�̍L��
	static constexpr float HEARING_RANGE = 400.0f;

	Enemy(Player* player);
	~Enemy(void);

	void Update(void)override;
	void Draw(void) override;
	void DrawDebugSearchRange(void);

	// �Փ˗p�̋��̔��a�̎擾
	float GetCollisionRadius(void);

protected:

	Player* player_;

	// ���m�t���O(���o)
	bool isNoticeView_;

	// ���m�t���O(���o)
	bool isNoticeHearing_;

	// �p�����[�^�ݒ�
	void SetParam(void) override;

	// ���G
	void Search(void);

	float speed_;

	// �Փ˔���p�̋��̔��a
	float collisionRadius_;
};

