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
  COutput(const std::string & filename,
          const unsigned & indent_width);
  ~COutput(void);

  void output(std::string & str);                    /** 出力の追加 */
  void indent_left(const unsigned & num_level = 0);  /** インデントを浅くする */
  void indent_right(const unsigned & num_level = 0); /** インデントを深くする */

  std::ostream &operator << (std::string & str); /** ファイルに出力する */

private:
  const unsigned _INDENT_WIDTH; /** インデント幅 (コンストラクタで初期化) */
  unsigned _indent_level;       /* インデントの深さ */
  std::ofstream _ofs;           /* ファイル出力ストリーム */
};

#endif
