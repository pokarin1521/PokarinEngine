/**
* @file CameraManager.cpp
*/
#include "CameraManager.h"

#include "Components/Camera.h"

#include <unordered_set>
#include <unordered_map>

namespace PokarinEngine
{
	/// <summary>
	/// カメラ管理用
	/// </summary>
	namespace CameraManager
	{
		// -------------------
		// 変数
		// -------------------

		using CameraList = std::unordered_set<const Camera*>;

		// カメラ管理用配列
		// 同じ番号のカメラが複数あることを考慮して、カメラ配列の配列にする
		std::vector<CameraList> cameraList;

		// カメラの要素番号の管理用配列
		std::unordered_map<const Camera*, size_t> cameraIndexList;

		// カメラ管理用配列の大きさ
		const size_t cameraListSize = 5;

		// -------------------
		// 関数
		// -------------------

		/// <summary>
		/// カメラを追加する
		/// </summary>
		/// <param name="[in] camera"> 追加するカメラ </param>
		void AddCamera(const Camera& camera)
		{
			// カメラ管理用配列が空なら
			// 要素数を確保する
			if (cameraList.empty())
			{
				cameraList.resize(cameraListSize);
			}

			// カメラ管理用配列に追加する
			// ややこしいので最初は必ず0番に追加する
			cameraList[0].emplace(&camera);

			// 要素番号配列に追加する
			cameraIndexList.emplace(&camera, 0);
		}

		/// <summary>
		/// カメラを削除する
		/// </summary>
		/// <param name="[in] camera"> 削除するカメラ </param>
		void DestroyCamera(const Camera& camera)
		{
			// 配列にないなら何もしない
			if (cameraIndexList.find(&camera) == cameraIndexList.end())
			{
				return;
			}

			// カメラの要素番号
			size_t index = cameraIndexList[&camera];

			// カメラ管理用配列から削除する
			cameraList[index].erase(&camera);

			// 要素番号を削除する
			cameraIndexList.erase(&camera);
		}

		/// <summary>
		/// カメラを取得する
		/// </summary>
		/// <param name="index"> 取得するカメラの要素番号 </param>
		/// <returns> 指定した要素番号のカメラ </returns>
		const Camera* GetCamera(size_t index)
		{
			// 指定した要素番号が配列の範囲外ならnullptrを返す
			if (index >= cameraListSize)
			{
				return nullptr;
			}

			// 指定した要素番号のカメラがないならnullptrを返す
			if (cameraList[index].empty())
			{
				return nullptr;
			}

			// 先頭にあるカメラを返す
			return *cameraList[index].begin();
		}
	}
}