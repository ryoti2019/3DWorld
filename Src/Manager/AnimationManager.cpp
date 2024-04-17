#include <DxLib.h>
#include "SceneManager.h"
#include "AnimationManager.h"

void AnimationManager::Animation(const int modelId)
{
	float deltaTime = 1.0f / SceneManager::DEFAULT_FPS;

	//�A�j���[�V�������Ԃ̎擾
	animStep += (animSpeed * deltaTime);
	if (animStep > animTotaltime && loopFlag)
	{
		animStep = 0.0f;
	}

	//�Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(modelId, animAttachNo, animStep);
}

void AnimationManager::BlendAnimation(const int modelId)
{
	auto& insS = SceneManager::GetInstance();

	float deltaTime = insS.GetDeltaTime();

	// �u�����h�����P�ȉ��̏ꍇ�͂P�ɋ߂Â���
	if (rate_ < 1.0f)
	{
		rate_ += 0.05f;
		if (rate_ > 1.0f)
		{
			rate_ = 1.0f;
		}
	}

	//���݂̃A�j���[�V�����̍Đ�
	if (animAttachNow != -1)
	{
		animStepNow += (animSpeed * deltaTime);
		if (animStepNow > animTotaltime && loopFlag)
		{
			animStepNow = fmodf(animStepNow,animTotaltime);
		}

		//�Đ�����A�j���[�V�������Ԃ̐ݒ�
		MV1SetAttachAnimTime(modelId, animAttachNow, animStepNow);

		MV1SetAttachAnimBlendRate(modelId, animAttachNow, rate_);
	}

	//�O�̃A�j���[�V�������u�����h�p�ɍĐ�
	if (animAttachOld != -1)
	{

		animStepOld += (animSpeed * deltaTime);
		if (animStepOld > animTotaltime && loopFlag)
		{
			animStepOld = fmodf(animStepOld, animTotaltime);
		}

		MV1SetAttachAnimTime(modelId, animAttachOld, animStepOld);

		MV1SetAttachAnimBlendRate(modelId, animAttachOld, 1.0f - rate_);

	}

}

void AnimationManager::AttachAnim(
	const int modelId, 
	const int animmodel,
	const float speed,
	bool loop
)
{

	loopFlag = loop;

	//�A�j���[�V�������A�^�b�`
	animAttachNo = MV1AttachAnim(modelId, 1, animmodel, true);

	//�A�j���[�V�����̑��Đ����Ԃ��擾
	animTotaltime = MV1GetAttachAnimTotalTime(modelId, animAttachNo);

	animSpeed = speed;

	animStep = 0.0f;

}

void AnimationManager::AttachBlendAnim(
	const int modelId, 
	const int animmodelnow,
	const float speed,
	const bool loop)
{

	loopFlag = loop;

	if (animAttachOld != -1)
	{
		MV1DetachAnim(modelId, animAttachOld);
		animAttachOld = -1;
	}

	//�O�̃A�j���[�V������old�ɓ����
	animAttachOld = animAttachNow;

	//�Đ��J�E���g�������p��
	animStepOld = animStepNow;

	//�Đ��������A�j���[�V���������f���ɃA�^�b�`
	animAttachNow = MV1AttachAnim(modelId, 0, animmodelnow, true);

	//���Đ����Ԃ̎擾
	animTotaltime = MV1GetAttachAnimTotalTime(modelId, animAttachNow);

	//�Đ��������A�j���[�V�����̍Đ��J�E���g�����Z�b�g
	animStepNow = 0.0f;

	animSpeed = speed;

	//�u�����h���[�g�̃��Z�b�g
	rate_ = animAttachOld == -1 ? 1.0f : 0.0f;

}

void AnimationManager::DettachBlendAnim(int modelId)
{
	MV1DetachAnim(modelId,animAttachNow);
	animAttachNow = -1;
	MV1DetachAnim(modelId, animAttachOld);
	animAttachOld = -1;
	animTotaltime = 0.0f;
	animStepNow = 0.0f;
	animSpeed = 1.0f;
	rate_ = 0.0f;
}

void AnimationManager::DettachAnim(int modelId)
{
	MV1DetachAnim(modelId, animAttachNo);
	animAttachNo = -1;
	animTotaltime = 0.0f;
	animSpeed = 1.0f;
	animStep = 0.0f;
}

float AnimationManager::GetTotaltime(void) const
{
	return animTotaltime;
}

float AnimationManager::GetAnimStepNow(void) const
{
	return animStepNow;
}

float AnimationManager::GetAnimStepOld(void) const
{
	return animStepOld;
}

float AnimationManager::GetAnimStep(void) const
{
	return animStep;
}

AnimationManager::AnimationManager(void)
{
	loopFlag = true;

	animAttachNow = -1;
	animAttachOld = -1;
	animStepNow = 0.0f;
	animStepOld = 0.0f;
	rate_ = 0.0f;

	animAttachNo = -1;
	animSpeed = 1.0f;
	animStep = 0.0f;

	animSpeed = 1.0f;
	animTotaltime = 0.0f;
}

AnimationManager::~AnimationManager()
{
	animAttachNow = -1;
	animAttachOld = -1;
	animTotaltime = 0.0f;
	animStepNow = 0.0f;
	rate_ = 0.0f;

	//delete instance_;
}
