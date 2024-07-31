/**
* @file NodeScript.h
*/
#ifndef POKARINENGINE_NODESCRIPT_H_INCLUDED
#define POKARINENGINE_NODESCRIPT_H_INCLUDED

#include "../UsingNames/UsingNodeEditor.h"

#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// �m�[�h�G�f�B�^���Ǘ�����@�\
	/// </summary>
	namespace NodeScript
	{
		/// <summary>
		/// ������
		/// </summary>
		void Initialize();

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
		/// �m�[�h�G�f�B�^���J��
		/// </summary>
		/// <param name="[in] nodeEditor"> �m�[�h�G�f�B�^ </param>
		void OpenNodeEditor(const NodeEditorPtr& nodeEditor);

		/// <summary>
		/// �m�[�h�G�f�B�^�����
		/// </summary>
		/// <param name="[in] nodeEditor"> �m�[�h�G�f�B�^ </param>
		void CloseNodeEditor(const NodeEditorPtr& nodeEditor);
	} 

} // namespace PokarinEngine

#endif // !POKARINENGINE_NODESCRIPT_H_INCLUDED
