#ifndef _CLABEL_H
#define _CLABEL_H

#include <string>

/**
   @class CLabel
   @brief LLVM IRのラベル(CFDのラベル)を管理するデータクラス
 */
class CLabel {
public:
  CLabel(const std::string & name,
         const unsigned & state);

  std::string & get_name(void);
  unsigned & get_state(void);

private:
  std::string _name;
  unsigned _state;
};

#endif
