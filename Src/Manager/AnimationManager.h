#pragma once


class AnimationManager
{
public:

	AnimationManager(void);
	~AnimationManager();
	
#pragma region �u�����h���Ȃ��ꍇ
	//�A�j���[�V�����Đ�
	void Animation(const int modelId);

	//�A�j���[�V�����A�^�b�`
	void AttachAnim(
		const int modelId,
		const int animmodel,
		const float speed,
		bool loop
	);

	//�A�j���[�V�����J�E���g
	float GetAnimStep(void) const;

	//�l�֘A�̃��Z�b�g
	void DettachAnim(int modelId);

#pragma endregion


#pragma region �u�����h����ꍇ
	
	//�A�j���[�V�����Đ��u�����h
	void BlendAnimation(const int modelId);

	//�A�j���[�V�����A�^�b�`(�u�����h)
	void AttachBlendAnim(
		const int modelId,
		const int animmodelnow,
		const float speed,
		bool loop
	);

	//�A�j���[�V�����J�E���g(���݂̃A�j���[�V����)
	float GetAnimStepNow(void) const;

	//�A�j���[�V�����J�E���g(�O�̃A�j���[�V����)
	float GetAnimStepOld(void) const;

	//�l�֘A�̃��Z�b�g
	void DettachBlendAnim(int modelId);

#pragma endregion

	//�A�j���[�V�������Đ�����
	float GetTotaltime(void) const;

private:

	//�A�j���[�V�������Đ�����
	float animTotaltime;

	//�A�j���[�V���������[�v���邩
	bool loopFlag;

	//�A�j���[�V�������x
	float animSpeed;


#pragma region �u�����h���Ȃ��ꍇ
	//�A�j���[�V�����A�^�b�`�ԍ�
	int animAttachNo;
	//�A�j���[�V�����J�E���g
	float animStep;
#pragma endregion


#pragma region �u�����h����ꍇ
	
	////���݂̃A�j���[�V����

	int animAttachNow;		//�A�^�b�`�ԍ�
	float animStepNow;		//�A�j���[�V�����J�E���g


	////�O�̃A�j���[�V����

	int animAttachOld;		//�A�^�b�`�ԍ�
	float animStepOld;		//�A�j���[�V�����J�E���g


	//�u�����h���[�g(����:1.0f���ő�)
	float rate_;


#pragma endregion


};

