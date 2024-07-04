/**
* @file Scene.cpp
*/
#include "Scene.h"

#include "Engine.h"
#include "Random.h"
#include "Debug.h"

#include "Components/Light.h"

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
		mainCameraInfo = mainCamera->AddComponent<Camera>().first;

		// ���s�������쐬
		auto directionalLight = CreateGameObject("Directional Light");
		std::shared_ptr<Light> lightComponent = directionalLight->AddComponent<Light>().first;
		lightComponent->SetType(LightParameter::Type::directional);
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g���쐬����
	/// </summary>
	/// <typeparam name="T"> �Q�[���I�u�W�F�N�g�N���X�܂��͂��̔h�� </typeparam>
	/// <param name="[in] name"> �I�u�W�F�N�g�̖��O </param>
	/// <param name="[in] position"> �I�u�W�F�N�g��z�u������W </param>
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

		// �V�[����ݒ�
		object->ownerScene = this;

		// �쐬���̖��O����ޖ��Ƃ��Đݒ�
		object->typeName = name;

		// ���O��ݒ�
		object->name = name;

		// �I�u�W�F�N�g��ޖ���ǉ�
		// �d�����Ă���ꍇ�́A���O���Đݒ肷��
		if (!objectTypeNameList.emplace(object->typeName, SetNumberNameList()).second)
		{
			// �d�����Ȃ��悤�ɖ��O��ݒ�
			object->name = GetSingleObjectName(object->typeName);

			// �d�����Ă���̂ŁA�����t���ɂȂ��Ă��閼�O��ǉ�����
			objectTypeNameList[object->typeName].emplace(object->name);
		}

		// �d�����Ȃ��悤�Ɏ��ʔԍ���ݒ�
		object->id = GetSingleObjectID();

		// �Q�[���I�u�W�F�N�g�̏�����
		// ���ʔԍ����K�v�ɂȂ�̂ŁA���̃^�C�~���O�ōs��
		object->Initialize();

		// �ʒu�Ɗp�x��ݒ�
		object->transform->position = position;
		object->transform->rotation = rotation;

		// �X�^�e�B�b�N���b�V��
		object->staticMesh = engine->GetStaticMesh(staticMeshFile);

		// �X�^�e�B�b�N���b�V��������Ȃ�ŗL�}�e���A����ݒ�
		if (object->staticMesh)
		{
			// ���L�}�e���A���̃R�s�[��
			// �ŗL�}�e���A���Ƃ��Đݒ肷��
			object->materials = CloneMaterialList(object->staticMesh);
		}

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

		// �������Ɠ����p�����[�^�̃I�u�W�F�N�g���쐬
		// ���O�̏d���m�F���o����悤�Ɏ�ޖ����g��
		GameObjectPtr copyObject = CreateGameObject(object->typeName,
			object->transform->position, object->transform->rotation,
			object->staticMesh->filename.c_str());
	}

#pragma endregion

