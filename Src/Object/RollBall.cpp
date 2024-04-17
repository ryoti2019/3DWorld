#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Object/Player.h"
#include "RollBall.h"

RollBall::RollBall(Player* follow)
{
	follow_ = follow;
	pos_ = AsoUtility::VECTOR_ZERO;
	step_ = 0.0f;
}

RollBall::~RollBall(void)
{
}

void RollBall::Init(void)
{
	pos_ = AsoUtility::VECTOR_ZERO;
	step_ = 0.0f;
}

void RollBall::Update(void)
{

	// ���Ԑi�s
	step_ += SceneManager::GetInstance().GetDeltaTime();

	// �Ǐ]����̍��W���擾
	VECTOR pos = follow_->GetPos();

	// ����
	pos_.y = 50.0f;

	// ���a
	//float dis = 100.0f;
	//pos_.x = pos.x + (dis * sinf(step_));
	//pos_.z = pos.z + (dis * cosf(step_));

	// ��]�s��
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotY(step_));

	// ���΍��W
	VECTOR localPos = { 0.0f, 50.0f, 100.0f };

	// ��]���������΍��W
	VECTOR localRotPos = VTransform(localPos, mat);

	// ���΍��W�Ɗ�_�ɂȂ�
	pos_ = VAdd(pos, localRotPos);

}

void RollBall::Draw(void)
{

	// ���̕`��
	DrawSphere3D(pos_, 10.0f, 50, 0xff0000, 0xff0000, true);

	// �c���`��
	VECTOR toLineY = pos_;
	toLineY.y = 0.0f;
	DrawLine3D(pos_, toLineY, 0xffffff);

}

void RollBall::Release(void)
{
}

VECTOR RollBall::GetPos(void) const
{
	return pos_;
}

float RollBall::GetAngle(void) const
{
	return atan2f(pos_.x, pos_.z);
}
