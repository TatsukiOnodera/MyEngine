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
	static Camera* GetInstance();

private: //メンバ変数
	//始点座標
	XMFLOAT3 eye;
	//注意点座標
	XMFLOAT3 target; 
	//上方向ベクトル
	XMFLOAT3 up;
	//ビュー行列
	XMMATRIX matView;
	//射影行列
	XMMATRIX matProjection;
	//ビルボード行列
	XMMATRIX matBillboard;
	//Y軸ビルボード行列
	XMMATRIX matBillboardY;
	//ダーティーフラグ
	bool dirty;

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera(XMFLOAT3 eye = { 0, 0, -25 }, XMFLOAT3 target = { 0, 0, 0 }, XMFLOAT3 up = { 0, 1, 0 });

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Camera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(int window_width, int window_height);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// カメラの移動
	/// </summary>
	void MoveCamera(XMFLOAT3 move);

public: //アクセッサ
	/// <summary>
	/// 視点情報
	/// </summary>
	XMFLOAT3 GetEye() { return eye; }
	void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// 焦点情報
	/// </summary>
	XMFLOAT3 GetTarget() { return target; }
	void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// 上下情報
	/// </summary>
	XMFLOAT3 GetUp() { return up; }
	void SetUp(XMFLOAT3 up);

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	XMMATRIX GetMatView() { return matView; }

	/// <summary>
	/// 射影行列
	/// </summary>
	XMMATRIX GetMatProject() { return matProjection; }

	/// <summary>
	/// ビルボード行列
	/// </summary>
	XMMATRIX GetMatBillboard() { return matBillboard; }
};