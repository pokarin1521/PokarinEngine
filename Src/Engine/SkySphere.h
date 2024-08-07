/**
* @file SkySphere.h
*/
#ifndef POKARINENGINE_SKYSPHERE_H_INCLUDED
#define POKARINENGINE_SKYSPHERE_H_INCLUDED

namespace PokarinEngine
{
	/// <summary>
	/// �X�J�C�X�t�B�A�Ǘ��p
	/// </summary>
	namespace SkySphere
	{
		// -----------------------
		// �O���錾
		// -----------------------

		class Camera;

		// -----------------------
		// �֐�
		// -----------------------

		/// <summary>
		/// �X�J�C�X�t�B�A��`�悷��
		/// </summary>
		void Draw(const Camera& camera);
	}
}

#endif // !POKARINENGINE_SKYSPHERE_H_INCLUDED
