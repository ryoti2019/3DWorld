#include <DxLib.h>
#include "SceneManager.h"
#include "AnimationManager.h"

void AnimationManager::Animation(const int modelId)
{
	float deltaTime = 1.0f / SceneManager::DEFAULT_FPS;

	//アニメーション時間の取得
	animStep += (animSpeed * deltaTime);
	if (animStep > animTotaltime && loopFlag)
	{
		animStep = 0.0f;
	}

	//再生するアニメーション時間の設定
	MV1SetAttachAnimTime(modelId, animAttachNo, animStep);
}

void AnimationManager::BlendAnimation(const int modelId)
{
	auto& insS = SceneManager::GetInstance();

	float deltaTime = insS.GetDeltaTime();

	// ブレンド率が１以下の場合は１に近づける
	if (rate_ < 1.0f)
	{
		rate_ += 0.05f;
		if (rate_ > 1.0f)
		{
			rate_ = 1.0f;
		}
	}

	//現在のアニメーションの再生
	if (animAttachNow != -1)
	{
		animStepNow += (animSpeed * deltaTime);
		if (animStepNow > animTotaltime && loopFlag)
		{
			animStepNow = fmodf(animStepNow,animTotaltime);
		}

		//再生するアニメーション時間の設定
		MV1SetAttachAnimTime(modelId, animAttachNow, animStepNow);

		MV1SetAttachAnimBlendRate(modelId, animAttachNow, rate_);
	}

	//前のアニメーションをブレンド用に再生
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

	//アニメーションをアタッチ
	animAttachNo = MV1AttachAnim(modelId, 1, animmodel, true);

	//アニメーションの総再生時間を取得
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

	//前のアニメーションをoldに入れる
	animAttachOld = animAttachNow;

	//再生カウントも引き継ぐ
	animStepOld = animStepNow;

	//再生したいアニメーションをモデルにアタッチ
	animAttachNow = MV1AttachAnim(modelId, 0, animmodelnow, true);

	//総再生時間の取得
	animTotaltime = MV1GetAttachAnimTotalTime(modelId, animAttachNow);

	//再生したいアニメーションの再生カウントをリセット
	animStepNow = 0.0f;

	animSpeed = speed;

	//ブレンドレートのリセット
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
