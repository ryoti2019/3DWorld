#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "UnitBase.h"

UnitBase::UnitBase(void)
{
}

UnitBase::~UnitBase(void)
{
}

void UnitBase::InitAnimation(void)
{
	// ���f���̓ǂݍ���
	modelId_ = MV1LoadModel((Application::PATH_MODEL + modelFileName_).c_str());

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(modelId_, animNo_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(modelId_, animAttachNo_);
	stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 20.0f;

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	MV1SetAttachAnimTime(modelId_, animAttachNo_, stepAnim_);
}

void UnitBase::Init(void)
{

	// �p�����[�^�ݒ�
	SetParam();

	// ���f���̊p�x�ݒ�
	SetRotation();


	InitAnimation();

}

void UnitBase::Update(void)
{

	// �ړ�����
	Move();

	// �A�j���[�V��������
	Animation();

	// ���f���̈ړ��ݒ�
	MV1SetPosition(modelId_, pos_);

	// ���f���̊p�x�ݒ�
	SetRotation();

}

void UnitBase::Draw(void)
{
	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(modelId_);
}

void UnitBase::Release(void)
{
	MV1DeleteModel(modelId_);
}

VECTOR UnitBase::GetPos(void) const
{
	return pos_;
}

VECTOR UnitBase::GetRot(void) const
{
	return rot_;
}

void UnitBase::SetParam(void)
{
}

void UnitBase::Move(void)
{
}

void UnitBase::Animation(void)
{

	// �A�j���[�V�����Đ�
	// �o�ߎ��Ԃ̎擾
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// �A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (speedAnim_ * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		// ���[�v�Đ�
		stepAnim_ = 0.0f;
	}

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(modelId_, animAttachNo_, stepAnim_);

}

void UnitBase::SetRotation(void)
{

	// ��]�s����g�p�����p�x�ݒ�
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(rot_.x));
	mat = MMult(mat, MGetRotY(rot_.y));
	mat = MMult(mat, MGetRotZ(rot_.z));

	// �����p�̉�]�s��
	MATRIX localMat = MGetIdent();
	localMat = MMult(localMat, MGetRotX(rotLocal_.x));
	localMat = MMult(localMat, MGetRotY(rotLocal_.y));
	localMat = MMult(localMat, MGetRotZ(rotLocal_.z));

	// �s��̍���
	mat = MMult(mat, localMat);

	// �s����g�p���ă��f���̊p�x��ݒ� �s��ɂāA���f���̊p�x
	MV1SetRotationMatrix(modelId_, mat);
}

void UnitBase::LazyRotation(float goalRot)
{

	// ��]����
	float degNowAngleY = AsoUtility::Rad2DegF(rot_.y);
	float degGoalAngleY = AsoUtility::Rad2DegF(goalRot);

	// 0�x�`360�x�ȓ��Ɋp�x�������߂�
	degGoalAngleY = static_cast<float>(AsoUtility::DegIn360(degGoalAngleY));

	// ��]�����Ȃ����̉�]�������擾����(���v���:1�A�����v���:-1)
	int aroundDir = AsoUtility::DirNearAroundDeg(degNowAngleY, degGoalAngleY);

	// ���B�������p�x�ɉ�]��������
	if (fabs(degGoalAngleY - degNowAngleY) >= 5)
	{
		rot_.y += SPEED_ROT_RAD * static_cast<float>(aroundDir);
	}
	else
	{
		rot_.y = goalRot;
	}

	// 0�x�`360�x�ȓ��Ɋp�x�������߂�
	rot_.y = static_cast<float>(AsoUtility::RadIn2PI(rot_.y));

}

