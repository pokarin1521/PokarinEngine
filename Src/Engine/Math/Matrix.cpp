/**
* @file Matrix.cpp
*/
#include "Matrix.h"

namespace PokarinEngine
{
#pragma region Matrix3x3

	/// <summary>
	/// Mat4型で初期化するコンストラク
	/// </summary>
	Matrix3x3::Matrix3x3(const Matrix4x4& m)
	{
		data[0] = Vector3(m.data[0]);
		data[1] = Vector3(m.data[1]);
		data[2] = Vector3(m.data[2]);
	}

	/// <summary>
	/// 回転角度から回転行列を作成
	/// </summary>
	/// <param name="[in] rotation"> 回転角度 </param>
	/// <returns> 作成した回転行列 </returns>
	Matrix3x3 Matrix3x3::CreateRotationMatrix(const Vector3& rotation)
	{
		/* X,Y,Z軸の回転行列を求めて掛けることで、XYZ軸回転の行列が求められる 
		
		右回転にしたいので、右手座標系の回転行列で書く
		ただし、OpenGLが右手座標系であるためZ軸がマイナスに向かっている
		なので、Z軸だけ左手座標系の回転行列にして右回転になるようにする */

		// X軸回転のSin・Cos
		const float sinX = sin(rotation.x), cosX = cos(rotation.x);

		// Y軸回転のSin・Cos
		const float sinY = sin(rotation.y), cosY = cos(rotation.y);

		// Z軸回転のSin・Cos
		const float sinZ = sin(rotation.z), cosZ = cos(rotation.z);

		// X軸回転の行列
		// 右手座標系の回転
		const Matrix3x3 matrix_X = {
				   { 1,     0,      0 },
				   { 0,  cosX,  -sinX },
				   { 0,  sinX,   cosX } };

		// Y軸回転の行列
		// 右手座標系の回転
		const Matrix3x3 matrix_Y = {
				   {  cosY,  0,  sinY },
				   {     0,  1,     0 },
				   { -sinY,  0,  cosY } };

		// Z軸回転の行列
		// 左手座標系の回転(Z軸の方向に対応するため)
		// -1は左手座標系のオブジェクトに対応するため
		const Matrix3x3 matrix_Z = {
					{  cosZ,  sinZ,  0 },
					{ -sinZ,  cosZ,  0 },
					{     0,     0, -1 } };

		// X,Y,Z軸回転を掛け合わせてXYZ軸回転の行列にする
		// Y軸はワールド軸で回転してほしいので、YXZの順にする
		return matrix_Y * matrix_X * matrix_Z;
	}

#pragma endregion

#pragma region Matrix4x4

	/// <summary>
	/// 座標変換行列を作成する
	/// </summary>
	/// <param name="[in] translate"> 平行移動量 </param>
	/// <param name="[in] rotation"> 回転角度 </param>
	/// <param name="[in] scale"> 拡大率 </param>
	/// <returns> 作成した座標変換行列 </returns>
	Matrix4x4 Matrix4x4::CreateTransformMatrix(
		const Vector3& translate, const Vector3& rotation, const Vector3& scale)
	{
		// 回転を反映
		const Matrix3x3 m = Matrix3x3::CreateRotationMatrix(rotation);

		// 拡大率と平行移動量を反映
		return {
			Vector4{ scale.x * m.data[0], 0 },
			Vector4{ scale.y * m.data[1], 0 },
			Vector4{ scale.z * m.data[2], 0 },
			Vector4{ translate, 1 } };
	}

	/// <summary>
	/// 座標変換行列から移動量を取得する
	/// </summary>
	/// <param name="[in] transform"> 座標変換行列 </param>
	/// <returns> 平行移動量 </returns>
	Vector3 Matrix4x4::Translate(const Matrix4x4& transform)
	{
		// 最後の要素に平行移動量を設定しているので、それを返す
		return Vector3(transform[3]);
	}

	/// <summary>
	/// 座標変換行列から拡大率を取得する
	/// </summary>
	/// <param name="[in] transform"> 座標変換行列 </param>
	/// <returns> 拡大率 </returns>
	Vector3 Matrix4x4::Scale(const Matrix4x4& transform)
	{
		// 左上3x3部分の各行を3Dベクトルとして長さを求めることで
		// 拡大率を求める
		Vector3 a = Vector3(transform[0]);
		Vector3 b = Vector3(transform[1]);
		Vector3 c = Vector3(transform[2]);

		const float sx = a.Length();
		const float sy = b.Length();
		const float sz = c.Length();

		return { sx, sy, sz };
	}

	/// <summary>
	/// 座標変換行列から回転行列を取得する
	/// </summary>
	/// <param name="[in] transform"> 座標変換行列 </param>
	/// <returns> 回転行列 </returns>
	Matrix3x3 Matrix4x4::Rotate(const Matrix4x4& transform)
	{
		// 拡大率
		Vector3 scale = Scale(transform);

		// 回転行列
		Matrix3x3 rotate = Matrix3x3(transform);

		// 座標変換行列を作成する際に、
		// 回転行列に拡大率を掛けているので、拡大率で割って求める
		rotate[0] /= scale.x;
		rotate[1] /= scale.y;
		rotate[2] /= scale.z;

		return rotate;
	}

#pragma endregion

}