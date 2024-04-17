#pragma once
#include <DxLib.h>
class Player;

class Shot
{

public:

	// �e�̐�������
	static constexpr float SHOT_ALIVE = 3.0f;

	// �e�̏��
	enum class STATE
	{
		NONE,
		SHOT,
		BLAST,
		END
	};

	Shot(int baseModelId, int* blastImgs, int blastAnimNum);

	~Shot(void);

	// �e�̐���
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

	// ��������̎擾
	bool IsAlive(void);

	// ��������̐ݒ�
	void SetAlive(bool alive);

	// �e����
	bool IsShot(void);

	// ���W�̎擾
	VECTOR GetPos(void);

	// �Փ˗p�̋��̔��a�̎擾
	float GetCollisionRadius(void);

	// ����
	void Blast(void);

private:

	// �e�̏��
	STATE state_;

	// ���W
	VECTOR pos_;

	// �e�̑傫��
	VECTOR scl_;

	// �e�̊p�x
	VECTOR rot_;

	// ����
	VECTOR dir_;

	// �e�̃X�s�[�h
	float speed_;

	// ��������
	float stepAlive_;

	// �e�̃��f��
	int shotModel_;

	// �Փ˔���p�̋��̔��a
	float collisionRadius_;

	// ��������
	bool isAlive_;

	// ��ԑJ��
	void ChangeState(STATE state);

	// �����̃A�j���[�V�����p�J�E���^
	int blastCntAnim_;

	// �����A�j���[�V������
	int blastAnimNum_;

	// �����̃A�j���[�V�������x
	float blastSpeedAnim_;

	// �����̃A�j���[�V�����ԍ�
	int blastIdxAnim_;

	// �����A�j���[�V�����摜�z��̃|�C���^
	int* blastImgs_;

};

