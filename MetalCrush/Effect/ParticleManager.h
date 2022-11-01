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
	static const int s_division = 50;					// 分割数
	static const float s_radius;				// 底面の半径
	static const float s_prizmHeight;			// 柱の高さ
	static const int s_planeCount = s_division * 2 + s_division * 2;		// 面の数
	static const int s_vertexCount = 1024; //長点数

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
		XMFLOAT3 w_position = {};
		//速度
		XMFLOAT3 velocity = {};
		//加速度
		XMFLOAT3 accel = {};
		// RGBA
		XMFLOAT4 color = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 0;
		//スケール
		float scale = 1.0f;
		// 初期値
		XMFLOAT4 s_color = {};
		// 最終値
		XMFLOAT4 e_color = {};
		//初期値
		float s_scale = 1.0f;
		//最終値
		float e_scale = 0.0f;
		// カメラに追従するか
		bool isFollow = false;
	};

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
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// ビュー行列
	static XMMATRIX matView;
	// 視点座標
	static Camera* s_camera;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// 頂点データ配列
	static VertexPos vertices[s_vertexCount];
	// ビルボード行列
	static XMMATRIX matBillboard;
	// Y軸回りビルボード行列
	static XMMATRIX matBillboardY;

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
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// モデル作成
	/// </summary>
	static bool CreateModel();

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
	// 追従対象座標
	XMFLOAT3 m_targetPos = {};

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>初期化できたか</returns>
	bool Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="textureName">テクスチャの名前</param>
	/// <returns>成否</returns>
	bool LoadTexture(const std::string& textureName);

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	void Add(const int life, const XMFLOAT3& w_position, const XMFLOAT3& velocity, const XMFLOAT3& accel, const XMFLOAT4& start_color, const XMFLOAT4& end_color, const float start_scale, const float end_scale, const bool isFollow);

	/// <summary>
	/// 追従座標をセット
	/// </summary>
	/// <param name="position">座標</param>
	void SetTargetPos(const XMFLOAT3& position)
	{
		m_targetPos = position;
	}
};