/**
* @file EventUpdateNode.h
*/
#ifndef EVENTUPDATENODE_H_INCLUDED
#define EVENTUPDATENODE_H_INCLUDED

#include "EventNode.h"

namespace PokarinEngine
{
	/// <summary>
	/// �X�V�C�x���g�p�m�[�h(�����N���Ă���m�[�h�𖈃t���[�����s����)
	/// </summary>
	class EventUpdateNode : public EventNode
	{
	public: // ----------------- �R���X�g���N�^�E�f�X�g���N�^ --------------------

		EventUpdateNode() = default;
		~EventUpdateNode() = default;

	private: // --------------------------- �m�[�h�\�� ----------------------------

		/// <summary>
		/// �m�[�h��\������
		/// </summary>
		void RenderNode() override
		{

		}
	};
}

#endif // !EVENTUPDATENODE_H_INCLUDED
