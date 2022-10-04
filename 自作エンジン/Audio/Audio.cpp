#include "Audio.h"
#include <fstream>
#include <cassert>
#include <SafeDelete.h>
#pragma comment(lib,"xaudio2.lib")

Audio* Audio::GetInstance()
{
	static Audio audio;

	return &audio;
}

Audio::Audio()
{
	
}

Audio::~Audio()
{
	Finalize();
}

bool Audio::Initialize()
{
	HRESULT result = S_FALSE;

	IXAudio2MasteringVoice* masterVoice;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
		return false;
	}

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
		return false;
	}

	return true;
}

void Audio::PlayWave(const std::string& fileName, bool loop, float volume)
{
	HRESULT result = S_FALSE;

	std::map<std::string, SoundData>::iterator it = soundList.find(fileName);
	//未読み込みの検出
	assert(it != soundList.end());

	//サウンドデータの参照を取得
	SoundData& soundData = it->second;

	// 波形フォーマットを元にSourceVoiceの生成
	CreateSoundData(soundData);

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.buffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = soundData.bufferSize;
	if (loop == true)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// 波形データの再生
	soundData.sourceVoice->SetVolume(volume);
	result = soundData.sourceVoice->SubmitSourceBuffer(&buf);
	result = soundData.sourceVoice->Start();

	//再生状態にする
	if (soundData.playNow == false)
	{
		soundData.playNow = true;
	}
}

void Audio::LoadSound(const std::string& fileName)
{
	HRESULT result = S_FALSE;

	// 重複しているか
	if (soundList.find(fileName) != soundList.end())
	{
		// 何もせず抜ける
		return;
	}

	// ファイルストリーム
	std::ifstream file;
	// Waveファイルを開く
	file.open(fileName, std::ios_base::binary);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format;
	file.read((char*)&format, sizeof(format));

	WAVEFORMATEX wfex{};
	// 波形フォーマットの設定
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	// Dataチャンクの読み込み
	Chunk data;
	file.read((char*)&data, sizeof(data));

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();

	// サウンドデータ
	SoundData soundData = {};
	soundData.wfex = wfex;
	soundData.buffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	//サウンドデータを連想配列に格納
	soundList.insert(std::make_pair(fileName, soundData));
}

void Audio::StopSound(const std::string& fileName)
{
	std::map<std::string, SoundData>::iterator it = soundList.find(fileName);
	//未読み込みの検出
	assert(it != soundList.end());
	//サウンドデータの参照を取得
	SoundData& soundData = it->second;

	if (soundData.playNow == false)
	{
		return;
	}

	// 停止
	soundData.sourceVoice->Stop(0);
	soundData.sourceVoice->FlushSourceBuffers();
	soundData.sourceVoice->SubmitSourceBuffer(&buf);

	soundData.playNow = false;
}

void Audio::CreateSoundData(SoundData& soundData)
{
	HRESULT result = S_FALSE;

	// 波形フォーマットを元にSourceVoiceの生成
	result = xAudio2->CreateSourceVoice(&soundData.sourceVoice, &soundData.wfex, 0, 2.0f, &voiceCallback);
	assert(SUCCEEDED(result));
}

void Audio::Unload(SoundData *soundList)
{
	//バッファのメモリを解放
	delete[] soundList->buffer;

	soundList->wfex = {};
	soundList->buffer = 0;
	soundList->bufferSize = 0;
}

void Audio::Finalize()
{
	//音声データ解放
	std::map<std::string, SoundData>::iterator it = soundList.begin();
	for (; it != soundList.end(); ++it)
	{
		Unload(&it->second);
	}
	soundList.clear();
}
