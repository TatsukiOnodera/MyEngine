#include "FbxLoader.h"

const std::string FbxLoader::baseDirectory = "Resources/";

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;

    return &instance;
}

bool FbxLoader::Initialize(ID3D12Device* device)
{
    //�ď������`�F�b�N
    if (device == nullptr || !(fbxManager == nullptr))
    {
        return false;
    }

    //�������烁���o�ϐ��ɑ��
    this->dev = device;

    //FBX�}�l�[�W���̐���
    fbxManager = FbxManager::Create();

    //FBX�}�l�[�W���̓��o�͐ݒ�
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);

    //FBX�C���|�[�^�̐���
    fbxImporter = FbxImporter::Create(fbxManager, "");

    return true;
}

void FbxLoader::Finalize()
{
    //�e��FBX�C���X�^���X�̔j��
    fbxImporter->Destroy();
    fbxManager->Destroy();
}

void FbxLoader::LoadModelFromFile(const string& modelName)
{
    //���f���Ɠ������O�̃t�H���_����ǂݍ���
    const string directoryPath = baseDirectory + modelName + "/";
    //�g���q.fbx��ǉ�
    const string fileName = modelName + ".fbx";
    //�A�����ăt���p�X�𓾂�
    const string fullPath = directoryPath + fileName;

    //�t�@�C�������w�肵��FBX�t�@�C����ǂݍ���
    if (!fbxImporter->Initialize(fullPath.c_str(), -1, fbxManager->GetIOSettings()))
    {
        assert(0);
    }

    //�V�[���쐬
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");
    //�t�@�C�����烍�[�h����FBX�̏����V�[���ɃC���|�[�g
    fbxImporter->Import(fbxScene);
}
