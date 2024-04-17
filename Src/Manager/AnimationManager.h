#pragma once


class AnimationManager
{
public:

	AnimationManager(void);
	~AnimationManager();
	
#pragma region ブレンドしない場合
	//アニメーション再生
	void Animation(const int modelId);

	//アニメーションアタッチ
	void AttachAnim(
		const int modelId,
		const int animmodel,
		const float speed,
		bool loop
	);

	//アニメーションカウント
	float GetAnimStep(void) const;

	//値関連のリセット
	void DettachAnim(int modelId);

#pragma endregion


#pragma region ブレンドする場合
	
	//アニメーション再生ブレンド
	void BlendAnimation(const int modelId);

	//アニメーションアタッチ(ブレンド)
	void AttachBlendAnim(
		const int modelId,
		const int animmodelnow,
		const float speed,
		bool loop
	);

	//アニメーションカウント(現在のアニメーション)
	float GetAnimStepNow(void) const;

	//アニメーションカウント(前のアニメーション)
	float GetAnimStepOld(void) const;

	//値関連のリセット
	void DettachBlendAnim(int modelId);

#pragma endregion

	//アニメーション総再生時間
	float GetTotaltime(void) const;

private:

	//アニメーション総再生時間
	float animTotaltime;

	//アニメーションをループするか
	bool loopFlag;

	//アニメーション速度
	float animSpeed;


#pragma region ブレンドしない場合
	//アニメーションアタッチ番号
	int animAttachNo;
	//アニメーションカウント
	float animStep;
#pragma endregion


#pragma region ブレンドする場合
	
	////現在のアニメーション

	int animAttachNow;		//アタッチ番号
	float animStepNow;		//アニメーションカウント


	////前のアニメーション

	int animAttachOld;		//アタッチ番号
	float animStepOld;		//アニメーションカウント


	//ブレンドレート(割合:1.0fが最大)
	float rate_;


#pragma endregion


};

