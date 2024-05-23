/**
* @file Node.h
*/
#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "../../UsingNames/UsingNodeEditor.h"
#include "../../UsingNames/UsingGameObject.h"

#include "../Pin/PinType.h"

#include <string>
#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// �m�[�h�̊��N���X
	/// </summary>
	class Node
	{
	public: // ------------------ �R���X�g���N�^�E�f�X�g���N�^ --------------------

		Node() = default;
		virtual ~Node() = default;

	public: // ------------------------------- �쐬 -------------------------------

		/// <summary>
		/// �m�[�h�쐬���̏���
		/// </summary>
		/// <param name="nodeEditor"> ������ł���m�[�h�G�f�B�^ </param>
		/// <param name="nodeID"> �m�[�h�̎��ʔԍ� </param>
		void CreateNode(NodeEditor& nodeEditor, int nodeID, const std::string& nodeTitle);

	public: // ------------------------------- ���� -------------------------------

		/// <summary>
		/// ���s����
		/// </summary>
		void Run();

		/// <summary>
		/// �m�[�h��\������
		/// </summary>
		void Render();

	public: // ------------------------- ���̃m�[�h��ݒ� -------------------------

		/// <summary>
		/// ���Ɏ��s����m�[�h��ݒ肷��
		/// </summary>
		/// <param name="node"> ���Ɏ��s����m�[�h </param>
		void SetNextNode(Node* node)
		{
			nextNode = node;
		}

	public: // ------------------------- �m�[�h�̎��ʔԍ� -------------------------

		/// <summary>
		/// �m�[�h�̎��ʔԍ����擾����
		/// </summary>
		/// <returns> �m�[�h�̎��ʔԍ� </returns>
		int GetID() const
		{
			return id;
		}

	protected: // ------------------------- �s���쐬�p ---------------------------

		/// <summary>
		/// �s�����쐬����
		/// </summary>
		/// <param name="pinType"> �s���̎�� </param>
		/// <returns> �쐬�����s���̎��ʔԍ� </returns>
		int CreatePin(PinType pinType);

	protected: // ------------------------- �s���ݒ�p ---------------------------

		/// <summary>
		/// �s���̓��o�͑���
		/// </summary>
		enum class PinAttribute
		{
			Input,  // ���͗p
			Output, // �o�͗p
		};

		/// <summary>
		/// �f�[�^�s���̕\�����J�n����
		/// </summary>
		/// <param name="pinID"> �s���̎��ʔԍ� </param>
		/// <param name="pinAttribute"> �s���̑��� </param>
		void BeginDataPin(int pinID, PinAttribute pinAttribute);

		/// <summary>
		/// ���s�s���̕\�����J�n����
		/// </summary>
		/// <param name="pinID"> �s���̎��ʔԍ� </param>
		/// <param name="pinAttribute"> �s���̑��� </param>
		void BeginRunPin(int pinID, PinAttribute pinAttribute);

		/// <summary>
		/// �s���̕\�����I������
		/// </summary>
		/// <param name="pinAttribute"> �s���̑��� </param>
		void EndPin(PinAttribute pinAttribute);

		/// <summary>
		/// ���ɐݒ肷��s���𓯂��s�ɕ\������
		/// </summary>
		void PinSameLin();

	protected: // ---------------------- �Q�[���I�u�W�F�N�g ----------------------

		/// <summary>
		/// �m�[�h�G�f�B�^�̎�����ł���Q�[���I�u�W�F�N�g���擾����
		/// </summary>
		/// <returns> ������ł���Q�[���I�u�W�F�N�g </returns>
		GameObject& GetOnwerObject();

	private: // --------------------------- �s���̐ݒ� ---------------------------

		/// <summary>;
		/// �s���̌`
		/// </summary>
		enum class PinShape;

		/// <summary>
		/// �s���̐ݒ���J�n����
		/// </summary>
		/// <param name="pinID"> �s���̎��ʔԍ� </param>
		/// <param name="pinAttribute"> �s���̑��� </param>
		/// <param name="pinShape"> �s���̌` </param>
		void BeginPin(int pinID, PinAttribute pinAttribute, PinShape pinShape);

	private: // ------------------------------ ���s ------------------------------

		/// <summary>
		/// ���̃m�[�h�̏��������s����
		/// </summary>
		void RunNextNode()
		{
			// ���̃m�[�h���ݒ肳��Ă���Ȃ���s����
			if (nextNode)
			{
				nextNode->Run();
			}
		}

		/// <summary>
		/// �m�[�h�ʂ̎��s����
		/// </summary>
		/// <returns>
		/// <para> true : ���̃m�[�h�̏��������s���� </para>
		/// <para> false : ���̃m�[�h�̏��������s���Ȃ� </para>
		/// </returns>
		virtual bool RunNode() = 0;

	private: // ----------------------------- ������ -----------------------------

		/// <summary>
		/// ������
		/// </summary>
		virtual void Initialize() = 0;

	private: // ------------------------------ �\�� ------------------------------

		/// <summary>
		/// �s����\������
		/// </summary>
		virtual void RenderPin() = 0;

		/// <summary>
		/// �^�C�g����\������
		/// </summary>
		void RenderTitle();

	private: // ------------------------------ ��� ------------------------------

		// �m�[�h�̎��ʔԍ����ݒ�ς݂Ȃ�true
		bool isSetID = false;

		// �m�[�h�̎��ʔԍ�
		int id = 0;

		// �m�[�h�̃^�C�g��
		std::string title = "";

		// ������ł���m�[�h�G�f�B�^
		NodeEditor* ownerEditor = nullptr;

		// ���Ɏ��s����m�[�h
		Node* nextNode = nullptr;
	};

} // namespace PokarinEngine

#endif // !NODE_H_INCLUDED
