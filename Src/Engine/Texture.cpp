/**
* @file Texture.cpp
*/
#include "Texture.h"
#include "Debug.h"

#include <vector>
#include <fstream>
#include <filesystem>

namespace PokarinEngine
{
	/// <summary>
	/// テクスチャを読み込むコンストラクタ
	/// (暗黙的キャストを禁止)
	/// </summary>
	/// <param name="filename"> 読み込むテクスチャファイルの名前 </param>
	Texture::Texture(const char* filename)
	{
		// ---------------------
		// ファイルを開く
		// ---------------------

		// バイナリモードを指定
		// 改行文字の変換等を行わないので、読み込みが速い
		std::ifstream file(filename, std::ios::binary);

		// ファイルが開いてなければ終了
		if (!file)
		{
			LOG_ERROR("%sを開けません", filename);
			return;
		}

		// --------------------------
		// ファイルを読み込む
		// --------------------------

		// ファイル全体を一気に読み込むために、ファイルサイズ取得
		const size_t filesize = std::filesystem::file_size(filename);
		std::vector<uint8_t> buffer(filesize);

		// ファイルを読み込み、bufferに格納
		file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

		// もうファイル操作をしないので閉じる
		file.close();

		// ------------------------------
		//	ヘッダから情報を取得
		// ------------------------------

		/* TGAファイルの先頭18バイトはTGAヘッダというデータ領域
		ヘッダには画像サイズなどの情報があり、画像データはその後ろにある */

		// TGAヘッダのバイト数
		const size_t tgaHeaderSize = 18;

		// 画像データの位置
		// (ヘッダサイズ + ヘッダの後に続くIDの長さ)
		const int imageOffset = tgaHeaderSize + buffer[0];

		// 画像形式
		const uint8_t imageType = buffer[2];

		// 1ピクセルのバイト数
		// 8で割り切れない場合を考えて
		// 1少ない数字を足すことで、小数を切り上げる
		const int pixelBytes = (buffer[16] + 7) / 8;

		// ------------------------------
		// 画像の幅、高さを求める
		// ------------------------------

		// 画像の幅と高さはそれぞれ16ビットのデータ
		// 値を復元するため上位8ビットを256倍(左に8シフト)して合成
		width = buffer[12] + buffer[13] * 256;
		fboHeight = buffer[14] + buffer[15] * 256;

		// ----------------------------------
		// 画像が圧縮形式なら展開する
		// ----------------------------------

		/* ----- 画像形式 -----
		 0(0b0000) : 画像なし

		 1(0b0001) : インデックス(無圧縮)
		 2(0b0010) : トゥルーカラー(無圧縮)
		 3(0b0011) : 白黒(無圧縮)

		 9(0b1001) : インデックス(RLE)
		10(0b1010) : トゥルーカラー(RLE)
		11(0b1011) : 白黒(RLE) */

		// 第3ビットが1の場合、RLE(圧縮形式)
		if (imageType & 0b1000)
		{
			// ------------ 展開前の準備 --------------

			// 展開後のバイト数
			const int imageBytes = width * fboHeight * pixelBytes;

			// 展開用のバッファ
			// 画像データより前の部分(ヘッダ + ID)をコピーするので
			// その分サイズをプラスしておく
			std::vector<uint8_t> tmp(imageOffset + imageBytes);

			// 展開後にtmpとbufferを入れ替えるので、
			// 画像データより前の部分(ヘッダ + ID)をコピー
			std::copy_n(buffer.begin(), imageOffset, tmp.begin());

			// ----------- 画像データを展開 ------------

			// 圧縮データのアドレス
			const uint8_t* src = buffer.data() + imageOffset;

			// データ展開先アドレス
			uint8_t* dest = tmp.data() + imageOffset;

			// 展開終了アドレス
			const uint8_t* const destEnd = dest + imageBytes;

			while (dest != destEnd)
			{
				// パケットID (パケットヘッダの最上位の1ビット)
				// 0 : 無圧縮パケット
				// 1 : 圧縮パケット
				const int isRLE = *src & 0x80;

				// データ数 (パケットヘッダの0～6ビット)
				//「データ数 - 1」が記録されているので+1している
				const int count = (*src & 0x7f) + 1;

				// アドレスをパケットデータの位置に進める
				++src;

				// 圧縮パケット
				if (isRLE)
				{
					// パケットデータを指定回数コピー
					for (int a = 0; a < count; ++a)
					{
						memcpy(dest, src, pixelBytes);
						dest += pixelBytes;
					}

					src += pixelBytes;
				}
				// 無圧縮パケット
				else
				{
					// パケットデータのバイト数
					const int dataBytes = pixelBytes * count;

					// パケットデータ全体をコピー
					memcpy(dest, src, dataBytes);

					dest += dataBytes;
					src += dataBytes;
				}

			} // while dest

			// bufferとtmpの内容を交換
			buffer.swap(tmp);
		}

		// --------------------------------------
		// データの格納方向を調べ、対応する
		// --------------------------------------

		/* TGAファイルは画像データの格納方向を選ぶことができる
		TGAのデフォルトでは画像は「下から上」に格納される
		しかし、変換に使ったツールによって「上から下」に格納される

		OpenGLの画像データは「下から上」に格納されるので、
		格納方向によって画像が反転してしまう

		これを防ぐため、TGAファイルの17バイト目にある格納方向を読み取る

		17バイト目の内訳は、
		bit:0-3 アルファ要素のビット数,
		bit:4	左右の格納方向(0 = 左から右, 1 = 右から左),
		bit:5	上下の格納方向(0 = 下から上, 1 = 上から下),
		bit:6-7 (未使用)

		上下の格納方向が知りたいので、5ビット目を調べる */

		// 5ビット目を調べる
		const bool topToBottom = buffer[17] & 0b0010'0000;

		// 格納方向が「上から下」の場合、
		// データを上下反転
		if (topToBottom)
		{
			// 画像の1ピクセルのビット数
			const int pixelDepth = buffer[16];

			// 一行のバイト数
			const int lineByteSize = width * pixelDepth / 8;

			// 先頭18バイト分飛ばして、画像の一番上の行を求める
			uint8_t* top = buffer.data() + tgaHeaderSize; // 上の行の位置		

			// 最後の行以外の画像面積 + 一番上の行の位置で、
			// 一番下の行の位置を求める
			uint8_t* bottom = top + lineByteSize * (fboHeight - 1); // 下の行の位置

			// 一行ずつ保管できるようにサイズ確保
			std::vector<uint8_t> tmp(lineByteSize); // 上下入れ替え用バッファ

			// 行単位で上下反転
			// 上下の行の位置が逆転するまで繰り返す
			while (top < bottom)
			{
				// 「上の行のコピー」を作る
				std::copy_n(top, lineByteSize, tmp.data());

				// 下の行を、上の行に上書き
				std::copy_n(bottom, lineByteSize, top);

				// 「上の行のコピー」を下の行に上書き
				std::copy_n(tmp.data(), lineByteSize, bottom);

				// 上の行の位置を1行下に移動
				top += lineByteSize;

				// 下の行の位置を1行上に移動
				bottom -= lineByteSize;
			}

		} // if topToBottom

		// ----------------------------------------------------
		// 1ピクセルのビット数に対応する形式のセットを検索
		// ----------------------------------------------------

		// 形式
		struct Format
		{
			int pixelDepth;		// TGAの1ピクセルのビット数

			GLenum imageFormat;	// TGAのデータ形式

			GLenum imageType;	// TGAのデータ型

			GLenum gpuFormat;	// GPU側のデータ形式
		};

		// 形式リスト
		constexpr Format formatList[] = {
			{ 32, GL_BGRA, GL_UNSIGNED_BYTE,			  GL_RGBA8 },
			{ 24, GL_BGR,  GL_UNSIGNED_BYTE,			  GL_RGB8 },
			{ 16, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_RGB5_A1 },
			{ 15, GL_BGR,  GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_RGB5 },
			{  8, GL_RED,  GL_UNSIGNED_BYTE,			  GL_R8 },
		};

		// 未対応のビット数のTGAファイルを読み込んだときに
		// 配列の最後の要素が入るように、end()-1する

		// ピクセルのビット数に応じた形式
		const Format* format = std::find_if(
			formatList, std::end(formatList) - 1,
			[pixelDepth = buffer[16]](const Format& e) {
				return e.pixelDepth == pixelDepth; });

		// ------------------------------------------------
		// 画像に合わせて、アラインメントを変更する
		// ------------------------------------------------

		/* OpenGLが画像データを読み取るときは、
		「データを4バイト単位で読み取る」という仕組みになっている
		この読み取り単位のことを「アラインメント(Alignment, 整列)」という

		TGAファイルの場合、
		「横一列のピクセルを記録したら、次の列のピクセルを記録する」となっている

		32ビット形式の画像データなら、
		横一列のデータ数は常にOpenGLのアラインメントで割り切れる
		しかし、8ビット形式や24ビット形式の場合、
		横幅によってはアラインメントで割り切れずエラーが発生する

		なので、画像に合わせてアラインメントを変更する
		(インターネットから画像を拾うことになるので、選別はきつい) */

		// アラインメント記録用
		// (元に戻すのに使う)
		GLint alignment;

		// 現在のアラインメントを記録
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);

