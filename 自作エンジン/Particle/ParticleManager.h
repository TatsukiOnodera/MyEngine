#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include <forward_list>

/// <summary>
/// 3Dオブジェクト
/// </summary>
class ParticleManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体(2_04)
	/*struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};*/

	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		float scale;//スケール(3_03)
		XMFLOAT4 color;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		//XMFLOAT4 color;	// 色 (RGBA)(2_04)
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX matBillboard;//ビルボード行列(3_01)
	};

	//パーティクル1粒(3_02)
	struct Particle
	{
		//DirectX::を省略
		using XMFLOAT3 = DirectX::XMFLOAT3;

		//座標
		XMFLOAT3 position = {};
		//速度
		XMFLOAT3 velocity = {};
		//加速度
		XMFLOAT3 accel = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 0;
		//スケール(3_03)
		float scale = 1.0f;
		//初期値(3_03)
		float s_scale = 1.0f;
		//最終値(3_03)
		float e_scale = 0.0f;

		XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
		XMFLOAT4 sColor = { 1.0f,1.0f,1.0f,1.0f };
		XMFLOAT4 eColor = { 1.0f,0.0f,0.0f,1.0f };
	};

	//パーティクル配列
	std::forward_list<Particle> particles;

private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	/*static const int planeCount = division * 2 + division * 2;// 面の数(1-03)
	static const int vertexCount = planeCount * 3;// 頂点数*/
	//static const int vertexCount = 4;// 頂点数(2_03)
	//static const int vertexCount = 1;(3_01)
	//static const int vertexCount = 30;(3_02)
	static const int vertexCount = 1024;
	//static const int indexCount = 3 * 2;		//インデックス数(2_03)

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static ParticleManager* Create();

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="move">移動量</param>
	static void CameraMoveVector(XMFLOAT3 move);

	//ベクトルによる視点移動
	static void CameraMoveEyeVector(XMFLOAT3 move);

	/// <summary>
	/// パーティクルの追加(3_02)(3_03)
	/// </summary>
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	//static ComPtr<ID3D12Resource> indexBuff;(2_03)
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	//static D3D12_INDEX_BUFFER_VIEW ibView;(2_03)
	// 頂点データ配列
	//static VertexPosNormalUv vertices[vertexCount];(2_04)
	static VertexPos vertices[vertexCount];
	// 頂点インデックス配列
	//static unsigned short indices[planeCount * 3];
	//static unsigned short indices[indexCount];(2_03)
	//ビルボード行列
	static XMMATRIX matBillboard;
	//Y軸回りビルボード行列
	static XMMATRIX matBillboardY;

private:// 静的メンバ関数
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
	static bool InitializeDescriptorHeap();

	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	static bool LoadTexture();

	/// <summary>
	/// モデル作成
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

public: // メンバ関数
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 稼働
	/// </summary>
	void Active(XMFLOAT3 p_pos);

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	//const XMFLOAT3& GetPosition() { return position; }(3_01)

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	//void SetPosition(XMFLOAT3 position) { this->position = position; }(3_01)

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	// 色
	//XMFLOAT4 color = { 1,1,1,1 };(3_01)
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	//XMFLOAT3 rotation = { 0,0,0 };(3_01)
	// ローカル座標
	//XMFLOAT3 position = { 0,0,0 };(3_01)
	// ローカルワールド変換行列
	//XMMATRIX matWorld;(3_01)
	// 親オブジェクト
	//ParticleManager* parent = nullptr;(3_01)
};

