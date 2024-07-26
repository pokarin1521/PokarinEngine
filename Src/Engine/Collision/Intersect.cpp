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
			const Vector3 centerSize = box.size / 2.0f;

			// X,Y,Z軸それぞれで求める
			for (int i = 0; i < Vector3::size; ++i)
			{
				/* OBBの中心から点に向かうベクトルを
				OBBのX,Y,Z軸に射影することで、
				それぞれの軸の向きに対応した
				OBBの中心から球体の中心までの距離を求める

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

		/// <summary>
		/// 球体同士の交差判定
		/// </summary>
		/// <param name="[in] a"> 判定対象の球体A </param>
		/// <param name="[in] b"> 判定対象の球体B </param>
		/// <param name="[out] penetration"> AのBに対する貫通ベクトル </param>
		/// <returns> 
		/// <para> true : 交差している </para> 
		/// <para> false : 交差していない </para> 
		/// </returns>
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
		/// OBBと球体の交差判定
		/// </summary>
		/// <param name="[in] box"> 判定対象のOBB </param>
		/// <param name="[in] sphere"> 判定対象の球体 </param>
		/// <param name="[out] penetration"> OBBの球体に対する貫通ベクトル </param>
		/// <returns> 
		/// <para> true : 交差している </para> 
		/// <para> false : 交差していない </para> 
		/// </returns>
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
				const Vector3 centerSize = box.size / 2.0f;

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