		// アラインメント配列
		// 1行のバイト数が4で割り切れる : 4
		// 2で割り切れる(4で割って2余る) : 2
		// それ以外	: 1
		constexpr int alignmentList[] = { 4, 1, 2, 1 };

		// 1行のバイト数
		const int lineByteSize = width * pixelBytes;

		// 画像に合わせたアラインメント
		const int imageAlignment = alignmentList[lineByteSize % 4];

		// 記録したOpenGLのアラインメントと
		// 画像に合わせたアラインメントが違う場合
		if (alignment != imageAlignment)
		{
			// OpenGLのアラインメントを変更
			glPixelStorei(GL_UNPACK_ALIGNMENT, imageAlignment);
		}

		// -----------------------
		// テクスチャを作成
		// -----------------------

		//	テクスチャの管理番号
		GLuint object = 0;

		// 画像データを管理するテクスチャオブジェクトを作成
		glCreateTextures(GL_TEXTURE_2D, 1, &object);

		/* 画像データのコピー先であるGPUメモリ領域を確保

		void glTextureStorage2D(管理番号, 作成するレベル数,
		ピクセル形式, 幅, 高さ)

		------- レベル数 -------
		ミップマップに関連する引数、
		今回は使わないので最小値の1を指定

		------ ミップマップ ------
		視点からオブジェクトまでの距離に応じて、
		大きさの違うテクスチャを使い分ける機能 */

