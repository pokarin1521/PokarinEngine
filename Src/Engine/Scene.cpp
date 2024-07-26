/**
* @file Scene.cpp
*/
#include "Scene.h"

#include "Json/Json.h"

#include "Engine.h"
#include "Random.h"
#include "Debug.h"

#include "Components/Light.h"
#include "Components/Colliders/BoxCollider.h"

#include "Configs/ShaderConfig.h"

#include "Collision/Collision.h"

#include <algorithm>
#include <filesystem>

namespace PokarinEngine
{
#pragma region Create

	/// <summary>
	/// �V�[���쐬�p�R���X�g���N�^
	/// </summary>
	/// <param name="[in] e"> �G���W���N���X�̎Q�� </param>
	/// <param name="[in] sceneID"> �V�[�����ʔԍ� </param>
	/// <param name="[in] sceneName"> �V�[���� </param>
	Scene::Scene(Engine& e, int sceneID, const char* sceneName)
		:engine(&e), id(sceneID), name(sceneName)
	{
		// ���C���J�����쐬
		// �I�u�W�F�N�g�������Ɍ�����悤�ɏ������ɔz�u����
		mainCamera = CreateGameObject("MainCamera", Vector3(0, 0, -5));
		mainCameraInfo = mainCamera->AddComponent<Camera>();

		// ���s�������쐬
		auto directionalLight = CreateGameObject("Directional Light");
		std::shared_ptr<Light> lightComponent = directionalLight->AddComponent<Light>();
		lightComponent->SetType(LightParameter::Type::directional);
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g���쐬����
	/// </summary>
	/// <typeparam name="T"> �Q�[���I�u�W�F�N�g�N���X�܂��͂��̔h�� </typeparam>
	/// <param name="[in] name"> �I�u�W�F�N�g�̖��O </param>
	/// <param name="[in] position"> �I�u�W�F�N�g��z�u����ʒu </param>
	/// <param name="[in] rotation"> �I�u�W�F�N�g�̉�]�p�x </param>
	/// <param name="[in] staticMeshFile"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
	/// <returns> �ǉ������Q�[���I�u�W�F�N�g�̃|�C���^ </returns>
	GameObjectPtr Scene::CreateGameObject(const std::string& name,
		const Vector3& position, const Vector3& rotation, const char* staticMeshFile)
	{
		// �V�[�����̃Q�[���I�u�W�F�N�g�����ő�l�ɒB������쐬���Ȃ�
		if (gameObjectList.size() >= gameObjectMax)
		{
			LOG_WARNING("�Q�[���I�u�W�F�N�g�����ő�l�ɒB�������߁A�Q�[���I�u�W�F�N�g���쐬�o���܂���");
			return nullptr;
		}

		// �Q�[���I�u�W�F�N�g�쐬
		GameObjectPtr object = std::make_shared<GameObject>();

		// �Q�[���I�u�W�F�N�g�̏�����
		object->Initialize(*this, GetSingleObjectID(),
			staticMeshFile, name,
			position, rotation);

		// �Q�[���I�u�W�F�N�g�Ǘ��p�z��ɒǉ�
		gameObjectList.push_back(object);

		return object;
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�𕡐�����
	/// </summary>
	/// <param name="[in] object"> �������̃Q�[���I�u�W�F�N�g </param>
	void Scene::CopyGameObject(const GameObjectPtr& object)
	{
		// ���C���J�����͕������Ȃ�
		if (object == mainCamera)
		{
			return;
		}

		// �������̏����i�[����Json�^
		Json data;

		// �������̏����i�[����
		object->ToJson(data);

		// �Q�[���I�u�W�F�N�g���쐬
		GameObjectPtr copyObject = CreateGameObject("Copy Object");

		// �������̏����擾����
		copyObject->FromJson(data);
	}

#pragma endregion

#pragma region ID

	/// <summary>
	/// �I�u�W�F�N�g�̎��ʔԍ����擾����
	/// </summary>
	/// <returns> �d�����Ȃ����ʔԍ� </returns>
	int Scene::GetSingleObjectID()
	{
		// �I�u�W�F�N�g����int�^�̍ő�l�܂ŒB������ID�𐶐����Ȃ�
		// INT_MIN �` INT_MAX�܂ō��邪�]�T�������߂ɔ����ɂ���
		if (objectIDList.size() >= INT_MAX)
		{
			LOG_WARNING("�I�u�W�F�N�g�����ő�l�ɒB�������߁AID�𐶐��o���܂���");
			return 0;
		}

		// �I�u�W�F�N�g���ʔԍ�
		int objectID = Random::Range(INT_MIN, INT_MAX);

		// ���̃I�u�W�F�N�g�Ɣԍ����d�����Ă���Ȃ痐�����擾������
		while (!objectIDList.emplace(objectID).second)
		{
			objectID = Random::Range(INT_MIN, INT_MAX);
		}

		return objectID;
	}

#pragma endregion

#pragma region StaticMesh

	/// <summary>
	/// �X�^�e�B�b�N���b�V�����擾����
	/// </summary>
	/// <param name="[in] fileName"> �t�@�C���� </param>
	/// <returns> �t�@�C��������v����X�^�e�B�b�N���b�V�� </returns>
	StaticMeshPtr Scene::GetStaticMesh(const std::string& fileName)
	{
		return engine->GetStaticMesh(fileName);
	}

#pragma endregion

#pragma region Update

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̏�Ԃ��X�V����
	/// </summary>
	void Scene::UpdateGameObject()
	{
		// �Q�[���I�u�W�F�N�g���X�V
		for (const auto& gameObject : gameObjectList)
		{
			if (!gameObject->IsDestroyed())
			{
				gameObject->Update(engine->IsPlayGame());
			}
		}

		if (engine->IsPlayGame())
		{
			Collision::GameObjectCollision(gameObjectList);
		}

	} // UpdateGameObject

#pragma endregion

#pragma region Destroy

	/// <summary>
	/// �V�[������S�ẴQ�[���I�u�W�F�N�g��j������
	/// </summary>
	void Scene::ClearGameObject()
	{
		// �Q�[���I�u�W�F�N�g�̍폜�C�x���g�����s
		for (auto& gameObject : gameObjectList)
		{
			gameObject->OnDestroy();
		}

		// �Q�[���I�u�W�F�N�g��S�폜
		gameObjectList.clear();

		// �I�u�W�F�N�g�̎��ʔԍ���S�폜
		objectIDList.clear();
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g���폜����
	/// </summary>
	/// <param name="[in] object"> �폜����Q�[���I�u�W�F�N�g </param>
	void Scene::DestroyObject(GameObjectPtr& object)
	{
		// ���C���J�����͍폜���Ȃ�
		if (object == mainCamera)
		{
			return;
		}

		// -----------------------------------
		// �Q�[���I�u�W�F�N�g���폜
		// -----------------------------------

		object->Destroy();

		// -------------------------------------
		// �I�u�W�F�N�g���ʔԍ����폜
		// -------------------------------------

		objectIDList.erase(object->GetID());
	}

	/// <summary>
	/// �폜����Q�[���I�u�W�F�N�g�����S�ɍ폜����
	/// </summary>
	void Scene::RemoveDestroyedGameObject()
	{
		// �Q�[���I�u�W�F�N�g�������Ă��Ȃ���Ή������Ȃ�
		if (gameObjectList.empty())
		{
			return;
		}

		/* ���������₷���悤�ɍ폜�̗L���ŐU�蕪��
		�폜���Ȃ��Q�[���I�u�W�F�N�g��z��̑O���ɁA
		�폜����Q�[���I�u�W�F�N�g����둤�ɏW�߂�

		std::stable_partition(�͈͂̐擪, �͈͂̏I�[, �U�蕪������)
		�ʒu�֌W��ۂ��Ȃ���U�蕪���� */

		// �폜�̗L���ŐU�蕪������́A
		// �폜����I�u�W�F�N�g�̐擪�C�e���[�^
		const auto destoryBegin = std::stable_partition(
			gameObjectList.begin(), gameObjectList.end(),
			[](const GameObjectPtr& e) {return !e->IsDestroyed(); });

		// �폜����I�u�W�F�N�g�z��
		GameObjectList destroyList(
			std::move_iterator(destoryBegin),
			std::move_iterator(gameObjectList.end()));

		// �z�񂩂�ړ��ς݃I�u�W�F�N�g���폜
		gameObjectList.erase(destoryBegin, gameObjectList.end());

		// �폜����I�u�W�F�N�g�̍폜�C�x���g�����s
		for (auto& destoryObject : destroyList)
		{
			destoryObject->OnDestroy();
		}

		// �����Ŏ��ۂɃQ�[���I�u�W�F�N�g���폜�����(destroyList�̎������I��邽��)
	}

#pragma endregion

#pragma region Draw

	/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̃p�����[�^��GPU�ɃR�s�[����
	/// </summary>
	/// <param name="[in] prog"> �V�F�[�_�v���O�����̊Ǘ��ԍ� </param>
	/// <param name="[in] gameObject"> �p�����[�^���R�s�[����Q�[���I�u�W�F�N�g </param>
	void CopyGameObjectParameters(GLuint prog, const GameObjectPtr& gameObject)
	{
		// ���W�ϊ��s��
		Matrix4x4 transformMatrix = gameObject->transform->GetTransformMatrix();

		// �@���ϊ��s��
		Matrix3x3 normalMatrix = gameObject->transform->GetNormalMatrix();

		// ------------------------------------
		// �p�����[�^��GPU�ɃR�s�[
		// ------------------------------------

		// �I�u�W�F�N�g�̐F
		glProgramUniform4fv(prog,
			UniformLocation::color, 1, &gameObject->color.r);

		// ���W�ϊ��s��
		glProgramUniformMatrix4fv(
			prog, UniformLocation::transformMatrix,
			1, GL_FALSE, &transformMatrix[0].x);

		// �@���ϊ��s��
		if (prog == Shader::GetProgram(Shader::ProgType::Standard))
		{
			glProgramUniformMatrix3fv(
				prog, UniformLocation::normalMatrix,
				1, GL_FALSE, &normalMatrix[0].x);
		}
	}

	/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
	/// <summary>
	/// �Q�[���I�u�W�F�N�g��`�悷��
	/// </summary>
	/// <param name="[in] prog"> �V�F�[�_�v���O�����̊Ǘ��ԍ� </param>
	/// <param name="[in] begin"> �`�悷��Q�[���I�u�W�F�N�g�z��̐擪�C�e���[�^ </param>
	/// <param name="[in] end"> �`�悷��Q�[���I�u�W�F�N�g�z��̖����C�e���[�^ </param>
	void DrawGameObject(GLuint prog,
		GameObjectList::const_iterator begin, GameObjectList::const_iterator end)
	{
		// ------------------------
		// �I�u�W�F�N�g�̕`��
		// ------------------------

		// �Q�[���I�u�W�F�N�g��`��
		for (auto& itr = begin; itr != end; ++itr)
		{
			// �Q�[���I�u�W�F�N�g
			GameObjectPtr gameObject = *itr;

			// �폜�ς݂̃Q�[���I�u�W�F�N�g�͕`�悵�Ȃ�
			if (gameObject->IsDestroyed())
			{
				continue;
			}

			// �X�^�e�B�b�N���b�V�����Ȃ��ꍇ
			if (!gameObject->staticMesh)
			{
				// �`�悹�����̃I�u�W�F�N�g��
				continue;
			}

			// -------- ���j�t�H�[���ϐ��Ƀf�[�^���R�s�[ -----------

			// �`��Ɏg���V�F�[�_���w��
			glUseProgram(prog);

			// �p�����[�^���R�s�[
			CopyGameObjectParameters(prog, gameObject);

			// ------------- �}�`��`�� --------------

			// �ŗL�}�e���A�����Ȃ�
			if (gameObject->materials.empty())
			{
				// ���L�}�e���A�����g����
				// �X�^�e�B�b�N���b�V����`��
				DrawMesh(gameObject->staticMesh, prog, gameObject->staticMesh->materials);
			}
			// �ŗL�}�e���A��������
			else
			{
				// �ŗL�}�e���A�����g����
				// �X�^�e�B�b�N���b�V����`��
				DrawMesh(gameObject->staticMesh, prog, gameObject->materials);
			}
		}
	}

	/// <summary>
	/// �V�[�����̑S�ẴQ�[���I�u�W�F�N�g��`�悷��
	/// </summary>
	void Scene::DrawGameObjectAll()
	{
		// ------------------------------
		// �`��D��x�ŕ��בւ�
		// ------------------------------

		// �`�悷��Q�[���I�u�W�F�N�g�z��
		// �{�̂̏��Ԃ��ς��ƍ���̂ŃR�s�[
		GameObjectList drawObjectList = gameObjectList;

		// �Q�[���I�u�W�F�N�g��`�揇�ɕ��בւ���
		// (�`��D��x���Ⴂ���̂��珇�ɕ��ׂ�)
		std::stable_sort(drawObjectList.begin(), drawObjectList.end(),
			[](const GameObjectPtr& a, const GameObjectPtr& b) {
				return a->renderQueue < b->renderQueue; });

		/* �\�[�g�ς݂͈̔͂ɑ΂��Č������s���ɂ́Alower_bound�֐����g��

		lower_bound(�͈͂̐擪, �͈͂̏I�[, ��������l, ��������)

		���������𖞂����Ȃ��ŏ��̗v�f�̈ʒu��Ԃ�

		��������l��RenderQueue_overlay�A
		�����������urenderQueue_overlay < ��������l�v�Ƃ���ƁA
		�uRenderQueue_overlay�ȏ�̍ŏ��̗v�f�̈ʒu�v��Ԃ�

		�Y������v�f��������Ȃ��ꍇ�́A�͈͂̏I�[��Ԃ� */

		// �������Ȑ}�`�̐擪������
		const auto transparentBegin = std::lower_bound(
			drawObjectList.begin(), drawObjectList.end(), renderQueue_transparent,
			[](const GameObjectPtr& e, int value) {
				return e->renderQueue < value; });

		// UI�E�G�t�F�N�g�Ȃǂ̐擪������
		const auto overlayBegin = std::lower_bound(
			transparentBegin, drawObjectList.end(), renderQueue_overlay,
			[](const GameObjectPtr& e, int value) {
				return e->renderQueue < value; });

		// ---------------------------------
		// �D��x���ɕ`��
		// ---------------------------------

		// �W���V�F�[�_�v���O�����̊Ǘ��ԍ�
		GLuint progStandard = Shader::GetProgram(Shader::ProgType::Standard);

		// ���C�e�B���O�����V�F�[�_�v���O�����̊Ǘ��ԍ�
		GLuint progUnlit = Shader::GetProgram(Shader::ProgType::Unlit);

		// ---------- transparent�ȑO�̃L���[��`��  -----------
		// ---------- �ʏ�̃I�u�W�F�N�g		     -----------

		// �`��
		DrawGameObject(progUnlit, drawObjectList.begin(), transparentBegin);

		// ------ transparent����overlay�܂ł̃L���[��`�� ------
		// ------ �������ȃI�u�W�F�N�g					   ------

		// �������ȃI�u�W�F�N�g�Ȃ̂Ō��̃I�u�W�F�N�g��������悤��
		// �[�x�o�b�t�@�ւ̏������݂��֎~
		glDepthMask(GL_FALSE);

		// �`��
		DrawGameObject(progStandard, transparentBegin, overlayBegin);

		// ���̃I�u�W�F�N�g��������͔̂������ȃI�u�W�F�N�g�����Ȃ̂�
		// �[�x�o�b�t�@�ւ̏������݂�����
		glDepthMask(GL_TRUE);

		// ---------------- overlay�ȍ~�̃L���[��`�� ----------------
		// ---------------- UI,�S��ʃG�t�F�N�g�Ȃ�   ----------------

		// overlay�ȍ~��
		// UI�n�̐[�x�l�֌W�Ȃ��`�悵�ė~�����I�u�W�F�N�g�Ȃ̂ŁA
		// �[�x�e�X�g�𖳌�������
		glDisable(GL_DEPTH_TEST);

		// �`��
		// UI�Ƀ��C�e�B���O�͂���Ȃ��̂ŃA�����b�g�V�F�[�_���g��
		DrawGameObject(progUnlit, overlayBegin, drawObjectList.end());

		// �Q�[���I�u�W�F�N�g�̃R���C�_�[��`��
		// �R���C�_�[�͐[�x�l�֌W�Ȃ��`�悵�Ȃ��ƕs�ւȂ̂ŁA�����ŕ`�悷��
		for (const auto& drawObject : drawObjectList)
		{
			drawObject->DrawCollider();
		}
	}

#pragma endregion

#pragma region Save,Load

	/// <summary>
	/// �V�[���̏����t�@�C���ɕۑ�����
	/// </summary>
	void Scene::SaveScene() const
	{
		// ---------------------------------------
		// �V�[���̏���Json�^�Ɋi�[����
		// ---------------------------------------

		// �ۑ�����f�[�^
		Json data;

		// �V�[���̖��O
		data["Scene"] = name;


		// ----------------------------------------------------
		// �Q�[���I�u�W�F�N�g�̏���Json�^�Ɋi�[����
		// ----------------------------------------------------

		// �Q�[���I�u�W�F�N�g���ʔԍ�(������)�̔z��
		// ���X�g�̏��Ԃ��ێ����邽�߂�vector�^�ɂ���
		std::vector<std::string> stringIDList;
		stringIDList.reserve(gameObjectList.size());

		// �Q�[���I�u�W�F�N�g�̏����i�[
		for (const auto& gameObject : gameObjectList)
		{
			gameObject->ToJson(data[gameObject->GetID_String()]);
			stringIDList.push_back(gameObject->GetID_String());
		}

		// �Q�[���I�u�W�F�N�g���ʔԍ��̔z����i�[
		data["ObjectIDList"] = stringIDList;

		// ----------------------------------------------
		// Json�t�@�C���ɏ���ۑ�����
		// ----------------------------------------------

		// �ۑ���̃f�B���N�g�����Ȃ���΍쐬����
		std::filesystem::create_directories(folderName);

		// �ۑ���̃t�@�C����
		const std::string fileName = GetFileName();

		// �ۑ�
		JsonFile::Save(fileName, data);
	}

	/// <summary>
	/// �V�[���̏����t�@�C������ǂݍ���
	/// </summary>
	void Scene::LoadScene()
	{
		// ---------------------------------------------
		// �t�H���_�����݂��Ȃ��ꍇ�͉������Ȃ�
		// ---------------------------------------------

		if (!std::filesystem::exists(folderName))
		{
			return;
		}

		// --------------------------------------------------------
		// ���ݐ�������Ă���Q�[���I�u�W�F�N�g���폜����
		// --------------------------------------------------------

		ClearGameObject();

		// ---------------------------------------
		// �t�@�C���������ǂݍ���
		// ---------------------------------------

		// �����i�[����Json�^
		Json data;

		// �V�[���̏���ǂݍ���
		JsonFile::Load(GetFileName(), data);

		// ---------------------------------------------
		// �Q�[���I�u�W�F�N�g�̏���ǂݍ���
		// ---------------------------------------------

		// �Q�[���I�u�W�F�N�g�̎��ʔԍ�(������)
		for (const std::string& objectID_string : data["ObjectIDList"])
		{
			// �Q�[���I�u�W�F�N�g���쐬
			GameObjectPtr gameObject = CreateGameObject("New Object");

			// ���ʔԍ��ɑΉ���������ǂݍ���
			gameObject->FromJson(data[objectID_string]);
		}
	}

#pragma endregion

} // namespace PokarinEngine