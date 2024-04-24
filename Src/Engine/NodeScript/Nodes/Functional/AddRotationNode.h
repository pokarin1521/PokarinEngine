/**
* @file AddRotation.h
*/
#ifndef ADDROTATION_H_INCLUDED
#define ADDROTATION_H_INCLUDED

#include "FunctionalNode.h"

#include "../../../Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// ��]�p�x�����Z����m�[�h
	/// </summary>
	class AddRotationNode : public FunctionalNode
	{
	public: // ----------------- �R���X�g���N�^�E�f�X�g���N�^ --------------------

		AddRotationNode() = default;
		~AddRotationNode() = default;

	public: // ----------------------------- ���s���� ----------------------------

		/// <summary>
		/// ���s����
		/// </summary>
		void Run() override;

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

		// ��]�p�x�̉��Z�l
		Vector3 addRotation = { 0, 0, 0 };
	};
}

#endif // !ADDROTATION_H_INCLUDED
