#include "../Application.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	// 外部ファイルの３Ｄモデルをロード
	modelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Stage.mv1").c_str());

	// ３Ｄモデルの位置(引数は、３Ｄ座標)
	MV1SetPosition(modelId_, { 0.0f, 75.0f, 0.0f });

	// 衝突判定情報(コライダ)の作成
	MV1SetupCollInfo(modelId_);

}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(modelId_);
}

void Stage::Release(void)
{
}

int Stage::GetModelId(void)
{
	return modelId_;
}
