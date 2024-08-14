/**
* @file Shader.h
*/
#ifndef POKARINENGINE_SHADER_H_INCLUDED
#define POKARINENGINE_SHADER_H_INCLUDED

#include "glad/glad.h"

#include "../Math/Matrix.h"
#include "../Math/Vector.h"
#include "../Color.h"
#include "../Texture.h"

#include <unordered_map>

namespace PokarinEngine
{
	// -----------------------
	// 前方宣言
	// -----------------------

	/// <summary>
	/// テクスチャイメージユニットの番号
	/// </summary>
	enum class TextureUnit;

	/// <summary>
	/// 4x4行列のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformMatrix4x4;

	/// <summary>
	/// 3x3行列のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformMatrix3x3;

	/// <summary>
	/// Vector4型配列のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformVector4List;

	/// <summary>
	/// Vector4型のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformVector4;

	/// <summary>
	/// Vector3型のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformVector3;

	/// <summary>
	/// Vector2型のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformVector2;

	/// <summary>
	/// Int型のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformInt;

	/// <summary>
	/// シェーダ管理用クラス
	/// </summary>
	class Shader
	{
	public: // ------------- コンストラクタ・デストラクタ --------------

		Shader() = default;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Shader();

	public: // ------------------------ 禁止事項 -----------------------

		/* シェーダ管理がおかしくなるので禁止する */

		// コピーコンストラクタの禁止
		Shader(const Shader&) = delete;

		// 代入の禁止
		Shader& operator=(const Shader&) = delete;

	public: // --------------------- シェーダの種類 --------------------

		/// <summary>
		/// シェーダプログラムの種類
		/// </summary>
		enum class ProgType
		{
			Standard, // 標準
			Unlit,    // ライティング無し 
		};

	public: // ------------------------- 初期化 ------------------------

		/* デストラクタで終了処理が行えるように、
		インスタンス化して初期化させるようにする */

		/// <summary>
		/// シェーダを初期化
		/// </summary>
		void Initialize();

	public: // --------------------- シェーダの使用 --------------------

		/// <summary>
		/// 使用するシェーダプログラムを設定する
		/// </summary>
		/// <param name="[in] progType"> 使用するシェーダプログラムの種類 </param>
		static void UseProgram(ProgType progType);

	public: // ----------------- テクスチャのバインド ------------------

		/// <summary>
		/// テクスチャをバインドする
		/// </summary>
		/// <param name="[in] textureUnit"> バインド先の番号 </param>
		/// <param name="[in] texture"> バインドするテクスチャ </param>
		static void BindTexture(TextureUnit textureUnit, const TexturePtr& texture);	
		
		/// <summary>
		/// テクスチャのバインドを解除する
		/// </summary>
		/// <param name="[in] textureUnit"> バインド先の番号 </param>
		static void UnBindTexture(TextureUnit textureUnit);

	public: // ------------------------ 値の取得 -----------------------

		/// <summary>
		/// Vector4型の値をシェーダから取得する
		/// </summary>
		/// <param name="[in] progType"> 値があるシェーダの種類 </param>
		/// <param name="[in] location"> 取得する値のロケーション番号 </param>
		static Vector4 GetVector4(ProgType progType, UniformVector4 location);

	public: // ------------------------ 値の設定 -----------------------

		/// <summary>
		/// Matrix4x4型の値をシェーダに設定する
		/// </summary>
		/// <param name="[in] progType"> 設定するシェーダの種類 </param>
		/// <param name="[in] location"> 設定する値のロケーション番号 </param>
		/// <param name="[in] value"> 設定する値 </param>
		static void SetMatrix4x4(ProgType progType, UniformMatrix4x4 location, const Matrix4x4& value);

		/// <summary>
		/// Matrix3x3型の値をシェーダに設定する
		/// </summary>
		/// <param name="[in] progType"> 設定するシェーダの種類 </param>
		/// <param name="[in] location"> 設定する値のロケーション番号 </param>
		/// <param name="[in] value"> 設定する値 </param>
		static void SetMatrix3x3(ProgType progType, UniformMatrix3x3 location, const Matrix3x3& value);

		/// <summary>
		/// Vector4型配列の値をシェーダに設定する
		/// </summary>
		/// <param name="[in] progType"> 設定するシェーダの種類 </param>
		/// <param name="[in] location"> 設定する値のロケーション番号 </param>
		/// <param name="[in] value"> 設定する値 </param>
		static void SetVector4List(ProgType progType, UniformVector4List location, const std::vector<Vector4>& value);

		/// <summary>
		/// Vector4型の値をシェーダに設定する
		/// </summary>
		/// <param name="[in] progType"> 設定するシェーダの種類 </param>
		/// <param name="[in] location"> 設定する値のロケーション番号 </param>
		/// <param name="[in] value"> 設定する値 </param>
		static void SetVector4(ProgType progType, UniformVector4 location, const Vector4& value);

