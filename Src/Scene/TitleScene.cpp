#include <cmath>
#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "TitleScene.h"
#include "../Manager/Camera.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Shot.h"
#include "../Object/Enemy.h"
#include "../Object/Grid.h"
#include "../Object/RollBall.h"

TitleScene::TitleScene(void) : SceneBase()
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{

	// �O���b�h���̏�����
	grid_ = new Grid();
	grid_->Init();

	// �X�e�[�W�̏�����
	stage_ = new Stage();
	stage_->Init();

	// �v���C���[�̏�����
	player_ = new Player();
	player_->Init();

	// �G�l�~�[�̏�����
	enemy_ = new Enemy(player_);
	enemy_->Init();

	// �{�[���̏�����
	rollBall_ = new RollBall(player_);
	rollBall_->Init();

	// �J�������[�h�ύX
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(player_);
	camera->ChangeMode(Camera::MODE::FOLLOW);

}

void TitleScene::Update(void)
{

	// �O���b�h���̍X�V
	grid_->Update();

	// �X�e�[�W�̍X�V
	stage_->Update();

	// �v���C���[�̍X�V
	player_->Update();

	// �X�e�[�W���f��ID
	int stageModelId = stage_->GetModelId();

	// �e�̍X�V
	auto shots = player_->GetShots();
	for (auto shot : shots)
	{

		shot->Update();

		// �e��SHOT��Ԃ���Ȃ�������Փ˔��肵�Ȃ�
		if (!shot->IsShot())
		{
			// �������⏈���I����́A�ȍ~�̏����͎��s���Ȃ�
			continue;
		}

		// �X�e�[�W���f���Ƃ̏Փ˔���
		auto info = MV1CollCheck_Sphere(
			stageModelId, -1, shot->GetPos(), shot->GetCollisionRadius());

		if (info.HitNum > 0)
		{
			// �Փ˂��Ă���
			shot->Blast();
		}

		MV1CollResultPolyDimTerminate(info);

		// �e�ƓG�̏Փ˔���
		if (AsoUtility::IsHitSphereCapsule(
			shot->GetPos(), shot->GetCollisionRadius(),
			VAdd(enemy_->GetPos(), { 0.0f,150.0f,0.0f }), VAdd(enemy_->GetPos(), {0.0f,30.0f,0.0f}),enemy_->GetCollisionRadius()))
		{
			// �G�Ƀ_���[�W��^����
			shot->Blast();
		}

	}
	// �G�l�~�[�̍X�V
	enemy_->Update();

	// �{�[���̍X�V
	rollBall_->Update();

}

void TitleScene::Draw(void)
{

	// �O���b�h���̕`��
	grid_->Draw();

	// �X�e�[�W�̕`��
	stage_->Draw();

	// �v���C���[�̕`��
	player_->Draw();

	// �e�̍X�V
	auto shots = player_->GetShots();
	for (auto shot : shots)
	{
		if (shot->IsAlive())
		{
			shot->Draw();
		}


		//DrawSphere3D(shot->GetPos(), shot->GetCollisionRadius(),
		//	10, 0xff0000, 0xff0000, false);

	}

	// �G�l�~�[�̕`��
	enemy_->Draw();

	// �{�[���̍X�V
	rollBall_->Draw();

	// �f�o�b�O�`��
	DrawDebug();

}

