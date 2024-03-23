/**
* @file Shader.cpp
*/
#include "Shader.h"

#include "../Debug.h"

#include <fstream>
#include <filesystem>

namespace PokarinEngine
{
	namespace Shader
	{
		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// シェーダファイルを読み込んでコンパイルする
		/// </summary>
		/// <param name="type"> シェーダの種類 </param>
		/// <param name="filename"> シェーダファイル名 </param>
		/// <returns> シェーダの管理番号 </returns>
		GLuint Compile(GLenum type, const char* filename)
		{
			// バイナリモードを指定
			// 改行文字の変換等を行わないので、読み込みが速い
			std::ifstream file(filename, std::ios::binary);

			if (!file)
			{
				LOG_ERROR("%sを開けません", filename);

				return 0;
			}

			//	ファイルを読み込む

			// ファイル全体を一気に読み込むために、ファイルサイズ取得
			const size_t filesize = std::filesystem::file_size(filename);
			std::vector<char> buffer(filesize);

			// ファイルを読み込み、bufferに格納
			file.read(buffer.data(), filesize);
			file.close();

			const char* source[] = { buffer.data() };
			const GLint length[] = { int(buffer.size()) };

			// 作成したシェーダオブジェクトの管理番号
			const GLuint object = glCreateShader(type);

			// ソースコードを設定しコンパイル
			glShaderSource(object, 1, source, length);
			glCompileShader(object);

			return object;
		}

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// 頂点シェーダとフラグメントシェーダをリンク
		/// </summary>
		/// <param name="prog"> プログラムオブジェクトの管理番号 </param>
		/// <param name="vertex"> 頂点シェーダの管理番号 </param>
		/// <param name="fragment"> フラグメントシェーダの管理番号 </param>
		void Link(GLuint prog, GLuint vertex, GLuint fragment)
		{
			// 頂点シェーダを指定
			glAttachShader(prog, vertex);

			// フラグメントシェーダを指定
			glAttachShader(prog, fragment);

			// 指定したシェーダをリンク
			glLinkProgram(prog);
		}

		/// <summary>
		/// シェーダを初期化
		/// </summary>
		/// <param name="[out] progList"> シェーダプログラムの管理番号配列 </param>
		void Initialize(ProgList& progList)
		{
			// -----------------------------
			// シェーダのコンパイル
			// -----------------------------

			// ----------- スタンダードシェーダ ------------

			// 頂点シェーダ
			GLuint vertexShader = Compile(
				GL_VERTEX_SHADER, "Res/Shader/standard.vert");

			// フラグメントシェーダ
			GLuint fragmentShader = Compile(
				GL_FRAGMENT_SHADER, "Res/Shader/standard.frag");

			// ------------ アンリットシェーダ -------------

			// 頂点シェーダ
			GLuint vertexShaderUnlit = Compile(
				GL_VERTEX_SHADER, "Res/Shader/Unlit.vert");

			// フラグメントシェーダシェーダ
			GLuint fragmentShaderUnlit = Compile(
				GL_FRAGMENT_SHADER, "Res/Shader/Unlit.frag");

			// --------------------------
			// シェーダをリンク
			// --------------------------

			// ----------- スタンダードシェーダ ------------

			// 作成したシェーダプログラムの管理番号を設定
			GLuint progStandard = glCreateProgram();

			// 頂点シェーダとフラグメントシェーダをリンク
			Link(progStandard, vertexShader, fragmentShader);

			// 作成したシェーダを配列に追加
			progList.emplace(ProgType::Standard, progStandard);

			// ------------ アンリットシェーダ -------------

			// 作成したシェーダプログラムの管理番号を設定
			GLuint progUnlit = glCreateProgram();

			// 頂点シェーダとフラグメントシェーダをリンク
			Link(progUnlit, vertexShaderUnlit, fragmentShaderUnlit);

			// 作成したシェーダを配列に追加
			progList.emplace(ProgType::Unlit, progUnlit);
		}

	} // namespace Shader

} // namespace PokarinEngine