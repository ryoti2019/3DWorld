#pragma once
#include <DxLib.h>
class Player;

class Camera
{
public:

	// カメラの高さ
	static constexpr float HEIGHT = 250.0f;

	// 追従対象からカメラまでの距離
	static constexpr float DIS_FOLLOW2CAMERA = 150.0f;

	// 追従対象から注視点までの距離
	static constexpr float DIS_FOLLOW2TARGET = 350.0f;

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT, // 定点カメラ
		FREE, // フリーモード
		FOLLOW, // 追従モード 
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFree(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;

	VECTOR GetAngles(void) const;

	VECTOR GetTargetPos(void) const;

	// 追従相手の設定
	void SetFollow(Player* follow);

	// カメラモードの変更
	void ChangeMode(MODE mode);

private:

	// 追従相手
	Player* follow_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラの角度(rad)
	VECTOR angles_;

	// 注視点
	VECTOR targetPos_;

	// XYZのどの回転にも
	void MoveXYZDirection(void);

	// 角度によるXZ平面の移動
	void MoveXZAngle(void);

	// 注視点をキャラクター位置に設定
	void SetTargetPosFollow(void);

	// 注視点をキャラクター前方位置に設定
	void SetTargetPosFollowForward(void);



};
