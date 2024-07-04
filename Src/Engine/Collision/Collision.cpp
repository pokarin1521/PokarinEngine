/**
* @file Collision.cpp
*/
#include "Collision.h"

#include "Intersect.h"

#include "../GameObject.h"

#include "../Components/Colliders/AabbCollider.h"
#include "../Components/Colliders/SphereCollider.h"

#include "../Math/Vector.h"
#include "../Math/Angle.h"

namespace PokarinEngine
{
	/// <summary>
	/// 衝突用
	/// </summary>
	namespace Collision
	{
		// ----------------------------------
		// 構造体
		// ----------------------------------

		/// <summary>
		/// ワールド座標系のコライダー
		/// </summary>
		struct WorldCollider
		{
			/// <summary>
			/// ワールドコライダーの座標を変更する
			/// </summary>
			/// <param name="[in] v"> 移動量 </param>
			void AddPosition(const Vector3& v)
			{
				// コライダーの持ち主の座標を変更
				origin->GetOwnerObject().transform->position += v;

				// コライダーの座標を変更
				world->AddPosition(v);
			}

			// オリジナルのコライダー
			ColliderPtr origin;

			// コライダー
			ColliderPtr world;
		};

		// ----------------------------------
		// 型の別名を定義
		// ----------------------------------

		using WorldColliderList = std::vector<WorldCollider>;

		// ----------------------------------
		// 関数
		// ----------------------------------

		/// ここでしか使わないのでcppのみに書く
		/// <summary>
		/// 型によって交差判定関数を呼び分けるための関数テンプレート
		/// </summary>
		/// <typeparam name="T"> 判定対象の型(衝突する側) </typeparam>
		/// <typeparam name="U"> 判定対象の型(衝突される側) </typeparam>
		/// <param name="[in] a"> 判定対象のコライダー() </param>
		/// <param name="[in] b"> 判定対象のコライダー2 </param>
		/// <param name="[out] penetration"> 貫通ベクトル </param>
		/// <returns>
		/// <para> true : 交差した </para>
		/// <para> false : 交差していない </para>
		/// </returns>
		template<class T, class U>
		bool CallIntersect(
			const ColliderPtr& a, const ColliderPtr& b, Vector3& penetration)
		{
			return Intersect(static_cast<T&>(*a).GetShape(),
				static_cast<U&>(*b).GetShape(), penetration);
		}

