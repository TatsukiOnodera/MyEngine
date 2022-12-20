#include "TouchableObject.h"
#include "MeshCollider.h"
#include <assert.h>

TouchableObject* TouchableObject::Create(Model* model, int div)
{
    TouchableObject* instance = new TouchableObject;
    if (instance == nullptr)
    {
        return nullptr;
    }

    if (instance->Initialize(model, div) == false)
    {
        delete instance;
        assert(0);
    }

    return instance;
}

bool TouchableObject::Initialize(Model* model, int div)
{
    Object3d::Initialize();

    SetModel(model);

    // コライダー追加
    MeshCollider* collider = new MeshCollider;
    SetCollider(collider);
    collider->ConstructTriangles(model, div);

    return true;
}