#pragma region Name,ID

	/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
	/// <summary>
	/// �I�u�W�F�N�g���̌��ɕt���Ă鐔�����擾����
	/// </summary>
	/// <param name="[in] objectName"> �I�u�W�F�N�g�� </param>
	/// <returns> ()�̒��ɂ��鐔�� </returns>
	int GetBackNumber(const std::string& objectName)
	{
		/* ������()�̒��ɂ���̂�
		()�̈ʒu���琔���̈ʒu�Ɛ������߂� */

		// �����̎n��
		// �u(�v�̎����琔��������̂�+1
		size_t first = objectName.find_last_of("(") + 1;

		// �����̏I���
		// �u)�v���I���̍��}�Ȃ̂Łu)�v�̈ʒu�����߂�
		size_t last = objectName.find_last_of(")");

		// ()���Ȃ��ꍇ�A0��Ԃ�
		if (first == objectName.npos ||
			last == objectName.npos)
		{
			return 0;
		}

		// �����̐�
		// �u�I���̈ʒu�v�Ɓu�n�߂̈ʒu�v�̍����琔�����߂�
		size_t numSize = last - first;

		// ���O�̌��ɕt���Ă鐔��(������)
		std::string backNum_str = objectName.substr(first, numSize);

		// int�^�ɕϊ����ĕԂ�
		return std::stoi(backNum_str);
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̖��O��ύX����
	/// </summary>
	/// <param name="[out] object"> ���O��ύX����I�u�W�F�N�g </param>
	/// <param name="[in] afterName"> �ύX��̖��O </param>
	void Scene::ChangeObjectName(GameObjectPtr& object, const std::string& afterName)
	{
		// �ύX�O�̎�ޖ�
		std::string beforName = object->typeName;

		// ���O��ύX
		object->name = afterName;

		// ��ޖ���ύX
		object->typeName = afterName;

		// �����t���̃I�u�W�F�N�g���z��
		auto& setNumberNameList = objectTypeNameList[object->typeName];

		// �����t���̖��O�Ƃ��ēo�^����Ă���Ȃ炻����폜
		if (setNumberNameList.size() > 0)
		{
			setNumberNameList.erase(object->name);
		}
		// �o�^����Ă��Ȃ��Ȃ��ޖ����폜
		else
		{
			objectTypeNameList.erase(object->typeName);
		}

		// �ύX��̎�ޖ���ǉ�����
		objectTypeNameList.emplace(afterName, SetNumberNameList());
	}

	/// <summary>
	/// <para> ���̃I�u�W�F�N�g�Əd�����Ȃ����O���擾���� </para>
	/// <para> (�d�����Ă����ꍇ�́A���ɐ�����t����) </para>
	/// </summary>
	/// <param name="[in] typeName"> �I�u�W�F�N�g�̎�ޖ� </param>
	/// <returns> 
	/// <para> �d�����Ȃ��悤�ɕύX�������O </para>
	/// <para> (�d�����Ă��Ȃ��Ȃ炻�̂܂܂̖��O) </para>
	/// </returns>
	std::string Scene::GetSingleObjectName(const std::string& typeName) const
	{
		// �V�����I�u�W�F�N�g��
		std::string newObjectName = typeName;

		// �����t���̖��O�̐�
		size_t setNumberNameCount = objectTypeNameList.at(typeName).size();

		// �����t���̖��O�̐���0�Ȃ�
		// (1)��t���ĕԂ�
		if (setNumberNameCount == 0)
		{
			return newObjectName += " (1)";
		}

		/* set�^�͎����Ń\�[�g����̂ŁA
		�Ō�̗v�f���猻�ݎg���Ă���ő�̐������擾�ł��� */

		// �Ō�̐����t���̖��O
		std::string lastName = *objectTypeNameList.at(typeName).rbegin();

		// �I�u�W�F�N�g���̌��ɕt���鐔��
		// �u���ݎg���Ă���ő�̐��� + 1�v�̐�����t����
		int backNumber = GetBackNumber(lastName) + 1;

		// ���O�̌��ɔԍ���t����
		newObjectName += " (" + std::to_string(backNumber) + ")";

		return newObjectName;
	}

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

		// �I�u�W�F�N�g�̎�ޖ��Ǝ��ʔԍ���S�폜
		objectTypeNameList.clear();
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

		objectIDList.erase(object->id);

		// -----------------------------------
		// �I�u�W�F�N�g��ޖ����폜
		// -----------------------------------

		// �����t���̖��O�̔z��
		auto& setNumberNameList = objectTypeNameList[object->typeName];

		// �����t���̖��O�Ƃ��ēo�^����Ă���Ȃ炻����폜
		if (setNumberNameList.size() > 0)
		{
			setNumberNameList.erase(object->name);
		}
		// �o�^����Ă��Ȃ��Ȃ��ޖ����폜
		else
		{
			objectTypeNameList.erase(object->typeName);
		}
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
				Draw(gameObject->staticMesh, prog, gameObject->staticMesh->materials);
			}
			// �ŗL�}�e���A��������
			else
			{
				// �ŗL�}�e���A�����g����
				// �X�^�e�B�b�N���b�V����`��
				Draw(gameObject->staticMesh, prog, gameObject->materials);
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
		DrawGameObject(progStandard, drawObjectList.begin(), transparentBegin);

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
	}

#pragma endregion

#pragma region Save

	/// <summary>
	/// �V�[���̏����t�@�C���ɕۑ�����
	/// </summary>
	void Scene::SaveScene() const
	{
		// �ۑ���̃t�H���_
		std::string folderName = "C:/PokarinEngine/My project/Scenes/" + name;

		// �V�[���ۑ��p�̃t�H���_�����݂��Ȃ��ꍇ�́A�쐬����
		std::filesystem::create_directories(folderName);

		// �Q�[���I�u�W�F�N�g�̏���ۑ�����
		for (const auto& gameObject : gameObjectList)
		{
			gameObject->SaveGameObject(folderName);
		}
	}

	/// <summary>
	/// �V�[���̏����t�@�C������ǂݍ���
	/// </summary>
	void Scene::LoadScene() const
	{
		// �ۑ���̃t�H���_
		std::string folderName = "C:/PokarinEngine/My project/Scenes/" + name;

		// �t�H���_�����݂��Ȃ��ꍇ�͉������Ȃ�
		if (!std::filesystem::exists(folderName))
		{
			return;
		}

		// �Q�[���I�u�W�F�N�g�̏���ۑ�����
		for (const auto& gameObject : gameObjectList)
		{
			gameObject->SaveGameObject(folderName);
		}
	}

#pragma endregion

} // namespace PokarinEngine