/**
* @file GameView.h
*/
#ifndef POKARINENGINE_GAMEVIEW_H_INCLUDED
#define POKARINENGINE_GAMEVIEW_H_INCLUDED

#include "../Color.h"

#include "../UsingNames/UsingScene.h"

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
	public: // --------------- �R���X�g���N�^�E�f�X�g���N�^ ----------------

		GameView() = default;
		~GameView() = default;

	public:

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="currentScene"> ���݂̃V�[�� </param>
		void Update(const ScenePtr& currentScene);

	private:

		// �w�i�F
		Color backGround = { 0.3f, 0.3f, 0.3f, 1.0f };
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_GAMEVIEW_H_INCLUDED
