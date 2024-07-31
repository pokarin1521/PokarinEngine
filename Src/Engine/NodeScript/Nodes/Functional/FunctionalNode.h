/**
* @file FunctionalNode.h
*/
#ifndef POKARINENGINE_FUNCTIONALNODE_H_INCLUDED
#define POKARINENGINE_FUNCTIONALNODE_H_INCLUDED

#include "../Node.h"

namespace PokarinEngine
{
	/// <summary>
	/// �@�\�p�m�[�h�̊��N���X
	/// </summary>
	class FunctionalNode : public Node
	{
	public: // ---------------- �R���X�g���N�^�E�f�X�g���N�^ -------------------

		FunctionalNode() = default;
		virtual ~FunctionalNode() = default;

	private: // --------------------------- ���s���� ---------------------------

		/// <summary>
		/// �m�[�h�ʂ̎��s����
		/// </summary>
		/// <returns>
		/// <para> true : ���̃m�[�h�����s���� </para>
		/// <para> false : ���̃m�[�h�����s���Ȃ� </para>
		/// </returns>
		bool RunNode() override = 0;

	private: // --------------------------- �s������ ---------------------------

		/// <summary>
		/// ������
		/// </summary>
		void Initialize() override
		{
			// --------------------------------
			// ���s�s�����쐬����
			// --------------------------------

			// ���͗p
			runInputPinID = CreatePin(PinType::Run);

			// �o�͗p
			runOutputPinID = CreatePin(PinType::Run);

			// --------------------------------
			// �f�[�^�s�����쐬����
			// --------------------------------

			CreateDataPin();
		}

		/// <summary>
		/// �s����\������
		/// </summary>
		void RenderPin() override
		{
			// ------------------------------
			// ���s�s����\������
			// ------------------------------

			// ���͗p
			BeginRunPin(runInputPinID, PinAttribute::Input);
			EndPin(PinAttribute::Input);

			// �o�͗p
			BeginRunPin(runOutputPinID, PinAttribute::Output);
			EndPin(PinAttribute::Output);

			// -----------------------------
			// �f�[�^�s����\������
			// -----------------------------

			RenderDataPin();
		}

		/// <summary>
		/// �f�[�^�s�����쐬����
		/// </summary>
		virtual void CreateDataPin() = 0;

		/// <summary>
		/// �f�[�^�s����\������
		/// </summary>
		virtual void RenderDataPin() = 0;

	private: // ---------------------------- ���s�s�� ----------------------------

		int runInputPinID = 0;
		int runOutputPinID = 0;
	};
}

#endif // !POKARINENGINE_FUNCTIONALNODE_H_INCLUDED
