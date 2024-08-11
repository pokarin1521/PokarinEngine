/**
* @file CameraManager.h
*/
#ifndef POKARINENGINE_CAMERAMANAGER_H_INCLUDED
#define POKARINENGINE_CAMERAMANAGER_H_INCLUDED

namespace PokarinEngine
{
	// ----------------------
	// 前方宣言
	// ----------------------

	class Camera;

	/// <summary>
	/// カメラ管理用
	/// </summary>
	namespace CameraManager
	{
		/// <summary>
		/// カメラを追加する
		/// </summary>
		/// <param name="[in] camera"> 追加するカメラ </param>
		void AddCamera(const Camera& camera);
	
		/// <summary>
		/// カメラを削除する
		/// </summary>
		/// <param name="[in] camera"> 削除するカメラ </param>
		void DestroyCamera(const Camera& camera);

		/// <summary>
		/// カメラを取得する
		/// </summary>
		/// <param name="index"> 取得するカメラの要素番号 </param>
		/// <returns> 指定した要素番号のカメラ </returns>
		const Camera* GetCamera(size_t index);
	}
}

#endif // !POKARINENGINE_CAMERAMANAGER_H_INCLUDED
