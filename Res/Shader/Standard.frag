/**
* @file Standard.frag
*/
#version 450

// -----------------------
// �V�F�[�_�ւ̓���
// -----------------------

layout(location = 0) in vec3 inPosition; // ���[���h���W
layout(location = 1) in vec2 inTexcoord; // �e�N�X�`�����W
layout(location = 2) in vec3 inNormal;   // �@���x�N�g��

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

/* ���j�t�H�[���ϐ��̃��P�[�V�����ԍ��͑S�X�e�[�W���L�Ȃ̂�
���_�V�F�[�_�Əd�����Ȃ��悤��100����w�肷�� */

// �I�u�W�F�N�g�̐F
layout(location = 100) uniform vec4 color;		

// ���̂̔����F��
// �G�~�b�V�����e�N�X�`���̊Ǘ��ԍ�
layout(location = 101) uniform vec4 emission;

// ����

// ���s����
struct DirectionalLight
{
	vec3 color;		// �F�Ɩ��邳
	vec3 direction; // ���̌���
};

// ���s����
layout(location = 108) uniform DirectionalLight directionalLight;


// ���C�g
// �_�����ƃX�|�b�g���C�g�ɑΉ�
struct Light
{
	// ���邳�𔽉f�����F, �����J�n�p�x
	vec4 colorAndFalloffAngle[16];	
	
	// ���W�Ɣ͈�(���a)
	vec4 positionAndRange[16];		
	
	// ����, �ő�Ǝˊp�x
	vec4 directionAndSpotAngle[16];
};

// ���C�g�̐�
layout(location = 110) uniform int lightCount;

// ���j�t�H�[���ϐ��ɍ\���̂�z����w�肷��ƁA
// �����o�܂��͔z��v�f���ƂɃ��P�[�V�����ԍ�������U����
// (����́Acolor = 111��, positionAndRange = 112��)

// ���C�g(�|�C���g�E�X�|�b�g)
layout(location = 111) uniform Light light;

// �o�͂���F�f�[�^
out vec4 outColor;

