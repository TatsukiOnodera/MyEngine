#include "FbxLoader.h"

using namespace DirectX;

const std::string FbxLoader::baseDirectory = "Resources/";

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;

    return &instance;
}

bool FbxLoader::Initialize(ID3D12Device* device)
{
    //�ď������`�F�b�N
    if (device == nullptr || fbxManager)
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

    //���f������
    FbxModel* fbxModel = new FbxModel();
    fbxModel->name = modelName;

    //FBX�m�[�h�̐����擾
    int nodeCount = fbxScene->GetNodeCount();

    //���炩���ߕK�v�����̃��������擾���邱�ƂŃA�h���X�������̂�\�h
    fbxModel->nodes.reserve(nodeCount);

    //���[�g�m�[�h���珇�ɉ�͂��ă��f���ɗ�������
    ParseNodeRecursive(fbxModel, fbxScene->GetRootNode());

    //FBX�V�[���J��
    fbxScene->Destroy();
}

void FbxLoader::ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, Node* parent)
{
    //���f���Ƀm�[�h�ǉ�
    fbxModel->nodes.emplace_back();
    Node& node = fbxModel->nodes.back();

    //�m�[�h�����擾
    node.name = fbxNode->GetName();

    //FBX�m�[�h�̃��[�J���ړ����
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();

    //�`���ϊ����đ��
    node.rotation = { (float)rotation[0], (float)rotation[1], (float)rotation[2], 0.0f };
    node.scaling = { (float)scaling[0], (float)scaling[1], (float)scaling[2], 0.0f };
    node.translation = { (float)translation[0], (float)translation[1], (float)translation[2], 1.0f };

    //�X�P�[���A��]�A���s�ړ��s��̌v�Z
    XMMATRIX matScaling, matRotatioon, matTranslation;
    matScaling = XMMatrixScalingFromVector(node.scaling);
    matRotatioon = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    matTranslation = XMMatrixTranslationFromVector(node.translation);

    //���[�J���ό`�s��̌v�Z
    node.transform = XMMatrixIdentity();
    node.transform *= matScaling; //���[���h�s��ɃX�P�[�����O�𔽉f
    node.transform *= matRotatioon; //���[���h�s��ɉ�]�𔽉f
    node.transform *= matTranslation; //���[���h�s��ɕ��s�ړ��𔽉f

    //�O���[�o���ό`�s��̌v�Z
    node.globalTransform = node.transform;
    if (parent)
    {
        node.parent = parent;
        //�e�̕ό`����Z
        node.globalTransform *= parent->globalTransform;
    }
  
    //fbx�m�[�h�̃��b�V��������́iTodo�j


    //�q�m�[�h�ɑ΂��čċA�Ăяo��
    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(fbxModel, fbxNode->GetChild(i), &node);
    }
}
