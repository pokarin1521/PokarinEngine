/**
* @file Shader.cpp
*/
#include "Shader.h"

#include "../Debug.h"

#include <fstream>
#include <filesystem>

namespace PokarinEngine
{
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
	/// デストラクタ
	/// </summary>
	Shader::~Shader()
	{
		// 作成したシェーダプログラムの解放
		for (const auto& [type, id] : progList)
		{
			glDeleteProgram(id);
		}

		// 作成したシェーダオブジェクトの解放
		for (const auto& id : shaderObjectList)
		{
			glDeleteShader(id);
		}
	}

	/// <summary>
	/// シェーダを初期化
	/// </summary>
	void Shader::Initialize()
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
	/// シェーダファイルを読み込んでコンパイルする
	/// </summary>
	/// <param name="[in] type"> シェーダの種類 </param>
	/// <param name="[in] fileName"> シェーダファイル名 </param>
	/// <returns> シェーダの識別番号 </returns>
	GLuint Shader::Compile(GLenum type, const char* fileName)
	{
		// ----------------------------
		// ファイルを開く
		// ----------------------------

		// バイナリモードを指定
		// 改行文字の変換等を行わないので、読み込みが速い
		std::ifstream file(fileName, std::ios::binary);

		if (!file)
		{
			LOG_ERROR("%sを開けません", fileName);

			return 0;
		}

		// ----------------------------------
		//	ファイルを読み込む
		// ----------------------------------
	
		// ファイルサイズ
		// ファイル全体を一気に読み込むために使う
		const size_t filesize = std::filesystem::file_size(fileName);

		// 読み込んだ情報の格納先
		std::vector<char> buffer(filesize);

		// ファイルを読み込み、bufferに格納
		file.read(buffer.data(), filesize);
		file.close();

		// ----------------------------------------------------
		// シェーダオブジェクトを作成してコンパイルする
		// ----------------------------------------------------

		// 読み込んだ情報
		const char* source[] = { buffer.data() };

		// 情報の長さ
		const GLint length[] = { int(buffer.size()) };

		// 作成したシェーダオブジェクトの識別番号
		const GLuint shader = glCreateShader(type);

		// ソースコードの数
		static const GLsizei sourceCount = 1;

		// シェーダオブジェクトにソースコードを設定
		glShaderSource(shader, sourceCount, source, length);

		// コンパイル
		glCompileShader(shader);

		// シェーダオブジェクトを配列に追加する
		shaderObjectList.push_back(shader);

		return shader;
	}

	/// <summary>
	/// 使用するシェーダプログラムを設定する
	/// </summary>
	/// <param name="progType"> 使用するシェーダプログラムの種類 </param>
	void Shader::UseProgram(ProgType progType)
	{
		glUseProgram(progList[progType]);
	}

	/// <summary>
	/// テクスチャをバインドする
	/// </summary>
	/// <param name="textureUnit"> バインド先の番号 </param>
	/// <param name="texture"> バインドするテクスチャの識別番号 </param>
	void Shader::BindTexture(TextureUnit textureUnit, const TexturePtr& texture)
	{
		// テクスチャの識別番号
		GLuint textureID = *texture;

		// バインド
		glBindTextures(GLuint(textureUnit), bindCount, &textureID);
	}

	/// <summary>
	/// テクスチャのバインドを解除する
	/// </summary>
	/// <param name="[in] textureUnit"> バインド先の番号 </param>
	void Shader::UnBindTexture(TextureUnit textureUnit)
	{
		// テクスチャの識別番号に0を入れることでバインドを解除する
		glBindTextures(GLuint(textureUnit), bindCount, 0);
	}

	/// <summary>
	/// Vector4型の値をシェーダから取得する
	/// </summary>
	/// <param name="[in] progType"> 値があるシェーダの種類 </param>
	/// <param name="[in] location"> 取得する値のロケーション番号 </param>
	/// <param name="[out] value"> 値の格納先 </param>
	Vector4 Shader::GetVector4(ProgType progType, UniformVector4 location)
	{
		/* ColorとVector4両方に対応できるように、
		ローカル変数で取得して返すようにする */

		// 取得した値の格納先
		static Vector4 value = Vector4::zero;

		// 値を取得して返す
		glGetUniformfv(progList[progType], GLint(location), &value.x);

		return value;
	}

