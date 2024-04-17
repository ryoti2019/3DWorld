#pragma once
#include <DxLib.h>
#include "../Object/UnitBase.h"

class Shot;

class Player : public UnitBase
{
public:
	// �A�j���[�V�����̍Đ����x
	static constexpr float SPEED_ANIM = 20.0f;

	// ��]�ɂ����鎞��
	static constexpr float TIME_ROT = 1.0f;

	// ��]��(deg)
	static constexpr float SPEED_ROT = 8.0f;

	// ��]��(rad)
	static constexpr float SPEED_ROT_RAD = SPEED_ROT * (DX_PI_F / 180.0f);

	// �e���ˌ�̍d������
	static constexpr float SHOT_DELAY = 0.3f;

	// �����̃T�C�Y
	static constexpr int BLAST_SIZE_X = 256;
	static constexpr int BLAST_SIZE_Y = 256;

	// �����̃A�j���[�V������
	static constexpr int BLAST_ANIM_NUM = 16;

	// �v���C���[�̏��
	enum class STATE
	{
		WALK,
		RUN
	};

	Player(void);

	~Player(void);

	void Draw(void) override;

	// �A�j���[�V�����̏����ݒ�
	void InitAnimation(void) override;

	// �e�̎擾
	std::vector<Shot*> GetShots(void);

protected:

	// �e
	std::vector<Shot*> shots_;

	// �v���C���[�̕������f��
	int playerWalk_;

	// �v���C���[�̑��郂�f��
	int playerRun_;

	// �e�̃��f��
	int shotModel_;

	// �v���C���[�̏��
	STATE state_;

	// ����
	VECTOR dir_;

	// �e���ˌ�̍d�����Ԍv�Z�p
	float stepShotDelay_;

	// �p�����[�^�ݒ�
	void SetParam(void) override;

	// �ړ�
	void Move(void) override;

	// �e�𔭎˂���
	void Attack(void);

	// �e�̐���
	Shot* GetValidShot(void);

	// �����̉摜(�{���͊O�����\�[�X�p�̊Ǘ��N���X�����ׂ��B�e���f�����B)
	int blastImgs_[BLAST_ANIM_NUM];

	// ��ԑJ��
	void ChangeState(STATE state);

	// �A�j���[�V�����̏����ݒ�
	void SetWalkAnimation(void);
	void SetRunAnimation(void);
};
