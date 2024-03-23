/**
* @file EasyAudio.h
*/
#ifndef EASYAUDIO_H_INCLUDED
#define EASYAUDIO_H_INCLUDED

/**
* ���̃t�@�C���͐搶�����������
* �׋��p�ɒu���Ă������ǁA
* �����ł����ƍ�蒼����
*/

/// <summary>
/// �����֘A�̃R�[�h���i�[���閼�O���
/// </summary>
namespace EasyAudio
{
	struct Vector { float x, y, z; };

	bool Initialize();
	void Finalize();
	void Update();

	// �v���C���[���w�肵�čĐ��E��~
	void Play(int playerId, const char* filename, float volume = 1.0f, bool isLoop = false);
	void Stop(int playerId);
	void Pause(int playerId);  // �ꎞ��~
	void Resume(int playerId); // �ĊJ(�ꎞ��~������)

	// ���ʂ̐ݒ�E�擾
	void SetVolume(int playerId, float volume);
	float GetVolume(int playerId);

	// �s�b�`�̐ݒ�E�擾
	void SetPitch(int playerId, float pitch);
	float GetPitch(int playerId);

	// �p���̐ݒ�E�擾
	void SetPan(int playerId, float pan);
	float GetPan(int playerId);

	// �^���I��3D����
	void SetListenr(const Vector& position, const Vector& right);
	void SetPanAndVolumeFromPosition(int playerId, const Vector& position, float volume);

	// �v���C���[���Đ������ǂ���(�Đ����Ȃ�true�A����ȊO��false)
	bool IsPlaying(int playerId);

	// �P���Đ�
	void PlayOneShot(const char* filename);
	void PlayOneShot(const char* filename, float volume);
	void PlayOneShot(const char* filename, float volume, float pitch);

	// �}�X�^�[�{�����[���̐ݒ�E�擾
	void SetMasterVolume(float volume);
	float GetMasterVolume();

} // namespace EasyAudio

#endif // EASYAUDIO_H_INCLUDED