/**
* @file MapObject.h
*/
#ifndef MAPOBJECT_H_INCLUDED
#define MAPOBJECT_H_INCLUDED

#include "Engine/Components/Component.h"

#include <vector>

namespace PokarinEngine
{
	// -------------------
	// �O���錾
	// -------------------

	class MapObject;

	// ------------------------
	// �^�̕ʖ����`
	// ------------------------

	using MapObjectPtr = std::shared_ptr<MapObject>;

	/**
	* �}�b�v�ɔz�u����I�u�W�F�N�g
	*/
	class MapObject : public Component
	{
	public: // �R���X�g���N�^�E�f�X�g���N�^

		MapObject() = default;
		virtual ~MapObject() = default;

	public: // ����p

		// Action�֐��̎��s����
		enum class ActionResult
		{
			noRemoteAction, // �����[�g�A�N�V���������s���Ȃ�
			executeRemoteAction, // �����[�g�A�N�V���������s����
		};

	public: // �C�x���g����

		/// <summary>
		/// �N���b�N���ꂽ�Ƃ��ɌĂяo�����֐�
		/// </summary>
		void OnClick()
		{
			if (Action() == ActionResult::executeRemoteAction)
			{
				// ���u��������s
				InvokeRemoteAction();
			}
		}

		/// <summary>
		/// ���u��������s����
		/// </summary>
		void InvokeRemoteAction()
		{
			for (auto& e : remoteObjects)
			{
				e->RemoteAction(this);
			}
		}

		/// <summary>
		/// ���u���삷��}�b�v�I�u�W�F�N�g��ǉ�����
		/// </summary>
		/// <param name="object"> �ǉ�����}�b�v�I�u�W�F�N�g </param>
		void AddRemoteObject(const MapObjectPtr& object)
		{
			remoteObjects.push_back(object);
		}

		/// <summary>
		/// �������N���b�N���ꂽ�Ƃ��Ɏ��s���鉼�z�֐�
		/// </summary>
		/// <returns> ���s���ʂ�\���񋓑� </returns>
		virtual ActionResult Action()
		{
			return ActionResult::executeRemoteAction;
		}

		/// <summary>
		/// �֘A�t����ꂽ�I�u�W�F�N�g��
		/// �N���b�N���ꂽ���ɌĂяo����鉼�z�֐�
		/// </summary>
		/// <param name="linkedObject"> �Ăяo�����̃}�b�v�I�u�W�F�N�g </param>
		virtual void RemoteAction(MapObject* linkedObject) {}

	private: // �}�b�v�I�u�W�F�N�g

		// ���u���삷��}�b�v�I�u�W�F�N�g�̔z��
		std::vector<MapObjectPtr> remoteObjects;
	};

} // namespace PokarinEngine

#endif // !MAPOBJECT_H_INCLUDED