void TitleScene::DrawDebug(void)
{

	// �J���������擾
	Camera* camera = SceneManager::GetInstance().GetCamera();
	VECTOR pos = camera->GetPos();
	VECTOR angles = camera->GetAngles();

	DrawFormatString(0, 10, 0xffffff, "�J�������W : (%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);
	DrawFormatString(0, 30, 0xffffff, "�J�����p�x : (%.1f, %.1f, %.1f)", 180 / DX_PI * angles.x, 180 / DX_PI * angles.y, 180 / DX_PI * angles.z);
	
	Player* player = player_;
	VECTOR pPos = player->GetPos();
	angles = player->GetRot();
	DrawFormatString(0, 50, 0xffffff, "�v���C���[���W : (%.1f, %.1f, %.1f)", pPos.x, pPos.y, pPos.z);
	DrawFormatString(0, 70, 0xffffff, "�v���C���[�p�x : (%.1f, %.1f, %.1f)", 180 / DX_PI * angles.x, 180 / DX_PI * angles.y, 180 / DX_PI * angles.z);
	
	//DrawSphere3D(pos, 20, 100, 0xffffff, 0xffffff, true);
	
	DrawSphere3D(camera->GetTargetPos(), 20, 1, 0xffffff, 0xffffff, true);
	//DrawDebugTest();

	RollBall* rollBall = rollBall_;
	VECTOR bPos = rollBall->GetPos();
	DrawFormatString(0, 90, 0xffffff, "�{�[�����W : (%.1f, %.1f, %.1f)", bPos.x, bPos.y, bPos.z);

	float rad = atan2f(bPos.x - pPos.x, bPos.z - pPos.z);
	float deg = AsoUtility::Rad2DegF(rad);

	DrawFormatString(0, 130, 0xffffff, "���{�[��");
	DrawFormatString(0, 150, 0xffffff, "���W�@�@�@�@�@�@�@: (%.2f, %.2f, %.2f", bPos.x, bPos.y, bPos.z);

	DrawFormatString(0, 170, 0xffffff, "�΃��[���h�p(Atan2) : %.2f", deg);

	// �v���C���[�̌�������{�[���p�x
	VECTOR pRot = player->GetRot();

	// ���̂̃��[���h���W����A�v���C���[�p������
	float viewRad = rad - pRot.y;

	// ���W�A������f�O���[�ɕϊ�
	float viewDeg = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(viewRad)));
	DrawFormatString(0, 190, 0xffffff, "�΃v���C���[�p(0-360) : %.2f", viewDeg);

	// �G�̍��W
	Enemy* enemy = enemy_;
	VECTOR ePos = enemy->GetPos();
	DrawFormatString(0, 210, 0xffffff, "�G���W : (%.1f, %.1f, %.1f)", ePos.x, ePos.y, ePos.z);

}

