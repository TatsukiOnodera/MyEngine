#pragma once
#include "BaseScene.h"
#include <string>

/// <summary>
/// �V�[���\���i�T�O�j
/// </summary>
class AbstractSceneFactory
{
public:
	//�f�X�g���N�^
	virtual ~AbstractSceneFactory() = default;

	/// <summary>
	/// �V�[������
	/// </summary>
	/// <param name="scene_name">�V�[����</param>
	/// <returns>���������V�[��</returns>
	virtual BaseScene* CreateScene(const std::string& scene_name) = 0;
};