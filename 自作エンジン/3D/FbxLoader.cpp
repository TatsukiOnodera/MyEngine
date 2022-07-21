#include "FbxLoader.h"

using namespace DirectX;

const std::string FbxLoader::baseDirectory = "Resources/";
const std::string FbxLoader::defaultTextureFileName = "Default/white1x1.png";

void FbxLoader::ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh, FbxModelMesh* modelMesh)
{
    auto& vertices = modelMesh->vertices;

    //���_���W�f�[�^�̐�
    const int controlPointsCount = fbxMesh->GetControlPointsCount();

    //FBX���b�V���̒��_���W�z����擾
    FbxVector4* pCoord = fbxMesh->GetControlPoints();

    //FBX���b�V���̑S���_���W�����f���̔z��ɃR�s�[
    FbxModelMesh::VertexPosNormalUvSkin vertex;
    for (int i = 0; i < controlPointsCount; i++)
    {
        vertex.pos.x = (float)pCoord[i][0];
        vertex.pos.y = (float)pCoord[i][1];
        vertex.pos.z = (float)pCoord[i][2];
        vertices.push_back(vertex);
    }
}

void FbxLoader::ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh, FbxModelMesh* modelMesh)
{
    auto& vertices = modelMesh->vertices;
    auto& indices = modelMesh->indices;

    //�ʂ̐�
    const int polygonCount = fbxMesh->GetPolygonCount();

    //UI�f�[�^�̐�
    const int textureUVCount = fbxMesh->GetTextureUVCount();

    //UV�����X�g
    FbxStringList uvNames;
    fbxMesh->GetUVSetNames(uvNames);

    //�ʂ��Ƃ̏��ǂݎ��
    for (int i = 0; i < polygonCount; i++)
    {
        //�ʂ��\�����钸�_�̐����擾
        const int polygonSize = fbxMesh->GetPolygonSize(i);
        assert(polygonSize <= 4);

        //1���_����
        for (int j = 0; j < polygonSize; j++)
        {
            //FBX���_�z��̃C���f�b�N�X
            int index = fbxMesh->GetPolygonVertex(i, j);
            assert(index >= 0);

            //���_�@���ǂݍ���
            FbxModelMesh::VertexPosNormalUvSkin& vertex = vertices[index];
            FbxVector4 normal;
            if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
            {
                vertex.normal.x = (float)normal[0];
                vertex.normal.y = (float)normal[1];
                vertex.normal.z = (float)normal[2];
            }

            //�e�N�X�`��UV�ǂݍ���
            if (textureUVCount > 0)
            {
                FbxVector2 uvs;
                bool lUnmappedUV;

                //0�Ԍ��ߑł��ǂݍ���
                if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
                {
                    vertex.uv.x = (float)uvs[0];
                    vertex.uv.y = (float)uvs[1];
                }
            }

            //�C���f�b�N�X�z��ɒ��_�C���f�b�N�X�ǉ�
            if (j < 3) //3�_�ڂȂ�
            {
                //1�_�ǉ����A����2�_�ƎO�p�`���\�z����
                indices.push_back(index);
            } 
            else //4�_�ڂȂ�
            {
                //3�_��ǉ���
                //�l�p�`��0, 1, 2, 3�̓�2, 3, 0�ō\�z
                int index2 = indices[indices.size() - 1];
                int index3 = index;
                int index0 = indices[indices.size() - 3];
                indices.push_back(index2);
                indices.push_back(index3);
                indices.push_back(index0);
            }
        }
    }
}

