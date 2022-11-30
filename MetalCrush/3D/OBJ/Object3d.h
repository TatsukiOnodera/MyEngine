#pragma once
#include "Model.h"
#include "Camera.h"
#include "LightGroup.h"
#include "CollisionInfo.h"
#include "PipelineManager.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class BaseCollider;

class Object3d
{
public: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //静的メンバ変数
	// デバイス
	static ID3D12Device* s_dev;
	// コマンドリスト
	static ID3D12GraphicsCommandList* s_cmdList;
	//カメラクラス
	static Camera *s_camera;
	//ライトグループクラス
	static LightGroup* s_lightGroup;

public: //サブクラス
//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX viewproj; //ビュープロジェクション行列
		XMMATRIX world; //ワールド行列
		XMFLOAT3 cameraPos; //カメラ座標(ワールド座標)
		float pad; //パディング
		XMFLOAT4 color; //色(RGBA)
		XMFLOAT2 tiling; // タイリング
		XMFLOAT2 offset; // オフセット
	};

	enum ShaderType
	{
		Phong, Toon, Mono, Blend, Specular, Bloom
	};

public: //静的関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static bool StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	static Object3d* Create(Model* model);

	/// <summary>
	/// カメラセット
	/// </summary>
	inline static Camera* GetCamera() { return s_camera; };

	/// <summary>
	/// ライトセット
	/// </summary>
	static void SetGroupLight(LightGroup* lightGroup) { Object3d::s_lightGroup = lightGroup; }

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

private: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> m_constBuff;
	//平行移動
	XMFLOAT3 m_position = { 0.0f, 0.0f, 0.0f };
	//スケール
	XMFLOAT3 m_scale = { 1.0f, 1.0f, 1.0f };
	//回転
	XMFLOAT3 m_rotation = { 0.0f, 0.0f, 0.0f };
	//色(RGBA)
	XMFLOAT4 m_color = { 1, 1, 1, 1 };
	// タイリング
	XMFLOAT2 m_tiling = { 1, 1 };
	// オフセット
	XMFLOAT2 m_offset = { 0, 0 };
	//ワールド行列
	XMMATRIX m_matWorld = {};
	//モデルデータ
	Model* m_model = nullptr;
	// ビルボード
	bool m_isBillboard = false;
	// ペアレント
	std::unique_ptr<Object3d> m_parent = nullptr;
	//コライダー
	std::unique_ptr<BaseCollider> m_collider = nullptr;
	//ダーティーフラグ
	bool m_dirty = true;

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3d() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Object3d();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

public: //アクセッサ
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	inline XMFLOAT3& GetPosition() { return m_position; }

	/// <summary>
	/// 座標をセット
	/// </summary>
	/// <param name="position">座標</param>
	inline void SetPosition(const XMFLOAT3& position)
	{
		m_position = position;
		m_dirty = true;
	}

	/// <summary>
	/// 回転角を取得
	/// </summary>
	/// <returns>回転</returns>
	inline XMFLOAT3& GetRotation() { return m_rotation; }

	/// <summary>
	/// 回転角をセット
	/// </summary>
	/// <param name="rotation"></param>
	inline void SetRotation(const XMFLOAT3& rotation)
	{
		m_rotation = rotation;
		m_dirty = true;
	}

	/// <summary>
	/// スケールを取得
	/// </summary>
	/// <returns>スケール</returns>
	inline XMFLOAT3& GetScale() { return m_scale; }

	/// <summary>
	/// スケールをセット
	/// </summary>
	/// <param name="scale">スケール</param>
	inline void SetScale(const XMFLOAT3& scale)
	{
		m_scale = scale;
		m_dirty = true;
	}

	/// <summary>
	/// 色を取得
	/// </summary>
	/// <returns>色</returns>
	inline XMFLOAT4& GetColor() { return m_color; }

	/// <summary>
	/// 色をセット
	/// </summary>
	/// <param name="color">色</param>
	inline void SetColor(const XMFLOAT4& color)
	{
		m_color = color;
		m_dirty = true;
	}

	/// <summary>
	/// タイリングの取得
	/// </summary>
	/// <returns>タイリング</returns>
	inline XMFLOAT2& GetTiling() { return m_tiling; }

	/// <summary>
	/// タイリングのセット
	/// </summary>
	/// <param name="tiling">タイリング</param>
	inline void SetTiling(XMFLOAT2 tiling)
	{
		m_tiling = tiling;
	}
		
	/// <summary>
	/// オフセットの取得
	/// </summary>
	/// <returns>オフセット</returns>
	inline XMFLOAT2& GetOffset() { return m_offset; }

	/// <summary>
	/// オフセットのセット
	/// </summary>
	/// <param name="offset">オフセット</param>
	inline void SetOffset(XMFLOAT2 offset)
	{
		m_offset = offset;
	}

	/// <summary>
	/// モデルデータセット
	/// </summary>
	inline void SetModel(Model* model)
	{
		m_model = model;
	}

	/// <summary>
	/// ビルボードセット
	/// </summary>
	inline void SetBillboard(const bool isBillboard)
	{
		m_isBillboard = isBillboard;
	}

	/// <summary>
	/// ワールド行列を取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	inline const XMMATRIX& GetMatWorld() { return m_matWorld; }

	/// <summary>
	/// ペアレントのワールド行列を取得
	/// </summary>
	/// <param name="parent">ペアレントのワールド行列</param>
	inline void SetParent(Object3d& parent)
	{ 
		m_parent.reset(&parent);
	}

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="collider">コライダー</param>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info);
};