	/// <summary>
	/// Matrix4x4型の値をシェーダに設定する
	/// </summary>
	/// <param name="[in] progType"> 設定するシェーダの種類 </param>
	/// <param name="[in] location"> 設定する値のロケーション番号 </param>
	/// <param name="[in] value"> 設定する値 </param>
	void Shader::SetMatrix4x4(ProgType progType, UniformMatrix4x4 location, const Matrix4x4& value)
	{
		// 行列の転置は行えるようにする
		glProgramUniformMatrix4fv(progList[progType], GLint(location),
			setCount, GL_FALSE, &value[0].x);
	}

	/// <summary>
	/// Matrix3x3型の値をシェーダに設定する
	/// </summary>
	/// <param name="[in] progType"> 設定するシェーダの種類 </param>
	/// <param name="[in] location"> 設定する値のロケーション番号 </param>
	/// <param name="[in] value"> 設定する値 </param>
	void Shader::SetMatrix3x3(ProgType progType, UniformMatrix3x3 location, const Matrix3x3& value)
	{
		// 行列の転置は行わないようにする
		glProgramUniformMatrix3fv(progList[progType], GLint(location),
			setCount, GL_FALSE, &value[0].x);
	}

	/// <summary>
	/// Vector4型配列の値をシェーダに設定する
	/// </summary>
	/// <param name="[in] progType"> 設定するシェーダの種類 </param>
	/// <param name="[in] location"> 設定する値のロケーション番号 </param>
	/// <param name="[in] value"> 設定する値 </param>
	void Shader::SetVector4List(ProgType progType, UniformVector4List location, const std::vector<Vector4>& value)
	{	
		glProgramUniform4fv(progList[progType], GLint(location),
			GLsizei(value.size()), &value[0].x);
	}

	/// <summary>
	/// Vector4型の値をシェーダに設定する
	/// </summary>
	/// <param name="[in] progType"> 設定するシェーダの種類 </param>
	/// <param name="[in] location"> 設定する値のロケーション番号 </param>
	/// <param name="[in] value"> 設定する値 </param>
	void Shader::SetVector4(ProgType progType, UniformVector4 location, const Vector4& value)
	{
		glProgramUniform4fv(progList[progType], GLint(location), setCount, &value.x);
	}

	/// <summary>
	/// Vector3型の値をシェーダに設定する
	/// </summary>
	/// <param name="[in] progType"> 設定するシェーダの種類 </param>
	/// <param name="[in] location"> 設定する値のロケーション番号 </param>
	/// <param name="[in] value"> 設定する値 </param>
	void Shader::SetVector3(ProgType progType, UniformVector3 location, const Vector3& value)
	{
		glProgramUniform3fv(progList[progType], GLint(location), setCount, &value.x);
	}

	/// <summary>
	/// Vector2型の値をシェーダに設定する
	/// </summary>
	/// <param name="[in] progType"> 設定するシェーダの種類 </param>
	/// <param name="[in] location"> 設定する値のロケーション番号 </param>
	/// <param name="[in] value"> 設定する値 </param>
	void Shader::SetVector2(ProgType progType, UniformVector2 location, const Vector2& value)
	{
		glProgramUniform2fv(progList[progType], GLint(location), setCount, &value.x);
	}

	/// <summary>
	/// int型の値をシェーダに設定する
	/// </summary>
	/// <param name="[in] progType"> 設定するシェーダの種類 </param>
	/// <param name="[in] location"> 設定する値のロケーション番号 </param>
	/// <param name="[in] value"> 設定する値 </param>
	void Shader::SetInt(ProgType progType, UniformInt location, int value)
	{
		glProgramUniform1i(progList[progType], GLint(location), value);
	}

} // namespace PokarinEngine