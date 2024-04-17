#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Shot.h"
#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Draw(void)
{
	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(modelId_);

	// �����蔻��̕`��
	DrawCapsule3D(VAdd(pos_, { 0.0f,150.0f,0.0f }), VAdd(pos_, { 0.0f,30.0f,0.0f }), 20, 10, 0xff0000, 0xff0000, false);
}

void Player::InitAnimation(void)
{

	// ���f���̓ǂݍ���
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Walking.mv1").c_str());
	//playerWalk_ = MV1LoadModel((Application::PATH_MODEL + "Human.mv1").c_str());
	playerRun_ = MV1LoadModel((Application::PATH_MODEL + "Running.mv1").c_str());

}

std::vector<Shot*> Player::GetShots(void)
{
	return shots_;
}

void Player::SetParam(void)
{

	// �e�̃��f���̃��[�h
	shotModel_ = MV1LoadModel((Application::PATH_MODEL + "SpikeShot.mv1").c_str());

	// �����G�t�F�N�g�ǂݍ���
	LoadDivGraph((Application::PATH_IMAGE + "BlastSpike.png").c_str(),
		BLAST_ANIM_NUM, 4, 4, BLAST_SIZE_X, BLAST_SIZE_Y, blastImgs_, true);

	// �R�c���f���̈ʒu(�����́A�R�c���W)
	pos_ = AsoUtility::VECTOR_ZERO;

	// ���f���̊p�x
	rot_ = { 0.0f, 0.0f, 0.0f };
	rotLocal_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	animNo_ = 1;

	dir_.y = rot_.y;

	// ��ԑJ��
	ChangeState(STATE::WALK);

}

void Player::Move(void)
{

	// �J�����̊p�x���擾
	VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();

	auto& ins = InputManager::GetInstance();
	float movePow = 3.0f;

	// ���̃V�t�g�L�[�ő���
	if (ins.IsNew(KEY_INPUT_SPACE))
	{
		ChangeState(STATE::RUN);
		movePow = 5.0f;
	}
	else if(!ins.IsNew(KEY_INPUT_SPACE))
	{
		ChangeState(STATE::WALK);
	}

	switch (state_)
	{
	case Player::STATE::WALK:
		SetWalkAnimation();
		break;
	case Player::STATE::RUN:
		SetRunAnimation();
		break;
	}

	// ����(direction)
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// WASD�ŃJ�����̈ʒu��ς���
	if (ins.IsNew(KEY_INPUT_W)) { dir = { 0.0f, 0.0f, 1.0f }; }
	if (ins.IsNew(KEY_INPUT_A)) { dir = { -1.0f, 0.0f, 0.0f }; }
	if (ins.IsNew(KEY_INPUT_S)) { dir = { 0.0f, 0.0f, -1.0f }; }
	if (ins.IsNew(KEY_INPUT_D)) { dir = { 1.0f, 0.0f, 0.0f }; }

	if (!AsoUtility::EqualsVZero(dir))
	{

		// XYZ�̉�]�s��
		// XZ���ʈړ��ɂ���ꍇ�́AXZ�̉�]���l�����Ȃ��悤�ɂ���
		MATRIX mat = MGetIdent();
		//mat = MMult(mat, MGetRotX(angles_.x));
		mat = MMult(mat, MGetRotY(cameraAngles.y));
		//mat = MMult(mat, MGetRotZ(angles_.z));
		
		// ��]�s����g�p���āA�x�N�g������]������
		VECTOR moveDir = VTransform(dir, mat);

		// �����~�X�s�[�h�ňړ��ʂ�����āA���W�ɑ����Ĉړ�
		pos_ = VAdd(pos_, VScale(moveDir, movePow));

		// �������p�x�ɕϊ�����(XZ���� Y��)
		float angle = atan2f(dir.x, dir.z);

		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		//rot_.y = cameraAngles.y + angle;

		// �J�����̊p�x����Ƃ��A�������̊p�x��������
		LazyRotation(cameraAngles.y + angle);

	}

	// �U��
	Attack();

}

void Player::Attack(void)
{

	// �U���L�[�������ƁA�e�𐶐�����
	auto& ins = InputManager::GetInstance();
	if (ins.IsNew(KEY_INPUT_Q) && stepShotDelay_ <= 0.0f)
	{

		// �L���ȋ����擾����
		Shot* shot = GetValidShot();

		// ���W�����炷
		VECTOR pos = pos_;
		pos.y += 70.0;
		
		// �e�𐶐�(�e���˂̏���)

		//// XYZ�̉�]�s��
		//// XZ���ʈړ��ɂ���ꍇ�́AXZ�̉�]���l�����Ȃ��悤�ɂ���
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(rot_.y));

		// ��]�s����g�p���āA�x�N�g������]������
		VECTOR moveDir = VTransform({0.0f,0.0f,1.0f}, mat);

		shot->CreateShot(pos, moveDir);

		// �e�̔��ˌ�̍d�����ԃZ�b�g
		stepShotDelay_ = SHOT_DELAY;

	}

	if (stepShotDelay_ > 0.0f)
	{
		// �e���ˌ�̍d�����Ԃ����炵�Ă���
		stepShotDelay_ -= 1.0f / SceneManager::DEFAULT_FPS;
	}

}

Shot* Player::GetValidShot(void)
{

	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		if (!shots_[i]->IsAlive())
		{
			return shots_[i];
		}
	}
	Shot* shot = nullptr;
	 shot = new Shot(shotModel_, blastImgs_, BLAST_ANIM_NUM);
	shots_.push_back(shot);

	return shot;

}

void Player::ChangeState(STATE state)
{
	// ��Ԃ̍X�V
	state_ = state;
}

void Player::SetWalkAnimation(void)
{

	MV1DetachAnim(modelId_, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(modelId_, animNo_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(modelId_, animAttachNo_);
	//stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 20.0f;

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	MV1SetAttachAnimTime(modelId_, animAttachNo_, stepAnim_);

}

void Player::SetRunAnimation(void)
{

	MV1DetachAnim(modelId_, animAttachNo_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(modelId_, animNo_, playerRun_);

	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(modelId_, animAttachNo_);
	//stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 20.0f;

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	MV1SetAttachAnimTime(modelId_, animAttachNo_, stepAnim_);

}
