/**
* @file Standard.vert
*/
#version 450

// ---------------------
// �V�F�[�_�ւ̓���
// ---------------------

layout(location = 0) in vec3 inPosition; //	���_���W
layout(location = 1) in vec2 inTexcoord; // �e�N�X�`�����W
layout(location = 2) in vec3 inNormal;   // �@���x�N�g��

// -----------------------
// �V�F�[�_����̏o��
// -----------------------

layout(location = 0) out vec3 outPosition; // ���[���h���W
layout(location = 1) out vec2 outTexcoord; // �e�N�X�`�����W
layout(location = 2) out vec3 outNormal; // �@���x�N�g��

// ---------------------------
// �v���O��������̓���
// ---------------------------

/* �v���O��������V�F�[�_�Ƀf�[�^�𑗂�ɂ̓��j�t�H�[���ϐ����g��
���P�[�V�����ԍ��̓f�[�^�̑����ԍ�
�C���ϐ��A�A�E�g�ϐ��Ƃ͕ʂ̏ꏊ�ɍ���邽�ߏd���� */

// ���W�ϊ��x�N�g���̍s��
layout(location = 0) uniform mat4 transformMatrix;

// �@���ϊ��x�N�g���̍s��
layout(location = 1) uniform mat3 normalMatrix;

// �A�X�y�N�g��Ǝ���p�ɂ��g�嗦
layout(location = 3) uniform vec2 aspectRatioAndScaleFov;

// �J�����̈ʒu
layout(location = 4) uniform vec3 cameraPosition;

// �J�����̉�]
layout(location = 5) uniform vec3 cameraRotation; 

/**
* �x�N�g����Z��->X��->Y���̏��ŉ�]������
* 
* @param v				��]������x�N�g��
* @param sinRotation	�e����Sin
* @param cosRotation    �e����Cos
* 
* @return �x�N�g��v���AZ��->X��->Y���̏��Ԃŉ�]�������x�N�g��
*/
vec3 RotateZXY(vec3 v, vec3 sinRotation, vec3 cosRotation)
{
	/* ��]�̏����͏d�v�ŁA�����p�x���w�肵�Ă��A
	��]�����鏇�����قȂ�ƑS���Ⴄ�����ɂȂ��Ă��܂� 
	���̂��߁A���񓯂������ŉ�]�����邱�Ƃ��d�v
	
	�����́u�p������̂��₷���v���l���đI��
	��ʓI�ɁuZ->X->Y�v�̏��������₷���Ƃ���� 
	
	3D���f�����쐬�����Ƃ��̌�����A
	���[���h���W�n�̎��̒�`�ɂ���ẮA�ʂ̏����̕��������ꍇ������ */

	// -------------------------------
	// Y->X->Z�̏��ŉ�]���v�Z����
	// -------------------------------

	/* �J�����ƃI�u�W�F�N�g�̉�]�������t�ɂȂ�̂ŁA
	ZXY�̏��ŉ�]���������Ƃ��́A�J������YXZ�̏��ŉ�]�����Ȃ��Ƃ����Ȃ� */

	// ---------- Y����] -----------

	v.xz = vec2(
		v.x * cosRotation.y + v.z * sinRotation.y,
		-v.x * sinRotation.y + v.z * cosRotation.y);

	// ---------- X����] -----------

	v.yz = vec2(
		v.z * -sinRotation.x + v.y * cosRotation.x,
		v.z * cosRotation.x + v.y * sinRotation.x); 

	// ---------- Z����] -----------
	
	v.xy = vec2(
		v.x * cosRotation.z + v.y * sinRotation.z,
		v.x * -sinRotation.z + v.y * cosRotation.z);

	return v;
}