void TitleScene::DrawDebugTest(void)
{
	const int CENTER_Y = Application::SCREEN_SIZE_Y / 2;
	// �����̒���
	const float LENGTH = 100.0f;
	// ���̒���
	const float ARROW_LENGTH = 20.0f;
	const float ARROW_RAD = DX_PI_F / 180.0f * 150.0f;

	VECTOR direction;
	VECTOR rotationVec;
	Vector2 rotationPos;

	// ��U�h��Ԃ�
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x0000ff, true);

	// ���T�|�[�g
	const Vector2 Q01_POS = { 200, CENTER_Y };
	DrawCircle(Q01_POS.x, Q01_POS.y, static_cast<int>(LENGTH), 0xffffff, true);
	const Vector2 Q02_POS = { 500, CENTER_Y };
	DrawCircle(Q02_POS.x, Q02_POS.y, static_cast<int>(LENGTH), 0xffffff, true);
	const Vector2 Q03_POS = { 800, CENTER_Y };
	DrawCircle(Q03_POS.x, Q03_POS.y, static_cast<int>(LENGTH), 0xffffff, true);

	//--------------------------------------------------
	// �@�x�N�g���O�x

	// 0�x�̕��������߂�
	direction.x = cosf(AsoUtility::Deg2RadF(30.0f));
	direction.y = sinf(AsoUtility::Deg2RadF(30.0f));
	// �ړ��ʁ������~�X�J���[�i�́A�����A�X�s�[�h�j
	rotationVec.x = direction.x * LENGTH;
	rotationVec.y = direction.y * LENGTH;
	// ���W�{�ړ��ʂŁA�ړ��������O�x�̕����ɐ���������
	rotationPos.x = Q01_POS.x + static_cast<int>(rotationVec.x);
	rotationPos.y = Q01_POS.y + static_cast<int>(rotationVec.y);
	DrawLine(Q01_POS.x, Q01_POS.y, rotationPos.x, rotationPos.y, 0x000000);

	// �O�x�̕��������߂�
	direction.x = cosf(AsoUtility::Deg2RadF(60.0f));
	direction.y = sinf(60.0f* DX_PI_F / 180.0f);
	// �ړ��ʁ������~�X�J���[�i�́A�����A�X�s�[�h�j
	rotationVec.x = direction.x * LENGTH;
	rotationVec.y = direction.y * LENGTH;
	// ���W�{�ړ��ʂŁA�ړ��������O�x�̕����ɐ���������
	rotationPos.x = Q01_POS.x + static_cast<int>(rotationVec.x);
	rotationPos.y = Q01_POS.y + static_cast<int>(rotationVec.y);
	DrawLine(Q01_POS.x, Q01_POS.y, rotationPos.x, rotationPos.y, 0xff0000);


	//----------------------------------------------------

	deg_ += 1.0f;
	direction.x = cosf(AsoUtility::Deg2RadF(deg_));
	direction.y = sinf(AsoUtility::Deg2RadF(deg_));

	rotationVec.x = direction.x * LENGTH;
	rotationVec.y = direction.y * LENGTH;

	rotationPos.x = Q02_POS.x + static_cast<int>(rotationVec.x);
	rotationPos.y = Q02_POS.y + static_cast<int>(rotationVec.y);
	DrawLine(Q02_POS.x, Q02_POS.y, rotationPos.x, rotationPos.y, 0xff0000);

	if (deg_ >= 360.0f)
	{
		deg_ = 0.0f;
	}

	//----------------------------------------------------
	
	// 110�x�̍�����(�C�Ӄx�N�g��=��]���������x�N�g��)
	direction.x = cosf(AsoUtility::Deg2RadF(110.0f));
	direction.y = sinf(AsoUtility::Deg2RadF(110.0f));

	rotationVec.x = direction.x * LENGTH;
	rotationVec.y = direction.y * LENGTH;

	rotationPos.x = Q03_POS.x + static_cast<int>(rotationVec.x);
	rotationPos.y = Q03_POS.y + static_cast<int>(rotationVec.y);
	DrawLine(Q03_POS.x, Q03_POS.y, rotationPos.x, rotationPos.y, 0x000000);

	// ���x�N�g���@�̕���
	VECTOR arrow1_Dir;
	arrow1_Dir.x = cosf(ARROW_RAD) * direction.x - (sinf(ARROW_RAD) * direction.y);
	arrow1_Dir.y = sinf(ARROW_RAD) * direction.x + (cosf(ARROW_RAD) * direction.y);

	// ���x�N�g���@�̍��W = ���W(�����̏I�_)+�ړ���(�����~����)
	VECTOR arrow1_Pos;
	arrow1_Pos.x = rotationPos.x + (arrow1_Dir.x * ARROW_LENGTH);
	arrow1_Pos.y = rotationPos.y + (arrow1_Dir.y * ARROW_LENGTH);
	DrawLine(rotationPos.x, rotationPos.y,
		static_cast<int>(arrow1_Pos.x), static_cast<int>(arrow1_Pos.y), 0xff0000);

	// ���x�N�g���A�̕���
	VECTOR arrow2_Dir;
	arrow2_Dir.x = cosf(-ARROW_RAD) * direction.x - (sinf(-ARROW_RAD) * direction.y);
	arrow2_Dir.y = sinf(-ARROW_RAD) * direction.x + (cosf(-ARROW_RAD) * direction.y);

	// ���x�N�g���A�̍��W = ���W(�����̏I�_)+�ړ���(�����~����)
	VECTOR arrow2_Pos;
	arrow2_Pos.x = rotationPos.x + (arrow2_Dir.x * ARROW_LENGTH);
	arrow2_Pos.y = rotationPos.y + (arrow2_Dir.y * ARROW_LENGTH);
	DrawLine(rotationPos.x, rotationPos.y,static_cast<int>(arrow2_Pos.x), static_cast<int>(arrow2_Pos.y),0xff0000);

}

void TitleScene::Release(void)
{

	// �O���b�h���̊J��
	grid_->Release();
	delete grid_;

	// �X�e�[�W�̊J��
	stage_->Release();
	delete stage_;

	// �v���C���[�̊J��
	player_->Release();
	delete player_;

	// �e�̍X�V
	auto shots = player_->GetShots();
	for (auto shot : shots)
	{
		shot->Release();
		delete shot;
	}

	// �{�[���̊J��
	rollBall_->Release();
	delete rollBall_;

}