		/// ここでしか使わないのでcppのみに書く
		/// <summary>
		/// <para> 型によって交差判定関数を呼び分けるための関数テンプレート </para>
		/// <para> 交差判定関数に渡す引数を逆数にするバージョン </para>
		/// </summary>
		/// <typeparam name="T"> 判定対象の型(衝突される側) </typeparam>
		/// <typeparam name="U"> 判定対象の型(衝突する側) </typeparam>
		/// <param name="[in] a"> 判定対象のコライダー(衝突される側) </param>
		/// <param name="[in] b"> 判定対象のコライダー(衝突する側) </param>
		/// <param name="[out] penetration"> 貫通ベクトル </param>
		/// <returns>
		/// <para> true  : 交差した </para>
		/// <para> false : 交差していない </para>
		/// </returns>
		template<class T, class U>
		bool CallIntersectReverse(
			const ColliderPtr& a, const ColliderPtr& b, Vector3& penetration)
		{
			if (Intersect(static_cast<U&>(*b).GetShape(),
				static_cast<T&>(*a).GetShape(), penetration))
			{
				// 衝突する側と衝突される側が逆になるので
				// 貫通ベクトルを逆向きにする
				penetration *= -1;

				return true;
			}

			return false;
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// 貫通ベクトルをゲームオブジェクトに反映する
		/// </summary>
		/// <param name="[in] penetration"> 貫通ベクトル </param>
		/// <param name="[out] worldColliderList"> ワールド座標系のコライダー配列 </param>
		/// <param name="[out] gameObject"> ゲームオブジェクト </param>
		void ApplyPenetration(const Vector3& penetration, 
			WorldColliderList& worldColliderList, GameObject& gameObject)
		{
			// ---------------------------------------------------------
			// 接地判定
			// 貫通ベクトルが垂直に近い場合に、床に触れたとみなす
			// ---------------------------------------------------------

			/* 「貫通ベクトル」と「垂直ベクトル」の角度で判定する

			内積を使って角度を求める
			貫通ベクトル(px, py, pz)と垂直ベクトル(0, 1, 0)で計算すると
			「なす角 = 貫通ベクトルのY要素」ということが分かる

			そして、貫通ベクトルは衝突面に対して垂直に作られる
			(それが最短距離だから)
			そのため、「貫通ベクトルがワールド座標系の垂直に近い」
			ならば「衝突面はワールド座標系の水平に近い」と言える */

			// 床とみなす角度
			static const float cosGround = cos(Radians(30));

			// 貫通ベクトルが下方向を向いている
			// (下方向でない場合は、計算するだけ無駄なので計算前に判定する)
			if (penetration.y > 0)
			{
				// 貫通距離
				const float distance = penetration.Length();

				// 貫通ベクトルの向きが30度以下なら床と判断
				// acosを避けるためcosで比較しているので、
				// 角度で比較する時と不等号の向きが逆
				if (penetration.y >= distance * cosGround)
				{
					// 接地した
					gameObject.isGrounded = true;
				}
			}

			// ----------------------------------------
			// ゲームオブジェクトとコライダーに
			// 衝突による貫通分の移動を反映
			// ----------------------------------------

			// ゲームオブジェクトを移動
			gameObject.transform->position += penetration;

			// 全てのワールドコライダーを移動
			for (auto& collider : worldColliderList)
			{
				collider.world->AddPosition(penetration);
			}
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// コライダー単位の衝突判定
		/// </summary>
		/// <param name="[in,out] colliderListA"> 判定対象のワールドコライダー配列A </param>
		/// <param name="[in,out] colliderListB"> 判定対象のワールドコライダー配列B </param>
		void WorldColliderCollision(
			WorldColliderList& colliderListA, WorldColliderList& colliderListB)
		{
			// 関数ポインタ型を定義
			using FuncType = bool(*)(
				const ColliderPtr&, const ColliderPtr&, Vector3&);

			// 組み合わせに対応する交差判定関数を選ぶための配列
			static const FuncType funcList[2][2] = {
				{
					CallIntersect<AabbCollider, AabbCollider>,
					CallIntersect<AabbCollider, SphereCollider>,
				},
				{
					CallIntersectReverse<SphereCollider, AabbCollider>,
					CallIntersect<SphereCollider, SphereCollider>,
				},
			};

			// ----------------------------------
			// コライダー単位の衝突判定
			// ----------------------------------

			// コライダー(衝突する側)
			for (auto& colA : colliderListA)
			{
				// コライダー(衝突される側)
				for (auto& colB : colliderListB)
				{
					// スタティックコライダー同士は衝突しない
					if (colA.origin->isStatic && colB.origin->isStatic)
					{
						continue;
					}

					// 貫通ベクトル
					Vector3 penetration = Vector3(0);

					// colAの図形の種類番号
					const int typeA = static_cast<int>(colA.origin->GetType());

					// colBの図形の種類番号
					const int typeB = static_cast<int>(colB.origin->GetType());

					// 衝突判定を行う
					if (funcList[typeA][typeB](
						colA.world, colB.world, penetration))
					{
						// 衝突したゲームオブジェクト
						GameObject& gameObjectA = colA.origin->GetOwnerObject();

						// 衝突されたゲームオブジェクト
						GameObject& gameObjectB = colB.origin->GetOwnerObject();

						// ----------------------------------------------
						// コライダーが重ならないように座標を調整
						// ----------------------------------------------

						// 両方が重複可能なコライダーでないことを確認
						if (!colA.origin->isTrigger && !colB.origin->isTrigger)
						{
							// Aは動かないので、Bを移動させる
							if (colA.origin->isStatic)
							{
								/* AがBにぶつかった際の処理なので、
								AがBに貫通した分、
								同じ方向にBを移動させる */

								// 貫通した分移動
								ApplyPenetration(penetration,
									colliderListB, gameObjectB);
							}
							// Bは動かないので、Aを移動させる
							else if (colB.origin->isStatic)
							{
								/* AがBにぶつかった際の処理なので、
								AがBに貫通した分、
								逆方向にAを移動させる */

								// 貫通した分移動
								ApplyPenetration(-penetration,
									colliderListA, gameObjectA);
							}
							// 両方動かないので、均等に移動させる
							else
							{
								// 貫通ベクトルの半分
								// 均等に移動させるため
								Vector3 halfPenetration = penetration * 0.5f;

								// 貫通距離の半分だけ
								// 貫通した方向に移動
								ApplyPenetration(halfPenetration,
									colliderListB, gameObjectB);

								// 貫通距離の半分だけ
								// 逆方向に移動
								ApplyPenetration(-halfPenetration, 
									colliderListA, gameObjectA);
							}
						}

						// ------------------------------
						// 衝突イベントを実行
						// ------------------------------

						//gameObjectA.OnCollision(colA.origin, colB.origin);
						//gameObjectB.OnCollision(colB.origin, colA.origin);

						// イベントの結果、
						// どちらかのゲームオブジェクトが破棄された場合
						if (gameObjectA.IsDestroyed() || gameObjectB.IsDestroyed())
						{
							// 衝突処理を終了
							return;
						}
					}

				} // for colB 
			} // for col A
		}

		/// <summary>
		/// ゲームオブジェクト単位の衝突を処理する
		/// </summary>
		/// <param name="[in] gameObjectList"> 衝突を処理するゲームオブジェクト配列 </param>
		void GameObjectCollision(const GameObjectList& gameObjectList)
		{
			// --------------------------------------
			// ワールド座標系の衝突判定を作成
			// --------------------------------------

			// ゲームオブジェクトごとのコライダーを管理する配列
			std::vector<WorldColliderList> colliderList;

			// ゲームオブジェクト分の容量を予約
			colliderList.reserve(gameObjectList.size());

			// ゲームオブジェクトにアクセス(読み取りのみ)
			for (const auto& gameObject : gameObjectList)
			{
				// ゲームオブジェクトにコライダーがなければ何もしない
				if (gameObject->colliderList.empty())
				{
					continue;
				}

				// 「接地していない」状態にする
				gameObject->isGrounded = false;

				// -------------------------------------------------
				// コライダーをコピーしてワールド座標に変換
				// -------------------------------------------------

				// ワールド座標系コライダーの配列
				WorldColliderList worldColliderList(gameObject->colliderList.size());

				// ワールド座標系に変換
				for (int i = 0; i < gameObject->colliderList.size(); ++i)
				{
					// オブジェクトのコライダー
					ColliderPtr collider = gameObject->colliderList[i];

					// 座標変換行列
					Matrix4x4 transformMatrix = gameObject->transform->GetTransformMatrix();

					// ゲームオブジェクトのコライダーを設定
					worldColliderList[i].origin = collider;

					// 座標変換をしたコライダーを設定
					worldColliderList[i].world = collider->GetTransformedCollider(transformMatrix);
				}

				// ワールド座標系コライダーを追加
				colliderList.push_back(worldColliderList);
			}

			// ----------------------------------
			// コライダーの衝突判定
			// ----------------------------------

			// コライダーを持つオブジェクトが
			// 2つ以上ないと衝突判定できないので、個数チェック
			if (colliderList.size() >= 2)
			{
				/* ゲームオブジェクトは複数のコライダーを持てるので、
				ゲームオブジェクトが持つ全てのコライダーを処理しなければならない */

				// 1つ目のオブジェクトのコライダー
				for (auto a = colliderList.begin(); a != colliderList.end() - 1; ++a)
				{
					// コライダーの持ち主が削除済みなら何もしない
					const GameObject& objectA = a->at(0).origin->GetOwnerObject();
					if (objectA.IsDestroyed())
					{
						continue;
					}

					// 2つ目のオブジェクトのコライダー
					for (auto b = a + 1; b != colliderList.end(); ++b)
					{
						// コライダーの持ち主が削除済みなら何もしない
						const GameObject& objectB = a->at(0).origin->GetOwnerObject();
						if (objectB.IsDestroyed())
						{
							continue;
						}

						// コライダー単位の衝突判定
						WorldColliderCollision(*a, *b);

					} // for b
				} // for a
			}

		} // GameObjectCollision

	} // namespace Collision
}