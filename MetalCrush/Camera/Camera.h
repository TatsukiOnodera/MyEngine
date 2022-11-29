#pragma once
#include <DirectXMath.h>
#include <list>

class Camera
{
private: // エイリアス
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
	// カメラの角度
	XMFLOAT3 m_angle = { 0, 0, 0 };
	// カメラの視野角
	float m_fovAngleY = 60.0f;
	// カメラの最近Z値
	float m_nearZ = 0.1f;
	// カメラの最遠Z値
	float m_farZ = 20000.0f;
	// ビュー行列
	XMMATRIX m_matView = {};
	// 射影行列
	XMMATRIX m_matProjection = {};
	// ビルボード行列
	XMMATRIX m_matBillboard = {};
	// Y軸ビルボード行列
	XMMATRIX m_matBillboardY = {};
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
	void UpdateMatView();

	/// <summary>
	/// 透視投影による射影行列の更新
	/// </summary>
	void UpdateMatProjection();

	/// <summary>
	/// カメラの移動
	/// </summary>
	/// <param name="move">移動量</param>
	void MoveCamera(const XMFLOAT3& move);

	/// <summary>
	/// 追従カメラ
	/// </summary>
	/// <param name="target">注視点</param>
	/// <param name="eyeDistance">注視点から始点への距離</param>
	/// <param name="addAngle">回転した角度</param>
	void FollowUpCamera(const XMFLOAT3& target, const XMFLOAT3& eyeDistance, const XMFLOAT3& addAngle);

	/// <summary>
	/// カメラのY軸を元に座標を移動
	/// </summary>
	/// <param name="pos">初期位置</param>
	/// <param name="vec">移動量</param>
	/// <returns>移動した後の座標</returns>
	XMFLOAT3 ConvertWindowYPos(const XMFLOAT3& pos, const XMFLOAT3& vec);

	/// <summary>
	/// カメラのX軸とY軸を元に座標を移動
	/// </summary>
	/// <param name="pos">初期位置</param>
	/// <param name="vec">移動量</param>
	/// <returns>移動した後の座標</returns>
	XMFLOAT3 ConvertWindowXYPos(const XMFLOAT3& pos, const XMFLOAT3& vec);

	/// <summary>
	/// 3D座標を2D座標に変換
	/// </summary>
	/// <param name="pos">3D座標</param>
	/// <returns>2D座標</returns>
	XMFLOAT2 Convert3DPosTo2DPos(const XMFLOAT3& pos);

	/// <summary>
	/// 視界内にいるか
	/// </summary>
	/// <param name="pos">判定対象の座標</param>
	/// <returns>成否</returns>
	bool ObjectComeInSight(const XMFLOAT3& pos);

public: //アクセッサ
	/// <summary>
	/// カメラ座標取得
	/// </summary>
	/// <returns>カメラ座標</returns>
	inline XMFLOAT3& GetEye() { return m_eye; }

	/// <summary>
	/// カメラ座標セット
	/// </summary>
	/// <param name="eye">カメラ座標</param>
	inline void SetEye(const XMFLOAT3& eye)
	{
		m_eye = eye;

		m_dirty = true;
	}

	/// <summary>
	/// 焦点座標取得
	/// </summary>
	/// <returns>頂点座標</returns>
	inline XMFLOAT3& GetTarget() { return m_target; }

	/// <summary>
	/// 焦点座標セット
	/// </summary>
	/// <param name="target">焦点座標</param>
	inline void SetTarget(const XMFLOAT3& target)
	{
		m_target = target;

		m_dirty = true;
	}

	/// <summary>
	/// 上下取得
	/// </summary>
	/// <returns>上の向き</returns>
	inline XMFLOAT3& GetUp() { return m_up; }

	/// <summary>
	/// 上下のセット
	/// </summary>
	/// <param name="up">上の向き</param>
	inline void SetUp(const XMFLOAT3& up)
	{
		m_up = up;

		m_dirty = true;
	}

	/// <summary>
	/// 注視点から始点までの距離取得
	/// </summary>
	/// <returns>注視点から始点までの距離</returns>
	inline XMFLOAT3& GetDistance() { return m_distance; }

	/// <summary>
	/// 注視点から始点までの距離セット
	/// </summary>
	/// <param name="distance">距離</param>
	void SetDistance(const XMFLOAT3& distance = { 0, 1, -5 });

	/// <summary>
	/// カメラの回転角のセット
	/// </summary>
	/// <param name="cameraAngle">回転角</param>
	inline void SetCameraAngles(const XMFLOAT3& cameraAngle = { 0, 0, 0 })
	{
		m_angle.x = cameraAngle.x;
		m_angle.y = cameraAngle.y;
		m_angle.z = cameraAngle.z;
	}

	/// <summary>
	/// カメラの最近遠Z値を取得
	/// </summary>
	/// <returns>最近遠Z値</returns>
	inline XMFLOAT2& GetNearFarZ() { return XMFLOAT2(m_nearZ, m_farZ); }

	/// <summary>
	/// カメラの最近遠Z値をセット
	/// </summary>
	/// <param name="nearZ">最近値</param>
	/// <param name="farZ">最遠値</param>
	void SetNearFarZ(const float nearZ, const float farZ);

	/// <summary>
	/// カメラの回転角度を取得
	/// </summary>
	/// <returns>回転角度</returns>
	inline XMFLOAT3& GetCameraAngles() { return m_angle; }

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	inline XMMATRIX& GetMatView() { return m_matView; }

	/// <summary>
	/// 射影行列
	/// </summary>
	/// <returns>射影行列</returns>
	inline XMMATRIX& GetMatProject() { return m_matProjection; }

	/// <summary>
	/// ビルボード行列
	/// </summary>
	/// <returns>ビルボード行列</returns>
	inline XMMATRIX& GetMatBillboard() { return m_matBillboard; }

	/// <summary>
	/// カメラ更新の成否
	/// </summary>
	/// <returns>成否</returns>
	inline bool GetDirty() { return m_isDirty; };
};