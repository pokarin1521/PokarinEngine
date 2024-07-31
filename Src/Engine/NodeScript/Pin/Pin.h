/**
* @file Pin.h
*/
#ifndef POKARINENGINE_PIN_H_INCLUDED
#define POKARINENGINE_PIN_H_INCLUDED

#include "PinType.h"

#include <memory>

namespace PokarinEngine
{
	/// <summary>
	/// �m�[�h�̃s��
	/// </summary>
	class Pin
	{
	public: // ------------------- �R���X�g���N�^�E�f�X�g���N�^ --------------------

		Pin(int _nodeID, int pinID, PinType pinType) :nodeID(_nodeID), id(pinID), type(pinType) {}
		~Pin() = default;

	public: // ----------------------------- �s���̎�� ----------------------------

		/// <summary>
		/// �s���̎�ނ��擾����
		/// </summary>
		/// <returns> �s���̎�� </returns>
		PinType GetType() const
		{
			return type;
		}

		/// <summary>
		/// ������ł���m�[�h�̎��ʔԍ����擾����
		/// </summary>
		/// <returns> ������ł���m�[�h�̎��ʔԍ� </returns>
		int GetNodeID() const
		{
			return nodeID;
		}

	private: // ------------------------------- ��� -------------------------------

		// �s���̎��
		PinType type = PinType::Data;

		// �s���̎�����ł���m�[�h�̎��ʔԍ�
		int nodeID = 0;

		// ���ʔԍ�
		int id = 0;
	};
}

#endif // !POKARINENGINE_PIN_H_INCLUDED
