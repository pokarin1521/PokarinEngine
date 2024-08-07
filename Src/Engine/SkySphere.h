/**
* @file SkySphere.h
*/
#ifndef POKARINENGINE_SKYSPHERE_H_INCLUDED
#define POKARINENGINE_SKYSPHERE_H_INCLUDED

namespace PokarinEngine
{
	/// <summary>
	/// スカイスフィア管理用
	/// </summary>
	namespace SkySphere
	{
		// -----------------------
		// 前方宣言
		// -----------------------

		class Camera;

		// -----------------------
		// 関数
		// -----------------------

		/// <summary>
		/// スカイスフィアを描画する
		/// </summary>
		void Draw(const Camera& camera);
	}
}

#endif // !POKARINENGINE_SKYSPHERE_H_INCLUDED
