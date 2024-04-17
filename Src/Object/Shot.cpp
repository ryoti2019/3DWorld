#include "Shot.h"

Shot::Shot(int baseModelId, int* blastImgs, int blastAnimNum)
{
	shotModel_ = MV1DuplicateModel(baseModelId);
	blastImgs_ = blastImgs;
	blastAnimNum_ = blastAnimNum;
}

Shot::~Shot(void)
{
}

void Shot::CreateShot(VECTOR pos, VECTOR dir)
{
	
	// �e�̑傫����ݒ�
	scl_ = { 0.8f, 0.8f, 0.8f };

	// �e�̊p�x��ݒ�
	rot_ = { 0.0f, 0.0f, 0.0f };

	// ���W
	pos_ = pos;

	// ����
	dir_ = dir;

	// �e�̔��a
	collisionRadius_ = 5.0f;

	// �e�̑��x
	speed_ = 8.0f;

	// �e�̐������Ԃ̐ݒ�
	stepAlive_ = SHOT_ALIVE;

	isAlive_ = true;


	blastSpeedAnim_ = 1.0f;

	// ��Ԃ̏�����
	state_ = STATE::NONE;

	// ��ԑJ��
	ChangeState(STATE::SHOT);
}

void Shot::Init(void)
{
}

void Shot::Update(void)
{

	switch (state_)
	{

	case Shot::STATE::SHOT:
		UpdateShot();
		break;
	case Shot::STATE::BLAST:
		UpdateBlast();
		break;
	case Shot::STATE::END:
		UpdateEnd();
		break;

	}

}

void Shot::UpdateShot(void)
{
	// �e���ړ�������
	// �ړ��ʂ̌v�Z(�����~�X�s�[�h)
	VECTOR movePow;
	movePow = VScale(dir_, speed_);

	// �ړ�����(���W+�ړ���)
	pos_ = VAdd(pos_, movePow);

	// �傫���̐ݒ�
	MV1SetScale(shotModel_, scl_);

	// �p�x�̐ݒ�
	MV1SetRotationXYZ(shotModel_, rot_);

	// �ʒu�̐ݒ�
	MV1SetPosition(shotModel_, pos_);
}

void Shot::UpdateBlast(void)
{
	// �����A�j���[�V����
	blastCntAnim_++;

	// �����̃A�j���[�V�����ԍ�
	blastIdxAnim_ = static_cast<int>(static_cast<float>(blastCntAnim_) * blastSpeedAnim_) % blastAnimNum_;

	// �����A�j���[�V�����̏I������
	if (blastIdxAnim_ + 1 >= blastAnimNum_)
	{
		ChangeState(STATE::END);
	}
}

void Shot::UpdateEnd(void)
{
}

void Shot::Draw(void)
{

	switch (state_)
	{

	case Shot::STATE::SHOT:
		DrawShot();
		break;
	case Shot::STATE::BLAST:
		DrawBlast();
		break;
	case Shot::STATE::END:
		DrawEnd();
		break;

	}

}

void Shot::DrawShot()
{
	MV1DrawModel(shotModel_);

	// �G�̉~�̕`��
	DrawSphere3D(VAdd(pos_, { 0.0f,50.0f,0.0f }), collisionRadius_, 10, 0xff0000, 0xff0000, false);
}

void Shot::DrawBlast()
{
	// 2D�̃A�j���[�V�����`��
	DrawBillboard3D(
		pos_, 0.5f, 0.5f, 200.0f, 0.0f, blastImgs_[blastIdxAnim_], true);
}

void Shot::DrawEnd()
{
}

void Shot::Release(void)
{
	MV1DeleteModel(shotModel_);
}

bool Shot::IsAlive(void)
{
	return isAlive_;
}

bool Shot::IsShot(void)
{
	//��STATE��SHOT��Ԃ�������true
	return state_ == STATE::SHOT;
}

VECTOR Shot::GetPos(void)
{
	return pos_;
}

float Shot::GetCollisionRadius(void)
{
	return collisionRadius_;
}

void Shot::ChangeState(STATE state)
{
	// ��Ԃ̍X�V
	state_ = state;

	// ��ԑJ�ڎ��̏�����
	switch (state_)
	{
	case Shot::STATE::SHOT:
		break;
	case Shot::STATE::BLAST:
		blastCntAnim_ = 0;
		break;
	case Shot::STATE::END:
		break;
	}
}

void Shot::Blast(void)
{
	//��BLAST��Ԃ֑J�ڂ�����
	ChangeState(STATE::BLAST);
}