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
	// 前方宣言
	// ------------------------------

	struct Material;

	// ------------------------------
	// 型の別名を定義
	// ------------------------------

	using MaterialPtr = std::shared_ptr<Material>;

	using MaterialList = std::vector<MaterialPtr>;

	/// <summary>
	/// マテリアル
	/// </summary>
	struct Material
	{
		// マテリアル名
		std::string name = "<Default>";

		// 基本色
		Color baseColor = Color::black;

		// 発光色
		Color emission = Color::black;

		// 基本色テクスチャ
		TexturePtr texBaseColor;

		// 発光色テクスチャ
		TexturePtr texEmission;
	};
}

#endif // !POKARINENGINE_MATERIAL_H_INCLUDED