		glTextureStorage2D(object, 1, format->gpuFormat, width, fboHeight);

		/* 画像データをGPUメモリにコピー

		void glTextureSubImage2D(テクスチャの管理番号, コピー先レイヤー番号,
		コピー先のX座標, コピー先のY座標,
		コピーする画像の幅, コピーする画像の高さ,
		ピクセルに含まれる要素と順序, 要素の型, 画像データのアドレス)

		画像データはテクスチャの
		(コピー先のX座標, コピー先のY座標)の位置にコピーされる
		コピー先の座標指定は、
		複数の画像データをひとつのテクスチャにまとめたい場合に使う */

		glTextureSubImage2D(object, 0, 0, 0, width, fboHeight,
			format->imageFormat, format->imageType,
			buffer.data() + tgaHeaderSize);

		id = object; // テクスチャオブジェクト番号
		name = filename; // ファイル名

		// --------------------------------
		// アラインメントを元に戻す
		// --------------------------------

		/* アラインメントを変更すると、
		GPUメモリへのコピー速度が低下するので、
		画像データのコピーが終わったら元に戻す */

		// 記録したOpenGLのアラインメントと
		// 画像に合わせたアラインメントが違う場合
		if (alignment != imageAlignment)
		{
			// アラインメントが変更されているので
			// 元に戻す
			glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
		}

		// ---------------------------
		// 白黒画像に対応する 
		// ---------------------------

		/* データ形式が「GL_RED(赤要素だけ)」で指定しているので
		このままだと赤黒で表示されてしまう

		スウィズルという機能を使う
		(Swizzle,「かきまぜる」という意味)
		この機能を使うと、シェーダでテクスチャを読み取る時の
		データの順序を入れ替えることができる

		スウィズルを設定するには、glTextureParameteri関数を使う

		void glTextureParameteri(
		テクスチャの管理番号, 設定するパラメータ名, 設定する値) */

		// グレートスケールテクスチャの場合、
		// 赤成分を緑と青にコピーするに設定する
		if (format->imageFormat == GL_RED)
		{
			glTextureParameteri(object, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTextureParameteri(object, GL_TEXTURE_SWIZZLE_G, GL_RED);
			glTextureParameteri(object, GL_TEXTURE_SWIZZLE_B, GL_RED);
		}
	}

	/// <summary>
	/// テクスチャを作成するコンストラクタ
	/// (暗黙的キャストを禁止)
	/// </summary>
	/// <param name="width"> 幅 </param>
	/// <param name="height"> 高さ </param>
	Texture::Texture(GLsizei w, GLsizei h)
	{
		// ------------------------
		// 幅と高さを設定
		// ------------------------

		width = w;
		fboHeight = h;

		// -------------------------
		// テクスチャ作成
		// -------------------------

		glCreateTextures(GL_TEXTURE_2D, 1, &id);

		// ---------------------------------
		// テクスチャイメージを設定
		// ---------------------------------

		// 設定できるようにバインド
		glBindTexture(GL_TEXTURE_2D, id);

		// 設定
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			width, fboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// 誤操作のないようにバインド解除
		glBindTexture(GL_TEXTURE_2D, 0);

		// ----------------------------
		// ラッピング方法を設定
		// ----------------------------

		/* テクスチャの範囲外を参照した時は
		そこに画像が敷き詰められているように扱う(ラッピング)

		今回は、テクスチャの端の色を敷き詰めるように設定する */

		// 横方向
		glTextureParameteri(
			id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		// 縦方向
		glTextureParameteri(
			id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// ----------------------------------
		// フィルタリング方法を設定
		// ----------------------------------

		/* テクスチャを拡大・縮小した時に
		画像が崩れないように綺麗にする(フィルタリング)

		今回は、周囲の色から平均的な色を求めて使うように設定する */

		// 拡大時
		glTextureParameteri(
			id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 縮小時
		glTextureParameteri(
			id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	/// <summary>
	/// テクスチャを削除するデストラクタ
	/// </summary>
	Texture::~Texture()
	{
		// テクスチャオブジェクトを削除
		glDeleteTextures(1, &id);
	}

} // namespace PokarinEngine