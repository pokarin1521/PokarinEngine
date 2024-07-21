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
		/// AABBから点への最近接点を求める
		/// </summary>
		/// <param name="[in] aabb"> 判定対象のAABB </param>
		/// <param name="[in] point"> 判定対象の点 </param>
		/// <returns> AABBの中で最も点に近い座標 </returns>
		Vector3 ClosestPoint(const AABB& aabb, const Vector3& point)
		{
			// AABBの中で最も点に近い座標
			Vector3 closestPoint = Vector3(0);

			// X,Y,Z座標それぞれで求める
			for (int i = 0; i < Vector3::size; ++i)
			{
				// 範囲内に制限することで、
				// AABBの中で最も点に近い座標を求める
				closestPoint[i] = std::clamp(point[i], aabb.min[i], aabb.max[i]);
			}

			return closestPoint;
		}

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
				distance = std::clamp(distance, -box.scale[i], box.scale[i]);

				// OBBの中心座標にベクトルを足して最近接点を求める
				closestPoint += distance * box.axis[i];
			}

			return closestPoint;
		}

		/// <summary>
		/// AABB同士の交差判定
		/// </summary>
		/// <param name="[in] a"> 判定対象のAABB(A) </param>
		/// <param name="[in] b"> 判定対象のAABB(B) </param>
		/// <param name="[out] penetration"> AのBに対する貫通ベクトル </param>
		/// <returns> 
		/// <para> true : 交差している </para> 
		/// <para> false : 交差していない </para> 
		/// </returns>
		bool Intersect(const AABB& a, const AABB& b, Vector3& penetration)
		{
			// 貫通距離が欲しいので、2つの値の差分で交差しているか判定する
			// 貫通距離は壁にめり込んだときの修正に使う

			// aの左側面がbの右側面より右にあるなら、交差していない
			const float dx0 = b.max.x - a.min.x; // aの左側面からの貫通距離

			// 貫通距離が0未満なら交差していない
			if (dx0 <= 0)
			{
				return false;
			}

			// aの右側面がbの左側面より左にあるなら、交差していない
			const float dx1 = a.max.x - b.min.x; // aの右側面からの貫通距離

			// 貫通距離が0未満なら交差していない
			if (dx1 <= 0)
			{
				return false;
			}

			// aの下面がbの上面より上にあるなら、交差していない
			const float dy0 = b.max.y - a.min.y; // aの下面からの貫通距離

			// 貫通距離が0未満なら交差していない
			if (dy0 <= 0)
			{
				return false;
			}

			// aの上面がbの下面より下にあるなら、交差していない
			const float dy1 = a.max.y - b.min.y; // aの上面からの貫通距離

			// 貫通距離が0未満なら交差していない
			if (dy1 <= 0)
			{
				return false;
			}

			// aの奥側面がbの手前側面より手前にあるなら、交差していない
			const float dz0 = b.max.z - a.min.z; // aの奥側面からの貫通距離

			// 貫通距離が0未満なら交差していない
			if (dz0 <= 0)
			{
				return false;
			}

			// aの手前側面がbの奥側面より奥にあるなら、交差していない
			const float dz1 = a.max.z - b.min.z; // aの手前側面からの貫通距離

			// 貫通距離が0未満なら交差していない
			if (dz1 <= 0)
			{
				return false;
			}

			// この時点で交差が確定

			// 壁にめり込んだ分修正するために、貫通距離と貫通方向を求める
			// 貫通距離は交差判定で求めたものを使う
			// 貫通方向は、貫通距離が最も短い方向を選び使用する

			Vector3 length = { dx1,dy1,dz1 }; // 貫通距離の絶対値
			Vector3 signedLength = length; // 符号付きの貫通距離

			// XYZの各軸について、貫通距離が短い方向を選択

			// 左からの貫通距離と、右からの貫通距離で短い方向を選ぶ
			if (dx0 < dx1)
			{
				length.x = dx0;

				// 方向を表すために符号をつける
				signedLength.x = -dx0;
			}

			// 下からの貫通距離と、上からの貫通距離で短い方向を選ぶ
			if (dy0 < dy1)
			{
				length.y = dy0;

				// 方向を表すために符号をつける
				signedLength.y = -dy0;
			}

			// 奥からの貫通距離と、手前からの貫通距離で短い方向を選ぶ
			if (dz0 < dz1)
			{
				length.z = dz0;

				// 方向を表すために符号をつける
				signedLength.z = -dz0;
			}

			// XYZのうち最も短い方向を選択

			// X軸方向が最も短い場合
			if (length.x < length.y)
			{
				if (length.x < length.z)
				{
					// 貫通ベクトルにX軸を選択
					penetration = { signedLength.x, 0, 0 };
					return true;
				}
			}

			// Y軸方向が最も短い場合
			else if (length.y < length.z)
			{
				// 貫通ベクトルにY軸を選択
				penetration = { 0, signedLength.y, 0 };
				return true;
			}

			// Z軸方向が最も短い場合
			// 貫通ベクトルにZ軸を選択
			penetration = { 0, 0, signedLength.z };
			return true;
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

			// aの中心からbの中心までのベクトル
			const Vector3 vector = b.center - a.center;

			// a～bまでの距離の2乗
			// (平方根を避けるため、2乗同士の比較に使う)
			const float sprDistance = vector.SqrMagnitude();

			//-------------------------------------
			// 距離(2乗)が半径の合計より長い場合は
			// 交差していない
			// ------------------------------------

			// aとbの半径の合計
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

			// a～bまでの距離
			const float distance = sqrt(sprDistance);

			// 「半径の合計 - a～bまでの距離」の「距離に対する比率」を求める
			const float ratio = (totalRadius - distance) / distance;

			// 貫通ベクトルを計算
			penetration = vector * ratio;

			return true;
		}

		/// <summary>
		/// AABBと球体の交差判定
		/// </summary>
		/// <param name="[in] aabb"> 判定対象のAABB </param>
		/// <param name="[in] sphere"> 判定対象の球体 </param>
		/// <param name="[out] penetration"> AABBの球体に対する貫通ベクトル </param>
		/// <returns> 
		/// <para> true : 交差している </para> 
		/// <para> false : 交差していない </para> 
		/// </returns>
		bool Intersect(const AABB& aabb, const Sphere& sphere, Vector3& penetration)
		{
			// --------------------------------------------------
			// 最近接点までの距離が球体の半径より長ければ、
			// 交差していない
			// --------------------------------------------------

			// AABBから球体の中心への最近接点
			const Vector3 point = ClosestPoint(aabb, sphere.center);

			// 最近接点から球体の中心までのベクトル
			const Vector3 vector = sphere.center - point;

			// 最近接点から球体の中心までの距離の2乗
			const float sqrDistance = vector.SqrMagnitude();

			// 平方根を避けるため、2乗同士で比較
			if (sqrDistance > sphere.radius * sphere.radius)
			{
				return false;
			}

			// ----------------------------------------------
			// 交差しているので、貫通ベクトルを求める
			// ----------------------------------------------

			// 球体の中心がAABBの外側にあるので
			// 球体の中心から最近接点へ向かう方向から衝突したとみなす
			if (sqrDistance > 0)
			{
				// 最近接点から球体の中心まで距離
				const float distance = sqrt(sqrDistance);

				// 「半径 - 接点から中心までの距離」の「距離に対する比率」を求める
				const float ratio = (sphere.radius - distance) / distance;

				// 接点から中心までのベクトルに比率を反映して
				// 貫通ベクトルを求める
				penetration = vector * ratio;
			}
			// 球体の中心がAABBの内側にあるので
			// 貫通距離が最も短い面から衝突したとみなす
			else
			{
				// AABBが球体に貫通した方向を示すインデックス
				// 奇数がプラス方向、偶数がマイナス方向
				int faceIndex = 0;

				// AABBが球体に貫通した距離
				float penetrationDistance = FLT_MAX;

				// X,Y,Zそれぞれで比較し、
				// 貫通距離が一番短い方向を調べてインデックスを設定する
				for (int i = 0; i < Vector3::size; ++i)
				{
					// AABBの最小座標から球体の中心までの距離
					float distance_sphere = point[i] - aabb.min[i];

					// 短い方を貫通距離として設定する
					if (distance_sphere < penetrationDistance)
					{
						// マイナス方向に貫通しているので
						// 偶数を設定する
						faceIndex = i * 2;

						// 貫通距離を更新
						penetrationDistance = distance_sphere;
					}

					// 球体の中心からAABBの最大座標までの距離
					float distance_aabb = aabb.max[i] - point[i];

					// 短い方を貫通距離として設定する
					if (distance_aabb < penetrationDistance)
					{
						// プラス方向に貫通している
						faceIndex = i * 2 + 1;

						// 貫通距離を更新
						penetrationDistance = distance_aabb;
					}
				}

				// 「AABBが球体に対してどれだけ貫通しているか」を示すベクトル
				// 面の外向きのベクトルを使う
				static const Vector3 faceNormals[] = {
					{ -1, 0, 0 }, { 1, 0, 0 }, // -X, +X
					{  0,-1, 0 }, { 0, 1, 0 }, // -Y, +Y
					{  0, 0,-1 }, { 0, 0, 1 }, // -Z, +Z
				};

				// 貫通距離が一番短い方向の貫通ベクトルを求める
				penetration = faceNormals[faceIndex] * penetrationDistance;
			}

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
				// 最近接点から球体の中心まで距離
				const float distance = sqrt(sqrDistance);

				// 貫通距離
				const float penetrationDistance = (sphere.radius - distance);

				// 最近接点から球体の中心への単位ベクトル
				const Vector3 normalize = vector / distance;

				// 最近接点から球体の中心へ向かう方向から衝突したとみなすので、
				// 単位ベクトルに距離を反映することで、貫通ベクトルを求める
				penetration = normalize * penetrationDistance;
			}
			else
			{
				// 距離が0の場合、球体の中心はOBBの内部にある
				// この場合、貫通距離が最も短い面から衝突したとみなす

				// OBBの中心から球体の中心へのベクトル
				const Vector3 obbVector = sphere.center - box.center;

				// 貫通方向を示すインデックス
				int faceIndex = 0;


			}

			return true;
		}
	}
}