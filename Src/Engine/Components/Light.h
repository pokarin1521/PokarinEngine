/**
* @file Light.h
*/
#ifndef LIGTH_H_INCLUDED
#define LIGTH_H_INCLUDED

#include "Component.h"
#include "../VecMath.h"

namespace PokarinEngine
{
	/**
	* ���C�g�R���|�[�l���g
	*/
	class Light : public Component
	{
	public: // -------- �R���X�g���N�^�E�f�X�g���N�^ ---------

		Light() = default;

		/// <summary>
		/// ���C�g���������f�X�g���N�^
		/// </summary>
		virtual ~Light();

	public: // ------------ �R���|�[�l���g���� --------------

		/// <summary>
		/// �R���|�[�l���g��������
		/// �Q�[���I�u�W�F�N�g�ɒǉ������Ƃ��Ɏ��s
		/// </summary>
		virtual void Awake() override;

		/// <summary>
		/// �R���|�[�l���g���X�V
		/// </summary>
		/// <param name="deltaTime"> �O��̍X�V����̌o�ߎ���(�b) </param>
		virtual void Update(float deltaTime) override;

	public: // ----------- ���C�g�̎�� -------------

		// ���C�g�̎��
		enum class Type
		{
			pointLight, // �_����
			spotLight,  // �X�|�b�g����
		};

		// ���C�g�̎��
		// ���� : �_����
		Type type = Type::pointLight;

	public: // ----------- ���C�g�̏�� -------------

		// �F
		Vec3 color = { 1, 1, 1 };

		// ���邳
		float intensity = 1;

		// ���C�g���͂��ő唼�a
		float radius = 1;

	public: // ---------- �X�|�b�g���C�g�p -----------

		// �X�|�b�g���C�g���Ƃ炷�p�x
		float coneAngle = Radians(30);

		// �X�|�b�g���C�g�̌����J�n�p�x
		float falloffAngle = Radians(20);

	private: // �Ǘ��p

		// ���C�g�C���f�b�N�X
		// -1 : ����
		int lightIndex = -1;
	};

} // namespace PokarinEngine

#endif // !LIGTH_H_INCLUDED
