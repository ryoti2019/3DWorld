#pragma once
#include <DxLib.h>
class Player;

class Camera
{
public:

	// �J�����̍���
	static constexpr float HEIGHT = 250.0f;

	// �Ǐ]�Ώۂ���J�����܂ł̋���
	static constexpr float DIS_FOLLOW2CAMERA = 150.0f;

	// �Ǐ]�Ώۂ��璍���_�܂ł̋���
	static constexpr float DIS_FOLLOW2TARGET = 350.0f;

	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT, // ��_�J����
		FREE, // �t���[���[�h
		FOLLOW, // �Ǐ]���[�h 
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFree(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;

	VECTOR GetAngles(void) const;

	VECTOR GetTargetPos(void) const;

	// �Ǐ]����̐ݒ�
	void SetFollow(Player* follow);

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

private:

	// �Ǐ]����
	Player* follow_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����̊p�x(rad)
	VECTOR angles_;

	// �����_
	VECTOR targetPos_;

	// XYZ�̂ǂ̉�]�ɂ�
	void MoveXYZDirection(void);

	// �p�x�ɂ��XZ���ʂ̈ړ�
	void MoveXZAngle(void);

	// �����_���L�����N�^�[�ʒu�ɐݒ�
	void SetTargetPosFollow(void);

	// �����_���L�����N�^�[�O���ʒu�ɐݒ�
	void SetTargetPosFollowForward(void);



};
