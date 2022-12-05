#include "CollisionManager.h"
#include "Collision.h"
#include "MeshCollider.h"

using namespace DirectX;

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

bool CollisionManager::RayCastHit(const Ray& ray, RayCast* hitInfo, float maxDistance)
{
	bool result = false;
	// 走査用
	std::forward_list<BaseCollider*>::iterator it;
	// 最短のコライダー
	std::forward_list<BaseCollider*>::iterator it_hit;
	// 距離
	float distance = maxDistance;
	// 交点
	XMVECTOR inter;

	it = m_colliders.begin();
	for (; it != m_colliders.end(); ++it)
	{
		BaseCollider* colA = *it;

		// 球
		if (colA->GetShapeType() == COLLISONSHAPE_SPHERE)
		{
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			XMVECTOR tempInter;
			// 当たってないなら除外
			if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter))
			{
				continue;
			}
			// 距離が最短でないなら除外
			if (tempDistance >= distance)
			{
				continue;
			}
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
		// 面
		else if (colA->GetShapeType() == COLLISONSHAPE_MESH)
		{
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

			float tempDistance;
			DirectX::XMVECTOR tempInter;
			// 当たってないなら除外
			if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter))
			{
				continue;
			}
			// 距離が最短でないなら除外
			if (tempDistance >= distance)
			{
				continue;
			}

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
	}

	// 結果の書き込み
	if (result && hitInfo)
	{
		hitInfo->m_distance = distance;
		hitInfo->m_inter = inter;
		hitInfo->m_collider = *it_hit;
		hitInfo->m_object = hitInfo->m_collider->GetObject3d();
	}

	return result;
}

void CollisionManager::CheckAllCollisions()
{
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	// すべての組み合わせ
	itA = m_colliders.begin();
	for (; itA != m_colliders.end(); ++itA)
	{
		itB = itA;
		++itB;
		for (; itB != m_colliders.end(); ++itB)
		{
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			// 球と球
			if (colA->GetShapeType() == COLLISONSHAPE_SPHERE && colB->GetShapeType() == COLLISONSHAPE_SPHERE)
			{
				Sphere* sphereA = dynamic_cast<Sphere*>(colA);
				Sphere* sphereB = dynamic_cast<Sphere*>(colB);
				DirectX::XMVECTOR inter;

				// 球と球の判定
				if (Collision::CheckSphere2Sphere(*sphereA, *sphereB, &inter) == true)
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			// 面と球
			else if (colA->GetShapeType() == COLLISONSHAPE_MESH && colB->GetShapeType() == COLLISONSHAPE_SPHERE)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
				Sphere* sphere = dynamic_cast<Sphere*>(colB);
				DirectX::XMVECTOR inter;

				// 球と球の判定
				if (meshCollider->CheckCollisionSphere(*sphere, &inter) == true)
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			// 球と面
			else if (colA->GetShapeType() == COLLISONSHAPE_SPHERE && colB->GetShapeType() == COLLISONSHAPE_MESH)
			{
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				DirectX::XMVECTOR inter;

				// 球と球の判定
				if (meshCollider->CheckCollisionSphere(*sphere, &inter) == true)
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
		}
	}
}
