#pragma once
class Vector2
{
public:

	int x;
	int y;

	// �R���X�g���N�^
	Vector2(void);

	// �R���X�g���N�^
	Vector2(int vX, int vY);

	// �f�X�g���N�^
	~Vector2(void);

	Vector2 operator+(const Vector2& v);

};
