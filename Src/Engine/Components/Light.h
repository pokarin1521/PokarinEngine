/**
* @file Light.h
*/
#ifndef LIGTH_H_INCLUDED
#define LIGTH_H_INCLUDED

#include "Component.h"

#include "../LightParameter.h"
#include "../Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���C�g�R���|�[�l���g
	/// </summary>
	class Light : public Component
	{
	public: // -------- �R���X�g���N�^�E�f�X�g���N�^ ---------

		Light() = default;

		/// <summary>
		/// ���C�g���������f�X�g���N�^
		/// </summary>
		~Light();

	public: // ------------- �R���|�[�l���g���� -------------

		/// <summary>
		/// <para> �R���|�[�l���g�������� </para> 
		/// <para> �Q�[���I�u�W�F�N�g�ɒǉ������Ƃ��Ɏ��s </para>
		/// </summary>
		void Awake() override;

		/// <summary>
		/// �R���|�[�l���g���X�V
		/// </summary>
		void Update() override;

		/// <summary>
		/// ����ҏW�ł���悤�ɕ\������
		/// </summary>
		void InfoEditor() override;

	public: // ----------------- ��ނ̐ݒ� ----------------

		/// <summary>
		/// ���C�g�̎�ނ�ݒ肷��
		/// </summary>
		/// <param name="type"> ���C�g�̎�� </param>
		void SetType(LightParameter::Type type)
		{
			lightData.type = type;
		}

	public: // ----------------- ���O�̎擾 ----------------

		/// <summary>
		/// �R���|�[�l���g�̖��O���擾����
		/// </summary>
		/// <returns> �R���|�[�l���g�̖��O </returns>
		const std::string& GetName() override
		{
			return name;
		}

	public: // -------------------- ���O -------------------

		// �R���|�[�l���g�̖��O
		inline static const std::string name = "Light";

	private: // ------------------- ��� -------------------

		// ���C�g�f�[�^
		LightParameter::LightData lightData;

	private: // ------------------ �Ǘ��p ------------------

		// ���C�g���擾���ĂȂ����̔ԍ�
		const int invaild = -1;

		// ���C�g�̗v�f�ԍ�
		int lightIndex = invaild;
	};

} // namespace PokarinEngine

#endif // !LIGTH_H_INCLUDED
