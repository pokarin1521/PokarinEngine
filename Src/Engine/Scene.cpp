/**
* @file Scene.cpp
*/
#include "Scene.h"

#include "Engine.h"
#include "Random.h"
#include "Debug.h"

#include "Settings/ShaderSettings.h"

#include <algorithm>

namespace PokarinEngine
{
#pragma region Create

	/// <summary>
	/// �V�[���쐬�p�R���X�g���N�^
	/// </summary>
	/// <param name="e"> �G���W���N���X�̎Q�� </param>
	/// <param name="sceneName"> �V�[���� </param>
	Scene::Scene(Engine& e, const char* sceneName)
		:engine(&e), name(sceneName)
	{
		// ���C���J�����쐬
		mainCamera = CreateGameObject("MainCamera");
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g���쐬����
	/// </summary>
	/// <typeparam name="T"> �Q�[���I�u�W�F�N�g�N���X�܂��͂��̔h�� </typeparam>
	/// <param name="name"> �I�u�W�F�N�g�̖��O </param>
	/// <param name="position"> �I�u�W�F�N�g��z�u������W </param>
	/// <param name="rotation"> �I�u�W�F�N�g�̉�]�p�x </param>
	/// <param name="staticMeshFile"> �X�^�e�B�b�N���b�V���̃t�@�C���� </param>
	/// <returns> �ǉ������Q�[���I�u�W�F�N�g�̃|�C���^ </returns>
	GameObjectPtr Scene::CreateGameObject(const std::string& name,
		const Vec3& position, const Vec3& rotation, const char* staticMeshFile)
	{
		// �V�[�����̃Q�[���I�u�W�F�N�g�����ő�l�ɒB������쐬���Ȃ�
		if (gameObjectList.size() >= gameObjectMax)
		{
			LOG_WARNING("�Q�[���I�u�W�F�N�g�����ő�l�ɒB�������߁A�Q�[���I�u�W�F�N�g���쐬�o���܂���");
			return nullptr;
		}

		// �Q�[���I�u�W�F�N�g�쐬
		GameObjectPtr object = std::make_shared<GameObject>();

		// �G���W����ݒ�
		object->engine = engine;

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
		object->Initialize();

		// �ʒu�Ɗp�x��ݒ�
		object->transform->position = position;
		object->transform->rotation = rotation;

		// �X�^�e�B�b�N���b�V��
		object->staticMesh = engine->GetStaticMesh(staticMeshFile);

		// ���݂̃V�[���ɓo�^
		AddGameObject(object);

		return object;
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�𕡐�����
	/// </summary>
	/// <param name="object"> �������̃Q�[���I�u�W�F�N�g </param>
	/// <returns> ���������Q�[���I�u�W�F�N�g </returns>
	GameObjectPtr Scene::CopyGameObject(GameObjectPtr object)
	{
		// �������Ɠ����p�����[�^�̃I�u�W�F�N�g���쐬
		// ���O�̏d���m�F���o����悤�Ɏ�ޖ����g��
		GameObjectPtr copyObject = CreateGameObject(object->typeName,
			object->transform->position, object->transform->rotation,
			object->staticMesh->filename.c_str());

		// ���������Q�[���I�u�W�F�N�g��Ԃ�
		return copyObject;
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g��ǉ�����
	/// </summary>
	/// <param name="object"> �ǉ�����Q�[���I�u�W�F�N�g </param>
	void Scene::AddGameObject(GameObjectPtr object)
	{
		// �Q�[���I�u�W�F�N�g��ǉ�
		gameObjectList.push_back(object);

		// �I�u�W�F�N�g���ʔԍ���ǉ�
		objectIDList.emplace(object->id);
	}

#pragma endregion

#pragma region Name,ID

	/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
	/// <summary>
	/// �I�u�W�F�N�g���̌��ɕt���Ă鐔�����擾����
	/// </summary>
	/// <param name="objectName"> �I�u�W�F�N�g�� </param>
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
	/// <param name="object"> ���O��ύX����I�u�W�F�N�g </param>
	/// <param name="afterName"> �ύX��̖��O </param>
	void Scene::ChangeObjectName(GameObjectPtr object, const std::string& afterName)
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
	/// �Q�[���I�u�W�F�N�g�̖��O���d�����Ȃ��悤�ɕύX����
	/// </summary>
	/// <param name="objectName"> �I�u�W�F�N�g�� </param>
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
	/// <para> ���̃I�u�W�F�N�g�Əd�����Ȃ����ʔԍ����擾���� </para>
	/// </summary>
	/// <returns> 
	/// <para> �d�����Ȃ����ʔԍ�(����) </para>
	/// </returns>
	int Scene::GetSingleObjectID() const
	{
		// �I�u�W�F�N�g�����ő�l�܂ŒB������ID�𐶐����Ȃ�
		if (objectIDList.size() >= INT_MAX)
		{
			LOG_WARNING("�I�u�W�F�N�g�����ő�l�ɒB�������߁AID�𐶐��o���܂���");
			return 0;
		}

		// �I�u�W�F�N�g���ʔԍ�
		// ����������ƃq�G�����L�[�̏����ō���̂�0�ȏ�̗���
		int objectID = Random::Range(0, INT_MAX);

		// ���̃I�u�W�F�N�g�Ɣԍ����d�����Ă���Ȃ痐�����擾������
		while (objectIDList.find(objectID) != objectIDList.end())
		{
			objectID = Random::Range(0, INT_MAX);
		}

		return objectID;
	}

#pragma endregion

#pragma region Update

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̏�Ԃ��X�V����
	/// </summary>
	/// <param name="deltaTime"> �O��̍X�V����̌o�ߎ���(�b) </param>
	void Scene::UpdateGameObject(float deltaTime)
	{
		for (const auto& gameObject : gameObjectList)
		{
			if (!gameObject->IsDestroyed())
			{
				gameObject->Update(deltaTime);
			}
		}

		// ----------------------------------
		// ���[�J�����W�ϊ��s����v�Z
		// ----------------------------------

		for (const auto& gameObject : gameObjectList)
		{
			// �I�u�W�F�N�g�̍��W�E��]�p�x�E�g�嗦
			TransformPtr transform = gameObject->transform;

			// �I�u�W�F�N�g�ɐݒ肷����W�ϊ��s��
			Mat4 transformMatrix = Mat4_Function::GetTransformMatrix(
				transform->scale, transform->rotation, transform->position);

			// �I�u�W�F�N�g�ɐݒ肷��@���ϊ��s��
			Mat3 normalMatrix = Mat3_Function::GetRotationMatrix(transform->rotation);

			// ���W�ϊ��s���ݒ�
			transform->SetTransformMatrix(transformMatrix);

			// �@���ϊ��s���ݒ�
			transform->SetNormalMatrix(normalMatrix);
		}

		// ------------------------------------
		// ���[���h���W�ϊ��s����v�Z
		// ------------------------------------

		// ���[���h���W�ϊ��s��
		std::vector<Mat4> worldTransforms(gameObjectList.size());

		// ���[���h�@���ϊ��s��
		std::vector<Mat3> worldNormals(gameObjectList.size());

		// �S�Ă̐e�̍��W�ϊ��s����|�����킹��
		for (size_t i = 0; i < gameObjectList.size(); ++i)
		{
			// �I�u�W�F�N�g�̍��W�E��]�p�x�E�g�嗦
			Transform* transform = gameObjectList[i]->transform.get();

			// ���g�̍��W�ϊ��s��
			Mat4 transformMatrix = transform->GetTransformMatrix();

			// ���g�̖@���ϊ��s��
			Mat3 normalMatrix = transform->GetNormalMatrix();

			// �e�����ǂ��Ă���
			for (Transform* parent = transform->GetParent(); parent; parent = parent->GetParent())
			{
				// �e�̍��W�ϊ��s��
				Mat4 parentTransformMatrix = parent->GetTransformMatrix();

				// �e�̖@���ϊ��s��
				Mat3 parentNormalMatrix = parent->GetNormalMatrix();

				// �e�̕ϊ��s����|�����킹��
				transformMatrix = parentTransformMatrix * transformMatrix;
				normalMatrix = parentNormalMatrix * normalMatrix;
			}

			// �v�Z�������[���h���W�ϊ��s�����
			worldTransforms[i] = transformMatrix;
			worldNormals[i] = normalMatrix;
		}

		// -----------------------------------
		// ���[���h���W�ϊ��s���
		// �Q�[���I�u�W�F�N�g�ɐݒ�
		// -----------------------------------

		for (size_t i = 0; i < gameObjectList.size(); ++i)
		{
			gameObjectList[i]->transform->SetTransformMatrix(worldTransforms[i]);

			gameObjectList[i]->transform->SetNormalMatrix(worldNormals[i]);
		}

	} // UpdateGameObject

#pragma endregion

#pragma region Destroy

/// <summary>
/// �V�[������S�ẴQ�[���I�u�W�F�N�g��j������
/// </summary>
	void Scene::ClearGameObjectAll()
	{
		// �Q�[���I�u�W�F�N�g�̍폜�C�x���g�����s
		for (auto& gameObject : gameObjectList)
		{
			gameObject->OnDestroy();
		}

		gameObjectList.clear();
		objectTypeNameList.clear();
		objectIDList.clear();
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g���폜����
	/// </summary>
	/// <param name="object"> �폜����Q�[���I�u�W�F�N�g </param>
	void Scene::DestroyObject(GameObjectPtr object)
	{
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
	/// <param name="prog"> �V�F�[�_�v���O�����̊Ǘ��ԍ� </param>
	/// <param name="gameObject"> �p�����[�^���R�s�[����Q�[���I�u�W�F�N�g </param>
	void CopyGameObjectParameters(
		const Engine& engine, GLuint prog, const GameObject& gameObject)
	{
		// ���W�ϊ��s��
		Mat4 transformMatrix = gameObject.transform->GetTransformMatrix();

		// �@���ϊ��s��
		Mat3 normalMatrix = gameObject.transform->GetNormalMatrix();

		// ------------------------------------
		// �p�����[�^��GPU�ɃR�s�[
		// ------------------------------------

		// �I�u�W�F�N�g�̐F
		glProgramUniform4fv(prog,
			LocationNum::color, 1, &gameObject.color.r);

		// ���W�ϊ��s��
		glProgramUniformMatrix4fv(
			prog, 0, 1, GL_FALSE, &transformMatrix[0].x);

		// �@���ϊ��s��
		if (prog == engine.GetShaderProgram(Shader::ProgType::Standard))
		{
			glProgramUniformMatrix3fv(
				prog, 1, 1, GL_FALSE, &normalMatrix[0].x);
		}
	}

	/// �����ł����g��Ȃ��̂ŁAcpp�݂̂ɏ���
	/// <summary>
	/// �Q�[���I�u�W�F�N�g��`�悷��
	/// </summary>
	/// <param name="prog"> �V�F�[�_�v���O�����̊Ǘ��ԍ� </param>
	/// <param name="begin"> �`�悷��Q�[���I�u�W�F�N�g�z��̐擪�C�e���[�^ </param>
	/// <param name="end"> �`�悷��Q�[���I�u�W�F�N�g�z��̖����C�e���[�^ </param>
	void DrawGameObject(const Engine& engine, GLuint prog,
		GameObjectList::iterator begin, GameObjectList::iterator end)
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
			CopyGameObjectParameters(engine, prog, *gameObject);

			// ------------- �}�`��`�� --------------

			// �ŗL�}�e���A�����Ȃ�
			if (gameObject->materials.empty())
			{
				// ���L�}�e���A�����g����
				// �X�^�e�B�b�N���b�V����`��
				Draw(*gameObject->staticMesh, prog, gameObject->staticMesh->materials);
			}
			// �ŗL�}�e���A��������
			else
			{
				// �ŗL�}�e���A�����g����
				// �X�^�e�B�b�N���b�V����`��
				Draw(*gameObject->staticMesh, prog, gameObject->materials);
			}
		}
	}

	/// <summary>
	/// �V�[�����̑S�ẴQ�[���I�u�W�F�N�g��`�悷��
	/// </summary>
	/// <param name="shaderProgList"> �V�F�[�_�v���O�����z�� </param>
	void Scene::DrawGameObjectAll(const Shader::ProgList& shaderProgList)
	{
		// ------------------------------
		// �`��D��x�ŕ��בւ�
		// ------------------------------

		// �`�悷��Q�[���I�u�W�F�N�g�z��
		// �{�̂̏��Ԃ��ς��ƍ���̂ŃR�s�[
		GameObjectList drawObjectList = gameObjectList;

		// �Q�[���I�u�W�F�N�g��`�揇�ɕ��בւ���
		// (�����_�[�L���[�����������̂��珇�ɕ��ׂ�)
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
		GLuint progStandard = shaderProgList.at(Shader::ProgType::Standard);

		// ���C�e�B���O�����V�F�[�_�v���O�����̊Ǘ��ԍ�
		GLuint progUnlit = shaderProgList.at(Shader::ProgType::Unlit);

		// ---------- transparent�ȑO�̃L���[��`��  -----------
		// ---------- �ʏ�̃I�u�W�F�N�g		     -----------

		// �e�X�g�p�Ƀ��C�e�B���O�����̃V�F�[�_���g��
		// �{���́uprogStandard�v���g��
		// �`��
		DrawGameObject(*engine, progUnlit, drawObjectList.begin(), transparentBegin);

		// ------ transparent����overlay�܂ł̃L���[��`�� ------
		// ------ �������ȃI�u�W�F�N�g					   ------

		// �[�x�o�b�t�@�ւ̏������݂��֎~
		glDepthMask(GL_FALSE);

		// �e�X�g�p�Ƀ��C�e�B���O�����̃V�F�[�_���g��
		// �{���́uprogStandard�v���g��
		// �`��
		DrawGameObject(*engine, progUnlit, transparentBegin, overlayBegin);

		// �[�x�o�b�t�@�ւ̏������݂�����
		glDepthMask(GL_TRUE);

		// ---------------- overlay�ȍ~�̃L���[��`�� ----------------
		// ---------------- UI,�S��ʃG�t�F�N�g�Ȃ�   ----------------

		// overlay�ȍ~��
		// UI�n�̐[�x�l�֌W�Ȃ��`�悵�ė~�����I�u�W�F�N�g�Ȃ̂ŁA
		// �[�x�e�X�g�𖳌�������
		glDisable(GL_DEPTH_TEST);

		// �`��
		DrawGameObject(*engine, progUnlit, overlayBegin, drawObjectList.end());
	}

#pragma endregion

} // namespace PokarinEngine