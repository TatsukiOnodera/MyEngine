#pragma once
#include "AbstractSceneFactory.h"

//�Q�[���p�̃V�[���H��
class SceneFactory : public AbstractSceneFactory
{
public: // �����o�֐�
	/// <summary>
	/// �V�[������
	/// </summary>
	/// <param name="scene_name">�V�[����</param>
	/// <returns>���������V�[��</returns>
	BaseScene* CreateScene(const std::string& scene_name) override;
};