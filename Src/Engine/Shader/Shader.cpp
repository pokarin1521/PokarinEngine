/**
* @file Shader.cpp
*/
#include "Shader.h"

#include "../Debug.h"

#include <fstream>
#include <filesystem>

namespace PokarinEngine
{
	/// <summary>
	/// シェーダ管理用
	/// </summary>
	namespace Shader
	{
		// ------------------------------
		// 変数
		// ------------------------------

		// シェーダプログラムの識別番号配列
		ProgList progList;

		// ------------------------------
		// 関数
		// ------------------------------

		/// ここでしか使わないので、cppのみに書く
		/// <summary>
		/// シェーダファイルを読み込んでコンパイルする
		/// </summary>
		/// <param name="[in] type"> シェーダの種類 </param>
		/// <param name="[in] fileName"> シェーダファイル名 </param>
		/// <returns> シェーダの識別番号 </returns>
		GLuint Compile(GLenum type, const char* fileName)
		{
			// バイナリモードを指定
			// 改行文字の変換等を行わないので、読み込みが速い
			std::ifstream file(fileName, std::ios::binary);

			if (!file)
			{
				LOG_ERROR("%sを開けません", fileName);

				return 0;
			}

			//	ファイルを読み込む
			// ファイル全体を一気に読み込むために、ファイルサイズ取得
			const size_t filesize = std::filesystem::file_size(fileName);
			std::vector<char> buffer(filesize);

			// ファイルを読み込み、bufferに格納
			file.read(buffer.data(), filesize);
			file.close();

			const char* source[] = { buffer.data() };
			const GLint length[] = { int(buffer.size()) };

			// 作成したシェーダオブジェクトの識別番号
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
		/// <param name="[in] prog"> プログラムオブジェクトの識別番号 </param>
		/// <param name="[in] vertex"> 頂点シェーダの識別番号 </param>
		/// <param name="[in] fragment"> フラグメントシェーダの識別番号 </param>
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
		void Initialize()
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

			// 作成したシェーダプログラムの識別番号を設定
			GLuint progStandard = glCreateProgram();

			// 頂点シェーダとフラグメントシェーダをリンク
			Link(progStandard, vertexShader, fragmentShader);

			// 作成したシェーダを配列に追加
			progList.emplace(ProgType::Standard, progStandard);

			// ------------ アンリットシェーダ -------------

			// 作成したシェーダプログラムの識別番号を設定
			GLuint progUnlit = glCreateProgram();

			// 頂点シェーダとフラグメントシェーダをリンク
			Link(progUnlit, vertexShaderUnlit, fragmentShaderUnlit);

			// 作成したシェーダを配列に追加
			progList.emplace(ProgType::Unlit, progUnlit);
		}

		/// <summary>
		/// シェーダプログラムの識別番号を取得する
		/// </summary>
		/// <param name="[in] progType"> シェーダプログラムの種類 </param>
		/// <returns> 指定した種類のシェーダプログラムの識別番号 </returns>
		GLuint GetProgram(ProgType progType)
		{
			return progList[progType];
		}

		/// <summary>
		/// 全てのシェーダプログラムの識別番号を取得する
		/// </summary>
		/// <returns> 
		/// <para> 全てのシェーダプログラムの識別番号 </para>
		/// <para> [シェーダプログラムの種類, シェーダプログラムの識別番号] </para>
		/// </returns>
		const ProgList& GetAllProgram()
		{
			return progList;
		}

	} // namespace Shader

} // namespace PokarinEngine