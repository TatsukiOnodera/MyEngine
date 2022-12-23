#include "TouchableObject.h"
#include "MeshCollider.h"
#include <assert.h>

TouchableObject* TouchableObject::Create(const UINT modelNumber)
{
    TouchableObject* instance = new TouchableObject;
    if (instance == nullptr)
    {
        return nullptr;
    }

    if (instance->Initialize(modelNumber) == false)
    {
        delete instance;
        assert(0);
    }

    return instance;
}

bool TouchableObject::Initialize(const UINT modelNumber)
{
    Object3d::Initialize();

    Model* model = Object3d::GetModel(modelNumber);
    SetModel(model);

    // コライダー追加
    MeshCollider* collider = new MeshCollider;
    SetCollider(collider);
    collider->ConstructTriangles(model);

    return true;
}
