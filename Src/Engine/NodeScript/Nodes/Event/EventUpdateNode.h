/**
* @file EventUpdateNode.h
*/
#ifndef EVENTUPDATENODE_H_INCLUDED
#define EVENTUPDATENODE_H_INCLUDED

#include "EventNode.h"

namespace PokarinEngine
{
	/// <summary>
	/// �X�V�C�x���g�p�m�[�h
	/// </summary>
	class EventUpdateNode : public EventNode
	{
	public: // ----------------- �R���X�g���N�^�E�f�X�g���N�^ --------------------

		EventUpdateNode() = default;
		~EventUpdateNode() = default;

	public: // ---------------------------- ���s���� -----------------------------

		/// <summary>
		/// ���s����
		/// </summary>
		void Run()
		{
			// ���̃m�[�h�̏��������s
			RunNextNode();
		}
	};
}

#endif // !EVENTUPDATENODE_H_INCLUDED
