/**
* @file Rigidbody.h
*/
#ifndef RIGIDBODY_H_INCLUDED
#define RIGIDBODY_H_INCLUDED

#include "Component.h"

#include "../Math/Vector.h"

#include <memory>

namespace PokarinEngine
{
	// -------------------------
	// �O���錾
	// -------------------------

	class Rigidbody;

	// --------------------------------
	// �^�̕ʖ����`
	// --------------------------------

	using RigidbodyPtr = std::shared_ptr<Rigidbody>;

	/// <summary>
	/// ���̂̕����I�����������R���|�[�l���g
	/// </summary>
	class Rigidbody : public Component
	{
	public: // --------- �R���X�g���N�^�E�f�X�g���N�^ ----------

		Rigidbody() = default;
		~Rigidbody() = default;

	public: // -------------- �R���|�[�l���g���� ---------------

		/// <summary>
		/// �Q�[���Đ����̍X�V
		/// </summary>
		void Update_PlayGame() override;

	public: // --------------------- Json ----------------------

		/// <summary>
		/// �R���|�[�l���g�̏���Json�^�Ɋi�[����
		/// </summary>
		/// <param name="[out] Json"> �����i�[����Json�^ </param>
		void ToJson(Json& data) const override {}

		/// <summary>
		/// �R���|�[�l���g�̏���Json�^����擾����
		/// </summary>
		/// <param name="[out] data"> �����i�[���Ă���Json�^ </param>
		void FromJson(const Json& data) override {}

	public: // -------------------- ��� -----------------------

		// �d�͉����x
		const float gravity = 9.81f;

		// �d�͂̉e���𐧌䂷��W��
		float gravityScale = 1;

		// ���x
		Vector3 velocity = { 0, 0, 0 };

	private: // --------------- �G�f�B�^�p -----------------

		/// <summary>
		/// ����ҏW�ł���悤�ɕ\������
		/// </summary>
		void InfoEditor() override {}
	};

} // namespace PokarinEngine

#endif // !RIGIDBODY_H_INCLUDED
