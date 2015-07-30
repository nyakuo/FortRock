#include <vector>

class CDFG_Array : public CDFG_Node {
public:

private:
  unsigned _dimension; //! 配列の次元数
  std::vector<unsigned> _length; //! 各次元の配列の長さ
};
