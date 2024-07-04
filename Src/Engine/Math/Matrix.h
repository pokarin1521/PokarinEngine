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
	public: // -------------------- �R���X�g���N�^ --------------------

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
		explicit Matrix3x3(const Matrix4x4& m);

	public: // ------------------------- �֐� -------------------------

		/// <summary>
		/// ��]�p�x�����]�s����쐬
		/// </summary>
		/// <param name="[in] rotation"> ��]�p�x </param>
		/// <returns> �쐬������]�s�� </returns>
		static Matrix3x3 CreateRotationMatrix(const Vector3& rotation);

	public: // ---------------------- �Y�����Z�q ----------------------

		Vector3& operator[](size_t i) { return data[i]; }
		const Vector3& operator[](size_t i) const { return data[i]; }

	public: // ------------------------- �ϐ� -------------------------

		Vector3 data[3];
	};

	/// <summary>
	/// 4x4�s��
	/// </summary>
	struct Matrix4x4
	{
	public: // -------------------- �R���X�g���N�^ --------------------

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

	public: // ------------------------- �֐� -------------------------

		/// <summary>
		/// ���W�ϊ��s����쐬����
		/// </summary>
		/// <param name="[in] translate"> ���s�ړ��� </param>
		/// <param name="[in] rotation"> ��]�p�x </param>
		/// <param name="[in] scale"> �g�嗦 </param>
		/// <returns> �쐬�������W�ϊ��s�� </returns>
		static Matrix4x4 CreateTransformMatrix(
			const Vector3& translate, const Vector3& rotation, const Vector3& scale);

		/// <summary>
		/// ���W�ϊ��s�񂩂�ړ��ʂ��擾����
		/// </summary>
		/// <param name="[in] transform"> ���W�ϊ��s�� </param>
		/// <returns> ���s�ړ��� </returns>
		static Vector3 Translate(const Matrix4x4& transform);

		/// <summary>
		/// ���W�ϊ��s�񂩂�g�嗦���擾����
		/// </summary>
		/// <param name="[in] transform"> ���W�ϊ��s�� </param>
		/// <returns> �g�嗦 </returns>
		static Vector3 Scale(const Matrix4x4& transform);
	
		/// <summary>
		/// ���W�ϊ��s�񂩂��]�s����擾����
		/// </summary>
		/// <param name="[in] transform"> ���W�ϊ��s�� </param>
		/// <returns> ��]�s�� </returns>
		static Matrix3x3 Rotate(const Matrix4x4& transform);

	public: // ---------------------- �Y�����Z�q ----------------------

		Vector4& operator[](size_t i) { return data[i]; }
		const Vector4& operator[](size_t i) const { return data[i]; }

	public: // ------------------------- �ϐ� -------------------------

		Vector4 data[4];
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

}

#endif // !MATRIX_H_INCLUDED
