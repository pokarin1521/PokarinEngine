/**
* @file EventNode.h
*/
#ifndef EVENTNODE_H_INCLUDED
#define EVENTNODE_H_INCLUDED

#include "../Node.h"

namespace PokarinEngine
{
	/// <summary>
	/// �C�x���g����p�m�[�h�̊��N���X
	/// </summary>
	class EventNode : public Node
	{
	public: // ---------------- �R���X�g���N�^�E�f�X�g���N�^ -------------------

		EventNode() = default;
		virtual ~EventNode() = default;

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
			// ���s�s�����쐬����
			runOutputPinID = CreatePin(PinType::Run);

			// �f�[�^�s�����쐬����
			CreateDataPin();
		}

		/// <summary>
		/// �s����\������
		/// </summary>
		void RenderPin() override
		{
			// ���s�s����\������
			BeginRunPin(runOutputPinID, PinAttribute::Output);
			EndPin(PinAttribute::Output);

			// �f�[�^�s����\������
			RenderDataPin();
		}

		/// <summary>
		/// �f�[�^�s�����쐬����
		/// </summary>
		virtual void CreateDataPin() {}

		/// <summary>
		/// �f�[�^�s����\������
		/// </summary>
		virtual void RenderDataPin() {}

	private: // ---------------------------- ���s�s�� ----------------------------

		int runOutputPinID = 0;
	};
}

#endif // !EVENTNODE_H_INCLUDED
