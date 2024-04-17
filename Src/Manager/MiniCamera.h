#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
class Camera;

class MiniCamera
{

public:

	static constexpr int SIZE_X = 200;
	static constexpr int SIZE_Y = 200;

	MiniCamera(Camera* camera);
	~MiniCamera(void);

	void Init(void);
	void Update(void);
	void DrawScreen(void);
	void Draw(void);
	void Release(void);

	// �X�N���[���̃n���h��ID�擾
	int GetScreen(void) const;

private:

	// �J����
	Camera* camera_;

	// �X�N���[���̃n���h��ID
	int miniScreen_;

	// �~�j�J�������f���̃n���h��ID
	int modelId_;
	
	// �~�j�J�����p�̕`��̈�T�C�Y
	Vector2 size_;

	// �~�j�J�������f���̈ʒu
	VECTOR pos_;

};

