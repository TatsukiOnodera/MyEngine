#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "CollisionInfo.h"
#include "PipelineManager.h"

class BaseCollider;

class Object3d
{
public: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //静的変数;
	// デバイス
	static ID3D12Device* s_dev;
	// コマンドリスト
	static ID3D12GraphicsCommandList* s_cmdList;
	//カメラクラス
	static Camera *s_camera;
	//ライトクラス
	static Light* s_light;

public: // シェーダータイプ
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
	static Object3d* Create(const std::string& modelName = "defaultTexture", const bool smooting = false);

	/// <summary>
	/// カメラセット
	/// </summary>
	static Camera* GetCamera() { return s_camera; };

	/// <summary>
	/// ライトセット
	/// </summary>
	static void SetLight(Light* light) { Object3d::s_light = light; }

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

private: //サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX viewproj; //ビュープロジェクション行列
		XMMATRIX world; //ワールド行列
		XMFLOAT3 cameraPos; //カメラ座標(ワールド座標)
		float pad; //パディング
		XMFLOAT4 color; //色(RGBA)
	};

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
	//ワールド行列
	XMMATRIX m_matWorld;
	//モデルデータ
	Model* m_model = nullptr;
	// ビルボード
	bool m_isBillboard = false;
	//クラス名（デバッグ用）
	const char* m_name = nullptr;
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
	XMFLOAT3 GetPosition() { return m_position; }

	/// <summary>
	/// 座標をセット
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const XMFLOAT3& position)
	{
		m_position = position;
		m_dirty = true;
	}

	/// <summary>
	/// 回転角を取得
	/// </summary>
	/// <returns>回転</returns>
	XMFLOAT3 GetRotation() { return m_rotation; }

	/// <summary>
	/// 回転角をセット
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(const XMFLOAT3& rotation)
	{
		m_rotation = rotation;
		m_dirty = true;
	}

	/// <summary>
	/// スケールを取得
	/// </summary>
	/// <returns>スケール</returns>
	XMFLOAT3 GetScale() { return m_scale; }

	/// <summary>
	/// スケールをセット
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(const XMFLOAT3& scale)
	{
		m_scale = scale;
		m_dirty = true;
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
	void SetColor(const XMFLOAT4& color)
	{
		m_color = color;
		m_dirty = true;
	}

	/// <summary>
	/// モデルデータセット
	/// </summary>
	void SetModel(Model* model) { m_model = model; }

	/// <summary>
	/// ビルボードセット
	/// </summary>
	void SetBillboard(const bool isBillboard) { m_isBillboard = isBillboard; }

	/// <summary>
	/// ワールド行列を取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const XMMATRIX& GetMatWorld() { return m_matWorld; }

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="collider">コライダー</param>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// シェーダーセット
	/// </summary>
	/// <param name="shaderType">シェーダーの種類</param>
	void SetShader(const int shaderType)
	{
		m_model->SetGraphicsPipeline(shaderType);
	}

	/// <summary>
	/// サブテクスチャのセット
	/// </summary>
	/// <param name="filename">ファイル名</param>
	void SetSubTexture(const std::string& filename)
	{
		m_model->SetSubTexture(filename);
	}

	/// <summary>
	/// マスクテクスチャのセット
	/// </summary>
	/// <param name="filename">ファイル名</param>
	void SetMaskTexture(const std::string& filename)
	{
		m_model->SetMaskTexture(filename);
	}

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info)
	{

	};
};