		/// <summary>
		/// Vector3型の値をシェーダに設定する
		/// </summary>
		/// <param name="[in] progType"> 設定するシェーダの種類 </param>
		/// <param name="[in] location"> 設定する値のロケーション番号 </param>
		/// <param name="[in] value"> 設定する値 </param>
		static void SetVector3(ProgType progType, UniformVector3 location, const Vector3& value);

		/// <summary>
		/// Vector2型の値をシェーダに設定する
		/// </summary>
		/// <param name="[in] progType"> 設定するシェーダの種類 </param>
		/// <param name="[in] location"> 設定する値のロケーション番号 </param>
		/// <param name="[in] value"> 設定する値 </param>
		static void SetVector2(ProgType progType, UniformVector2 location, const Vector2& value);

		/// <summary>
		/// int型の値をシェーダに設定する
		/// </summary>
		/// <param name="[in] progType"> 設定するシェーダの種類 </param>
		/// <param name="[in] location"> 設定する値のロケーション番号 </param>
		/// <param name="[in] value"> 設定する値 </param>
		static void SetInt(ProgType progType, UniformInt location, int value);

	public: // ------------------------- ライト ------------------------

		// 使用可能なライトの最大数
		inline static constexpr size_t lightMax = 16;

	private: // ---------------------- コンパイル ----------------------

		/// <summary>
		/// シェーダファイルを読み込んでコンパイルする
		/// </summary>
		/// <param name="[in] type"> シェーダの種類 </param>
		/// <param name="[in] fileName"> シェーダファイル名 </param>
		/// <returns> シェーダの識別番号 </returns>
		static GLuint Compile(GLenum type, const char* fileName);

	private: // -------------------- 型の別名を定義 --------------------

		// シェーダプログラムの識別番号配列
		// <シェーダプログラムの種類, シェーダプログラムの識別番号>
		using ProgList = std::unordered_map<ProgType, GLuint>;

	private: // -------------------- シェーダ管理用 --------------------

		// シェーダプログラムの識別番号配列
		// <シェーダプログラムの種類, シェーダプログラムの識別番号>
		inline static ProgList progList;

		// シェーダオブジェクトの識別番号配列
		inline static std::vector<GLuint> shaderObjectList;

	private: // ------------------- 設定・バインド用 -------------------

		// バインドするテクスチャの数
		static const GLsizei bindCount = 1;

		// シェーダに設定する値の数
		static const GLsizei setCount = 1;
	};

	// ---------------------------------
	// 列挙型の定義
	// ---------------------------------

	/* Shaderクラスの値を使う列挙型があるので、ここでまとめて定義する */

	/// <summary>
	/// テクスチャイメージユニットの番号
	/// </summary>
	enum class TextureUnit
	{
		// 通常の色用
		color = 0,

		// 発光色用
		emission = 1,
	};

	/// <summary>
	/// 4x4行列のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformMatrix4x4
	{
		// 座標変換行列
		transformMatrix = 0,
	};

	/// <summary>
	/// 3x3行列のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformMatrix3x3
	{
		// 法線変換行列
		normalMatrix = 1,
	};

	/// <summary>
	/// Vector4型配列のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformVector4List
	{
		/* ライト構造体の要素は配列なので
		配列の数分ロケーション番号をずらす必要がある

		配列のインデックス0番に合わせる */

		// ライトの色と減衰開始角度
		lightColorAndFalloffAngle = 111,

		// ライトの位置と範囲(半径)
		lightPositionAndRange = lightColorAndFalloffAngle + Shader::lightMax,

		// ライトの向きと最大照射角度
		lightDirectionAndSpotAngle = lightPositionAndRange + Shader::lightMax,
	};

	/// <summary>
	/// Vector4型のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformVector4
	{
		// オブジェクトの色
		color = 100,

		// オブジェクトの発光色と
		// 発光色テクスチャの識別番号
		emission = 101,
	};

	/// <summary>
	/// Vector3型のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformVector3
	{
		// -------------------
		// カメラ
		// -------------------

		// カメラの座標
		cameraPosition = 4,

		// カメラの回転角度
		cameraRotation = 5,

		// -------------------
		// ライト
		// -------------------

		// 環境光
		// 方向を持たないので色だけ
		ambientLight = 107,

		// 平行光源の色
		directionalLight_Color = 108,

		// 平行光源の向き
		directionalLight_Direction = 109,
	};

	/// <summary>
	/// Vector2型のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformVector2
	{
		// アスペクト比の逆数と視野角による拡大率の逆数
		inverse_aspectRatioAndScaleFov = 3,
	};

	/// <summary>
	/// Int型のユニフォーム変数のロケーション番号
	/// </summary>
	enum class UniformInt
	{
		// 使用するライトの数
		lightCount = 110,
	};

} // namespace PokarinEngine

#endif // !POKARINENGINE_SHADER_H_INCLUDED
