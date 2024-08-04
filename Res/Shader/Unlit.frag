/**
* @file Unlit.frag
*/
#version 450

// -----------------------
// �V�F�[�_�ւ̓���
// -----------------------

layout(location = 0) in vec3 inPosition; // ���[���h���W
layout(location = 1) in vec2 inTexcoord; // �e�N�X�`�����W

// -------------------------
// �e�N�X�`���T���v��
// -------------------------

// �e�N�X�`���C���[�W���j�b�g�Ɋ��蓖�Ă�ꂽ�e�N�X�`�����g���ɂ�
// sampler�^�̃��j�t�H�[���ϐ����g��
// ���j�b�g�ԍ���binding���ʎq�Ŏw��

// �e�N�X�`��
layout(binding = 0) uniform sampler2D texColor;

// �G�~�b�V�����e�N�X�`��
layout(binding = 1) uniform sampler2D texEmission;

// ----------------------------
// �v���O��������̓���
// ----------------------------

// �I�u�W�F�N�g�̐F
layout(location = 100) uniform vec4 color;		

// ���̂̔����F��
// �G�~�b�V�����e�N�X�`���̎��ʔԍ�
layout(location = 101) uniform vec4 emission;

// ------------------------
// �V�F�[�_����̏o��
// ------------------------

// �o�͂���F�f�[�^
out vec4 outColor;

void main()
{
	// -------------------
	// �e�N�X�`���̐ݒ�
	// -------------------

	// �T�v���C�ϐ����Q�Ƃ��郆�j�b�g�Ɋ��蓖�Ă�ꂽ�e�N�X�`������A
	// �e�N�X�`�����W�̃s�N�Z���l��ǂݎ���ĕԂ�
	vec4 c = texture(texColor, inTexcoord);

	// CRT���j�^�[�̃K���}�l
	const float crtGamma = 2.2; 

	// �قƂ�ǂ̉摜�́A���������邳�ŏo�͂����悤�ɁA
	// 1/2.2��̖��邳�ō쐬����Ă���̂ŁA�K���}�l�ŏC��
	c.rgb = pow(c.rgb, vec3(crtGamma)); 

	// �K���}�l�ŏC�������F���o��
	outColor = c * color;

	// ------------- �����F�𔽉f --------------
	
	// w�v�f�Ɏ��ʔԍ��������Ă��邽�߁A
	// w�v�f��0���傫���ꍇ�A
	//�u�G�~�b�V�����e�N�X�`������v�Ƃ݂Ȃ�
	if(emission.w > 0)
	{
		// �e�N�X�`���𔽉f����
		outColor.rgb += texture(texEmission, inTexcoord).rgb * emission.rgb;
	}
	// �G�~�b�V�����e�N�X�`�����Ȃ��ꍇ
	else
	{
		// �����F�̂ݔ��f����
		outColor.rgb += emission.rgb;
	}

	// �K���}�␳���s��
	// ��ʂɕ\������閾�邳�͓��͒l��2.2��ɂȂ�Ƃ���
	// �u���E����(CRT)���j�^�[�̓���������
	// �Ȃ̂ŁA���C�g�̖��邳��1/2.2�悷�邱�ƂŁA���邳���C������
	outColor.rgb = pow(outColor.rgb, vec3(1 / crtGamma));
}
