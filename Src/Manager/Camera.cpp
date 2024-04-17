#include "../Manager/InputManager.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Player.h"
#include "Camera.h"

Camera::Camera(void)
{
	mode_ = MODE::NONE;
	pos_ = { 0.0f, 0.0f, 300.0f };
	angles_ = { 0.0f, 0.0f, 0.0f };
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{

	// �J�����̏����ʒu
	pos_ = { 0.0f, HEIGHT, -DIS_FOLLOW2CAMERA };
	// �J�����̏����p�x
	angles_ = { 0.0f, 0.0f, 0.0f };
	angles_.x = AsoUtility::Deg2RadF(30.0f);

}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{
	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(10.0f, 30000.0f);
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	}

}

void Camera::SetBeforeDrawFixedPoint(void)
{

	// �J�����̐ݒ�(�ʒu�Ɗp�x�ɂ�鐧��)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);

}

void Camera::SetBeforeDrawFollow(void)
{

	auto& ins = InputManager::GetInstance();
	// ���L�[�ŃJ�����̊p�x��ς���
	float rotPow = 1.0f * DX_PI_F / 180.0f;
	if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x += rotPow; }
	if (ins.IsNew(KEY_INPUT_UP)) { angles_.x -= rotPow; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }

	// �����_���L�����N�^�[�ʒu�ɐݒ�
	//SetTargetPosFollow();

	// �����_���L�����N�^�[�O���ʒu�ɐݒ�
	SetTargetPosFollowForward();

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		{ 0.0f, 1.0f, 0.0f }
	);

}

void Camera::SetBeforeDrawFree(void)
{

	MoveXYZDirection();

	// �J�����̐ݒ�(�ʒu�Ɗp�x�ɂ�鐧��)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);

}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return pos_;

}

VECTOR Camera::GetAngles(void) const
{
	return angles_;

}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

void Camera::SetFollow(Player* follow)
{
	follow_ = follow;
}

void Camera::ChangeMode(MODE mode)
{
	// �J�������[�h�̕ύX
	mode_ = mode;
	// �ύX���̏���������
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	}
}

void Camera::MoveXZAngle(void)
{

	auto& ins = InputManager::GetInstance();
	float rad = 0.0f;

}

void Camera::SetTargetPosFollow(void)
{

	// �����_�̈ړ�
	VECTOR followPos = follow_->GetPos();
	targetPos_ = followPos;

	// �J�����ʒu(�v���C���[����̑��΍��W�Ő���)
	VECTOR localPos = { 0.0f, HEIGHT, -DIS_FOLLOW2CAMERA };

	// �J�����̉�]�s����쐬
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(angles_.x));
	mat = MMult(mat, MGetRotY(angles_.y));

	// ���΍��W����]�����āA��]��̑��΍��W���擾����
	VECTOR localRotPos = VTransform(localPos, mat);

	// ���΍��W���烏�[���h���W�ɒ����āA�J�������W�Ƃ���
	pos_ = VAdd(followPos, localRotPos);

}

void Camera::SetTargetPosFollowForward(void)
{

	// �J�����ʒu(�v���C���[����̑��΍��W�Ő���)
	VECTOR localCameraPos = { 0.0f, HEIGHT, -DIS_FOLLOW2CAMERA };
	VECTOR localTargetPos = { 0.0f, 0.0f, DIS_FOLLOW2TARGET };

	// �����_�̈ړ�
	VECTOR followPos = follow_->GetPos();

	// �J�����̉�]�s����쐬
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(angles_.x));
	mat = MMult(mat, MGetRotY(angles_.y));

	VECTOR localRotPos;

	// �����_�̈ړ�
	localRotPos = VTransform(localTargetPos, mat);
	targetPos_ = VAdd(followPos, localRotPos);

	// �J�����̈ړ�
	localRotPos = VTransform(localCameraPos, mat);
	pos_ = VAdd(followPos, localRotPos);

}

void Camera::MoveXYZDirection(void)
{

	auto& ins = InputManager::GetInstance();
	float movePow = 3.0f;

	// ����(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	//// WASD�ŃJ�����̈ʒu��ς���
	//if (ins.IsNew(KEY_INPUT_W)) { dir = { 0.0f, 0.0f, 1.0f }; }
	//if (ins.IsNew(KEY_INPUT_A)) { dir = { -1.0f, 0.0f, 0.0f }; }
	//if (ins.IsNew(KEY_INPUT_S)) { dir = { 0.0f, 0.0f, -1.0f }; }
	//if (ins.IsNew(KEY_INPUT_D)) { dir = { 1.0f, 0.0f, 0.0f }; }

	if (!AsoUtility::EqualsVZero(dir))
	{
		// XYZ�̉�]�s��
		// XZ���ʈړ��ɂ���ꍇ�́AXZ�̉�]���l�����Ȃ��悤�ɂ���
		MATRIX mat = MGetIdent();
		//mat = MMult(mat, MGetRotX(angles_.x));
		mat = MMult(mat, MGetRotY(angles_.y));
		//mat = MMult(mat, MGetRotZ(angles_.z));
		// 
		// ��]�s����g�p���āA�x�N�g������]������
		VECTOR moveDir = VTransform(dir, mat);

		// �����~�X�s�[�h�ňړ��ʂ�����āA���W�ɑ����Ĉړ�
		pos_ = VAdd(pos_, VScale(moveDir, movePow));

	}

	if (ins.IsNew(KEY_INPUT_Q)) { pos_.y += movePow; }
	if (ins.IsNew(KEY_INPUT_E)) { pos_.y -= movePow; }

	// ���L�[�ŃJ�����̊p�x��ς���
	float rotPow = 1.0f * DX_PI_F / 180.0f;
	if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x += rotPow; }
	if (ins.IsNew(KEY_INPUT_UP)) { angles_.x -= rotPow; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }

}
