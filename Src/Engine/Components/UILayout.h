/**
* @file UILayout.h
*/
#ifndef UILAYOUT_H_INCLUDED
#define UILAYOUT_H_INCLUDED

#include "Component.h"
#include "../Math/Vector.h"

namespace PokarinEngine
{
	/**
	* UI����ʋ�ԂŔz�u���邽�߂̃R���|�[�l���g
	*/
	class UILayout : public Component
	{
	public: // �R���X�g���N�^�E�f�X�g���N�^

		UILayout() = default;
		~UILayout() = default;

	public: // �R���|�[�l���g����

		/// <summary>
		/// UI���C�A�E�g�̎��s���J�n���� 
		/// </summary>
		void Start() override;

		/// <summary>
		/// �X�V�C�x���g
		/// </summary>
		/// <param name="deltaTime"> 
		/// �O��̍X�V����̌o�ߎ���(�b) 
		/// </param>
		void Update(float deltaTime) override;

	public: // �C�x���g�p�ϐ�

		// �J�������猩��UI�̍��W
		Vector3 basePosition = { 0, 0, 0 };
	};

} // namespace PokarinEngine

#endif // !UILAYOUT_H_INCLUDED
