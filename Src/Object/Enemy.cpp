#include "../Utility/AsoUtility.h"
#include "../Object/Player.h"
#include "Enemy.h"

Enemy::Enemy(Player* player)
{
	player_ = player;
}

Enemy::~Enemy(void)
{
}

void Enemy::Update(void)
{
	// ���N���X�̊֐����Ă�
	UnitBase::Update();
	// ���G
	Search();
}

void Enemy::SetParam(void)
{
	// �O���t�@�C���̂R�c���f�������[�h
	modelFileName_ = "Enemy.mv1";

	// �R�c���f���̈ʒu(�����́A�R�c���W)
	pos_ = { 300.0f, 0.0f, 300.0f };

	// ���f���̊p�x
	rot_ = { 0.0f, 45.0f, 0.0f };
	rotLocal_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	animNo_ = 0;

	// ���m�t���O
	isNoticeHearing_ = false;

	// �ǂ��Ă���X�s�[�h
	speed_ = 1.0f;

	// �e�̔��a
	collisionRadius_ = 40.0f;

}

void Enemy::Search(void)
{

	// ���m�t���O���Z�b�g
	isNoticeHearing_ = false;
	isNoticeView_ = false;

	// �v���C���[�̍��W���擾
	VECTOR pPos = player_->GetPos();

	// �G�l�~�[����v���C���[�܂ł̃x�N�g��
	VECTOR diff = VSub(pPos, pos_);

	// XZ����
	float distance = diff.x * diff.x + diff.z * diff.z;

	// ���o
	if (distance <= HEARING_RANGE * HEARING_RANGE)
	{
		isNoticeHearing_ = true;
	}

	// ���o
	if (distance <= VIEW_RANGE * VIEW_RANGE)
	{

		// �������猩���v���C���[�̊p�x�����߂�
		float rad = atan2(pPos.x - pos_.x, pPos.z - pos_.z);
		float viewRad = rad - rot_.y;
		float viewDeg = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(viewRad)));
		// ����p
		if (viewDeg <= VIEW_ANGLE || viewDeg >= (360.0f - VIEW_ANGLE))
		{
			isNoticeView_ = true;
		}

	}

	// �v���C���[�̒ǐ�
	// �v���C���[�Ɍ������x�N�g��
	VECTOR vec;
	vec.x = static_cast<float>(pPos.x - pos_.x);
	vec.y = static_cast<float>(pPos.y - pos_.y);
	vec.z = static_cast<float>(pPos.z - pos_.z);

	// �x�N�g���̑傫��
	float size = sqrtf(vec.x * vec.x + vec.z * vec.z + vec.y * vec.y);

	// �ړ����������߂�
	VECTOR direction;
	direction.x = vec.x / size;
	direction.y = vec.y / size;
	direction.z = vec.z / size;

	// �ړ�����W���v�Z����
	// �ړ���
	VECTOR movePow;
	movePow.x = direction.x * speed_;
	movePow.z = direction.z * speed_;

	// �ړ�����(���W+�ړ���)
	pos_.x += (movePow.x);
	pos_.z += (movePow.z);

	// �����̏���
	float dir = atan2(direction.x, direction.z);
	rot_.y = dir;

}

void Enemy::Draw(void)
{

	if (isNoticeView_)
	{
		// ���o�͈͓��F�f�B�t���[�Y�J���[��ԐF�ɂ���
		MV1SetMaterialDifColor(modelId_, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else if(isNoticeHearing_)
	{
		// ���o�͈͊O�F�f�B�t���[�Y�J���[���D�F�ɂ���
		MV1SetMaterialDifColor(modelId_, 0, GetColorF(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		MV1SetMaterialDifColor(modelId_, 0, GetColorF(0.5f, 0.5f, 0.5f, 1.0f));
	}
	MV1DrawModel(modelId_);

	// ����͈͂̕`��
	DrawDebugSearchRange();

	// �����蔻��̕`��
	DrawCapsule3D(VAdd(pos_, { 0.0f,150.0f,0.0f }), VAdd(pos_, { 0.0f,30.0f,0.0f }), collisionRadius_, 10, 0xff0000, 0xff0000, false);

}

void Enemy::DrawDebugSearchRange(void)
{

	// ���o
	DrawCone3D(VAdd(pos_, { 0.0f,0.0f,0.0f }),
			   VAdd(pos_, { 0.0f,1.0f,0.0f }),
			   HEARING_RANGE, 10, 0xffff7f, 0xffff7f, true);

	// ���o
	float viewRad = AsoUtility::Deg2RadF(VIEW_ANGLE);

	// ���΍��W
	VECTOR localPos = { 0.0f,10.0f,VIEW_RANGE };

	// Enemy�̉�]
	MATRIX selfMat = MGetIdent();
	selfMat = MMult(selfMat, MGetRotY(rot_.y));

	// Enemy�̑O��
	VECTOR rotForwardLocalPos = VTransform(localPos, selfMat);

	// ����p�̉�]
	MATRIX viewMat = MGetIdent();
	viewMat = MMult(selfMat, MGetRotY(viewRad));
	VECTOR rotViewRLocalPos = VTransform(localPos, viewMat);

	// �ق��̕`��Əd�Ȃ�Ȃ��悤�ɍ��W��1�グ��
	VECTOR pos = pos_;
	pos.y += 10.0f;
	VECTOR pos0 = pos;
	VECTOR pos1 = VAdd(pos_, rotForwardLocalPos);
	VECTOR pos2 = VAdd(pos_, rotViewRLocalPos);

	// �`��
	DrawTriangle3D(pos0, pos1, pos2 ,0xffdead, true);
	DrawLine3D(pos0, pos1, 0x000000);
	DrawLine3D(pos1, pos2, 0x000000);
	DrawLine3D(pos2, pos0, 0x000000);

	// ���Α�
	viewMat = MMult(selfMat, MGetRotY(-viewRad));
	VECTOR rotViewLLocalPos = VTransform(localPos, viewMat);
	VECTOR pos3 = VAdd(pos, rotViewLLocalPos);

	// �`��
	DrawTriangle3D(pos0, pos3, pos1, 0xffdead, true);
	DrawLine3D(pos0, pos3, 0x000000);
	DrawLine3D(pos3, pos1, 0x000000);
	DrawLine3D(pos1, pos0, 0x000000);

}

float Enemy::GetCollisionRadius(void)
{
	return collisionRadius_;
}
