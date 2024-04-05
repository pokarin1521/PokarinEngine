/**
* @file Collision.cpp
*/
#include "Collision.h"

#include <algorithm>

namespace PokarinEngine
{
	namespace Collision
	{
		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// スラブ(ある軸に垂直な平面)と光線の交差判定
		/// </summary>
		/// <param name="[in] min"> スラブの開始位置 </param>
		/// <param name="[in] max"> スラブの終了位置 </param>
		/// <param name="[in] start"> 光線の発射点 </param>
		/// <param name="[in] direction"> 光線の向き </param>
		/// <param name="[out] tmin"> AABBと光線の交差開始時間 </param>
		/// <param name="[out] tmax"> AABBと光線の交差終了時間 </param>
		/// <returns>
		/// <para> true : 交差している </para>
		/// <para> false : 交差していない </para>
		/// </returns>
		bool IntersectSlab(float min, float max, float start, float direction,
			float& tmin, float& tmax)
		{
			// ----------------------------
			// 光線とスラブが平行か確認
			// ----------------------------

			/* 光線とスラブが平行な場合、
			軸成分が0になるため時間を求めることができない
			この場合は「発射点がスラブ内にあるかどうか」で判定する*/

			// 光線がスラブと平行な場合
			// 発射点がスラブ内にあれば交差している、外にあれば交差していない
			if (abs(direction) < 0.0001f)
			{
				return (start >= min) && (start <= max);
			}

			// ----------------------------------------------------
			// 光線とスラブが交差する開始時間と終了時間を求める
			// ----------------------------------------------------

			/* 光線の向きベクトルは
			「光線がX,Y,Zの各軸方向に進む速度」と考えられる

			そのため、ある軸における「発射点からスラブまでの距離」を
			向きベクトルの同じ軸成分で割ることで、
			「交差開始時間(光線が出てから交差し始めるまでの時間)」と
			「交差終了時間(光線が出てから交差が終わるまでの時間)」が求められる */

			// 光線とスラブが交差する開始時間
			// (光線が出てから交差し始めるまでの時間)
			float t0 = (min - start) / direction;

			// 光線とスラブが交差する終了時間
			// (光線が出てから交差が終わるまでの時間)
			float t1 = (max - start) / direction;

			// 時間の早い側を開始時間とする
			if (t0 > t1)
			{
				// t0とt1を入れ替え
				std::swap(t0, t1);
			}

			// -------------------------
			// 共通の交差範囲を求める
			// -------------------------

			/* 光線がXスラブと交差している最中に、
			Yスラブと交差した場合、

			X : 交差開始時間 <------------------> 交差終了時間
			Y :          交差開始時間 <=================> 交差終了時間

			となるか

			X : 交差開始時間 <---------------------------------> 交差終了時間
			Y :          交差開始時間 <=================> 交差終了時間

			となる

			このとき、共通の交差範囲は
			「遅い方の交差開始時間 ～ 早い方の交差終了時間」となる */

			// 以前の交差開始時間と今回の交差開始時間を比較し、
			// 遅い方を選択
			if (t0 > tmin)
			{
				tmin = t0;
			}

			// 以前の交差終了時間と今回の交差終了時間を比較し、
			// 早い方を選択
			if (t1 < tmax)
			{
				tmax = t1;
			}

			// ------------
			// 交差判定
			// ------------

			// 「交差開始時間 <= 交差終了時間」の場合は交差している
			// それ以外は「共通の交差範囲」がないことになる
			return tmin <= tmax;
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// AABBから点への最近接点を求める
		/// </summary>
		/// <param name="aabb"> 判定対象のAABB </param>
		/// <param name="point"> 判定対象の点 </param>
		/// <returns> AABBの中で最も点に近い座標 </returns>
		Vector3 ClosestPoint(const AABB& aabb, const Vector3& point)
		{
			// AABBの中で最も点に近い座標
			Vector3 closestPoint = Vector3(0);

			// X,Y,Z座標それぞれで求める
			for (int i = 0; i < Vector_Size::vector3; ++i)
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
		/// <param name="box"> 判定対象のOBB </param>
		/// <param name="point"> 判定対象の点 </param>
		/// <returns> OBBの中で最も点に近い座標 </returns>
		Vector3 ClosestPoint(const Box& box, const Vector3& point)
		{
			// OBBの中心から点に向かうベクトル
			const Vector3 vector = point - box.position;

			// OBBの中で最も点に近い座標
			Vector3 closestPoint = box.position;

			// X,Y,Z軸それぞれで求める
			for (int i = 0; i < Vector_Size::vector3; ++i)
			{
				/* OBBの中心から点に向かうベクトルを
				OBBのX,Y,Z軸に射影することで、
				それぞれの軸の向きに対応した
				OBBの中心から球体の中心までの距離を求める

				射影は内積で求める */

				// OBBのi軸の向きに対応したOBBの中心からの距離
				float distance = vector.Dot(box.axis[i]);

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
			const Vector3 vector = b.position - a.position;

			// a～bまでの距離の2乗
			// (平方根を避けるため、2乗同士の比較に使う)
			const float sprDistance = vector.SprMagnitude();

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

			// 球体の中心からの最近接点
			const Vector3 point = ClosestPoint(aabb, sphere.position);

			// 最近接点から球体の中心までのベクトル
			const Vector3 vector = sphere.position - point;

			// 最近接点から球体の中心までの距離の2乗
			const float sqrDistance = vector.SprMagnitude();

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
				for (int i = 0; i < Vector_Size::vector3; ++i)
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
		/// <param name="[in] aabb"> 判定対象のOBB </param>
		/// <param name="[in] sphere"> 判定対象の球体 </param>
		/// <param name="[out] penetration"> OBBが球体に貫通した距離 </param>
		/// <returns> 
		/// <para> true : 交差している </para> 
		/// <para> false : 交差していない </para> 
		/// </returns>
		bool Intersect(const Box& box, const Sphere& sphere, Vector3& penetration)
		{
			return false;
		}

		/// <summary>
		/// AABBと光線の交差判定
		/// </summary>
		/// <param name="[in] aabb"> 判定対象のAABB </param>
		/// <param name="[in] ray"> 判定対象の光線 </param>
		/// <param name="[out] distance"> 光線がAABBと最初に交差するまでの距離 </param>
		/// <returns>
		/// <para> true : 交差している </para>
		/// <para> false : 交差していない </para>
		/// </returns>
		bool Intersect(const AABB& aabb, const Ray& ray, float& distance)
		{
			// ------------------------------
			// 共通の交差範囲
			// 最初は0 ～ 最大値にしておく
			// ------------------------------

			// 交差開始時間
			float tmin = 0;

			// 交差終了時間
			float tmax = FLT_MAX;

			// --------------------------------
			// 各軸のスラブと光線の交差判定
			// --------------------------------

			/* 光線がAABBと交差するというのは、
			光線が各軸のスラブと同時に交差するということ

			IntersectSlab関数で「共通の交差範囲」を求め
			同時に交差しているか確認する */

			// Xスラブと光線の交差判定
			if (!IntersectSlab(aabb.min.x, aabb.max.x,
				ray.start.x, ray.direction.x, tmin, tmax))
			{
				// 交差していない
				return false;
			}

			// Yスラブと光線の交差判定
			if (!IntersectSlab(aabb.min.y, aabb.max.y,
				ray.start.y, ray.direction.y, tmin, tmax))
			{
				// 交差していない
				return false;
			}

			// Zスラブと光線の交差判定
			if (!IntersectSlab(aabb.min.z, aabb.max.z,
				ray.start.z, ray.direction.z, tmin, tmax))
			{
				// 交差していない
				return false;
			}

			// --------------------------------
			// 交点までの距離を設定し、
			// 交差していることを確定させる
			// --------------------------------

			// 交差し始めるまでの時間が、
			// そのまま交点までの距離になる

			// 交点までの距離を設定
			// 光線とコライダーの交差開始時間が
			// そのまま交点までの距離になる
			distance = tmin;

			// 交差している
			return true;
		}

		/// <summary>
		/// 球体と光線の交差判定
		/// </summary>
		/// <param name="[in] sphere"> 判定対象の球体 </param>
		/// <param name="[in] ray"> 判定対象の光線 </param>
		/// <param name="[out] distance"> 光線がAABBと最初に交差するまでの距離 </param>
		/// <returns>
		/// <para> true : 交差している </para>
		/// <para> false : 交差していない </para>
		/// </returns>
		bool Intersect(const Sphere& sphere, const Ray& ray, float& distance)
		{
			/* 光線(直線)の式Rは、
			「始点P、向きD、光線上の任意の位置を表す変数t」で定義できる

			R(t) = P + t * D

			球体の式は、
			「球体の中心C、半径r、球体表面の任意の座標X」で定義できる

			(X - C)・(X - C) = r^2		(・は内積)

			球体と光線が交差する座標は、
			Xに光線の式R(t)を代入し、球体の式を満たす変数tを求めることで見つかる

			m = P - C とする
			(t * D + m)・(t * D + m) = r^2

			展開する(Dは向きベクトルなので長さは1、D・D = 1)

			t^2 + 2(m・D)t + m・m = r^2

			t以外の記号は定数なので、tの2次方程式になる
			解の公式を使ってtを求める (b = m・D, c = m・m - r^2)
			ただし、光線には始点があるので、
			tが始点より手前を指す場合を除外しなくてはならない */

			// ---------------------------------------------
			// 「球体と光線が交差する座標t」を
			// 求めるのに必要な「m, b, c」を求める
			// ---------------------------------------------

			// 球体の中心から光線の始点までのベクトル
			const Vector3 m = ray.start - sphere.position;

			// 球体に対する光線の方向(内積で判断する)
			// プラス   : 球体から離れていく方向
			// マイナス : 球体に向かっていく方向
			const float b = m.Dot(ray.direction);

			//「球体の中心から光線の始点までの距離」と「球体の半径」の差
			// 平方根を避けるために2乗値同士で計算
			// プラス   : 光線の始点が球体の外にある
			// マイナス : 光線の始点が球体の中にある
			const float c = m.SprMagnitude() - sphere.radius * sphere.radius;

			// ------------------------------------------
			// 球体と光線が交差するのか判定する
			// ------------------------------------------

			// 光線の始点が球体の外にあり、
			// 光線が球体から離れていく方向に発射された場合、
			if (c > 0 && b > 0)
			{
				// 交差しない
				return false;
			}

			// --------------------------------------
			// 「球体と光線が交差する座標t」が
			// 求められるのか判定する
			// --------------------------------------

			// 判別式
			const float d = b * b - c;

			// 判別式が負の場合は
			// 解なし
			if (d < 0)
			{
				return false;
			}

			// ---------------------------------
			// 最初に交差する座標を計算
			// ---------------------------------

			distance = -b - sqrt(d);

			// ---------------------------------------------
			// 光線の始点より後ろ側で交差している場合
			// 0にして「始点 = 交点」とする
			// ---------------------------------------------

			// 求めた交点が負の場合、光線の始点より手前を指し
			// 光線が球体内から発射されたことを意味する
			// (始点が球体外にある状況は、交差するかの判定で除外済み)
			if (distance < 0)
			{
				// 始点 = 交点とする
				distance = 0;
			}

			return true;
		}

	} // namespace Collision

} // namespace PokarinEngine