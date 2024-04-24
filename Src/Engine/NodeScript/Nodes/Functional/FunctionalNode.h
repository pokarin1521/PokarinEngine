/**
* @file FunctionalNode.h
*/
#ifndef FUNCTIONALNODE_H_INCLUDED
#define FUNCTIONALNODE_H_INCLUDED

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

	public: // --------------------------- ���s����  ---------------------------

		/// <summary>
		/// �m�[�h�̎��s����
		/// </summary>
		virtual void Run() = 0;

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

#endif // !FUNCTIONALNODE_H_INCLUDED
