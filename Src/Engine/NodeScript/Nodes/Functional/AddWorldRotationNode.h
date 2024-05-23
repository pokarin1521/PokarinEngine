/**
* @file AddWorldRotationNode.h
*/
#ifndef ADDWORLDROTATION_H_INCLUDED
#define ADDWORLDROTATION_H_INCLUDED

#include "FunctionalNode.h"

#include "../../../Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// ��]�p�x�����Z����m�[�h
	/// </summary>
	class AddWorldRotationNode : public FunctionalNode
	{
	public: // ----------------- �R���X�g���N�^�E�f�X�g���N�^ --------------------

		AddWorldRotationNode() = default;
		~AddWorldRotationNode() = default;

	private: // ----------------------------- ���s���� ----------------------------

		/// <summary>
		/// �m�[�h�ʂ̎��s����
		/// </summary>
		/// <returns>
		/// <para> true : ���̃m�[�h�����s���� </para>
		/// <para> false : ���̃m�[�h�����s���Ȃ� </para>
		/// </returns>
		bool RunNode() override;

	private: // --------------------------- �f�[�^�s�� ---------------------------

		/// <summary>
		/// �f�[�^�s�����쐬����
		/// </summary>
		void CreateDataPin() override
		{
			inputRotationPin = CreatePin(PinType::Data);
		}

		/// <summary>
		/// �f�[�^�s����\������
		/// </summary>
		void RenderDataPin() override;

	private: // ------------------------------ ��� -----------------------------

		// ��]�p�x���͗p�f�[�^�s���̎��ʔԍ�
		int inputRotationPin = 0;

		// ��]��(�x���@)
		Vector3 rotationDeg = { 0, 0, 0 };
	};
}

#endif // !ADDWORLDROTATION_H_INCLUDED
