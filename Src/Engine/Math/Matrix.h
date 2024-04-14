/**
* @file Matrix.h
*/
#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "Vector.h"

namespace PokarinEngine
{
	// -----------------------
	// �O���錾
	// -----------------------

	struct Matrix3x3;
	struct Matrix4x4;

	// ----------------------
	// �\����
	// ----------------------

#pragma region Matrix

	/// <summary>
	/// 3x3�s��
	/// </summary>
	struct Matrix3x3
	{
		Vector3 data[3];

		// ---------------------
		// �R���X�g���N�^
		// ---------------------

		// �f�t�H���g�R���X�g���N�^
		Matrix3x3() = default;

		/// <summary>
		/// Vec3�^�ŏ���������R���X�g���N�^
		/// </summary>
		Matrix3x3(const Vector3& v0, const Vector3& v1, const Vector3& v2)
		{
			data[0] = v0;
			data[1] = v1;
			data[2] = v2;
		}

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit Matrix3x3(float f)
		{
			data[0] = { f, 0, 0 };
			data[1] = { 0, f, 0 };
			data[2] = { 0, 0, f };
		}

		/// <summary>
		/// Mat4�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit Matrix3x3(const struct Matrix4x4& m);

		// ---------------------
		// �Y�����Z�q
		// ---------------------

		Vector3& operator[](size_t i) { return data[i]; }
		const Vector3& operator[](size_t i) const { return data[i]; }
	};

	/// <summary>
	/// 4x4�s��
	/// </summary>
	struct Matrix4x4
	{
		Vector4 data[4];

		// --------------------
		// �R���X�g���N�^
		// --------------------

		// �f�t�H���g�R���X�g���N�^
		Matrix4x4() = default;

		/// <summary>
		/// Vec4�^�ŏ���������R���X�g���N�^
		/// </summary>
		Matrix4x4(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3)
		{
			data[0] = v0;
			data[1] = v1;
			data[2] = v2;
			data[3] = v3;
		}

		/// <summary>
		/// float�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit Matrix4x4(float f)
		{
			data[0] = { f, 0, 0, 0 };
			data[1] = { 0, f, 0, 0 };
			data[2] = { 0, 0, f ,0 };
			data[3] = { 0, 0, 0, f };
		}

		/// <summary>
		/// Mat3�^�ŏ���������R���X�g���N�^
		/// </summary>
		explicit Matrix4x4(const struct Matrix3x3& m)
		{
			data[0] = { m.data[0], 0 };
			data[1] = { m.data[1], 0 };
			data[2] = { m.data[2], 0 };
			data[3] = { 0, 0, 0, 1 };
		}

		// ------------------
		// �Y�����Z�q
		// ------------------

		Vector4& operator[](size_t i) { return data[i]; }
		const Vector4& operator[](size_t i) const { return data[i]; }
	};

#pragma endregion

	// ------------------------------
	// �I�y���[�^�[
	// ------------------------------

#pragma region Mat3_Operator

	// -----------------
	// ��Z
	// -----------------

	// --------- Mat3��Vec3�̏�Z ---------

	inline Vector3 operator*(const Matrix3x3& m, const Vector3& v)
	{
		return m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z;
	}

	// ---------- Mat3���m�̏�Z ----------

	inline Matrix3x3 operator*(const Matrix3x3& a, const Matrix3x3& b)
	{
		Matrix3x3 m = Matrix3x3(0);

		m.data[0] = a * b.data[0];
		m.data[1] = a * b.data[1];
		m.data[2] = a * b.data[2];

		return m;
	}

	inline Matrix3x3& operator*=(Matrix3x3& a, const Matrix3x3& b)
	{
		a = a * b;
		return a;
	}

#pragma endregion

#pragma region Mat4_Operator

	// ----------------
	// ��Z
	// ----------------

	// --------- Mat4��Vec4�̏�Z ----------

	inline Vector4 operator*(const Matrix4x4& m, const Vector4& v)
	{
		return m.data[0] * v.x + m.data[1] * v.y + m.data[2] * v.z + m.data[3] * v.w;
	}

	// ---------- Mat4���m�̏�Z -----------

	inline Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b)
	{
		Matrix4x4 m = Matrix4x4(0);

		m.data[0] = a * b.data[0];
		m.data[1] = a * b.data[1];
		m.data[2] = a * b.data[2];
		m.data[3] = a * b.data[3];

		return m;
	}

	inline Matrix4x4& operator*=(Matrix4x4& a, const Matrix4x4& b)
	{
		a = a * b;
		return a;
	}

