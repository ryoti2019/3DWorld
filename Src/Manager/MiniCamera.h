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

	// スクリーンのハンドルID取得
	int GetScreen(void) const;

private:

	// カメラ
	Camera* camera_;

	// スクリーンのハンドルID
	int miniScreen_;

	// ミニカメラモデルのハンドルID
	int modelId_;
	
	// ミニカメラ用の描画領域サイズ
	Vector2 size_;

	// ミニカメラモデルの位置
	VECTOR pos_;

};

