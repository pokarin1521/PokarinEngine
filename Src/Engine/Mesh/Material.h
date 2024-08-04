/**
* @file Material.h
*/
#ifndef POKARINENGINE_MATERIAL_H_INCLUDED
#define POKARINENGINE_MATERIAL_H_INCLUDED

#include "../Color.h"
#include "../Texture.h"

#include <string>
#include <vector>
#include <memory>

namespace PokarinEngine
{
	// ------------------------------
	// �O���錾
	// ------------------------------

	struct Material;

	// ------------------------------
	// �^�̕ʖ����`
	// ------------------------------

	using MaterialPtr = std::shared_ptr<Material>;

	using MaterialList = std::vector<MaterialPtr>;

	/// <summary>
	/// �}�e���A��
	/// </summary>
	struct Material
	{
		// �}�e���A����
		std::string name = "<Default>";

		// ��{�F
		Color baseColor = Color::black;

		// �����F
		Color emission = Color::black;

		// ��{�F�e�N�X�`��
		TexturePtr texBaseColor;

		// �����F�e�N�X�`��
		TexturePtr texEmission;
	};
}

#endif // !POKARINENGINE_MATERIAL_H_INCLUDED
