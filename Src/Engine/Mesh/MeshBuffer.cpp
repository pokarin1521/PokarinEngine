/**
* @file MeshBuffer.cpp
*/
#define _CRT_SECURE_NO_WARNINGS

#include "MeshBuffer.h"

#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <filesystem>
#include <fstream>

namespace PokarinEngine
{
	/// ここでしか使わないので、cppのみに書く
	/// <summary>
	/// 欠けている法線を補う
	/// </summary>
	/// <param name="[in,out] vertices"> 頂点配列 </param>
	/// <param name="[in] vertexCount"> 頂点配列の要素数 </param>
	/// <param name="[in] indices"> インデックス配列 </param>
	/// <param name="[in] indexCount"> インデックス配列の要素数 </param>
	void FillMissingNormals(Vertex* vertices, size_t vertexCount,
		const uint16_t* indices, size_t indexCount)
	{
		// ---------------------------------------
		// 法線が設定されていない頂点を見つける
		// ---------------------------------------

		// 法線確認用
		// false : 法線が設定されている
		// true : 法線が設定されていない
		std::vector<bool> missingNormals(vertexCount, false);

		for (int i = 0; i < vertexCount; ++i)
		{
			// 法線ベクトル
			const Vector3& n = vertices[i].normal;

			// 法線の長さが0の場合を「設定されていない」とみなす
			if (n.x == 0 && n.y == 0 && n.z == 0)
			{
				missingNormals[i] = true;
			}
		}

		// ----------------
		// 法線を計算
		// ----------------

		/* 今回求める法線は「頂点を含むポリゴン面の法線の平均」と定義する

		頂点の法線を計算するために、まず「面の法線」を求める

		面の法線は「面に対して垂直なベクトル」で
		これは「外積」によって計算できる */

		// 外積を求めるために、
		// 頂点座標から面を構成する2辺を求める
		for (int i = 0; i < indexCount; i += 3)
		{
			// 頂点3つのインデックス取得
			const int i0 = indices[i + 0];
			const int i1 = indices[i + 1];
			const int i2 = indices[i + 2];

			// 頂点3つの座標を取得
			const Vector3& v0 = vertices[i0].position;
			const Vector3& v1 = vertices[i1].position;
			const Vector3& v2 = vertices[i2].position;

			// 取得した頂点座標から
			// 面を構成する2辺a, bを求める
			const Vector3 a = { v1.x - v0.x, v1.y - v0.y, v1.z - v0.z };
			const Vector3 b = { v2.x - v0.x, v2.y - v0.y, v2.z - v0.z };

			// 外積によってaとbに垂直なベクトル(法線)を求める
			const float cx = a.y * b.z - a.z * b.y;
			const float cy = a.z * b.x - a.x * b.z;
			const float cz = a.x * b.y - a.y * b.x;

			// 法線を正規化して単位ベクトルにする
			// 法線ベクトルは「向きを表すベクトル」なので、
			// 単位ベクトルにしておくと今後の計算が楽になる

			// ベクトルの大きさ
			const float l = sqrt(cx * cx + cy * cy + cz * cz);

			// 正規化した法線ベクトル
			const Vector3 normal = { cx / l, cy / l, cz / l };

			// 法線が設定されていない頂点にだけ法線を加算
			// 複数の面の法線を加算することで、法線の向きを平均化する
			if (missingNormals[i0])
			{
				vertices[i0].normal += normal;
			}

			if (missingNormals[i1])
			{
				vertices[i1].normal += normal;
			}

			if (missingNormals[i2])
			{
				vertices[i2].normal += normal;
			}
		}

		// ----------------
		// 法線を正規化  
		// ----------------

		// 法線を加算したことで、正規化されていない状態になったので
		// 平均化された法線を正規化する
		for (int i = 0; i < vertexCount; ++i)
		{
			if (missingNormals[i])
			{
				// 法線ベクトル
				Vector3& n = vertices[i].normal;

				// ベクトルの大きさ
				const float l = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);

				// 法線ベクトルを正規化
				n = { n.x / l, n.y / l, n.z / l };
			}

		} // for i
	}

	/// ここでしか使わないので、cppのみに書く
	/// <summary>
	/// MTLファイルを読み込む
	/// </summary>
	/// <param name="[in] folderName"> OBJファイルのあるフォルダ名 </param>
	/// <param name="[in] fileName"> MTLファイル名 </param>
	/// <returns> MTLファイルに含まれるマテリアル配列 </returns>
	MaterialList LoadMTL(
		const std::string& folderName, const char* fileName)
	{
		// ---------------------
		// MTLファイルを開く
		// ---------------------

		// MTLファイルのパス
		// OBJファイルにおいて、MTLファイル名は相対パスで指定される
		// そのため、OBJファイルのフォルダ名を補足する必要がある
		const std::string fullpath = folderName + fileName;
		std::ifstream file(fullpath);

		// ファイルが開けない
		if (!file)
		{
			// エラー表示
			LOG_ERROR("%sを開けません", fullpath.c_str());

			// 読み込み中断
			return MaterialList();
		}

		// -----------------------
		// MTLファイルを解析する
		// -----------------------

		// マテリアル配列
		MaterialList materialList;

		// マテリアル
		MaterialPtr material;

		// ファイルの終端まで
		while (!file.eof())
		{
			// 1行ずつ読み込む
			std::string line;
			std::getline(file, line);

			// -------- マテリアル定義の読み取りを試みる ----------

			// マテリアル名格納用
			char name[1000] = { 0 };

			// %sで文字列を読み取るとき、%13sのようにsの手前に数字を書くと、
			// 読み取る文字数を制限することができる(%13s = 最大13文字)
			// 末尾に\0が追加されることを考慮して、「配列サイズ - 1」の値を指定する
			// 今回はname配列のサイズに合わせて最大999文字としている

			// マテリアル名を読み取る
			if (sscanf(line.data(), " newmtl %999s", name) == 1)
			{
				// 読み取ったマテリアルを追加
				material = std::make_shared<Material>();
				material->name = name;
				materialList.push_back(material);

				// 次の行へ
				continue;
			}

			// マテリアルが定義されていない場合は行を無視する
			if (!material)
			{
				// 次の行へ
				continue;
			}

			// ---------- 基本色の読み取りを試みる ------------

			// 基本色を読み取る
			if (sscanf(line.data(), " Kd %f %f %f",
				&material->baseColor.r,
				&material->baseColor.g,
				&material->baseColor.b) == 3)
			{
				// 次の行へ
				continue;
			}

			// ---------- 透明度の読み取りを試みる ------------

			// 透明度を読み取る
			if (sscanf(line.data(), " d %f", &material->baseColor.a) == 1)
			{
				// 次の行へ
				continue;
			}

			// ---------- 基本色テクスチャ名の読み取りを試みる ---------

			// テクスチャ名格納用
			char textureName[1000] = { 0 };

			// 基本色テクスチャ名を読み取る
			// 配列サイズを超えないように、読み込む文字数を制限
			if (sscanf(line.data(), " map_Kd %999s", &textureName) == 1)
			{
				// テクスチャファイルのパス
				// MTLファイル名と同様に、map_Kd構文のテクスチャ名は相対パスで指定される
				// そのため、OBJファイルのフォルダ名を補足する
				const std::string fullpath = folderName + textureName;

				// 指定したファイルまたはフォルダが存在する
				if (std::filesystem::exists(fullpath))
				{
					// テクスチャを取得する
					material->texBaseColor = TextureGetter::Get(fullpath.c_str());
				}
				// 存在しない
				else
				{
					// 警告を表示
					LOG_WARNING("%sを開けません", fullpath.c_str());
				}

				// 次の行へ
				continue;
			}

			// --------------- 発光色の読み取りを試みる ----------------

			if (sscanf(line.data(), " Ke %f %f %f",
				&material->emission.r, &material->emission.g,
				&material->emission.b) == 3)
			{
				// 次の行へ
				continue;
			}

			// --------- 発光色テクスチャ名の読み取りを試みる ----------

			if (sscanf(line.data(), " map_Ke %999s", &textureName) == 1)
			{
				// テクスチャファイル
				const std::string filename = folderName + textureName;

				// ファイルが存在する
				if (std::filesystem::exists(filename))
				{
					// エミッションテクスチャを読み込む
					material->texEmission = TextureGetter::Get(filename.c_str());
				}
				// ファイルが存在しない
				else
				{
					// 警告
					LOG_WARNING("%sを開けません", filename.c_str());
				}

				// 次の行へ
				continue;

			} // if map_Ke
		}

		// 基本色テクスチャがない場合は、
		// 他のオブジェクトのテクスチャが適用されるのを防ぐために
		// 白色のテクスチャで代用する
		if (!materialList[0]->texBaseColor)
		{
			// テクスチャのファイル名
			const char* path = "Res/Textures/White.tga";

			// 指定したファイルまたはフォルダが存在する
			if (std::filesystem::exists(path))
			{
				// テクスチャを読み込む
				materialList[0]->texBaseColor = TextureGetter::Get(path);
			}
			// 存在しない
			else
			{
				// 警告を表示
				LOG_WARNING("%sを開けません", fullpath);
			}
		}

		// 読み込んだマテリアルの配列を返す
		return materialList;
	}

	/// <summary>
	/// メッシュバッファを作成するコンストラクタ
	/// </summary>
	/// <param name="[in] bufferSize"> バッファサイズ(バイト数) </param>
	MeshBuffer::MeshBuffer(size_t bufferSize)
	{
		// バッファオブジェクトを作成
		// 動的にメモリを確保出来るようにフラグを設定する
		buffer = BufferObject::Create(
			bufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

		// VAOを作成
		vao = VertexArrayObject::Create();

		/* 基本的にOpenGLの関数は、
		OpenGLコンテキストに割り当てられているオブジェクトを操作する仕組み
		そのため、関数を実行する前に、
		操作したいオブジェクトをOpenGLコンテキストに割り当てる */

		// VAOをOpenGLコンテキストにバインド
		glBindVertexArray(*vao);

		/* VBOは個々の頂点属性にバインドされる
		これは頂点属性ごとに異なるVBOをバインド出来るということ

		バインドは「glVertexAttribArray関数が実行されたとき」に行われる
		glBindBuffer関数は「VBOをOpenGLコンテキストにバインド」するだけで、
		頂点属性にはバインドされない

		IBOの場合はglBindBuffer関数によってVAOにバインドされる */

		// VBOとIBOを
		// VAOとOpenGLコンテキストにバインド
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *buffer);
		glBindBuffer(GL_ARRAY_BUFFER, *buffer);

		// 頂点属性を設定
		vao->SetAttribute(0, 3, sizeof(Vertex), offsetof(Vertex, position));
		vao->SetAttribute(1, 2, sizeof(Vertex), offsetof(Vertex, texcoord));
		vao->SetAttribute(2, 3, sizeof(Vertex), offsetof(Vertex, normal));

		// 誤った操作が行われないようにバインドを解除
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// スタティックメッシュの容量を予約
		staticMeshList.reserve(100);
	}

	/// <summary>
	/// スタティックメッシュを取得する
	/// </summary>
	/// <param name="[in] fileName"> ファイル名 </param>
	/// <returns> ファイル名が一致するスタティックメッシュ </returns>
	StaticMeshPtr MeshBuffer::GetStaticMesh(const std::string& fileName)
	{
		// スタティックメッシュが登録されているなら検索
		if (!staticMeshList.empty())
		{
			// 検索結果
			auto itr = staticMeshList.find(fileName);

			// 要素が見つかった
			if (itr != staticMeshList.end())
			{
				// スタティックメッシュ(値)を返す
				return itr->second;
			}
		}

		// 見つからない場合は、読み込む
		StaticMeshPtr obj = LoadOBJ(fileName);

		// 読み込んだスタティックメッシュを返す
		return obj;
	}

	/// <summary>
	/// OBJファイルを読み込む
	/// </summary>
	/// <param name="[in] fileName"> OBJファイル名 </param>
	/// <returns> filenameから作成したメッシュ </returns>
	StaticMeshPtr MeshBuffer::LoadOBJ(const std::string& fileName)
	{
		// --------------------------------------------------------
		// 以前に読み込んだファイルなら、作成済みのメッシュを返す
		// --------------------------------------------------------
		{
			// ファイル名と関連付けられたデータがあるか調べる

			// キーで検索
			auto itr = staticMeshList.find(fileName);

			// 見つかったらメッシュを返す
			if (itr != staticMeshList.end())
			{
				return itr->second;
			}
		}

		// ------------------------------
		// OBJファイルを開く
		// ------------------------------
		std::ifstream file(fileName);

		// ファイルが開けない
		if (!file)
		{
			// ファイル読み込みを中断
			return nullptr;
		}

		// ------ OBJファイルのあるフォルダ名を取得 -------

		// OBJファイルのあるフォルダ名
		std::string foldername(fileName);
		{
			// 一番最後の区切り文字の位置を探す
			const size_t p = foldername.find_last_of("\\ /");

			// 区切り文字が見つかった場合
			if (p != std::string::npos)
			{
				// 区切り文字のところまでサイズを縮めることで
				// フォルダ名を取得する
				// 要素番号は0番からなので+1する
				foldername.resize(p + 1);
			}
		}

		// -------------------------------------------
		// OBJファイルを解析して、
		// 頂点データとインデックスデータを読み込む
		// -------------------------------------------

		/* データ構造が違うため、
		OBJファイルのデータをそのまま描画に使うことはできない
		そのため、とりあえず全てのデータを読み込んでおいて、
		それからOpenGLで描画できるデータに変換する */

		// 頂点座標
		std::vector<Vector3> positions;

		// テクスチャ座標
		std::vector<Vector2> texcoords;

		// 法線ベクトル
		std::vector<Vector3> normals;

		// 頂点座標, テクスチャ座標, 法線ベクトルの
		// インデックスデータの組
		struct IndexSet
		{
			// v = 頂点座標の要素番号, 
			// vt = テクスチャ座標の要素番号
			// vn = 法線ベクトルの要素番号(0 : 法線なし)
			int v, vt, vn;
		};

		// インデックスデータの組を管理する配列
		std::vector<IndexSet> indexSets;

		// 配列の容量を予約
		// インデックスデータは3個で1つの三角形を作るので、
		// 頂点数と同じ数の三角形を扱えるように、
		// インデックスデータの初期容量は頂点データの3倍にする
		positions.reserve(20'000);
		texcoords.reserve(20'000);
		normals.reserve(20'000);
		indexSets.reserve((size_t)20'000 * 3);

		// マテリアル配列
		std::vector<MaterialPtr> materialList;

		// 配列の容量を予約
		materialList.reserve(100);

		// マテリアルの使用範囲
		struct UseMaterial
		{
			std::string name;	// マテリアル名
			size_t startOffset; // 割り当て範囲の先頭位置
		};

		// 使用マテリアル配列
		std::vector<UseMaterial> useMaterialList;
		useMaterialList.reserve(100);

		// 仮データを追加
		// マテリアル指定がない場合、
		// デフォルトのマテリアルを設定できるようにするため
		useMaterialList.push_back({ std::string(), 0 });

		// --------- ファイルの終端まで読み込む ----------

		while (!file.eof())
		{
			// 1行ずつ読み取る
			std::string line;
			std::getline(file, line);
			const char* p = line.c_str();

			/* sscanf(読み込みバッファ, 書式指定文字列, データ格納先アドレス, ...);

			scanfは「標準入力」からデータを読み取るが、
			sscanfは「第一引数で指定したバッファ」からデータを読み取る
			それ以外は同じ動作

			戻り値は「読み取りに成功したデータの数」*/

			// -----------------------------
			// 頂点座標の読み取りを試みる
			// -----------------------------

			// データ格納用(頂点座標)
			Vector3 v(0);

			//「読み取ったデータの数」が「データ格納用変数の数」と一致したら
			// 読み取り成功と判断
			// 
			// (書式指定文字列の先頭に空白があるのは、
			// 行頭の空白やタブを飛ばしてくれるため
			// インデントがある場合に対応できる)
			if (sscanf(p, " v %f %f %f", &v.x, &v.y, &v.z) == 3)
			{
				// 読み取ったデータを追加
				positions.push_back(v);

				// 次の行の処理へ
				continue;
			}

			// ------------------------------------
			// テクスチャ座標の読み取りを試みる
			// ------------------------------------

			// データ格納用(テクスチャ座標)
			Vector2 vt(0);

			// 「読み取ったデータの数」が「データ格納用変数の数」と一致したら
			// 読み取り成功と判断		
			//
			// (書式指定文字列の先頭に空白があるのは、
			//  行頭の空白やタブを飛ばしてくれるため
			//  インデントがある場合に対応できる)
			if (sscanf(p, " vt %f %f", &vt.x, &vt.y) == 2)
			{
				// 読み取ったデータを追加
				texcoords.push_back(vt);

				// 次の行の処理へ
				continue;
			}

			// --------------------------
			// 法線の読み取りを試みる
			// --------------------------

			// データ格納用(法線ベクトル)
			Vector3 vn(0);

			// 法線を読み取る
			if (sscanf(p, " vn %f %f %f", &vn.x, &vn.y, &vn.z) == 3)
			{
				// 法線ベクトルを追加
				normals.push_back(vn);

				// 次の行へ
				continue;
			}

			// ----------------------------------------
			// インデックスデータの読み取りを試みる
			// ----------------------------------------

			/* f構文(GL_TRIANGLES)は3つ以上のインデックスデータの組を持つ

			まず1,2,3番目の3つで三角形を作り、
			次は1,3,4番目、その次は1,4,5番目というように
			1番目の頂点を共有して次々に三角形を定義する

			この動作を実現するために、
			まず最初のsscanfで1,2番目のデータを取得し
			次のsscanfで3番目以降のデータを取得しつつ三角形を作成する */

			// データ格納用
			IndexSet f0, f1, f2;
			f0 = f1 = f2 = { 0 };

			// 読み取った文字数の格納用
			int readByte;

			// -------- 頂点座標 + テクスチャ座標 + 法線 ----------

			// 1,2番目のデータを取得
			// %nは「%nが現れるまでに読み取った文字数を引数に格納する」というもの
			if (sscanf(p, " f %u/%u/%u %u/%u/%u%n",
				&f0.v, &f0.vt, &f0.vn, &f1.v, &f1.vt, &f1.vn, &readByte) == 6)
			{
				p += readByte; // 読み取り位置を更新

				// 3番目以降のデータを取得しつつ三角形を作成する
				for (;;)
				{
					// データが読み取れなかったら終了
					if (sscanf(p, " %u/%u/%u%n", &f2.v, &f2.vt, &f2.vn, &readByte) != 3)
					{
						break;
					}

					p += readByte; // 読み取り位置を更新

					// 読み取ったデータを追加
					indexSets.push_back(f0);
					indexSets.push_back(f1);
					indexSets.push_back(f2);

					f1 = f2; // 次の三角形のためにデータを移動
				}

				continue;
			}

			// -------- 頂点座標 + テクスチャ座標 ----------

			// 1,2番目のデータを取得
			// %nは「%nが現れるまでに読み取った文字数を引数に格納する」というもの
			if (sscanf(p, " f %u/%u %u/%u%n",
				&f0.v, &f0.vt, &f1.v, &f1.vt, &readByte) == 4)
			{
				f0.vn = f1.vn = 0; // 法線なし
				p += readByte; // 読み取り位置を更新

				// 3番目以降のデータを取得しつつ三角形を作成する
				for (;;)
				{
					// データが読み取れなかったら終了
					if (sscanf(p, " %u/%u%n", &f2.v, &f2.vt, &readByte) != 2)
					{
						break;
					}

					f2.vn = 0; // 法線なし
					p += readByte; // 読み取り位置を更新

					// 読み取ったデータを追加
					indexSets.push_back(f0);
					indexSets.push_back(f1);
					indexSets.push_back(f2);

					f1 = f2; // 次の三角形のためにデータを移動
				}

				continue;
			}

			// --------------------------------
			// MTLファイルの読み取りを試みる
			// --------------------------------

			// MTLファイル名
			char mtlFilename[1000] = "";

			// MTLファイル名を読み取る
			if (sscanf(line.data(), " mtllib %999s", mtlFilename) == 1)
			{
				// 読み取った名前のMTLファイルを読み込む
				const auto tmp = LoadMTL(foldername, mtlFilename);

				// マテリアル配列に追加
				materialList.insert(materialList.end(), tmp.begin(), tmp.end());

				continue;
			}

			// ------------------------------------
			// 使用マテリアル名の読み取りを試みる
			// ------------------------------------

			// 使用マテリアル名
			char materialName[1000] = "";

			// 使用マテリアル名を読み取る
			if (sscanf(line.data(), " usemtl %999s", materialName) == 1)
			{
				// 読み取った使用マテリアル名と
				// usemtlが見つかった時点での
				// インデックスデータ配列サイズを追加
				// (マテリアル割り当て範囲を確認するのに使う)
				useMaterialList.push_back({ materialName, indexSets.size() });

				// 次の行へ
				continue;
			}
		} // while eof

		// --------------- 番兵の追加 ----------------

		/* 最後尾にあるマテリアルの割り当て範囲を設定できるように
		末尾に番兵を追加
		番兵を追加した時のサイズが読み込み終了値となる
		(番兵 : 配列やリストの終端に置く特殊なデータ) */

		useMaterialList.push_back({ std::string(), indexSets.size() });

		// ---------- f構文と頂点インデックスの「対応表」-----------

		/* ------ 対応表 --------
		f構文を添え字、頂点インデックスを値とする配列

		f構文の値を添え字にして、
		頂点インデックスを値として記録すると
		v, vn, vtのそれぞれがint型で保持され、
		組み合わせの総数が「20億 × 20億 × 20億」という膨大な数になる

		制限を設けるために、
		int型の32bitの内、下位20bitだけ扱う
		20bitは約100万まで扱えるので、
		ゲームに使う3Dモデルにとっては十分 */

		// OBJファイルのf構文(インデックスデータの組)と
		// OpenGLの頂点インデックス配列の対応表
		// (キーは「v,vt,vn (20bit)」, 値は頂点インデックス)
		std::unordered_map<uint64_t, uint16_t> indexMap;
		indexMap.reserve(10'000);

		// --------------------------------------------------
		// 読み込んだデータを、OpenGLで使えるデータに変換
		// --------------------------------------------------

		// 頂点データ配列
		std::vector<Vertex> vertices;
		vertices.reserve(indexSets.size());

		// インデックスデータ配列
		std::vector<uint16_t> indices;
		indices.reserve(indexSets.size());

		// インデックスデータに対応する頂点データから
		// Vertex型の作成と、実際の描画で使用するインデックスデータの作成
		for (const auto& indexSet : indexSets)
		{
			// -------- f構文の値を64ビットの「キー」に変換 ---------

			// 対応表のキー
			// 20bitずつずらして設定
			const uint64_t key = static_cast<uint64_t>(indexSet.v) +
				(static_cast<uint64_t>(indexSet.vt) << 20) +
				(static_cast<uint64_t>(indexSet.vn) << 40);

			// -------- 頂点インデックスを配列に追加 ---------

			// 対応表からキーに一致するデータを検索
			// 見つからなかった場合、end()が入る
			const auto itr = indexMap.find(key);

			// データが対応表にあるので
			if (itr != indexMap.end())
			{
				// 既存の頂点インデックスを使う
				indices.push_back(itr->second);
			}
			// データが対応表にない
			else
			{
				// 新しい頂点データを作成
				Vertex v;

				/* 添字を「-1」しているのは、
				OBJファイルのインデックスは1から始まるのに対して、
				C++言語の添字は0から始まるため */

				// 頂点座標の要素番号
				size_t vertexIndex = static_cast<size_t>(indexSet.v - 1);

				// テクスチャ座標の要素番号
				size_t texIndex = static_cast<size_t>(indexSet.vt - 1);

				// 法線ベクトルの要素番号
				size_t normalIndex = static_cast<size_t>(indexSet.vn - 1);

				v.position = positions[vertexIndex];
				v.texcoord = texcoords[texIndex];

				// 法線が設定されていない場合は0を設定(あとで計算)
				if (indexSet.vn == 0)
				{
					v.normal = Vector3::zero;
				}
				// 法線が設定されている場合は取得
				else
				{
					v.normal = normals[normalIndex];
				}

				// 作成した頂点データを追加
				vertices.push_back(v);

				// 新しい頂点データのインデックスを、
				// 頂点インデックス配列に追加
				const uint16_t index = static_cast<uint16_t>(vertices.size() - 1);
				indices.push_back(index);

				// キーと頂点インデックスのペアを対応表に追加
				indexMap.emplace(key, index);
			}
		}

		// 設定されていない法線を補う
		FillMissingNormals(vertices.data(), vertices.size(),
			indices.data(), indices.size());

		// データの位置
		const void* indexOffset = nullptr;

		// インデックス0となる頂点配列の要素番号
		GLint baseVertex = 0;

		// 変換したデータをバッファに追加
		AddVertexData(
			vertices.data(), vertices.size() * sizeof(Vertex),
			indices.data(), indices.size() * sizeof(uint16_t),
			indexOffset, baseVertex);

		// ----------------------------------------------------------
		// マテリアルに対応した描画パラメータを作成する
		// ----------------------------------------------------------

		// 描画パラメータ配列
		DrawParameterList drawParameterList;

		// 使用中マテリアル配列の要素番号
		size_t i = 0;

		// 仮データと番兵以外のマテリアルがある場合、仮データを飛ばす
		if (useMaterialList.size() > 2)
		{
			i = 1;
		}

		// 使用マテリアル配列の最後の要素番号
		size_t end = useMaterialList.size() - 1;

		// 次の要素を調べるので、最後の要素番号は含まない
		for (; i < end; ++i)
		{
			// 使用中のマテリアル
			const UseMaterial& current = useMaterialList[i];

			// 次のマテリアル
			const UseMaterial& next = useMaterialList[i + 1];

			// インデックスデータがない場合は飛ばす
			if (next.startOffset == current.startOffset)
			{
				continue;
			}

			// 描画パラメータ
			DrawParameter drawParameter;

			// プリミティブの種類
			drawParameter.mode = GL_TRIANGLES;

			// 描画するインデックス数
			// 次のマテリアル割り当て範囲の先頭位置まで
			drawParameter.count = static_cast<GLsizei>(next.startOffset - current.startOffset);

			// 描画開始インデックスの位置
			drawParameter.indices = indexOffset;

			// インデックス0となる頂点配列の要素番号
			drawParameter.baseVertex = baseVertex;

			// デフォルト値を設定
			drawParameter.materialNo = 0;

			// 名前の一致するマテリアルを探す
			for (int i = 0; i < materialList.size(); ++i)
			{
				if (materialList[i]->name == current.name)
				{
					// 名前の一致するマテリアルを設定
					drawParameter.materialNo = i;

					break;
				}
			}

			// 作成した描画パラメータを追加
			drawParameterList.push_back(drawParameter);

			// インデックスオフセットを変更
			// 描画した分ずらす
			indexOffset = reinterpret_cast<void*>(
				reinterpret_cast<size_t>(indexOffset) + sizeof(uint16_t) * drawParameter.count);
		}

		// ---------------------------------------
		// スタティックメッシュを作成する
		// ---------------------------------------

		// マテリアル配列が空の場合は
		// デフォルトマテリアルを追加する
		if (materialList.empty())
		{
			materialList.push_back(std::make_shared<Material>());
		}

		// スタティックメッシュを作成する
		StaticMeshPtr staticMesh = std::make_shared<StaticMesh>(fileName, drawParameterList, materialList);

		// 作成したメッシュを管理用配列に追加する
		staticMeshList.emplace(fileName, staticMesh);

		// 読み込み通知
		LOG("%sを読み込みました(頂点数 = %d, インデックス数 = %d)",
			fileName.c_str(), vertices.size(), indices.size());

		// 作成したスタティックメッシュを返す
		return staticMesh;
	}

	/// <summary>
	/// 頂点データを追加する
	/// </summary>
	/// <param name="[in] vertices"> 頂点データ </param>
	/// <param name="[in] vertexBytes"> 頂点データのバイト数 </param>
	/// <param name="[in] indices"> インデックスデータ </param>
	/// <param name="[in] indexBytes"> インデックスデータのバイト数 </param>
	/// <param name="[in] mode"> プリミティブの種類(デフォルトで三角形) </param>
	/// <param name="[out] indexOffset"> インデックスデータの位置(バイト数で指定) </param>
	/// <param name="[out] baseVertex"> インデックス0となる頂点配列の要素番号 </param>
	void MeshBuffer::AddVertexData(const Vertex* vertices, size_t vertexBytes,
		const uint16_t* indices, size_t indexBytes,
		const void* indexOffset, GLint& baseVertex)
	{
		// -------------------------------------------------------------------
		// 空き容量が足りていることを確認
		// 必要なバイト数が空きバイト数より大きい場合は追加できない
		// -------------------------------------------------------------------

		// 使用する容量(バイト)
		// (追加する頂点データとインデックスデータの合計サイズ)
		const size_t totalBytes = vertexBytes + indexBytes;

		// 使用できる容量(バイト)
		const size_t freeBytes = buffer->GetSize() - usedBytes;

		// 使用する容量が使用できる容量を超えたら
		if (totalBytes > freeBytes)
		{
			// 容量不足のエラー
			LOG_ERROR("容量が足りません(要求サイズ = %d, 使用量/最大容量 = %d/%d, 空き容量 = %d)",
				totalBytes, usedBytes, buffer->GetSize(), freeBytes);

			// 関数を終了
			return;
		}

		// ------------------------------------------------------------------------
		// glCopyNamedBufferSubData関数で
		// 図形データごとの頂点データとインデックスデータをGPUにコピー
		// ------------------------------------------------------------------------

		/* glCopyNamedBufferSubData関数は
		GPUメモリにあるバッファの間でしかコピーできないので、
		glNamedBufferStorage関数でCPUメモリからGPUメモリにデータをコピーする必要がある

		glNamedBufferStorage関数は
		新しいGPUメモリ領域を確保するので、既存のバッファに上書きできない
		(上書きする方法はあるが、描画速度が低下してしまう)
		そのため、バッファ間でデータをコピーする必要がある */

		// 頂点データをGPUメモリにコピー
		GLuint tmp[2];

		// コピーするデータを入れる一時的なGPU側バッファを作成
		// 頂点データとインデックスデータをコピーするので2つ作る
		glCreateBuffers(2, tmp);

		// 作成した一時的なGPU側バッファに、CPU側にあるデータをコピー
		glNamedBufferStorage(tmp[0], vertexBytes, vertices, 0);
		glNamedBufferStorage(tmp[1], indexBytes, indices, 0);

		/* void glCopyNamedBufferSubData(コピー元バッファの識別番号,
		コピー先バッファの識別番号, コピー元の読み取り開始位置,
		コピー先の書き込み開始位置, コピーするバイト数) */

		/* 一時的なバッファから、既存のバッファにコピー
		後ろに追加していきたいので、書き込み開始位置をサイズ分ずらす
		今回は同じバッファにコピーするので、
		頂点データをコピーした後、頂点データのサイズ分ずらしてから
		インデックスデータをコピー */
		glCopyNamedBufferSubData(tmp[0], *buffer, 0, usedBytes, vertexBytes);
		glCopyNamedBufferSubData(tmp[1], *buffer, 0, usedBytes + vertexBytes, indexBytes);

		// 一時的なバッファを削除
		glDeleteBuffers(2, tmp);

		// ---------------------------------------------------------------
		// インデックスデータの位置と頂点データの位置を求める
		// ---------------------------------------------------------------

		// インデックスデータの位置(バイト数で指定)は
		// コピー順序を考慮するので、頂点データの直後
		indexOffset = reinterpret_cast<void*>(usedBytes + vertexBytes);

		// 頂点データの位置は頂点データ数で指定する
		baseVertex = static_cast<GLuint>(usedBytes / sizeof(Vertex));

		// -------------------------------------------
		// 次のデータ格納開始位置を計算
		// -------------------------------------------

		/* GPUはデータサイズ単位でデータにアクセスするようになっているので、
		「N * データサイズ」バイト目にコピーしなければならない

		メモリ領域の管理を単純化させるために、違うサイズのデータを同じメモリ領域に混在させている
		なのでコピー先の位置を、頂点データとインデックスデータの共通の境界で合わせる
		共通の境界に合わせることで、どちらも正しい境界にデータをコピーできる

		境界サイズが2バイトのデータと、境界サイズが20バイトのデータの場合、
		共通の境界は「最小公倍数」に一致するので、20バイトになる */

		// 共通の境界サイズ(最小公倍数)
		constexpr size_t size = std::lcm(sizeof(uint16_t), sizeof(Vertex));

		/* 使った分を使用済み容量に加える

		境界サイズ分増えるので、
		境界サイズに合わせた数字を求める

		境界サイズの倍数(確保数) = 元の値 + 境界サイズ - 1
		使用した容量 = 境界サイズの倍数 * 境界サイズ

		倍数を求める際の「-1」は倍数が大きくなりすぎないようにするためのもの

		「整数型の除算はあまりを切り捨てる」という特徴を利用して、
		「元の値以上かつ、最小の境界サイズの倍数」を求めている

		常に境界サイズに合わせた値を加算することで
		「usedBytesが常に境界サイズと一致する」ことが保証出来る */

		// 境界サイズの倍数(確保数)
		size_t count = (totalBytes + size - 1) / size;

		// 使用した容量を加算する
		usedBytes += count * size;
	}
}