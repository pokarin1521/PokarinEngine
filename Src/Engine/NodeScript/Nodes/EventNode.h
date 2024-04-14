/**
* @file EventNode.h
*/
#ifndef EVENTNODE_H_INCLUDED
#define EVENTNODE_H_INCLUDED

#include "Node.h"

namespace PokarinEngine
{
	/// <summary>
	/// �C�x���g����p�m�[�h�̊��N���X
	/// </summary>
	class EventNode : public Node
	{
	public: // ----------------- �R���X�g���N�^�E�f�X�g���N�^ --------------------

		EventNode() = default;
		virtual ~EventNode() = default;

	public: // ---------------------------- ������ -------------------------------

		/// <summary>
		/// ������
		/// </summary>
		void Initialize() override
		{
			linkOutputPin = GetSinglePinID();
		}

	public: // ----------------------------- ���O --------------------------------

		inline static const char* name = "No Title";

	private: // ----------------------------- ���� -------------------------------

		/// <summary>
		/// �m�[�h�ʂ̏���\������
		/// </summary>
		virtual void RenderNode() = 0;

		/// <summary>
		/// �m�[�h����\������
		/// </summary>
		void RenderInfo() override;

	private: // ----------------------------- ��� -------------------------------

		// �����N�p�o�̓s���̎��ʔԍ�
		int linkOutputPin = 0;
	};
}

#endif // !EVENTNODE_H_INCLUDED