void main()
{
	// �e�N�X�`�����W���t���O�����g�V�F�[�_�ɓn��
	outTexcoord = inTexcoord;

	// ------------------------------------------
	// ���[�J�����W�n���烏�[���h���W�n�ɕϊ�
	// ------------------------------------------

	// �g�嗦�AX��Y���̉�]�A���s�ړ��𔽉f
	// �v�Z���܂Ƃ߂����ʁA�x�N�g�����m�̏�Z�ō��W�ϊ��ł���悤�ɂȂ���
	// �x�N�g���̔z����s��^�Ŏ󂯎���Ă�̂ŁA�s�� * �x�N�g���Ōv�Z
	gl_Position = transformMatrix * vec4(inPosition, 1);

	// -------- �t���O�����g�V�F�[�_�Ƀ��[���h���W��n�� -------
	
	outPosition = gl_Position.xyz;

	// ------------ ���[���h���W�n�̖@�����v�Z ---------------

	// �g�嗦�AX��Y���̉�]�𔽉f
	// �v�Z���܂Ƃ߂����ʁA�x�N�g�����m�̏�Z�ō��W�ϊ��ł���悤�ɂȂ���
	// �x�N�g���̔z����s��^�Ŏ󂯎���Ă�̂ŁA�s�� * �x�N�g���Ōv�Z
	outNormal = normalMatrix * inNormal;

	// ------------------------------------------------------
	// ���[���h���W�n����r���[���W�n�ɕϊ�(�J�������W�n)
	// ------------------------------------------------------

	/* �J��������]���� = �J�����𒆐S�ɃI�u�W�F�N�g����]����
	�Ƃ������ƂȂ̂ŁA�J���������_�Ƃ��ăI�u�W�F�N�g����]������ */

	// �J��������̑��΍��W
	vec3 pos = gl_Position.xyz - cameraPosition;

	// �J�����̉�]�𔽉f
	pos = RotateZXY(pos, sin(cameraRotation), cos(cameraRotation));

	// �J�����̉�]�𔽉f�������W����
	gl_Position.xyz = pos;

	// ---------------------------------------------
	// �r���[���W�n����N���b�v���W�n�ɕϊ�
	// ---------------------------------------------

	/* �N���b�v���W�n�́A
	��ʂɉf��Ȃ��������������鏈��(�N���b�s���O)�̂��߂̍��W�n
	
	���_�V�F�[�_(�e�b�Z���[�^��W�I���g���V�F�[�_���g���ꍇ�A
	�����̒��ōŌ�Ɏ��s�����V�F�[�_)����o�͂������W�́A
	���̍��W�n�Œ�`����Ă��Ȃ���΂Ȃ�Ȃ� */

	// ------------ x���W���A�X�y�N�g��Ŋ��� --------------
	// ------------ �c���̔䗦��1:1�ɂ���	  --------------
	
	// CPU���� 1 / �A�X�y�N�g�� �����Ă���̂ŏ�Z
	const float aspectRatio = aspectRatioAndScaleFov.x;
	gl_Position.x *= aspectRatio;

	// --------- x,y���W���ꂼ�������p�ɂ��g�嗦�Ŋ��� -----------
	// --------- �\������ʒu�𒲐�(����p�𔽉f)		   -----------

	// CUP���� 1 / ����p�ɂ��g�嗦 �����Ă���̂ŏ�Z
	const float scaleFov = aspectRatioAndScaleFov.y;
	gl_Position.xy *= scaleFov;

	// -------- �[�x�l�̌v�Z���ʂ�-1�`+1�ɂȂ�悤�� ------------
	// -------- �p�����[�^A,B���v�Z					 ------------

	/* OpenGL�̐[�x�o�b�t�@�́A�ŏ��l(��O)��-1�A�ő�l(��)��+1�ƂȂ�
	�����ŁA�[�x�l��near�̂Ƃ�-1�Afar�̂Ƃ�+1�ƂȂ�悤�Ȍv�Z�������߂�
	(gl_Position.z / gl_Position.w �̌��ʂ�-1 �` +1�ɂȂ�悤�ɂ���)
	
	�萔A,B������Ƃ��āA���̏����𖞂����������߂�
	
	-1 <= (A * �[�x�l + B) <= +1 
	
	�������A���ۂɂ͐[�x�l�ł͂Ȃ��A
	gl_Position.w�ŏ��Z�������̂��v�Z���ʂɂȂ�
	w�͐[�x�l���̂��̂Ȃ̂ŁA���������͎��̂悤�ɂȂ�
	
	-1 <= (A / �[�x�l + B) <= +1
	
	�[�x�l��near�̂Ƃ�-1�Afar�̂Ƃ�+1�ɂȂ�̂�����
	
	A / near + B = -1
	A / far + B = 1  
	
	�𖞂���A��B�����߂�΂悢���ƂɂȂ�
	�v�Z�����
	
	A = 2 * far * near / (near - far)
	B = (far + near) / (far - near) 
	
	�Ō�ɕ�������킹�邽�߂ɁAA�̎��̕��q�����-1���|����
	
	A = -2 * far * near / -(near - far)
	A = -2 * far * near / (far - near)
	
	�����A��B�����߂邱�Ƃ��ł��� 
	
	A = -2 * far * near / (near - far)
	B = (far + near) / (far - near) */
	
	// �ŏ��`��͈�
	// �I�u�W�F�N�g�̒��g�������Ȃ��悤��
	// �J�����̏Փ˔���̔��a�́ucos45�v�{�ɂ���
	const float near = 0.35f; 
	
	// �ő�`��͈�
	const float far = 1000;

	// -1 <= A * �[�x�l + B <= 1 �ƂȂ�
	// �萔A,B�����߂�
	const float A = -2 * far * near / (far - near);
	const float B = (far + near) / (far - near);
	
	// ------------ ���ߖ@��L���ɂ��� -------------

	/* GPU��gl_Positoin /= gl_Position.w 
	(�N���b�v���W�n��NDC���W�n�ɕϊ�)������̂ŁA
	w�ɋ���(z)�������A�����������ق�xy�̒l���������Ȃ�悤�ɂ���

	OpenGL�͉E����W�n�ŁAGPU�͍�����W�n�Ȃ̂ŕ����𔽓]������ */
	gl_Position.w = -gl_Position.z;
	
	/* ���߂��萔A,B���g���āA�[�x�l��␳����
	
	gl_Position.z / gl_Position.w = A / gl_Position.w + B
	
	���ŏI�I�Ȍ��ʂȂ̂ŁA��������z�̒l�����߂� */

	gl_Position.z = -gl_Position.z * B + A;
}