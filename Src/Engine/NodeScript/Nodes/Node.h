/**
* @file Node.h
*/
#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <string>
#include <vector>
#include <memory>

namespace PokarinEngine
{
	// -----------------------
	// �O���錾
	// -----------------------

	class Node;

	// -----------------------------
	// �^�̕ʖ����`
	// -----------------------------

	using NodePtr = std::shared_ptr<Node>;
	using NodeList = std::vector<NodePtr>;

	/// <summary>
	/// �m�[�h�̊��N���X
	/// </summary>
	class Node
	{
	public: // ------------------ �R���X�g���N�^�E�f�X�g���N�^ -------------------

		Node() = default;
		virtual ~Node() = default;

	public: // ------------------------------ ��� ------------------------------

		// �m�[�h�̖��O
		std::string name = "";
	};

} // namespace PokarinEngine

#endif // !NODE_H_INCLUDED