void FbxLoader::ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode)
{
    const int materialCount = fbxNode->GetMaterialCount();

    if (materialCount > 0)
    {
        //�擪�}�e���A�����擾
        FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);

        //�e�N�X�`����ǂݍ��񂾂�
        bool textureLoaded = false;

        if (material)
        {
            if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

                //�����W��
                FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
                fbxModel->ambient.x = (float)ambient.Get()[0];
                fbxModel->ambient.y = (float)ambient.Get()[1];
                fbxModel->ambient.z = (float)ambient.Get()[2];

                //�g�U���ˌ��W��
                FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
                fbxModel->diffuse.x = (float)diffuse.Get()[0];
                fbxModel->diffuse.y = (float)diffuse.Get()[1];
                fbxModel->diffuse.z = (float)diffuse.Get()[2];
            }

            //�f�B�t���[�Y�e�N�X�`�������o��
            const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            if (diffuseProperty.IsValid())
            {
                const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
                if (texture)
                {
                    const char* filepath = texture->GetFileName();

                    //�t�@�C���p�X����t�@�C�������o
                    string path_str(filepath);
                    string name = ExtractFileName(path_str);

                    //�e�N�X�`���ǂݍ���
                    LoadTexture(fbxModel, baseDirectory + fbxModel->name + "/" + name);

                    textureLoaded = true;
                }
            }
        }
        //�e�N�X�`�����Ȃ��ꍇ
        if (textureLoaded == false)
        {
            LoadTexture(fbxModel, baseDirectory + defaultTextureFileName);
        }
    }
}

void FbxLoader::LoadTexture(FbxModel* fbxModel, const std::string& fullpath)
{
    HRESULT result = S_FALSE;

    //WIC�e�N�X�`���̃��[�h
    TexMetadata& metadata = fbxModel->metadata;
    ScratchImage& scratchImg = fbxModel->scratchImg;

    //���j�R�[�h������ɕϊ�
    wchar_t wfilepath[128];
    MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));
    result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
    if (FAILED(result))
    {
        assert(0);
    }
}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
    size_t pos1;
    //��؂蕶���f\\�f���o�Ă����ԍŌ�̕���������
    pos1 = path.rfind('\\');
    if (pos1 != string::npos)
    {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }
    //��؂蕶���f/�f���o�Ă����ԍŌ�̕���������
    pos1 = path.rfind('/');
    if (pos1 != string::npos)
    {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    return path;
}

