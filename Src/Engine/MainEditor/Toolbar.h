/**
* @file Toolbar.h
*/
#ifndef POKARINENGINE_TOOLBAR_H_INCLUDED
#define POKARINENGINE_TOOLBAR_H_INCLUDED

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
		void Initialize();

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="[out] isPlayGame"> �Q�[�����Đ����Ȃ�true </param>
		void Update(bool& isPlayGame);
	}
}

#endif // !POKARINENGINE_TOOLBAR_H_INCLUDED
