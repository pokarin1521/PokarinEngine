/**
* @file GameView.h
*/
#ifndef POKARINENGINE_GAMEVIEW_H_INCLUDED
#define POKARINENGINE_GAMEVIEW_H_INCLUDED

#include "RenderView.h"

#include "../Color.h"

namespace PokarinEngine
{
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
		void Update();

	private:

		// �w�i�F
		Color backGround = { 0.3f, 0.3f, 0.3f, 1.0f };
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_GAMEVIEW_H_INCLUDED
