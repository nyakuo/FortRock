#include "COutput.hpp"

/**
   コンストラクタ
   @paramm[in] filename 出力ファイル名
   @param[in] indent_width インデントのスペース文字の数
 */
COutput::COutput(void)
  : _INDENT_WIDTH(2),
    _indent_level(0)
{}

/**
   デストラクタ
   @brief ofstreamを閉じる
 */
COutput::~COutput(void) {
  this->_ofs.close();
}

/**
   ofstreamを開く
   @param[in] 出力するファイル名
 */
void COutput::open_ofstream(const std::string filename) {
  this->_ofs.open(filename, std::ios::out);
}

void COutput::close_ofstream(void) {
  this->_ofs.close();
}

/**
   現在のインデントレベルでスペース文字を返す
   @return 現在のインデントレベル分のスペース文字列
*/
std::string COutput::output_indent(void) {
  std::string spaces = "";
  for (auto i=0;
       i<this->_INDENT_WIDTH * this->_indent_level;
       ++i)
    spaces += ' ';

  return spaces;
}

/**
   インデントの深さを浅くする
   @param[in] num_level 浅くする数
   @note _INDET_WIDTH * num_level 分だけインデントが浅くなる
         this->_indent_levelが0以下になる場合は0となる
 */
void COutput::indent_left(const unsigned & num_level) {
  if ((this->_indent_level - num_level) > 0)
    this->_indent_level -= num_level;
  else
    this->_indent_level = 0;
}

/**
   インデントの深さを深くする
   @param[in] num_level 深くする数
   @note _INDET_WIDTH * num_level 分だけインデントが浅くなる
 */
void COutput::indent_right(const unsigned & num_level) {
  this->_indent_level += num_level;
}

/**
   ファイルに文字列を出力する
   @param[in] str ファイルに出力する文字列
 */
std::ostream & COutput::operator << (const std::string & str) {
  this->_ofs << this->output_indent() << str;
  return this->_ofs.flush();
}

/**
   char *型の文字列をファイルに出力する
   @param[in] str ファイルに出力する文字列
   @note operator << (const std::string & str)に処理を渡している
 */
std::ostream & COutput::operator << (const char * str) {
  return (*this) << std::string(str);
}

/**
   char型をファイルに出力する
   @param[in] i ファイルに出力する文字
   @note operator << (const std::string & str)に処理を渡している
*/
std::ostream & COutput::operator << (const char & i) {
  return (*this) << std::string(1, i);
}

/**
   unsigned int型をファイルに出力
   @param[in] i ファイルに出力する数字
   @note operator << (const std::string & str)に処理を渡している
 */
std::ostream & COutput::operator << (const unsigned int & i) {
  return (*this) << std::to_string(i);
}

/**
   文字列をインデントなしでファイルに出力する
   @param[in] str ファイルに出力する文字列
 */
std::ostream & COutput::operator <<= (const std::string & str) {
  this->_ofs << str;
  return this->_ofs.flush();
}

/**
   char型の文字列をインデントなしでファイルに出力する
   @param[in] str ファイルに出力する文字列
   @note operator <<= (const std::string & str)に処理を渡している
 */
std::ostream & COutput::operator <<= (const char * str) {
  return (*this) <<= std::string(str);
}

/**
   char型をインデントなしでファイルに出力する
   @param[in] i ファイルに出力する文字
   @note operator <<= (const std::string & str)に処理を渡している
*/
std::ostream & COutput::operator <<= (const char & i) {
  return (*this) <<= std::string(1, i);
}

/**
   unsigned int型をインデントなしでファイルに出力
   @param[in] i ファイルに出力する数字
   @note operator <<= (const std::string & str)に処理を渡している
 */
std::ostream & COutput::operator <<= (const unsigned int & i) {
  return (*this) <<= std::to_string(i);
}
