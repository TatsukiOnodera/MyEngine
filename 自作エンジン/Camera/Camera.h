#pragma once
#include <DirectXMath.h>

#include <list>

using namespace std;

class Camera
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static Camera* GetInstance();

private: // メンバ変数
	// 注視点座標
	XMFLOAT3 m_target = { 0, 0, 0 };
	// 始点座標
	XMFLOAT3 m_eye = { 0, 0, -50 };
	// 上方向ベクトル
	XMFLOAT3 m_up = { 0, 1, 0 };
	// 注視点から始点までの距離
	XMFLOAT3 m_distance = { 0, 0, 0 };
	// カメラの最近Z値
	float m_nearZ = 0.1f;
	// カメラの最遠Z値
	float m_farZ = 1000.0f;
	// ビュー行列
	XMMATRIX m_matView;
	// 射影行列
	XMMATRIX m_matProjection;
	// ビルボード行列
	XMMATRIX m_matBillboard;
	// Y軸ビルボード行列
	XMMATRIX m_matBillboardY;
	// X軸の角度
	float m_angleX = 0.0f;
	// Y軸の角度
	float m_angleY = 0.0f;
	// Z軸の角度
	float m_angleZ = 0.0f;
	// ダーティーフラグ
	bool m_dirty = false;
	//	 更新したか
	bool m_isDirty = false;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Camera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 透視投影による射影行列の更新
	/// </summary>
	void UpdateMatProjection();

	/// <summary>
	/// カメラの移動
	/// </summary>
	/// <param name="move">移動量</param>
	void MoveCamera(XMFLOAT3 move);

	/// <summary>
	/// 追従カメラ
	/// </summary>
	/// <param name="target">注視点</param>
	/// <param name="eyeDistance">注視点から始点への距離</param>
	/// <param name="addAngleX">X軸の角度</param>
	/// <param name="addAngleY">Y軸の角度</param>
void FollowUpCamera(XMFLOAT3 target, XMFLOAT3 eyeDistance, float addAngleX, float addAngleY);

	/// <summary>
	/// カメラを軸に座標を移動
	/// </summary>
	/// <param name="pos">初期位置</param>
	/// <param name="vec">移動量</param>
	/// <returns>移動した後の座標</returns>
	XMFLOAT3 ConvertWindowPos(XMFLOAT3 pos, XMFLOAT3 vec);

	/// <summary>
	/// 3D座標を2D座標に変換
	/// </summary>
	/// <param name="pos">3D座標</param>
	/// <returns>2D座標</returns>
	XMFLOAT2 Convert3Dto2D(XMFLOAT3 pos);

public: //アクセッサ
	/// <summary>
	/// カメラ座標取得
	/// </summary>
	/// <returns>カメラ座標</returns>
	XMFLOAT3 GetEye() { return m_eye; }

	/// <summary>
	/// カメラ座標セット
	/// </summary>
	/// <param name="eye">カメラ座標</param>
	void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// 焦点座標取得
	/// </summary>
	/// <returns>頂点座標</returns>
	XMFLOAT3 GetTarget() { return m_target; }

	/// <summary>
	/// 焦点座標セット
	/// </summary>
	/// <param name="target">焦点座標</param>
	void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// 上下取得
	/// </summary>
	/// <returns>上の向き</returns>
	XMFLOAT3 GetUp() { return m_up; }

	/// <summary>
	/// 上下のセット
	/// </summary>
	/// <param name="up">上の向き</param>
	void SetUp(XMFLOAT3 up);

	/// <summary>
	/// 注視点から始点までの距離取得
	/// </summary>
	/// <returns>注視点から始点までの距離</returns>
	XMFLOAT3 GetDistance() { return m_distance; }

	/// <summary>
	/// 注視点から始点までの距離セット
	/// </summary>
	void SetDistance();

	/// <summary>
	/// カメラの回転角を取得
	/// </summary>
	/// <returns>三方向のカメラの回転角</returns>
	XMFLOAT3 GetAngle() { return XMFLOAT3(m_angleX, m_angleY, m_angleZ); }

	/// <summary>
	/// カメラの最近遠Z値を取得
	/// </summary>
	/// <returns>最近遠Z値</returns>
	XMFLOAT2 GetNearFarZ() { return XMFLOAT2(m_nearZ, m_farZ); }

	/// <summary>
	/// カメラの最近遠Z値をセット
	/// </summary>
	/// <param name="nearZ">最近値</param>
	/// <param name="farZ">最遠値</param>
	void SetNearFarZ(float nearZ, float farZ);

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	XMMATRIX GetMatView() { return m_matView; }

	/// <summary>
	/// 射影行列
	/// </summary>
	/// <returns>射影行列</returns>
	XMMATRIX GetMatProject() { return m_matProjection; }

	/// <summary>
	/// ビルボード行列
	/// </summary>
	/// <returns>ビルボード行列</returns>
	XMMATRIX GetMatBillboard() { return m_matBillboard; }

	/// <summary>
	/// カメラ更新の成否
	/// </summary>
	/// <returns>成否</returns>
	bool GetDirty() { return m_isDirty; };
};