#ifndef _COUTPUT_H
#define _COUTPUT_H

#include <string>
#include <fstream>

/**
   @class COutput
   @brief ファイル出力を補助するクラス
 */
class COutput {
public:
  COutput(void);
  ~COutput(void);

  void open_ofstream(const std::string filename);    ///< ofstreamを開く
  void indent_left(const unsigned & num_level = 1);  ///< インデントを浅くする
  void indent_right(const unsigned & num_level = 1); ///< インデントを深くする
  std::string output_indent(void);                   ///< 現在のインデントレベルでスペース文字を返す

  std::ostream & operator << (const std::string & str);  ///< ファイルに出力する
  std::ostream & operator << (const char * str);         ///< ファイルに出力する
  std::ostream & operator << (const char & i);           ///< ファイルに出力する
  std::ostream & operator << (const unsigned int & i);   ///< ファイルに出力する
  std::ostream & operator <<= (const std::string & str); ///< ファイルに出力する
  std::ostream & operator <<= (const char * str);        ///< ファイルに出力する
  std::ostream & operator <<= (const char & i);          ///< ファイルに出力する
  std::ostream & operator <<= (const unsigned int & i);  ///< ファイルに出力する

private:
  const unsigned _INDENT_WIDTH; ///< インデント幅 (コンストラクタで初期化)
  unsigned _indent_level;       ///< インデントの深さ
  std::ofstream _ofs;           ///< ファイル出力ストリーム
};

#endif
