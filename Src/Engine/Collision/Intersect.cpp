/**
* @file Intersect.cpp
*/
#include "Intersect.h"

#include <algorithm>

namespace PokarinEngine
{
	/// <summary>
	/// 衝突用
	/// </summary>
	namespace Collision
	{
		// ----------------------------
		// 型の別名を定義
		// ----------------------------

		using Vertices = std::vector<Vector3>;

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// OBBから点への最近接点を求める
		/// </summary>
		/// <param name="[in] box"> 判定対象のOBB </param>
		/// <param name="[in] point"> 判定対象の点 </param>
		/// <returns> OBBの中で最も点に近い座標 </returns>
		Vector3 ClosestPoint(const Box& box, const Vector3& point)
		{
			// OBBの中心から点に向かうベクトル
			const Vector3 vector = point - box.center;

			// OBBの中で最も点に近い座標
			Vector3 closestPoint = box.center;

			// 中心からの大きさ
			const Vector3 centerSize = box.scale / 2.0f;

			// X,Y,Z軸それぞれで求める
			for (int i = 0; i < Vector3::size; ++i)
			{
				/* OBBの中心から点に向かうベクトルを
				OBBのX,Y,Z軸に射影することで、
				それぞれの軸の向きに対応した
				OBBの中心から対象の点までの距離を求める

				射影は内積で求める */

				// OBBのi軸の向きに対応したOBBの中心からの距離
				float distance = Vector3::Dot(vector, box.axis[i]);

				// 射影で得られた距離をOBBの範囲内に制限
				distance = std::clamp(distance, -centerSize[i], centerSize[i]);

				// OBBの中心座標にベクトルを足して最近接点を求める
				closestPoint += distance * box.axis[i];
			}

			return closestPoint;
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// OBBの頂点座標配列を取得する
		/// </summary>
		/// <param name="[in] box"> 頂点座標配列を取得するOBB </param>
		/// <returns> OBBの頂点座標配列 </returns>
		Vertices GetVertices(const Box& box)
		{
			// 中心からの大きさ
			const Vector3 centerSize = box.scale / 2.0f;

			// X方向のベクトル
			const Vector3 x = box.axis[0] * centerSize.x;

			// Y方向のベクトル
			const Vector3 y = box.axis[1] * centerSize.y;

			// Z方向のベクトル
			const Vector3 z = box.axis[2] * centerSize.z;

			/* 頂点座標の番号
			*
			*   5----4
			*  /|   /|
			* 1----0 |
			* | 7--|-6
			* |/   |/
			* 3----2
			*
			*/

			// 左手座標系
			return {
				box.center + x + y - z, // 0
				box.center - x + y - z, // 1
				box.center + x - y - z, // 2
				box.center - x - y - z, // 3
				box.center + x + y + z, // 4
				box.center - x + y + z, // 5
				box.center + x - y + z, // 6
				box.center - x - y + z, // 7
			};
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// OBBの頂点を軸に射影し、最大値と最小値を求める
		/// </summary>
		/// <param name="[in] vertices"> OBBの頂点座標配列 </param>
		/// <param name="[in] axis"> 射影先の軸ベクトル </param>
		/// <param name="[out] min"> 射影した影の最小値 </param>
		/// <param name="[out] max"> 射影した影の最大値 </param>
		void ProjectionBoxToAxis(const Vertices& vertices, const Vector3& axis,
			float& min, float& max)
		{
			// 基準として頂点座標0番を射影する
			// 座標をベクトルに射影するには内積を使う
			min = max = Vector3::Dot(axis, vertices[0]);

			// 各頂点を射影して影の最大値、最小値を求める
			for (int i = 0; i < vertices.size(); ++i)
			{
				// 頂点座標の射影した影
				float projection = Vector3::Dot(axis, vertices[i]);

				// 影の最小値を更新
				min = std::min(min, projection);

				// 影の最大値を更新
				max = std::max(max, projection);
			}
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// 2つのOBBを軸ベクトルに射影し、影の貫通距離を求める
		/// </summary>
		/// <param name="[in] verticesA"> OBB(A)の頂点座標配列 </param>
		/// <param name="[in] verticesB"> OBB(B)の頂点座標配列 </param>
		/// <param name="[in] axis"> 射影先の軸ベクトル </param>
		/// <param name="[in] shouldFlip"> 貫通ベクトルの反転が必要ならtrue </param>
		/// <returns> 影の貫通距離 </returns>
		float GetPenetrationDistance(const Vertices& verticesA, const Vertices& verticesB,
			const Vector3& axis, bool& shouldFlip)
		{
			// ------------------------------------
			// 2つのOBBを軸に射影する
			// ------------------------------------

			// 影の最小値と最大値
			float minA = 0, maxA = 0, minB = 0, maxB = 0;

			// 2つのOBBを軸に射影
			ProjectionBoxToAxis(verticesA, axis, minA, maxA);
			ProjectionBoxToAxis(verticesB, axis, minB, maxB);

			// 射影した範囲が交差していないなら0を返す
			if (minB > maxA || minA > maxB)
			{
				return 0;
			}

			// --------------------------------------------
			// 貫通ベクトルの反転の有無を設定する
			// --------------------------------------------

			// 「OBB(A)」が左、「OBB(B)」が右にあると想定しているので、
			// 順次が逆になっている場合、貫通ベクトルの向きを反転する必要がある
			// ということを呼び出し元に伝える
			shouldFlip = minB < minA;

			// --------------------------------
			// 貫通距離を計算する
			// --------------------------------

			// OBB(A)の影の長さ
			float lengthA = maxA - minA;

			// OBB(B)の影の長さ
			float lengthB = maxB - minB;

			// 2つのOBBの影の最小値
			float min = std::min(minA, minB);

			// 2つのOBBの影の最大値
			float max = std::max(maxA, maxB);

			// 2つのOBBの影の長さ
			float length = max - min;

			// 貫通距離のマイナス値
			// 「2つのOBBの影の長さ - それぞれのOBBの影の長さ」で
			// 2つのOBBの影が離れている距離(貫通距離のマイナス値)が求められる
			float minusDistance = length - lengthA - lengthB;

			// 貫通距離を返す
			return -minusDistance;
		}

		/// <summary>
		/// 球体同士の交差判定
		/// </summary>
		/// <param name="[in] a"> 判定対象の球体A </param>
		/// <param name="[in] b"> 判定対象の球体B </param>
		/// <param name="[out] penetration"> AのBに対する貫通ベクトル </param>
		/// <returns> 交差しているならtrue </returns>
		bool Intersect(const Sphere& a, const Sphere& b, Vector3& penetration)
		{
			// ---------------------------------
			// 中心の間の距離の2乗を計算
			// ---------------------------------

			// 球体Aの中心から球体Bの中心までのベクトル
			const Vector3 vector = b.center - a.center;

			// 球体Aの中心から球体Bの中心までの距離の2乗
			// 平方根を避けるため、2乗の値を使う
			const float sprDistance = vector.SqrMagnitude();

			//---------------------------------------
			// 距離が半径の合計より長い場合は
			// 交差していない
			// --------------------------------------

			// 球体Aと球体Bの半径の合計
			const float totalRadius = a.radius + b.radius;

			// 平方根を避けるため、2乗同士で比較する
			if (sprDistance > totalRadius * totalRadius)
			{
				return false;
			}

			// ------------------------------
			// 交差しているので
			// 貫通ベクトルを求める
			// ------------------------------

			// 球体Aの中心から球体Bの中心までの距離
			const float distance = sqrt(sprDistance);

			// 球体Aの中心から球体Bの中心までの単位ベクトル(向きベクトル)
			const Vector3 normalize = vector / distance;

			// 貫通距離
			const float penetrationDistance = totalRadius - distance;

			// 向きベクトルに貫通距離を反映させて、貫通ベクトルを求める
			penetration = normalize * penetrationDistance;

			return true;
		}

		/// <summary>
		/// OBB同士の交差判定
		/// </summary>
		/// <param name="[in] a"> 判定対象のOBB(A) </param>
		/// <param name="[in] b"> 判定対象のOBB(B) </param>
		/// <param name="[out] penetration"> 貫通ベクトル </param>
		/// <returns> 交差しているならtrue </returns>
		bool Intersect(const Box& a, const Box& b, Vector3& penetration)
		{
			// ------------------------------
			// 下準備
			// ------------------------------

			// OBB(A)の頂点座標配列
			const auto verticesA = GetVertices(a);

			// OBB(B)の頂点座標配列
			const auto verticesB = GetVertices(b);

			// 最終的な貫通距離
			// 徐々に小さくして求めるので最大値で初期化
			float penetrationDistance = FLT_MAX;

			/* 分離軸判定を行うには、以下の軸ベクトルが必要

			1.オブジェクトAの全ての面法線(OBBの場合3方向)
			2.オブジェクトBの全ての面法線(OBBの場合3方向)
			3.Aの全ての辺とBの全ての辺の外積
			(OBB対OBBの場合、3x3 = 9方向)

			1.2は回転角度が違うのでそれぞれで判定が必要
			3に関しては、3次元では面だけでは不十分なので
			辺単位でも射影して分離軸判定を行う */

			// 軸ベクトル配列の要素数
			static const int axisListSize = 15;

			// 軸ベクトル配列
			// OBB(A)を基準に貫通距離を求めるので、OBB(B)の軸ベクトルを反転させておく
			Vector3 axisList[axisListSize] = {
				a.axis[0], a.axis[1], a.axis[2],
				-b.axis[0], -b.axis[1], -b.axis[2] };

			for (int i = 0; i < Vector3::size; ++i)
			{
				// AとBの全ての辺の外積の格納開始位置
				const int startIndex = 6 + i * 3;

				// AとBの全ての辺の外積を求める
				// 辺は面法線と同じなので、面法線で外積を求める
				axisList[startIndex + 0] = Vector3::Cross(axisList[i], axisList[3]);
				axisList[startIndex + 1] = Vector3::Cross(axisList[i], axisList[4]);
				axisList[startIndex + 2] = Vector3::Cross(axisList[i], axisList[5]);
			}

			// ----------------------------------------------------
			// 全ての軸ベクトルに対して分離軸判定を実行する
			// ----------------------------------------------------

			for (int i = 0; i < axisListSize; ++i)
			{
				// 軸ベクトルの長さ(2乗値)
				const float sqrMagunitude = axisList[i].SqrMagnitude();

				// 外積によって作られた軸ベクトルの場合、
				// 元の軸が平行だと長さが0になる
				// この場合、AまたはBの軸による分離軸判定と等しいのでスキップする
				if (sqrMagunitude <= 0)
				{
					continue;
				}

				// 貫通ベクトルの反転が必要ならtrue
				bool shouldFlip = false;

				// OBB(A)がOBB(B)に貫通している距離
				float distance = GetPenetrationDistance(verticesA, verticesB, axisList[i], shouldFlip);

				// 貫通距離が0以下なら分離超平面を入れる隙間があるので、
				// 交差していない
				if (distance <= 0)
				{
					return false;
				}

				// 外積で作成した軸ベクトルの長さはsinθなので1にならず
				// distanceに「実際の貫通距離 * 軸ベクトルの長さ」が入ることになる
				// このままだと比較ができないので、
				// 「軸ベクトルの長さ」で除算して「実際の貫通距離」を取り出す
				distance /= std::sqrt(sqrMagunitude);

				// 貫通距離がより短い方向から衝突したとみなす
				if (distance < penetrationDistance)
				{
					// 貫通距離を更新
					penetrationDistance = distance;

					// 貫通方向
					Vector3 penetrationAxis = axisList[i];

					// 必要なら貫通方向を反転させる
					if (shouldFlip)
					{
						penetrationAxis *= -1;
					}

					// 軸ベクトル(単位ベクトル)に貫通距離を反映して
					// 貫通ベクトルを求める
					penetration = penetrationAxis * penetrationDistance;
				}
			}

			// 衝突している
			return true;
		}

		/// <summary>
		/// OBBと球体の交差判定
		/// </summary>
		/// <param name="[in] box"> 判定対象のOBB </param>
		/// <param name="[in] sphere"> 判定対象の球体 </param>
		/// <param name="[out] penetration"> OBBの球体に対する貫通ベクトル </param>
		/// <returns> 交差しているならtrue </returns>
		bool Intersect(const Box& box, const Sphere& sphere, Vector3& penetration)
		{
			// OBBから球体の中心への最近接点
			const Vector3 point = ClosestPoint(box, sphere.center);

			// 最近接点から球体の中心までのベクトル
			const Vector3 vector = sphere.center - point;

			// 最近接点から球体の中心までの距離の2乗
			// 平方根を避けるために2乗の値を使う
			const float sqrDistance = vector.SqrMagnitude();

			// 最近接点から球体の中心までの距離が
			// 球体の半径より大きい場合は、交差していない
			if (sqrDistance > sphere.radius * sphere.radius)
			{
				return false;
			}

			// 距離が0より大きい場合、球体の中心はOBBの外側にある
			// この場合、最近接点から球体の中心へ向かう方向から衝突したとみなす
			if (sqrDistance > 0)
			{
				/* 貫通距離を単位ベクトル(向きベクトル)に反映することで
				貫通ベクトルを求める */

				const float distance = sqrt(sqrDistance);

				// 貫通距離
				const float penetrationDistance = (sphere.radius - distance);

				// 最近接点から球体の中心への単位ベクトル(向きベクトル)
				const Vector3 normalize = vector / distance;

				// 貫通ベクトルを求める
				penetration = normalize * penetrationDistance;
			}
			// 距離が0の場合、球体の中心はOBBの内部にある
			// この場合、貫通距離が最も短い面から衝突したとみなす
			else
			{
				/* 貫通距離が最も短い面を求めて
				向きベクトルに反映させることで、貫通ベクトルを求める*/

				// OBBの中心から球体の中心へのベクトル
				const Vector3 obbVector = sphere.center - box.center;

				// OBBの中心からの大きさ
				const Vector3 centerSize = box.scale / 2.0f;

				// 貫通方向(面)を示すインデックス
				int faceIndex = 0;

				// 球体の中心がOBBに貫通した距離
				float distance = FLT_MAX;

				// 貫通ベクトルの符号
				float sign = 1;

				// 軸ごとに「球体の中心からOBBの面までの距離」を調べて
				// 短いものを「球体の中心がOBBに貫通した距離」として採用する
				for (int i = 0; i < Vector3::size; ++i)
				{
					// OBBのi軸の向きに対応したOBBの中心からの距離
					const float obbDistance = Vector3::Dot(obbVector, box.axis[i]);

					// マイナス方向の球体の中心からOBBの面までの距離
					const float minus = obbDistance - (-centerSize[i]);

					// 距離が短ければマイナス方向で交差したと判断する
					if (minus < distance)
					{
						faceIndex = i;
						distance = minus;
						sign = -1;
					}

					// プラス方向の球体の中心からOBBの面までの距離
					const float plus = centerSize[i] - obbDistance;

					// 距離が短ければプラス方向で交差したと判断する
					if (plus < distance)
					{
						faceIndex = i;
						distance = plus;
						sign = 1;
					}
				}

				// 球体がOBBに貫通した距離
				// 方向も分かるように符号を付ける
				const float penetrationDistance = (distance * sphere.radius) * sign;

				// 符号付きの貫通距離を向きベクトルに反映させることで、
				// 貫通ベクトルを求める
				penetration = box.axis[faceIndex] * penetrationDistance;
			}

			return true;
		}
	}
}