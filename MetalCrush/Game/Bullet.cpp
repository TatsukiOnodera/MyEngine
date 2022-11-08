#include "Bullet.h"

using namespace DirectX;

Bullet::Bullet(const XMFLOAT3& pos, const XMFLOAT3& vel, const XMFLOAT3& acc, const bool& alive)
{
	m_object.reset(Object3d::Create("Bullet", true));
	Initialize(pos, vel, acc, alive);
}

Bullet::~Bullet()
{

}

void Bullet::Initialize(const XMFLOAT3& pos, const XMFLOAT3& vel, const XMFLOAT3& acc, const bool& alive)
{
	if (m_object == nullptr)
	{
		assert(0);
	}

	m_pos = pos;
	m_vel = vel;
	m_acc = acc;
	m_alive = alive;

	m_object->SetPosition(m_pos);
	m_object->SetScale({ 1.0f, 1.0f, 1.0f });
	m_object->SetColor({ 0.9f, 0.1f, 0.3f, 1.0f });
	m_object->SetShader(Object3d::ShaderType::Mono);
	m_object->Update();
}

void Bullet::Update()
{
	if (m_alive == true)
	{
		// ‘¬“x‚É‰Á‘¬“x‚ð‰ÁŽZ
		m_vel.x += m_acc.x;
		m_vel.y += m_acc.y;
		m_vel.z += m_acc.z;

		// À•W‚É‘¬“x‚ð‰ÁŽZ
		m_pos.x += m_vel.x;
		m_pos.y += m_vel.y;
		m_pos.z += m_vel.z;

		m_object->SetPosition(m_pos);
	}
}

void Bullet::Draw()
{
	if (m_alive == true)
	{
		m_object->Draw();
	}
}
