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
	// �O���t�@�C���̂R�c���f�������[�h
	modelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Stage.mv1").c_str());

	// �R�c���f���̈ʒu(�����́A�R�c���W)
	MV1SetPosition(modelId_, { 0.0f, 75.0f, 0.0f });

	// �Փ˔�����(�R���C�_)�̍쐬
	MV1SetupCollInfo(modelId_);

}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(modelId_);
}

void Stage::Release(void)
{
}

int Stage::GetModelId(void)
{
	return modelId_;
}
