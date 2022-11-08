#pragma once
#include "Camera.h"

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

private: // 定数
	static const int c_division = 50;					// 分割数
	static const float c_radius;				// 底面の半径
	static const float c_prizmHeight;			// 柱の高さ
	static const int c_planeCount = c_division * 2 + c_division * 2;		// 面の数
	static const int c_vertexCount = 1024; //長点数

public: // サブクラス
	// 頂点データ構造体
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		float scale; //スケール
		XMFLOAT4 color; // RGBA
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX matBillboard; //ビルボード行列
	};

	//パーティクル1粒
	struct Particle
	{
		//座標
		XMFLOAT3 position = {};
		// ワールド座標
		XMFLOAT3 worldPosition = {};
		//速度
		XMFLOAT3 velocity = {};
		//加速度
		XMFLOAT3 accel = {};
		// RGBA
		XMFLOAT4 color = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int endFrame = 0;
		//スケール
		float scale = 1.0f;
		// 初期値
		XMFLOAT4 startColor = {};
		// 最終値
		XMFLOAT4 endColor = {};
		//初期値
		float startScale = 1.0f;
		//最終値
		float endScale = 0.0f;
		// カメラに追従するか
		bool isFollow = false;
		// 追従先の座標
		XMFLOAT3* targetPosition = nullptr;
	};

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* s_device;
	// デスクリプタサイズ
	static UINT s_descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* s_cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> s_rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> s_pipelinestate;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> s_vertBuff;
	// ビュー行列
	static XMMATRIX s_matView;
	// 視点座標
	static Camera* s_camera;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW s_vbView;
	// 頂点データ配列
	static VertexPos s_vertices[c_vertexCount];
	// ビルボード行列
	static XMMATRIX s_matBillboard;
	// Y軸回りビルボード行列
	static XMMATRIX s_matBillboardY;

private: // 静的メンバ関数
	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

public:// 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// モデル作成
	/// </summary>
	static bool InitializeVertBuffer();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static ParticleManager* Create(const std::string& textureName);

private: // メンバ変数
	//パーティクル配列
	std::forward_list<Particle> m_partices;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> m_descHeap;
	// 定数バッファ
	ComPtr<ID3D12Resource> m_constBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> m_texbuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpuDescHandleSRV;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParticleManager();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParticleManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="textureName">テクスチャの名前</param>
	/// <returns>成否</returns>
	bool LoadTexture(const std::string& textureName);

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name="life">寿命</param>
	/// <param name="w_position">座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="start_color">初期色</param>
	/// <param name="end_color">終了色</param>
	/// <param name="start_scale">初期スケール</param>
	/// <param name="end_scale">終了スケール</param>
	/// <param name="isFollow">追従するか否か</param>
	void Add(const int life, const XMFLOAT3& position, const XMFLOAT3& velocity, const XMFLOAT3& accel, const XMFLOAT4& startColor, const XMFLOAT4& endColor, const float startScale, const float endScale, const bool isFollow, XMFLOAT3* targetPosition = nullptr);
};