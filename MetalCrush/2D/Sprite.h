#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Sprite
{
public: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

protected: // 定数
	// テクスチャの最大枚数
	static const int c_spriteSRVCount = 512;
	// テクスチャの最大枚数
	static const int c_vertNum = 4;

protected: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color; // 色

		XMMATRIX mat; // 行列
	};

	// スプライトの頂点データ型
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz座標

		XMFLOAT2 uv; // uv座標
	};

protected: // 静的変数
	// デバイス
	static ID3D12Device* s_dev;
	// コマンドリスト
	static ID3D12GraphicsCommandList* s_cmdList; 
	// デスクリプタサイズ
	static UINT s_descriptorHandleIncrementSize;
	// 射影行列
	static XMMATRIX s_matProjection;
	// テクスチャ用デスクリプタヒープの生成
	static ComPtr<ID3D12DescriptorHeap> s_descHeap; 
	// テクスチャリソース（テクスチャバッファ）の配列
	static ComPtr<ID3D12Resource> s_texBuff[c_spriteSRVCount];
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> s_pipelinestate;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> s_rootsignature; 

public: // 静的関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns>完了か否か</returns>
	static bool StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// パイプライン生成
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// スプライトの共通データ生成
	/// </summary>
	static void CreateSpriteCommon();

	/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	/// <param name="texNumber">テクスチャの割り振り番号</param>
	/// <param name="filename">ファイルのパス</param>
	static void LoadTexture(const UINT texNumber, const wchar_t* filename);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 作成
	/// </summary>
	/// <param name="texNumber">テクスチャの番号</param>
	/// <param name="texLeftTop">左上の座標</param>
	/// <param name="anchorpoint">アンカーポイントの座標</param>
	/// <param name="isFilpX">横を軸に反転</param>
	/// <param name="isFilpY">縦を軸に反転</param>
	/// <param name="isInvisible">表示するか</param>
	/// <returns>スプライト</returns>
	static Sprite* Create(const UINT texNumber, const XMFLOAT2& texLeftTop = { 0, 0 }, const XMFLOAT2& anchorpoint = { 0.0f, 0.0f }, const bool isFilpX = false, const bool isFilpY = false, const bool isInvisible = false);

protected: // メンバ変数
	// 頂点バッファ
	ComPtr<ID3D12Resource> m_vertBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW m_vbView;
	// 定数バッファ
	ComPtr<ID3D12Resource> m_constBuff;
	// テクスチャ番号
	UINT m_texNumber = 0;
	// 座標
	XMFLOAT2 m_position = { 0, 0 };
	// 色
	XMFLOAT4 m_color = { 1, 1, 1, 1 };
	// 大きさ
	XMFLOAT2 m_size = { 10, 10 };
	// テクスチャの左上座標
	XMFLOAT2 m_texLeftTop = { 0, 0 };
	// テクスチャの切り出しサイズ
	XMFLOAT2 m_texSize = { 100, 100 };
	// アンカーポイント
	XMFLOAT2 m_anchorpoint = { 0.0f, 0.0f };
	// ワールド行列
	XMMATRIX m_matWorld;
	// Z軸回りの回転角
	float m_rotation = 0.0f;
	// 左右反転
	bool m_isFilpX = false;
	// 上下反転
	bool m_isFilpY = false;
	// 非表示
	bool m_isInvisible = false;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="size">サイズ</param>
	/// <param name="texNumber">テクスチャの番号</param>
	/// <param name="texLeftTop">左上の座標</param>
	/// <param name="anchorpoint">アンカーポイントの座標</param>
	/// <param name="isFilpX">横を軸に反転</param>
	/// <param name="isFilpY">縦を軸に反転</param>
	/// <param name="isInvisible">表示するか</param>
	Sprite(const XMFLOAT2& size, const UINT texNumber, const XMFLOAT2& texLeftTop, const XMFLOAT2& anchorpoint, const bool isFilpX, const bool isFilpY, const bool isInvisible);
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
	/// <summary>
	/// 頂点データ更新
	/// </summary>
	void SpriteTransferVertexBuffer();

public: // アクセッサ
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	XMFLOAT2 GetPostion() { return m_position; }

	/// <summary>
	/// 座標をセット	
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT2 position)
	{
		m_position = position;

		Update();
	}

	/// <summary>
	/// 色を取得
	/// </summary>
	/// <returns>色</returns>
	XMFLOAT4 GetColor() { return m_color; }

	/// <summary>
	/// 色をセット
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(XMFLOAT4 color)
	{
		m_color = color;

		Update();
	}

	/// <summary>
	/// サイズを取得
	/// </summary>
	/// <returns>サイズ</returns>
	XMFLOAT2 GetSize() { return m_size; }

	/// <summary>
	/// サイズをセット
	/// </summary>
	/// <param name="size">画像サイズ</param>
	void SetSize(XMFLOAT2 size)
	{
		m_size = size;

		SpriteTransferVertexBuffer();
	}

	/// <summary>
	/// テクスチャのサイズをセット
	/// </summary>
	/// <param name="texSize">テクスチャのサイズ</param>
	void SetTexSize(XMFLOAT2 texSize)
	{
		m_texSize = texSize;

		SpriteTransferVertexBuffer();
	}

	/// <summary>
	/// 画像の左上の座標をセット
	/// </summary>
	/// <param name="LeftTop">左上の座標</param>
	void SetLeftTop(XMFLOAT2 LeftTop)
	{
		m_texLeftTop = LeftTop;

		SpriteTransferVertexBuffer();
	}

	/// <summary>
	/// 回転角をセット
	/// </summary>
	/// <param name="rotation">回転角</param>
	void SetRotation(float rotation)
	{
		m_rotation = rotation;

		Update();
	}

	/// <summary>
	/// 反転情報をセット
	/// </summary>
	/// <param name="isFilpX">X軸を反転</param>
	/// <param name="isFilpY">Y軸を反転</param>
	void SetFilp(bool isFilpX, bool isFilpY)
	{
		m_isFilpX = isFilpX;
		m_isFilpY = isFilpY;

		SpriteTransferVertexBuffer();
	}

	/// <summary>
	/// 不可視情報をセット
	/// </summary>
	/// <param name="isInvisible">不可視情報</param>
	void SetInvisible(bool isInvisible)
	{
		m_isInvisible = isInvisible;
	}
};