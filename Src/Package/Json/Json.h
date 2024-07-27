/**
* @file Json.h
*/
#ifndef JSON_H_INCLUDED
#define JSON_H_INCLUDED

#include "nlohmann/json.hpp"

#include <string>
#include <fstream>

namespace PokarinEngine
{
	/// <summary>
	/// Json�t�@�C���p
	/// </summary>
	namespace JsonFile
	{
		/// <summary>
		/// Json�t�@�C����ǂݍ���
		/// </summary>
		/// <param name="[in] fileName"> �ǂݍ��ރt�@�C���� </param>
		/// <param name="[out] data"> �ǂݍ��񂾃f�[�^ </param>
		inline void Load(const std::string& fileName, Json& data)
		{
			// �ǂݍ��ݐ�̃t�@�C��
			std::ifstream file;

			// �ǂݎ���p�ŊJ��
			file.open(fileName, std::ios::in);
			
			// �t�@�C�����J������ǂݍ���
			if (file)
			{
				file >> data;
			}

			// �J�����t�@�C�������
			file.close();
		}

		/// <summary>
		/// Json�t�@�C���ɕۑ�����
		/// </summary>
		/// <param name="[in] fileName"> �ۑ���̃t�@�C���� </param>
		/// <param name="[in] data"> �f�[�^ </param>
		inline void Save(const std::string& fileName, const Json& data)
		{
			// �ۑ���t�@�C��
			std::ofstream file;

			// �㏑�����[�h�ŊJ��
			file.open(fileName, std::ios::out);

			// �t�@�C���ɕۑ�
			// �t�@�C�������������琶�������
			file << data.dump(2);

			// �t�@�C�������
			file.close();
		}
	}
}

#endif // !JSON_H_INCLUDED
