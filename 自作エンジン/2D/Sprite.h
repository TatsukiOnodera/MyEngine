#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Sprite
{
public: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

protected: //定数
	//テクスチャの最大枚数
	static const int spriteSRVCount = 512;
	//テクスチャの最大枚数
	static const int vertNum = 4;

protected: //静的変数
	//DirectXCommon->dev
	static ID3D12Device* dev;
	//DirectXCommon->cmdList
	static ID3D12GraphicsCommandList* cmdList; 
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	//射影行列
	static XMMATRIX matProjection;
	//テクスチャ用デスクリプタヒープの生成
	static ComPtr<ID3D12DescriptorHeap> descHeap; 
	//テクスチャリソース（テクスチャバッファ）の配列
	static ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature; 

public: //静的関数
	//静的初期化
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//パイプライン生成
	static void CreateGraphicsPipeline();

	//スプライトの共通データ生成
	static void CreateSpriteCommon(int window_width, int window_height);

	//テクスチャのロード
	static void LoadTexture(UINT texNumber, const wchar_t* filename);

	//描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	//描画後処理
	static void PostDraw();

	//スプライト作成
	static Sprite* Create(UINT texNumber, XMFLOAT2 texLeftTop = { 0, 0 }, XMFLOAT2 anchorpoint = { 0.0f, 0.0f }, bool isFilpX = false, bool isFilpY = false, bool isInvisible = false);

protected: //サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color; //色

		XMMATRIX mat; //行列
	};

	//スプライトの頂点データ型
	struct VertexPosUv
	{
		XMFLOAT3 pos; //xyz座標

		XMFLOAT2 uv; //uv座標
	};

protected: //メンバ変数
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//テクスチャ番号
	UINT texNumber = 0;
	//座標
<<<<<<< HEAD
	XMFLOAT2 position = { 0, 0 };
=======
	XMFLOAT3 position = { 0, 0, 0 };
>>>>>>> parent of 00f20c5... no message
	//色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//大きさ
	XMFLOAT2 size = { 10, 10 };
	//テクスチャの左上座標
	XMFLOAT2 texLeftTop = { 0, 0 };
	//テクスチャの切り出しサイズ
	XMFLOAT2 texSize = { 100, 100 };
	//アンカーポイント
	XMFLOAT2 anchorpoint = { 0.0f, 0.0f };
	//ワールド行列
	XMMATRIX matWorld;
	//Z軸回りの回転角
	float rotation = 0.0f;
	//左右反転
	bool isFilpX = false;
	//上下反転
	bool isFilpY = false;
	//非表示
	bool isInvisible = false;

public: //アクセッサ
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	XMFLOAT2 GetPostion() { return position; }

	/// <summary>
	/// 座標をセット	
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT2 position);

	/// <summary>
	/// 色を取得
	/// </summary>
	/// <returns>色</returns>
	XMFLOAT4 GetColor() { return color; }

	/// <summary>
	/// 色をセット
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(XMFLOAT4 color);

	/// <summary>
	/// サイズをセット
	/// </summary>
	/// <param name="size">画像サイズ</param>
	void SetSize(XMFLOAT2 size);

	/// <summary>
	/// 画像の左上の座標をセット
	/// </summary>
	/// <param name="LeftTop">左上の座標</param>
	void SetLeftTop(XMFLOAT2 LeftTop);

	/// <summary>
	/// テクスチャのサイズをセット
	/// </summary>
	/// <param name="texSize">テクスチャのサイズ</param>
	void SetTexSize(XMFLOAT2 texSize);

	/// <summary>
	/// 回転角をセット
	/// </summary>
	/// <param name="rotation">回転角</param>
	void SetRotation(float rotation);

	/// <summary>
	/// 反転情報をセット
	/// </summary>
	/// <param name="isFilpX">X軸を反転</param>
	/// <param name="isFilpY">Y軸を反転</param>
	void SetFilp(bool isFilpX, bool isFilpY);

	/// <summary>
	/// 不可視情報をセット
	/// </summary>
	/// <param name="isInvisible">不可視情報</param>
	void SetInvisible(bool isInvisible);

public: //メンバ関数
	//コンストラクタ
	Sprite(XMFLOAT2 size, UINT texNumber, XMFLOAT2 texLeftTop, XMFLOAT2 anchorpoint, bool isFilpX, bool isFilpY, bool isInvisible);

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

	//頂点データ更新
	void SpriteTransferVertexBuffer();
};