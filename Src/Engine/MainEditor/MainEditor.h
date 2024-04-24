/**
* @file MainEditor.h
*/
#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

#include <memory>
#include <vector>

namespace PokarinEngine
{
	// -------------------
	// �O���錾
	// -------------------

	class Engine;
	class SceneView;
	class GameView;

	/// <summary>
	/// ���C���G�f�B�^�Ǘ��p
	/// </summary>
	namespace MainEditor
	{
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="engine"> �G���W���N���X�̎Q�� </param>
		void Initialize(Engine& engine);

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		/// <summary>
		/// �`��
		/// </summary>
		void Render();

		/// <summary>
		/// �I��
		/// </summary>
		void Finalize();

		/// <summary>
		/// �V�[���r���[���擾����
		/// </summary>
		/// <returns> �V�[���r���[ </returns>
		const SceneView& GetSceneView();

		/// <summary>
		/// �Q�[���r���[���擾����
		/// </summary>
		/// <returns> �Q�[���r���[ </returns>
		const GameView& GetGameView();
	}

} // namespace PokarinEngine

#endif // !EDITOR_H_INCLUDED
