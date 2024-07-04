/**
* @file Matrix.cpp
*/
#include "Matrix.h"

namespace PokarinEngine
{
#pragma region Matrix3x3

	/// <summary>
	/// Mat4�^�ŏ���������R���X�g���N
	/// </summary>
	Matrix3x3::Matrix3x3(const Matrix4x4& m)
	{
		data[0] = Vector3(m.data[0]);
		data[1] = Vector3(m.data[1]);
		data[2] = Vector3(m.data[2]);
	}

	/// <summary>
	/// ��]�p�x�����]�s����쐬
	/// </summary>
	/// <param name="[in] rotation"> ��]�p�x </param>
	/// <returns> �쐬������]�s�� </returns>
	Matrix3x3 Matrix3x3::CreateRotationMatrix(const Vector3& rotation)
	{
		/* X,Y,Z���̉�]�s������߂Ċ|���邱�ƂŁAXYZ����]�̍s�񂪋��߂��� 
		
		�E��]�ɂ������̂ŁA�E����W�n�̉�]�s��ŏ���
		�������AOpenGL���E����W�n�ł��邽��Z�����}�C�i�X�Ɍ������Ă���
		�Ȃ̂ŁAZ������������W�n�̉�]�s��ɂ��ĉE��]�ɂȂ�悤�ɂ��� */

		// X����]��Sin�ECos
		const float sinX = sin(rotation.x), cosX = cos(rotation.x);

		// Y����]��Sin�ECos
		const float sinY = sin(rotation.y), cosY = cos(rotation.y);

		// Z����]��Sin�ECos
		const float sinZ = sin(rotation.z), cosZ = cos(rotation.z);

		// X����]�̍s��
		// �E����W�n�̉�]
		const Matrix3x3 matrix_X = {
				   { 1,     0,      0 },
				   { 0,  cosX,  -sinX },
				   { 0,  sinX,   cosX } };

		// Y����]�̍s��
		// �E����W�n�̉�]
		const Matrix3x3 matrix_Y = {
				   {  cosY,  0,  sinY },
				   {     0,  1,     0 },
				   { -sinY,  0,  cosY } };

		// Z����]�̍s��
		// ������W�n�̉�](Z���̕����ɑΉ����邽��)
		// -1�͍�����W�n�̃I�u�W�F�N�g�ɑΉ����邽��
		const Matrix3x3 matrix_Z = {
					{  cosZ,  sinZ,  0 },
					{ -sinZ,  cosZ,  0 },
					{     0,     0, -1 } };

		// X,Y,Z����]���|�����킹��XYZ����]�̍s��ɂ���
		// Y���̓��[���h���ŉ�]���Ăق����̂ŁAYXZ�̏��ɂ���
		return matrix_Y * matrix_X * matrix_Z;
	}

#pragma endregion

#pragma region Matrix4x4

	/// <summary>
	/// ���W�ϊ��s����쐬����
	/// </summary>
	/// <param name="[in] translate"> ���s�ړ��� </param>
	/// <param name="[in] rotation"> ��]�p�x </param>
	/// <param name="[in] scale"> �g�嗦 </param>
	/// <returns> �쐬�������W�ϊ��s�� </returns>
	Matrix4x4 Matrix4x4::CreateTransformMatrix(
		const Vector3& translate, const Vector3& rotation, const Vector3& scale)
	{
		// ��]�𔽉f
		const Matrix3x3 m = Matrix3x3::CreateRotationMatrix(rotation);

		// �g�嗦�ƕ��s�ړ��ʂ𔽉f
		return {
			Vector4{ scale.x * m.data[0], 0 },
			Vector4{ scale.y * m.data[1], 0 },
			Vector4{ scale.z * m.data[2], 0 },
			Vector4{ translate, 1 } };
	}

	/// <summary>
	/// ���W�ϊ��s�񂩂�ړ��ʂ��擾����
	/// </summary>
	/// <param name="[in] transform"> ���W�ϊ��s�� </param>
	/// <returns> ���s�ړ��� </returns>
	Vector3 Matrix4x4::Translate(const Matrix4x4& transform)
	{
		// �Ō�̗v�f�ɕ��s�ړ��ʂ�ݒ肵�Ă���̂ŁA�����Ԃ�
		return Vector3(transform[3]);
	}

	/// <summary>
	/// ���W�ϊ��s�񂩂�g�嗦���擾����
	/// </summary>
	/// <param name="[in] transform"> ���W�ϊ��s�� </param>
	/// <returns> �g�嗦 </returns>
	Vector3 Matrix4x4::Scale(const Matrix4x4& transform)
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

	/// <summary>
	/// ���W�ϊ��s�񂩂��]�s����擾����
	/// </summary>
	/// <param name="[in] transform"> ���W�ϊ��s�� </param>
	/// <returns> ��]�s�� </returns>
	Matrix3x3 Matrix4x4::Rotate(const Matrix4x4& transform)
	{
		// �g�嗦
		Vector3 scale = Scale(transform);

		// ��]�s��
		Matrix3x3 rotate = Matrix3x3(transform);

		// ���W�ϊ��s����쐬����ۂɁA
		// ��]�s��Ɋg�嗦���|���Ă���̂ŁA�g�嗦�Ŋ����ċ��߂�
		rotate[0] /= scale.x;
		rotate[1] /= scale.y;
		rotate[2] /= scale.z;

		return rotate;
	}

#pragma endregion

}