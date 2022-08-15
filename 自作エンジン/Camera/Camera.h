#pragma once
#include <DirectXMath.h>

class Camera
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //静的メンバ関数
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static Camera* GetInstance();

private: //メンバ変数
	//注視点座標
	XMFLOAT3 target = { 0, 0, 0 };
	//始点座標
	XMFLOAT3 eye = { 0, 0, -50 };
	//上方向ベクトル
	XMFLOAT3 up = { 0, 1, 0 };
	//注視点から始点までの距離
	XMFLOAT3 distance = { 0, 0, 0 };
	//ビュー行列
	XMMATRIX matView;
	//射影行列
	XMMATRIX matProjection;
	//ビルボード行列
	XMMATRIX matBillboard;
	//Y軸ビルボード行列
	XMMATRIX matBillboardY;
	//X軸の角度
	float angleX = 0.0f;
	//Y軸の角度
	float angleY = 0.0f;
	//ダーティーフラグ
	bool dirty = false;
	//	更新したか
	bool isDirty = false;

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="window_width">ウィンドウ横幅</param>
	/// <param name="window_height">ウィンドウ縦幅</param>
	void Initialize(int window_width, int window_height);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// カメラの移動
	/// </summary>
	/// <param name="move">移動量</param>
	void MoveCamera(XMFLOAT3 move);

	/// <summary>
	/// 追従カメラ
	/// </summary>
	/// <param name="target">注視点</param>
	/// <param name="eye">注視点から始点への成分</param>
	/// <param name="addAngleX">X軸の角度</param>
	/// <param name="addAngleY">Y軸の角度</param>
void FollowUpCamera(XMFLOAT3 target, XMFLOAT3 eye, float addAngleX, float addAngleY);

	/// <summary>
	/// カメラを軸に座標を移動
	/// </summary>
	/// <param name="pos">初期位置</param>
	/// <param name="vec">移動量</param>
	/// <returns>移動した後の座標</returns>
	XMFLOAT3 ConvertWindowPos(XMFLOAT3 pos, XMFLOAT3 vec);

public: //アクセッサ
	/// <summary>
	/// カメラ座標取得
	/// </summary>
	/// <returns>カメラ座標</returns>
	XMFLOAT3 GetEye() { return eye; }

	/// <summary>
	/// カメラ座標セット
	/// </summary>
	/// <param name="eye">カメラ座標</param>
	void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// 焦点座標取得
	/// </summary>
	/// <returns>頂点座標</returns>
	XMFLOAT3 GetTarget() { return target; }

	/// <summary>
	/// 焦点座標セット
	/// </summary>
	/// <param name="target">焦点座標</param>
	void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// 上下取得
	/// </summary>
	/// <returns>上の向き</returns>
	XMFLOAT3 GetUp() { return up; }

	/// <summary>
	/// 上下のセット
	/// </summary>
	/// <param name="up">上の向き</param>
	void SetUp(XMFLOAT3 up);

	/// <summary>
	/// 注視点から始点までの距離取得
	/// </summary>
	/// <returns>注視点から始点までの距離</returns>
	XMFLOAT3 GetDistance() { return distance; }

	/// <summary>
	/// 注視点から始点までの距離セット
	/// </summary>
	void SetDistance();

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	XMMATRIX GetMatView() { return matView; }

	/// <summary>
	/// 射影行列
	/// </summary>
	/// <returns>射影行列</returns>
	XMMATRIX GetMatProject() { return matProjection; }

	/// <summary>
	/// ビルボード行列
	/// </summary>
	/// <returns>ビルボード行列</returns>
	XMMATRIX GetMatBillboard() { return matBillboard; }

	/// <summary>
	/// カメラ更新の成否
	/// </summary>
	/// <returns>成否</returns>
	bool GetDirty() { return isDirty; };
};