void FbxLoader::ParseSkin(FbxModel* fbxModel, FbxMesh* fbxMesh, FbxModelMesh* modelMesh)
{
    //�X�L�j���O���
    FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));
    //��񂪂Ȃ��Ȃ�
    if (fbxSkin == nullptr)
    {
        //�e���_�ɂ��Ă̏���
        for (int i = 0; i < modelMesh->vertices.size(); i++)
        {
            //�ŏ��̃{�[���i�P�ʍs��j�̉e��100%�ɂ���
            modelMesh->vertices[i].boneIndex[0] = 0;
            modelMesh->vertices[i].boneWeight[0] = 1.0f;
        }
        return;
    }

    //�{�[���z��̎Q��
    std::vector<FbxModel::Bone>& bones = fbxModel->bones;

    //�{�[���̐�
    int clusterCount = fbxSkin->GetClusterCount();

    //���ׂẴ{�[���ɂ���
    if (bones.empty())
    {
        for (int i = 0; i < clusterCount; i++)
        {
            //FBX�{�[�����
            FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

            //�{�[�����̂̃m�[�h�����擾
            const char* boneName = fbxCluster->GetLink()->GetName();

            //�V�����{�[����ǉ����A�ǉ������{�[���̎Q�Ƃ𓾂�
            bones.emplace_back(FbxModel::Bone(boneName));
            FbxModel::Bone& bone = bones.back();
            //����{�[����FBX�{�[����R�Â���
            bone.fbxCluster = fbxCluster;

            //FBX���珉���p���s����擾����
            FbxAMatrix fbxMat;
            fbxCluster->GetTransformLinkMatrix(fbxMat);

            //XMMatrix�^�ɕϊ�����
            XMMATRIX initialPose;
            ConvertMatrixFromFbx(&initialPose, fbxMat);

            //�����p���s��̋t�s��𓾂�
            bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);
        }
    }

    //�{�[���ԍ��ƃX�L���E�F�C�g�̃y�A
    struct WeightSet
    {
        UINT index;
        float weight;
    };

    //�񎟌��z��i�W���O�z��j
    //list�F���_���e�����󂯂�{�[���̃��X�g
    //vecto�F�S���_��:
    std::vector<std::list<WeightSet>> weightLists(modelMesh->vertices.size());

    //���ׂẴ{�[���ɂ���
    for (int i = 0; i < clusterCount; i++)
    {
        //FBX�{�[�����
        FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
        //�{�[���ɉe�����󂯂钸�_�̐�
        int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
        //�{�[���ɉe�����󂯂钸�_�̔z��
        int* controlPointIndices = fbxCluster->GetControlPointIndices();
        double* controlPointWeights = fbxCluster->GetControlPointWeights();

        //�e�����󂯂�S���_�ɂ���
        for (int j = 0; j < controlPointIndicesCount; j++)
        {
            //���_�ԍ�
            int vertIndex = controlPointIndices[j];
            //�X�L���E�F�C�g
            float weight = (float)controlPointWeights[j];
            //���̒��_�̉e�����󂯂�{�[�����X�g�ɁA�{�[���ƃE�F�C�g�̃y�A��g��
            weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i, weight });
        }
    }

    //���_���������p�̎Q��
    auto& vertices = modelMesh->vertices;
    //�S���_�ɂ��Ă̏��u
    for (int i = 0; i < vertices.size(); i++)
    {
        //���_�̃E�F�C�g����ł��傫���l��I��
        auto& weightList = weightLists[i];
        //�召��r�p�̃����_�����w�肵�č~���Ƀ\�[�g
        weightList.sort(
            [](auto const& lhs, auto const& rhs)
            {
                //�����傫�����true
                return lhs.weight > rhs.weight;
            }
        );

        int weightArrayIndex = 0;
        //�~���\�[�g�ς݂̃E�F�C�g���X�g����
        for (auto& weightSet : weightList)
        {
            //���_�f�[�^����������
            vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
            vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;
            //�l�B������I��
            if (++weightArrayIndex >= FbxModelMesh::MAX_BONE_INDICES)
            {
                float weight = 0.0f;
                //��T�ڈȍ~�̃E�F�C�g���v�Z
                for (int j = 1; j < FbxModelMesh::MAX_BONE_INDICES; j++)
                {
                    weight += vertices[i].boneWeight[j];
                }
                //���v��1.0f�i100%�j�ɂȂ�悤�ɒ���
                vertices[i].boneWeight[0] = 1.0f - weight;
                break;
            }
        }
    }
}

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;

    return &instance;
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{
    //�s
    for (int i = 0; i < 4; i++)
    {
        //��
        for (int j = 0; j < 4; j++)
        {
            //�v�f���R�s�[
            dst->r[i].m128_f32[j] = (float)src.Get(i, j);
        }
    }
}

bool FbxLoader::Initialize(ID3D12Device* device)
{
    //�ď������`�F�b�N
    if (device == nullptr)
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

FbxModel* FbxLoader::LoadModelFromFile(const string& modelName)
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

    //FBX���f������FBX�V�[��
    fbxModel->fbxScene = fbxScene;

    //�o�b�t�@����
    fbxModel->CreateBuffers(dev);
    
    return fbxModel;
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
  
    //FBX�m�[�h�̃��b�V��������́iTodo�j
    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

    if (fbxNodeAttribute)
    {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            fbxModel->meshNode = &node;
            ParseMesh(fbxModel, fbxNode);
        }
    }

    //�q�m�[�h�ɑ΂��čċA�Ăяo��
    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ParseNodeRecursive(fbxModel, fbxNode->GetChild(i), &node);
    }
}

void FbxLoader::ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode)
{
    //�m�[�h�̃��b�V�����擾
    FbxMesh* fbxMesh = fbxNode->GetMesh();

    FbxModelMesh* modelMesh = new FbxModelMesh;

    //���_���W�ǂݎ��
    ParseMeshVertices(fbxModel, fbxMesh, modelMesh);
    //�ʏ��ǂݎ��
    ParseMeshFaces(fbxModel, fbxMesh, modelMesh);
    //�}�e���A���ǂݎ��
    ParseMaterial(fbxModel, fbxNode);
    //�X�L�j���O���ǂݎ��
    ParseSkin(fbxModel, fbxMesh, modelMesh);

    //���b�V���f�[�^���i�[
    fbxModel->AddModelMesh(modelMesh);
}
