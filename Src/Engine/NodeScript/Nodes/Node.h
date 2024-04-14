/**
* @file Node.h
*/
#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "../../UsingNames/UsingNodeEditor.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// �m�[�h�̊��N���X
	/// </summary>
	class Node
	{
	public: // ------------------ �R���X�g���N�^�E�f�X�g���N�^ -------------------

		Node() = default;
		virtual ~Node() = default;

	public: // ------------------------------- ���� ------------------------------

		/// <summary>
		/// �m�[�h�쐬���̏���
		/// </summary>
		/// <param name="nodeEditor"> ������ł���m�[�h�G�f�B�^ </param>
		void CreateNode(NodeEditor* nodeEditor);

		/// <summary>
		/// �m�[�h��\������
		/// </summary>
		void Render();

	public: // -------------------------- ���ʔԍ��̐ݒ� --------------------------

		/// <summary>
		/// ���ݒ�̏ꍇ�����m�[�h�̎��ʔԍ���ݒ肷��
		/// </summary>
		void SetID_OnlyOnce(int setID);

	public: // -------------------------------- ���O -------------------------------

		// �m�[�h�̖��O
		inline static const char* name = "No Name";

	protected: // ----------------------- ���ʔԍ��̎擾 -------------------------

		/// <summary>
		/// �m�[�h�̎��ʔԍ����擾����
		/// </summary>
		/// <returns> �m�[�h�̎��ʔԍ� </returns>
		int GetID() const
		{
			return id;
		}

		/// <summary>
		/// �d�����Ȃ����o�͗p�s���̎��ʔԍ����擾����
		/// </summary>
		/// <returns> �d�����Ȃ����ʔԍ� </returns>
		int GetSinglePinID();

	protected: // ------------------------ �m�[�h�\���p --------------------------

		/// <summary>
		/// �m�[�h�̓��͗p�s����ݒ肷��
		/// </summary>
		/// <param name="pinID"> �ݒ肷��s���̎��ʔԍ� </param>
		/// <param name="pinName"> �ݒ肷��s���̖��O </param>
		void SetInputPin(int pinID, const char* pinName);

		/// <summary>
		/// �m�[�h�̏o�͗p�s����ݒ肷��
		/// </summary>
		/// <param name="pinID"> �ݒ肷��s���̎��ʔԍ� </param>
		/// <param name="pinName"> �ݒ肷��s���̖��O </param>
		void SetOutputPin(int pinID, const char* pinName);

		/// <summary>
		/// �m�[�h�̓��o�͗p�s����ݒ肷��
		/// </summary>
		/// <param name="inputID"> ���͗p�s���̎��ʔԍ� </param>
		/// <param name="outputID"> �o�͗p�s���̎��ʔԍ� </param>
		/// <param name="inputName"> ���͗p�s���̖��O </param>
		/// <param name="outputName"> �o�͗p�s���̖��O </param>
		void SetInOutPin(int inputID, int outputID, const char* inputName, const char* outputName);

	private: // ------------------------------ ���� ------------------------------

		/// <summary>
		/// �m�[�h�̏���\������
		/// </summary>
		virtual void RenderInfo() = 0;

		/// <summary>
		/// ������
		/// </summary>
		virtual void Initialize() {}

	private: // ------------------------------ ��� ------------------------------

		// �m�[�h�̎��ʔԍ����ݒ�ς݂Ȃ�true
		bool isSetID = false;

		// �m�[�h�̎��ʔԍ�
		int id = 0;

		// ������ł���m�[�h�G�f�B�^
		NodeEditor* owner = nullptr;
	};

} // namespace PokarinEngine

#endif // !NODE_H_INCLUDED
