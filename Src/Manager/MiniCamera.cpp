#include "../Application.h"
#include "Camera.h"
#include "MiniCamera.h"

MiniCamera::MiniCamera(Camera* camera)
{
	camera_ = camera;
	size_ = Vector2(SIZE_X, SIZE_Y);
	miniScreen_ = MakeScreen(size_.x, size_.y);
	modelId_ = -1;
	pos_ = { 0.0f, 0.0f, 0.0f };
}

MiniCamera::~MiniCamera(void)
{
}

void MiniCamera::Init(void)
{

	// �~�j�J�������f���̓ǂݍ���
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Camera.mv1").c_str());

	// �~�j�J�������f���̑傫����ݒ�
	MV1SetScale(modelId_, { 3.0f, 3.0f, 3.0f });

	// �~�j�J�������f���̈ʒu��ݒ�
	pos_ = { 0.0f, 0.0f, 0.0f };
	MV1SetPosition(modelId_, pos_);

}

void MiniCamera::Update(void)
{
	// �g�p���Ă���J�����̊p�x�ɏ�ɍ��킹��
	MV1SetRotationXYZ(modelId_, camera_->GetAngles());
}

void MiniCamera::DrawScreen(void)
{

	// �`��̈��ύX
	SetDrawScreen(miniScreen_);

	// ��ʂ�������
	ClearDrawScreen();

	// �~�j�J�������ʂ��J�����̐ݒ�
	VECTOR pos = pos_;

	// �~�j�J�������f���̎�O
	pos.z -= 500.0f;

	// �J�����ʒu���Z�b�g
	SetCameraPositionAndAngle(
		pos,
		0.0f,
		0.0f,
		0.0f
	);

	// ���C�g�̐ݒ�
	ChangeLightTypeDir({ 0.0f, -1.0f, 0.0f });

	// �~�j�J�������f����`��
	MV1DrawModel(modelId_);

	// ���C�g�̐ݒ�
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

}

void MiniCamera::Draw(void)
{

	// ��ʉE��ɘg�\��
	int width = 10;
	DrawBox(
		Application::SCREEN_SIZE_X - size_.x - (width * 2),
		0,
		Application::SCREEN_SIZE_X,
		size_.y + (width * 2),
		0x000000,
		true
	);

	// �g���Ƀ~�j�J�������ʂ����摜��`��
	DrawGraph(Application::SCREEN_SIZE_X - size_.x - width, width, miniScreen_, false);

}

void MiniCamera::Release(void)
{
	MV1DeleteModel(modelId_);
}

int MiniCamera::GetScreen(void) const
{
	return miniScreen_;
}
