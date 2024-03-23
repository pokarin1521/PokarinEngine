/**
* @file GameView.h
*/
#ifndef GAMEVIEW_H_INCLUDED
#define GAMEVIEW_H_INCLUDED

#include "RenderView.h"

#include "../Color.h"

namespace PokarinEngine
{
	/// <summary>
	/// �Q�[���r���[(�Q�[����ʂ�\������r���[)
	/// </summary>
	class GameView : public RenderView
	{
	public: // --------------- �R���X�g���N�^�E�f�X�g���N�^ ----------------

		GameView() = default;
		~GameView() = default;

	public:

		/// <summary>
		/// �X�V
		/// </summary>
		void Update() override;

	private:

		// �w�i�F
		Color backGround = { 0.3f, 0.3f, 0.3f, 1.0f };
	};

} // namespace PokarinEngine

#endif // !GAMEVIEW_H_INCLUDED