#pragma endregion

	// ------------------------------
	// �֐�
	// ------------------------------

#pragma region Mat3_Function

	/// <summary>
	/// Mat4�^�ŏ���������R���X�g���N
	/// </summary>
	inline Matrix3x3::Matrix3x3(const Matrix4x4& m)
	{
		data[0] = Vector3(m.data[0]);
		data[1] = Vector3(m.data[1]);
		data[2] = Vector3(m.data[2]);
	}

	namespace Mat3_Function
	{
		/// <summary>
		/// ��]�p�x�����]�s����쐬
		/// </summary>
		/// <param name="rotation"> ��]�p�x </param>
		/// <returns> �쐬������]�s�� </returns>
		inline Matrix3x3 GetRotationMatrix(const Vector3& rotation)
		{
			/* �s��́A�x�N�g���z��Ɣ�ׂ�
			�u�c�Ɖ������]���Ă���v�̂�

			{  cosZ, -sinZ,  0 }
			{  sinZ,  cosZ,  0 }
			{     0,     0, -1 } 	
			
					 ��

			{  cosZ,  sinZ,  0 }
			{ -sinZ,  cosZ,  0 }
			{     0,     0, -1 } 

			�ɂȂ�
			
			X,Y,Z���̉�]�s������߂Ċ|���邱�ƂŁAXYZ����]�̍s�񂪋��߂���*/

			// X����]��Sin�ECos
			const float sinX = sin(rotation.x), cosX = cos(rotation.x);

			// Y����]��Sin�ECos
			const float sinY = sin(rotation.y), cosY = cos(rotation.y);

			// Z����]��Sin�ECos
			const float sinZ = sin(rotation.z), cosZ = cos(rotation.z);

			// X����]�̍s��
			const Matrix3x3 matrix_X = {
					   { 1,     0,      0 },
					   { 0,  cosX,  -sinX },
					   { 0,  sinX,   cosX } };

			// Y����]�̍s��
			const Matrix3x3 matrix_Y = {
					   {  cosY,  0, sinY },
					   {     0,  1,    0 },
					   { -sinY,  0, cosY } };

			// Z����]�̍s��
			const Matrix3x3 matrix_Z = {
							 {  cosZ, sinZ, 0 },
							 { -sinZ, cosZ, 0 },
							 {     0,    0, -1 } };

			// X,Y,Z����]���|�����킹��
			// XYZ����]�̍s��ɂ���
			return matrix_X * matrix_Y * matrix_Z;
		}
	}

#pragma endregion

#pragma region Mat4_Function

	namespace Mat4_Function
	{
		/// <summary>
		/// ���W�ϊ��s����쐬
		/// </summary>
		/// <param name="scale"> �g�嗦 </param>
		/// <param name="rotation"> ��]�p�x </param>
		/// <param name="position"> ���s�ړ��� </param>
		/// <returns> �쐬�������W�ϊ��s�� </returns>
		inline Matrix4x4 GetTransformMatrix(
			const Vector3& scale, const Vector3& rotation, const Vector3& position)
		{
			// ��]�𔽉f
			const Matrix3x3 m = Mat3_Function::GetRotationMatrix(rotation);

			// �g�嗦�ƕ��s�ړ��ʂ𔽉f
			return {
				Vector4{ scale.x * m.data[0], 0 },
				Vector4{ scale.y * m.data[1], 0 },
				Vector4{ scale.z * m.data[2], 0 },
				Vector4{ position, 1 } };
		}

		/// <summary>
		/// 4x4�s�񂩂�g�嗦�𒊏o����
		/// </summary>
		/// <param name="transform"> �g�嗦�𒊏o�������s�� </param>
		/// <returns> �g�嗦 </returns>
		inline Vector3 ExtractScale(const Matrix4x4& transform)
		{
			// ����3x3�����̊e�s��3D�x�N�g���Ƃ��Ē��������߂邱�Ƃ�
			// �g�嗦�����߂�
			Vector3 a = Vector3(transform[0]);
			Vector3 b = Vector3(transform[1]);
			Vector3 c = Vector3(transform[2]);

			const float sx = a.Length();
			const float sy = b.Length();
			const float sz = c.Length();

			return { sx, sy, sz };
		}
	}

#pragma endregion

}

#endif // !MATRIX_H_INCLUDED
