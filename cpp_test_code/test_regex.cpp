/**
   テストプログラム
   正規表現を用いた，ファイルパスからファイル名の抽出に関するテスト
 */
#include <iostream>
#include <string>
#include <regex>

int main(int argc, char **argv) {
  std::string mod_name = "./fortran/lcm/lcm_opt.s";
  std::regex pattern(".*/(.*)\\.s");
  std::smatch match;

  std::regex_match(mod_name, match, pattern);

  std::cout << "match.size(): " << std::to_string(match.size()) << std::endl;
  for(int i=0; i<match.size(); ++i)
    std::cout << match[i].str() << std::endl;

  std::cout << "ary: " << match[match.size()-1].str() << std::endl;
  std::string str(match[match.size()-1].str(),
                  1, match[match.size()-1].str().length() - 3);
  std::cout << str << std::endl;

  return 0;
}
