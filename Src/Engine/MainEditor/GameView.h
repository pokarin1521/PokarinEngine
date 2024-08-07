/**
* @file GameView.h
*/
#ifndef POKARINENGINE_GAMEVIEW_H_INCLUDED
#define POKARINENGINE_GAMEVIEW_H_INCLUDED

#include "../UsingNames/UsingScene.h"
#include "../UsingNames/UsingFramebufferObject.h"

namespace PokarinEngine
{
	// -------------------
	// �O���錾
	// -------------------

	class Scene;

	/// <summary>
	/// �Q�[���r���[(�Q�[����ʂ�\������r���[)
	/// </summary>
	class GameView
	{
	public: // -------------- �R���X�g���N�^�E�f�X�g���N�^ ----------------

		GameView() = default;
		~GameView() = default;

	public: // -------------------------- �֎~���� -------------------------

		/* �G�f�B�^�Ǘ������������Ȃ�̂ŋ֎~���� */

		// �R�s�[�R���X�g���N�^�̋֎~
		GameView(const GameView&) = delete;

		// ����̋֎~
		GameView& operator=(const GameView&) = delete;

	public: // ---------------------------- �X�V ---------------------------

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="currentScene"> ���݂̃V�[�� </param>
		void Update(const ScenePtr& currentScene);
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_GAMEVIEW_H_INCLUDED
