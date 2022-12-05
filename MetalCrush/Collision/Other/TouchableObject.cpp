#include "TouchableObject.h"
#include "MeshCollider.h"
#include <assert.h>

TouchableObject* TouchableObject::Create(Model* model)
{
    TouchableObject* instance = new TouchableObject;
    if (instance == nullptr)
    {
        return nullptr;
    }

    if (instance->Initialize(model) == false)
    {
        delete instance;
        assert(0);
    }

    return instance;
}

bool TouchableObject::Initialize(Model* model)
{
    Object3d::Initialize();

    SetModel(model);

    // コライダー追加
    MeshCollider* collider = new MeshCollider;
    SetCollider(collider);
    collider->ConstructTriangles(model);

    return true;
}
