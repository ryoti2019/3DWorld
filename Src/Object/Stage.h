#pragma once
#include <DxLib.h>
class Stage
{
public:
	Stage(void);
	~Stage(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// モデルIDのゲッター
	int GetModelId(void);

private:
	// ステージモデルのハンドルID
	int modelId_;
	// ステージモデルの位置
	VECTOR pos_;
};
