/**
* @file AddWorldPositionNode.h
*/
#ifndef ADDWORLDPOSITIONNODE_H_INCLUDED
#define ADDWORLDPOSITIONNODE_H_INCLUDED

#include "FunctionalNode.h"

#include "../../../Math/Vector.h"

namespace PokarinEngine
{
	/// <summary>
	/// ���W�����Z����m�[�h
	/// </summary>
	class AddWorldPositionNode : public FunctionalNode
	{
	public: // ----------------- �R���X�g���N�^�E�f�X�g���N�^ --------------------

		AddWorldPositionNode() = default;
		~AddWorldPositionNode() = default;

	private: // ---------------------------- ���s���� ----------------------------

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
			inputPositionPin = CreatePin(PinType::Data);
		}

		/// <summary>
		/// �f�[�^�s����\������
		/// </summary>
		void RenderDataPin() override;

	private: // ------------------------------ ��� -----------------------------

		// ��]�p�x���͗p�f�[�^�s���̎��ʔԍ�
		int inputPositionPin = 0;

		// �ړ���
		Vector3 translate = { 0, 0, 0 };
	};
}

#endif // !ADDWORLDPOSITIONNODE_H_INCLUDED
