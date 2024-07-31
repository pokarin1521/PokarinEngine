/**
* @file EventUpdateNode.h
*/
#ifndef POKARINENGINE_EVENTUPDATENODE_H_INCLUDED
#define POKARINENGINE_EVENTUPDATENODE_H_INCLUDED

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

	private: // ---------------------------- ���s���� -----------------------------

		/// <summary>
		/// �m�[�h�ʂ̎��s����
		/// </summary>
		/// <returns>
		/// <para> true : ���̃m�[�h�����s���� </para>
		/// <para> false : ���̃m�[�h�����s���Ȃ� </para>
		/// </returns>
		bool RunNode() override
		{
			// ���̃m�[�h�����s����
			return true;
		}
	};
}

#endif // !POKARINENGINE_EVENTUPDATENODE_H_INCLUDED
