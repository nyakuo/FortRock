#include "COutput.hpp"

/**
   コンストラクタ
   @paramm[in] filename 出力ファイル名
   @param[in] indent_width インデントのスペース文字の数
 */
COutput::COutput(const std::string & filename,
                 const unsigned & indent_width)
  : _INDENT_WIDTH(indent_width)
{
  this->_ofs .open(filename, std::ios::out);
}

/**
   デストラクタ
   @brief ofstreamを閉じる
 */
COutput::~COutput(void) {
  this->_ofs.close();
}

/**
   インデントの深さを浅くする
   @param[in] num_level 浅くする数
   @note _INDET_WIDTH * num_level 分だけインデントが浅くなる
         this->_indent_levelが0以下になる場合は0となる
 */
void COutput::indent_left(const unsigned & num_level) {
  if (this->_indent_level - num_level > 0)
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
   ファイルに出力する
   @param[in] str ファイルに出力する文字列
 */
std::ostream & COutput::operator  << (std::string & str) {
  std::string spaces = "";
  for (auto i=0;
       i<(this->_INDENT_WIDTH * this->_indent_level);
       ++i)
    spaces += ' ';

  this->_ofs << spaces<< str;
  return this->_ofs.flush();
}