void main()
{
	// -------------------------
	// �e�N�X�`���̐ݒ�
	// -------------------------

	// �T�v���C�ϐ����Q�Ƃ��郆�j�b�g�Ɋ��蓖�Ă�ꂽ�e�N�X�`������A
	// �e�N�X�`�����W�̃s�N�Z���l��ǂݎ���ĕԂ�
	vec4 c = texture(texColor, inTexcoord);

	// �قƂ�ǂ̉摜�́A���������邳�ŏo�͂����悤�ɁA
	// 1/2.2��̖��邳�ō쐬����Ă���̂ŁA�K���}�l�ŏC��
	const float crtGamma = 2.2; // CRT���j�^�[�̃K���}�l
	c.rgb = pow(c.rgb, vec3(crtGamma)); 

	outColor = c * color;

	// -----------------------------
	// �@���̐��K��
	// -----------------------------

	/* ���_�V�F�[�_�̏o�͕ϐ��̒l�́A
	���̂܂܃t���O�����g�V�F�[�_�ɃR�s�[�����킯�ł͂Ȃ�

	���_�V�F�[�_�̌v�Z���ʂ́u���_���W�ɂ�����l�v
	�������A�t���O�����g�V�F�[�_��
	�u���_�Ԃɂ���S�Ẵs�N�Z���v�ɑ΂��Ď��s����� 
	
	�����ŁA�u�s�N�Z�����璸�_�܂ł̋����ɉ����Ē��_�̒l�������v���邱�ƂŁA
	���ԗ̈�ɂ���s�N�Z���̒l�����o��
	���̑���́u���`��ԁv�Ƃ������@�ōs����

	�Ⴆ�΁A���_�V�F�[�_��outColor�ɒ��_�̐F���������݁A
	�t���O�����g�V�F�[�_��inColor�Ŏ󂯎��ꍇ

	���_A����(1, 0, 0)�A���_B����(0, 0, 1)��outColor�ɑ�������Ƃ��A
	A��B�̒��Ԃɂ���s�N�Z����inColor�ɂ�(0.5, 0, 0.5)�Ƃ����l���������� */

	// ���`��Ԃɂ���Ē�����1�ł͂Ȃ��Ȃ��Ă���̂ŁA
	// ���K�����Ē�����1�ɕ�������
	// (���ς�cos�Ƃ����߂邽��)
	vec3 normal = normalize(inNormal);

	// ----------------------------------------
	// ���̖��邳���v�Z(�����o�[�g����)
	// ----------------------------------------

	/* �������E�̌����Č�����ɂ͖c��Ȍv�Z���K�v�Ŏ��Ԃ�������A
	�Q�[���̂悤�ȃ��A���^�C���������߂��镪��ɂ͎g���Ȃ�
	�Ȃ̂ŁA�u�ȒP�Ȍv�Z�ł�����ۂ������ڂɂȂ���@�v���g�� 
	
	����́A�����o�[�g���˂��g��
	���̂����˂�����́u�g�U���ˁv�Ɓu���ʔ��ˁv�ɑ�ʂ����
	�����o�[�g���˂́u�g�U���ˁv���Č�����
	
	�����o�[�g���˂�
	�u���ˌ��͕��̂̕\�ʂőS���ʂɋϓ��ɔ��˂���v�Ɖ��肵�Ă���
	(�\�ʂ����ɍr�����̂̔���) */
	
	// �g�U���̖��邳�̍��v
	vec3 diffuse = vec3(0);

	// ���C�g�̌v�Z
	for(int i = 0; i < lightCount; ++i)
	{
		// ----------------------------
		// �v�Z�ɕK�v�Ȓl�����߂� 
		// ----------------------------

		// �|�C���g���C�g�̍��W
		vec3 lightPosition = light.positionAndRange[i].xyz;

		// �t���O�����g���猩���|�C���g���C�g�̕���
		vec3 direction = lightPosition - inPosition;

		// �����܂ł̋���
		float sqrDistance = dot(direction, direction);
		float distance = sqrt(sqrDistance);

		// �����𐳋K�����Ē�����1�ɂ���
		// (���ς�cos�Ƃ����߂邽��)
		direction = normalize(direction);

		// ----------------------------------------------
		// �����x���g�̗]�������g���Ė��邳���v�Z 
		// ----------------------------------------------
		
		/* �����o�[�g���˂̌v�Z�Ɂu�����x���g�̗]�����v�Ƃ����@���𗘗p����
		
		----- �����x���g�̗]���� -----
		���̂̂���_�Ŕ��˂�����(���ˌ�)�̋����́A
		���̓_�̖@���ƌ��������̂Ȃ��p�Ƃ̗]��(cos)�Ɛ���Ⴗ�� */

		/* �Ȃ��p�͈̔͂�0 �` 90�Ȃ̂ŁA
		max�֐���cos�Ƃ�0�ȏ�ɂȂ�悤�ɂ��Ă���
		(�x�N�g���𐳋K�����Ă���̂ŁAdot�̌��ʂ�cos�ƂɂȂ�) */

		// �����̕����x�N�g���Ɩ@���x�N�g���̂Ȃ��p
		float theta = max(dot(direction, normal), 0);

		/* �g�U���˂ł͓��ˌ����u�S���ʂɋϓ��ɔ��ˁv�����
		���̂Ƃ��A��������ɔ��˂�����̗ʂ͓��ˌ��́u1/�Δ{�v�ɂȂ�
		���̂��߁A���������邳�𓾂�ɂ͓��ς̌��ʂ��΂Ŋ���K�v������ */

		// �Ɠx
		float illnuminance = theta / 3.14159265;

		// ----------- �X�|�b�g���C�g���ǂ����m�F  -------------

		// �ő�Ǝˊp�x
		const float spotAngle = light.directionAndSpotAngle[i].w;

		// �Ǝˊp�x��0���傫����΃X�|�b�g���C�g�Ƃ݂Ȃ�
		if(spotAngle > 0)
		{
			// �X�|�b�g���C�g�̌���
			vec3 lightDiretion = light.directionAndSpotAngle[i].xyz;

			//�u���C�g����t���O�����g�֌������x�N�g���v��
			//�u�X�|�b�g���C�g�̌����x�N�g���v�̂Ȃ��p���v�Z
			float angle = acos(dot(-direction, lightDiretion));

			// �p�x��spotAngle�ȏ�Ȃ�͈͊O
			if(angle >= spotAngle)
			{
				// �͈͊O�Ȃ�v�Z�̕K�v�͂Ȃ�
				continue;
			}

			// �����J�n�p�x
			const float falloffAngle = light.colorAndFalloffAngle[i].w;

			// �ő�Ǝˊp�x�̂Ƃ�0, 
			// �����J�n�p�x�̂Ƃ�1�ɂȂ�悤�ɕ��
			const float a = min(
				(spotAngle - angle) / (spotAngle - falloffAngle), 1);

			illnuminance *= a;

		} // if spotAngle

		// --------- ���C�g�̍ő勗���𐧌�				   ---------
		// --------- �v�Z��UE4�Ȃǂō̗p����Ă��鎮���g�� ---------

		/* 
				  saturate(1 - (distance / lightRadius)^4)^2
		fallof = ----------------------------------------------
								distance^2 + 1                    
		*/
	
		// ���C�g�͈̔�(���a)
		const float range = light.positionAndRange[i].w;

		// clamp�֐��ŁA0 �` 1�͈̔͂ɂȂ�悤�ɂ���
		const float smoothFactor = clamp(1 - pow(distance / range, 4), 0, 1);

		// ���C�g�̍ő勗���𐧌�
		illnuminance *= smoothFactor * smoothFactor;

		// �t2��̖@���ɂ���Ė��邳������������
		// 0���Z���N���Ȃ��悤��1�𑫂��Ă���
		illnuminance /= sqrDistance + 1;

		// ���邳�𔽉f�����F
		vec3 lightColor = light.colorAndFalloffAngle[i].xyz;

		// �g�U���̖��邳�����Z(�Ɠx * ���C�g�̖��邳)
		diffuse += lightColor * illnuminance;

	} // for lightCount

	// --------- ���s�����̖��邳���v�Z ---------

	/* ���s�����ɂ́u�����̍��W(���˓_)�v���Ȃ��̂ŁA
	���̌����ɂ́u���s�����̌����v�����̂܂܎g��
	
	���邳�́u�@���ƌ����̓��ρv�ŋ��߂� */

	/* �Ȃ��p�͈̔͂�0 �` 90�Ȃ̂ŁA
	max�֐���cos�Ƃ�0�ȏ�ɂȂ�悤�ɂ��Ă���
	(�x�N�g���𐳋K�����Ă���̂ŁAdot�̌��ʂ�cos�ƂɂȂ�) */

	// ���s�����̕����x�N�g���Ɩ@���x�N�g���̂Ȃ��p
	// �u�t���O�����g���猩�����̌����v���K�v�Ȃ̂Ō������t�ɂ���
	float theta = max(dot(-directionalLight.direction, normal), 0);

	/* �g�U���˂ł͓��ˌ����u�S���ʂɋϓ��ɔ��ˁv�����
	���̂Ƃ��A��������ɔ��˂�����̗ʂ͓��ˌ��́u1/�Δ{�v�ɂȂ�
	���̂��߁A���������邳�𓾂�ɂ͓��ς̌��ʂ��΂Ŋ���K�v������ */

	// �Ɠx
	float illnuminance = theta / 3.14159265;

	// �g�U���̖��邳�����Z
	diffuse += directionalLight.color * illnuminance;

	// ---------- �g�U���̉e���𔽉f ------------

	outColor.rgb *= diffuse;

	// ------------- �����F�𔽉f --------------
	
	// w�v�f�ɊǗ��ԍ��������Ă��邽�߁A
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
	outColor.rgb = pow(outColor.rgb, vec3(1 / 2.2));
}
