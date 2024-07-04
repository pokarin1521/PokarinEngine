/**
* @file Toolbar.h
*/
#ifndef TOOLBAR_H_INCLUDED
#define TOOLBAR_H_INCLUDED

namespace PokarinEngine
{
	// ----------------------
	// �O���錾
	// ----------------------

	class Engine;

	/// <summary>
	/// �c�[���o�[
	/// </summary>
	namespace Toolbar
	{
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="[in] engine"> �G���W���N���X </param>
		void Initialize(Engine& engine);

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="[out] isPlayGame"> �Q�[�����Đ����Ȃ�true </param>
		void Update(bool& isPlayGame);
	}
}

#endif // !TOOLBAR_H_INCLUDED
