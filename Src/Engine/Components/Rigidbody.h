/**
* @file Rigidbody.h
*/
#ifndef RIGIDBODY_H_INCLUDED
#define RIGIDBODY_H_INCLUDED

#include "Component.h"
#include "../GameObject.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���������������R���|�[�l���g
	/// </summary>
	class Rigidbody : public Component
	{
	public: // --------- �R���X�g���N�^�E�f�X�g���N�^ ----------

		Rigidbody() = default;
		~Rigidbody() = default;

	public: // ------------- �R���|�[�l���g���� ----------------

		/// <summary>
		/// Rigibody�R���|�[�l���g�̍X�V
		/// </summary>
		/// <param name="deltaTime"></param>
		void Update(float deltaTime) override;

	public: // ------------------- ��� ---------------------

		// �d�͉����x
		static constexpr float gravity = 9.81f;

		// �d�͂̉e���𐧌䂷��W��
		float gravityScale = 1;

		// ���x
		Vector3 velocity = { 0, 0, 0 };
	};

} // namespace PokarinEngine

#endif // !RIGIDBODY_H_INCLUDED
