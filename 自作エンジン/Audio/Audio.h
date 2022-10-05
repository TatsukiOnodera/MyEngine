#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <cstdint>
#include <map>
#include <string>

/// <summary>
/// コールバック
/// </summary>
class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	// ボイス処理パスの開始時
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};

	// ボイス処理パスの終了時
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};

	// バッファストリームの再生が終了した時
	STDMETHOD_(void, OnStreamEnd) (THIS) {};

	// バッファの使用開始時
	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};

	// バッファの末尾に達した時
	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext)
	{
		// バッファを解放する
		delete[] pBufferContext;
	};

	// 再生がループ位置に達した時
	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};

	// ボイスの実行エラー時
	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
};

class Audio
{
private: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	// チャンクヘッダ
	struct Chunk
	{
		char	id[4]; // チャンク毎のID
		int		size;  // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		Chunk	chunk;   // "RIFF"
		char	type[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		Chunk		chunk; // "fmt "
		WAVEFORMAT	fmt;   // 波形フォーマット
	};

	// サウンドデータ
	struct SoundData
	{
		// 波形フォーマット
		WAVEFORMATEX wfex;
		// バッファの先頭アドレス
		BYTE* buffer;
		// バッファのサイズ
		unsigned int bufferSize;
		// 波形フォーマットを元にSourceVoiceの生成する奴
		IXAudio2SourceVoice* sourceVoice;
		// ループ
		bool loop = false;
		// 再生状態
		bool playNow = false;
	};

public: // 静的メンバ関数
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static Audio* GetInstance();

private: // メンバ変数
	ComPtr<IXAudio2> xAudio2;
	XAudio2VoiceCallback voiceCallback;
	std::map<std::string, SoundData> soundList;
	XAUDIO2_BUFFER buf{};

public: //メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Audio();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Audio();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// サウンドファイルの再生
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="loop">ループの成否</param>
	/// <param name="volume">音量</param>
	void PlayWave(const std::string& fileName, bool loop, float volume);

	/// <summary>
	/// サウンドファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void LoadSound(const std::string& fileName);

	/// <summary>
	/// サウンドファイルの停止
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void StopSound(const std::string& fileName);

	/// <summary>
	/// サウンドデータの作成
	/// </summary>
	/// <param name="soundData"></param>
	void CreateSoundData(SoundData& soundData);

	/// <summary>
	/// サウンドリストのアンロード
	/// </summary>
	/// <param name="soundList">サウンドリスト</param>
	void Unload(SoundData* soundList);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();
};

