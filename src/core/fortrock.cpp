#include "fortrock.hpp"

/**
   入力された数を表すために必要なビット幅を計算する
   @param[in] num 表現したい種類の数
   @return 必要なビット幅
   @attention unsignedのビット幅を32ビットと仮定
*/
unsigned
FortRock::_get_required_bit_width
(const unsigned & num)
{
  unsigned val = num;

  val |= val >> 1;
  val |= val >> 2;
  val |= val >> 4;
  val |= val >> 8;
  val |= val >> 16;

  val = val - ((val >> 1) & 0x55555555);
  val = (val & 0x33333333) + ((val >> 2) & 0x33333333);
  val = (val + (val >> 4)) & 0x0f0f0f0f;
  val += val >> 8;
  val += val >> 16;

  return val;
} // _get_required_bit_width

/**
   moduleの名前を取得
   @param[in] M モジュールの参照
   @return モジュールの名前
*/
std::string
FortRock::_get_module_name
(const Module & M)
{
  std::string mod_name = M.getModuleIdentifier();
  std::regex pattern(".*/(.*)\\.s");
  std::smatch match;
  std::regex_match(mod_name, match, pattern);

  // ファイル名の抽出
  mod_name = std::string(match[match.size()-1].str(),
                         1,
                         match[match.size()-1].str().length() - 3);

  return mod_name;
} // _get_module_name

/**
   llvm::Value型の変数から変数名を取得する
   @param[in] v 取得対象のValue型変数
   @return 変数名
   @note 引数vが定数の場合は定数値のstringを返す
 */
std::string
FortRock::_get_value_name
(const Value * v)
{
  std::string ret_str ("");
  auto type = v->getType();

  ///< @todo getelementptr inbounds対応
  if (type->isPointerTy())
    type = type->getPointerElementType();

  if (!v->hasName()) { // 定数
    if (type->isIntegerTy()) { //! @todo Integer以外の型への対応
      auto int_value = dyn_cast<ConstantInt>(v);
      //! @todo is signed 対応する
      ret_str.append(int_value->getValue()
                     .toString(10,     // 基数
                               true)); // is signed
    } // if : isIntegerTy()
  } // if : hasName()
  else { // 変数
    ret_str.append(v->getName());
  }

  return ret_str;
} // _get_value_name

/**
   moduleの入出力を定義
*/
void FortRock::_set_IO
(const Module::FunctionListType::iterator & funct)
{
  auto arg_it  = funct->arg_begin();
  auto arg_end = funct->arg_end();

  for (auto num = 1; // 最後の引数(返り値)を知るため
       arg_it != arg_end;
       ++num, ++arg_it) {
    auto type = arg_it->getType();
    bool is_ptr = false;

    // ポインタならポインタの型を取得
    if (type->isPointerTy()) {
      type = type->getPointerElementType();
      is_ptr = true;
    }

    if (type->isIntegerTy()) {
      std::shared_ptr<CDFG_Node> node;
      bool is_input = false;

      // 入出力のインスタンス化
      if (num !=  funct->arg_size()) {
        node = std::make_shared<CDFG_Wire>
          (arg_it->getName(),
           type->getPrimitiveSizeInBits(),
           true, //! @todo is signed 対応
           CDFG_Wire::eWireType::IN_ORIG);

        is_input = true;
      }
      else { // 最後の引数は返り値
        node = std::make_shared<CDFG_Reg>
          (arg_it->getName(),
           type->getPrimitiveSizeInBits(),
           true, //! @todo is signed 対応
           CDFG_Reg::eRegType::OUT);
      }

      this->_module_gen->add_node(node);

      // 入力をコピーするレジスタの確保
      if (is_input) {
        auto copy_node = std::make_shared<CDFG_Reg>
          (arg_it->getName(),
           type->getPrimitiveSizeInBits(),
           true,
           CDFG_Reg::eRegType::IN_COPY);

        this->_module_gen->add_node(copy_node);

        // 入力をコピーする命令の確保
        auto elem = std::make_shared<CDFG_Element>
          (CDFG_Operator::eType::COPY,
           1, // 入力数
           0, // state
           0); // step

        elem->set_input(node, 0);
        elem->set_output(copy_node, 0);

        this->_module_gen->add_element(elem);

        node = copy_node;
      } // if : is_input

      // ポインタの確保
      if (is_ptr) {
        auto ptr = std::make_shared<CDFG_Addr>
          (arg_it->getName(),
           type->getPrimitiveSizeInBits(),
           node);

        this->_module_gen->add_node(ptr);
      } // if : is_ptr
    } // if : isIntegerTy()
    else if (type->isFloatTy()
             || type->isDoubleTy()) {
      std::shared_ptr<CDFG_Node> node;
      bool is_input = false;
      const unsigned bit_width
        = type->isFloatTy() ? 32 : 64;

      // 入出力のインスタンス化
      if (num !=  funct->arg_size()) {
        node = std::make_shared<CDFG_Wire>
          (arg_it->getName(),
           bit_width,
           true, //! @todo is signed 対応
           CDFG_Wire::eWireType::IN_ORIG);

        is_input = true;
      }
      else { // 最後の引数は返り値
        node = std::make_shared<CDFG_Reg>
          (arg_it->getName(),
           bit_width,
           true, //! @todo is signed 対応
           CDFG_Reg::eRegType::OUT);
      }

      this->_module_gen->add_node(node);

      // 入力をコピーするレジスタの確保
      if (is_input) {
        auto copy_node = std::make_shared<CDFG_Reg>
          (arg_it->getName(),
           bit_width,
           true,
           CDFG_Reg::eRegType::IN_COPY);

        this->_module_gen->add_node(copy_node);

        // 入力をコピーする命令の確保
        auto elem = std::make_shared<CDFG_Element>
          (CDFG_Operator::eType::COPY,
           1, // 入力数
           0, // state
           0); // step

        elem->set_input(node, 0);
        elem->set_output(copy_node, 0);

        this->_module_gen->add_element(elem);

        node = copy_node;
      } // if : is_input

      // ポインタの確保
      if (is_ptr) {
        auto ptr = std::make_shared<CDFG_Addr>
          (arg_it->getName(),
           type->getPrimitiveSizeInBits(),
           node);

        this->_module_gen->add_node(ptr);
      } // if : is_ptr
    } // if : float type
  } // for
} // _set_IO

/**
   FortRock passが最初に呼ばれる関数
*/
bool FortRock::runOnModule
(Module &M)
{
  this->_module_gen = std::make_shared<CModuleGenerator>
    ("output.v", this->_get_module_name(M));

  this->_grub_global_variables(M);

  // -------------------- Functions --------------------
  auto it = M.begin();
  auto end = M.end();

  try {
    if(++it != end)
      throw "ERROR: 1つのファイルに記述できるのは1つのモジュールだけです";
  }
  catch (char * str) {
    std::cerr << str << std::endl;
    return true;
  }

  this->_set_IO(--it);

  this->_grub_labels(it);
  this->_grub_variables(it);

  CDebug::output_node_info(this->_module_gen->get_node_list());

  // 演算器のインスタンス化
  CInstancingOperator cinst;
  cinst.instancing_operators(this->_module_gen,
                             this->_OPERATOR_CONFIG_FILENAME);

  // 処理のDFG化
  for (auto bb_it = it->begin(); // Basic Block
       bb_it != it->end();
       ++bb_it) {

    for (auto inst = bb_it->begin();
         inst != bb_it->end();
         ++inst)  // Instruction
      this->_parse_instructions(inst);

    this->_step = 0;
    ++this->_state;
  } // for : bb_it

  // step信号の追加
  auto step_bit_width = this->_get_required_bit_width
    (this->_module_gen->get_max_step());

  auto step_node = std::make_shared<CDFG_Reg>
    (this->_STEP_NAME,
     step_bit_width,
     false, /* is signed */
     CDFG_Reg::eRegType::STEP);

  this->_module_gen->add_node(step_node);

  // モジュールのファイル出力
  this->_module_gen->generate();

  return false;
} // runOnModule

/**
   各命令の処理関数を呼び出す
   @brief 処理関数がモジュールはDFGの登録などを行う
*/
void FortRock::_parse_instructions
(const Instruction * inst)
{
  try {
    switch (inst->getOpcode()) {
    case Instruction::Ret:    this->_add_ret_inst(inst);    break;
    case Instruction::Br:     this->_add_br_inst(inst);     break;
    case Instruction::Load:   this->_add_load_inst(inst);   break;
    case Instruction::Store:  this->_add_store_inst(inst);  break;
    case Instruction::ICmp:   this->_add_icmp_inst(inst);   break;
    case Instruction::PHI:    this->_add_phi_inst(inst);    break;
    case Instruction::Select: this->_add_select_inst(inst); break;
    case Instruction::SRem:   this->_add_srem_inst(inst);   break;
    case Instruction::Mul:    this->_add_mul_inst(inst);    break;
    case Instruction::FMul:   this->_add_fmul_inst(inst);   break;
    case Instruction::SDiv:   this->_add_sdiv_inst(inst);   break;
    case Instruction::Add:    this->_add_add_inst(inst);    break;
    case Instruction::FAdd:   this->_add_fadd_inst(inst);   break;
    case Instruction::Sub:    this->_add_sub_inst(inst);    break;
    case Instruction::FSub:   this->_add_fsub_inst(inst);   break;
    case Instruction::Switch: this->_add_switch_inst(inst); break;
    case Instruction::Shl:    this->_add_shift_inst(inst,
                             /* is left shift == */ true,
                          /* is logical shift == */ true);
      break;

    case Instruction::LShr:   this->_add_shift_inst(inst,
                             /* is left shift == */ false,
                          /* is logical shift == */ true);
      break;

    case Instruction::AShr:   this->_add_shift_inst(inst,
                             /* is left shift == */ false,
                          /* is logical shift == */ false);
      break;

    case Instruction::And:    this->_add_and_inst(inst);    break;
    case Instruction::Or:     this->_add_or_inst(inst);     break;
    case Instruction::Xor:    this->_add_xor_inst(inst);    break;
    case Instruction::Trunc:  this->_add_trunc_inst(inst);  break;

    case Instruction::GetElementPtr:
      this->_add_getelementptr_inst(inst);
      break;

    default:
      throw std::string(std::string("ERROR (") + __func__ + " :"
                        + std::string(inst->getOpcodeName())
                        + " "
                        + std::to_string(inst->getOpcode())
                        + std::to_string(Instruction ::Add)
                        + " 未定義のオペランド\n");
    } // switch
  } // try
  catch (std::string err) {
    errs() << err;
  } // catch
} // _parse_instructions

/**
   ADD命令をモジュールのDFGに追加する
   @brief b = load a
          b <= a; // latency == 0
 */
void FortRock::_add_load_inst
(const Instruction * inst)
{
  std::shared_ptr<CDFG_Addr> a;
  bool is_gepope = false;

  // 入力
  // getelementptr の場合
  auto ope = dyn_cast<LoadInst>(inst)->getPointerOperand();
  if (isa<GEPOperator>(ope)) {
    auto gepope = dyn_cast<GEPOperator>(ope);
    is_gepope = true;
    auto ptr = gepope->getPointerOperand();

    auto array
      = this->_module_gen->get_node
      (ptr->getName(),
       CDFG_Node::eNode::MEM);

    a = std::dynamic_pointer_cast<CDFG_Addr>
      (this->_module_gen->get_node
       (gepope->getPointerOperand()->getName(),
        CDFG_Node::eNode::ADDR));

    a->set_ref(array);

    // アドレスの追加
    for (auto ite = gepope->idx_begin() + 1; // 最初は無視
         ite != gepope->idx_end();
         ++ite) {
      auto value = ite->get();
      auto type = CDFG_Node::eNode::REG;

      if (!value->hasName())
        type = CDFG_Node::eNode::PARAM;

      a->add_addr
        (this->_module_gen->get_node
         (this->_get_value_name(value),
          type));
    } // for : ite
  } // if : isa<GEPOperator>
  // 通常のload命令
  else {
    a = std::dynamic_pointer_cast<CDFG_Addr>
      (this->_module_gen->get_node
       (this->_get_value_name
        (inst->getOperand(0)),
        CDFG_Node::eNode::ADDR));
  }

  // 出力
  auto b = this->_module_gen
    ->get_node(inst->getName().str(),
               CDFG_Node::eNode::REG);

  auto elem = std::make_shared<CDFG_LoadElem>
    (is_gepope,
     this->_state,
     this->_step);

  elem->set_input(a, 0);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
 } // _add_load_inst

/**
   STORE命令をモジュールのDFGに追加する
   @brief store a b
          b <= a; // latency == 0
*/
void FortRock::_add_store_inst
(const Instruction * inst)
{
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Operator::eType::STORE,
     1, // Number of operator input
     this->_state,
     this->_step);

  // 入力
  auto a = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(0)),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(1)),
     CDFG_Node::eNode::ADDR);

  elem->set_input(a, 0);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
} // _add_store_inst

/**
   ICMP命令をモジュールのDFGに追加する
   @brief b = icmp cond a0 a1
   b = (a0 cond a1)
 */
void FortRock::_add_icmp_inst
(const Instruction * inst)
{
  // 比較条件
  auto icmp_inst = dynamic_cast<ICmpInst*>
    (const_cast<Instruction*>(inst));
  auto condition = icmp_inst->getUnsignedPredicate();

  if (!condition)
    condition = icmp_inst->getSignedPredicate();

  CDFG_IcmpElem::eCond cond;
  switch (condition) {
  case CmpInst::Predicate::ICMP_EQ:
    cond = CDFG_IcmpElem::eCond::EQ; break;
  case CmpInst::Predicate::ICMP_NE:
    cond = CDFG_IcmpElem::eCond::NE; break;
  case CmpInst::Predicate::ICMP_UGT:
    cond = CDFG_IcmpElem::eCond::UGT; break;
  case CmpInst::Predicate::ICMP_UGE:
    cond = CDFG_IcmpElem::eCond::UGE; break;
  case CmpInst::Predicate::ICMP_ULT:
    cond = CDFG_IcmpElem::eCond::ULT; break;
  case CmpInst::Predicate::ICMP_ULE:
    cond = CDFG_IcmpElem::eCond::ULE; break;
  case CmpInst::Predicate::ICMP_SGT:
    cond = CDFG_IcmpElem::eCond::SGT; break;
  case CmpInst::Predicate::ICMP_SGE:
    cond = CDFG_IcmpElem::eCond::SGE; break;
  case CmpInst::Predicate::ICMP_SLT:
    cond = CDFG_IcmpElem::eCond::SLT; break;
  case CmpInst::Predicate::ICMP_SLE:
    cond = CDFG_IcmpElem::eCond::SLE; break;
  default:;
  }

  auto elem = std::make_shared<CDFG_IcmpElem>
    (cond,
     this->_state,
     this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(0)),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(1)),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
} // _add_icmp_inst

/**
   SELECT命令をモジュールのDFGに追加する
   @brief b = select t/f a0 a1
   b <= (a0 t/f a1)
 */
void FortRock::_add_select_inst
(const Instruction * inst)
{
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Operator::eType::SELECT,
     3, /* Number of operator input */
     this->_state,
     this->_step);

  // cond
  auto tf = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(0)),
     CDFG_Node::eNode::REG);

  // 入力
  auto a0 = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(1)),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(2)),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(tf, 0);
  elem->set_input(a0, 1);
  elem->set_input(a1, 2);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
} // _add_select_inst

/**
   SREM命令を module の DFG に追加
   @brief b = srem a0 a1
         演算器の信号線による接続
   @todo 使用した演算器にフラグを立てる
         div命令と演算器の共有
   @note dividerのfractionalに接続
 */
void FortRock::_add_srem_inst
(const Instruction * inst)
{
  auto srem = this->_module_gen->get_operator
    (CDFG_Operator::eType::SREM);

  auto elem = std::make_shared<CDFG_Element>(srem);
  elem->set_state(this->_state);
  elem->set_step(this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName(),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName(),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 1); // 余剰に接続

  this->_module_gen->add_element(elem);

  this->_step += srem->get_latency() + 2;
} // _add_srem_inst

/**
   SDIV命令を module の DFG に追加
   @brief b = sdiv a0 a1
         演算器の信号線による接続
   @todo 使用した演算器にフラグを立てる
         rem命令と演算器の共有
   @note dividerのquotientに接続
 */
void FortRock::_add_sdiv_inst
(const Instruction * inst)
{
  auto sdiv = this->_module_gen->get_operator
    (CDFG_Operator::eType::SDIV);

  auto elem = std::make_shared<CDFG_Element>(sdiv);
  elem->set_state(this->_state);
  elem->set_step(this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName(),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName(),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  this->_step += sdiv->get_latency() + 2;
} // _add_sdiv_inst

/**
   mul命令をモジュールの DFG に追加
   @brief b = fmul a0 a1
         演算器の信号線による接続

   @todo 使用した演算器にフラグを立てる
 */
void FortRock::_add_mul_inst
(const Instruction * inst)
{
  auto mul = this->_module_gen->get_operator
    (CDFG_Operator::eType::MUL);

  auto elem = std::make_shared<CDFG_Element>(mul);
  elem->set_state(this->_state);
  elem->set_step(this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName(),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName(),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  this->_step += mul->get_latency() + 2;
} // _add_mul_inst

/**
   fmul命令をモジュールの DFG に追加
   @brief b = fmul a0 a1
         演算器の信号線による接続

   @todo 使用した演算器にフラグを立てる
 */
void FortRock::_add_fmul_inst
(const Instruction * inst)
{
  auto fmul = this->_module_gen->get_operator
    (CDFG_Operator::eType::FMUL);

  auto elem = std::make_shared<CDFG_Element>(fmul);
  elem->set_state(this->_state);
  elem->set_step(this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName(),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName(),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  this->_step += fmul->get_latency() + 2;
} // _add_fmul_inst

/**
   BR命令をモジュールのDFGに追加する
   @brief br cond ltrue, lfalse
   state <= (cond) ? ltrue : lfalse;
 */
void FortRock::_add_br_inst
(const Instruction * inst)
{
  unsigned num_ope_input = 1;
  auto binst = dyn_cast<BranchInst>(&*inst);

  // 条件付き分岐かどうか
  if (binst->isConditional())
    num_ope_input = 3;

  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Operator::eType::BR,
     num_ope_input,
     this->_state,
     this->_step);

  if (binst->isConditional()) { // 条件付き分岐
    auto tf = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::REG);
    auto lfalse = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::LABEL);
    auto ltrue = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(2)),
       CDFG_Node::eNode::LABEL);
    elem->set_input(tf, 0);
    elem->set_input(ltrue, 1);
    elem->set_input(lfalse, 2);
  }
  else { // 無条件分岐
    auto label = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::LABEL);
    elem->set_input(label, 0);
  }

  auto state = this->_module_gen->get_node
    (CDFG_Reg::eRegType::STATE);
  elem->set_output(state, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
} // _add_br_inst

/**
   PHI命令をモジュールの DFG に追加する

   @brief b = phi [a0 x] [a1 y] ...

   function [bit-width] phi_b;
     input [bit-width] prev_state;
     begin
       case (prev_state)
         x : b = a0;
         y : b = a1;
       endcase
     end
   endfunction

   ...

   b = phi_b(prev_state);
 */
void FortRock::_add_phi_inst
(const Instruction * inst)
{
  auto phinode = dynamic_cast<PHINode*>
    (const_cast<Instruction*>(inst));
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Operator::eType::PHI,
     phinode->getNumIncomingValues() << 1, /* Number of operator input */
     this->_state,
     this->_step);

  for (auto i = 0;
       i < phinode->getNumIncomingValues();
       ++i) {
    auto label_name = phinode->getIncomingBlock(i)->getName();
    auto prev_label = this->_module_gen->
      get_node(label_name,
               CDFG_Node::eNode::LABEL);

    auto value_name = phinode->getIncomingValue(i)->getName();
    auto in = this->_module_gen->
      get_node(value_name,
               CDFG_Node::eNode::REG);

    // 入力の定数対応
    if (!phinode->getIncomingValue(i)->hasName())
      in = this->_module_gen->get_node
        (this->_get_value_name(phinode->getIncomingValue(i)),
         CDFG_Node::eNode::PARAM);

    elem->set_input(prev_label, i << 1);
    elem->set_input(in, (i << 1) + 1);
  }

  auto destination_node =
    this->_module_gen->
    get_node(inst->getName(),
             CDFG_Node::eNode::REG);

  elem->set_output(destination_node, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
} // _add_phi_inst

/**
   RET命令をモジュールのDFGに追加する
 */
void FortRock::_add_ret_inst
(const Instruction * inst)
{
  auto finish_state_label
    = this->_module_gen->get_node
    (CDFG_Label::eLabelType::FINISH);

  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Operator::eType::RET,
     1, /* Number of operator input */
     this->_state,
     this->_step);

  elem->set_input(finish_state_label, 0);

  this->_module_gen->add_element(elem);

  this->_step = 0;
} // _add_ret_inst

/**
   ADD命令をモジュールのDFGに追加する
   @brief b = add a0, a1
 */
void FortRock::_add_add_inst
(const Instruction * inst)
{
  auto add = this->_module_gen->get_operator
    (CDFG_Operator::eType::ADD);

  auto elem = std::make_shared<CDFG_Element>(add);
  elem->set_state(this->_state);
  elem->set_step(this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName(),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName(),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);
  this->_step += add->get_latency() + 2;
} // _add_add_inst

/**
   fadd命令をモジュールのDFGに追加する
   @brief b = fadd a0, a1
 */
void FortRock::_add_fadd_inst
(const Instruction * inst)
{
  auto fadd = this->_module_gen->get_operator
    (CDFG_Operator::eType::FADD);

  auto elem = std::make_shared<CDFG_Element>(fadd);
  elem->set_state(this->_state);
  elem->set_step(this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName(),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName(),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);
  this->_step += fadd->get_latency() + 2;
} // _add_fadd_inst

/**
   SUB命令をモジュールのDFGに追加する
   @brief b = sub a0, a1
 */
void FortRock::_add_sub_inst
(const Instruction * inst)
{
  auto sub = this->_module_gen->get_operator
    (CDFG_Operator::eType::SUB);

  auto elem = std::make_shared<CDFG_Element>(sub);
  elem->set_state(this->_state);
  elem->set_step(this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName(),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName(),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);
  this->_step += sub->get_latency() + 2;
} // _add_sub_inst

/**
   fsub命令をモジュールのDFGに追加する
   @brief b = fsub a0, a1
 */
void FortRock::_add_fsub_inst
(const Instruction * inst)
{
  auto fsub = this->_module_gen->get_operator
    (CDFG_Operator::eType::FSUB);

  auto elem = std::make_shared<CDFG_Element>(fsub);
  elem->set_state(this->_state);
  elem->set_step(this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName(),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName(),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);
  this->_step += fsub->get_latency() + 2;
} // _add_fsub_inst

/**
   switch命令をモジュールのDFGに追加する
   @brief switch %val, label %otherwise
            [ 0, label %onzero
              1, label %onone
              2, label %ontwo
              ...]

          case (val)
            0: state <= onzero;
            1: state <= onone;
            2: state <= ontwo;
            default: state <= otherwise;
          endcase
 */
void FortRock::_add_switch_inst
(const Instruction * inst)
{
  auto sw_inst = dyn_cast<SwitchInst>(&*inst);

  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Operator::eType::SWITCH,
     2 + (sw_inst->getNumCases() << 1), // condition + default +  (vals + labels) * cases
     this->_state,
     this->_step);

  // 条件変数
  auto condition_node = this->_module_gen->get_node
    (sw_inst->getCondition()->getName(),
     CDFG_Node::eNode::REG);
  elem->set_input(condition_node, 0);

  // default遷移先
  auto default_label = this->_module_gen->get_node
    (sw_inst->getDefaultDest()->getName(),
     CDFG_Node::eNode::LABEL);
  elem->set_input(default_label, 1);

  // case文内の出力
  auto i = 2;
  for (auto ite = sw_inst->case_begin();
       ite != sw_inst->case_end();
       ++ite, i+=2) {
    int val = ite.getCaseValue()->getSExtValue();
    auto label = ite.getCaseSuccessor();

    auto val_node = std::make_shared<CDFG_Parameter>
      (std::to_string(val),
       this->_get_required_bit_width(val),
       true, /* is signed */ //! @todo 常にtrue
       CDFG_Parameter::eParamType::INTEGER, //! @todo 常にINTEGER
       val);

    auto label_node = this->_module_gen->get_node
      (label->getName(),
       CDFG_Node::eNode::LABEL);

    elem->set_input(val_node, i);
    elem->set_input(label_node, i+1);
  }

  this->_module_gen->add_element(elem);
  this->_step += 1;
} // _add_switch_inst

/**
   シフト演算(shl, lshr, ashr命令)をモジュールのDFGに追加する
   @brief b = shl a0, a1
          b <= a0 << a1
   @param[in] inst 命令の参照
   @param[in] is_left 左シフトか否か
   @param[in] is_logical 論理シフトか否か
 */
void FortRock::_add_shift_inst
(const Instruction * inst,
 const bool is_left,
 const bool is_logical)
{
  CDFG_Operator::eType type;

  // シフト演算の種類を確定
  if (is_left)
    type = CDFG_Operator::eType::LSHIFTL;
  else {
    if (is_logical)
      type = CDFG_Operator::eType::LSHIFTR;
    else
      type = CDFG_Operator::eType::ASHIFTR;
  }

  auto elem = std::make_shared<CDFG_Element>
    (type, 2, this->_state, this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(0)),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(1)),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName().str(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
} // _add_shift_inst

/**
   and命令をDFGに追加
   @param[in] inst 命令の参照
   @brief b = a0 and a1
          b <= a0 && a1
 */
void FortRock::_add_and_inst
(const Instruction * inst)
{
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Operator::eType::AND,
     2, // 入力の数
     this->_state,
     this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName(),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName(),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
} // _add_and_inst

/**
   or命令をDFGに追加
   @param[in] inst 命令の参照
   @brief b = a0 or a1
          b <= a0 || a1
 */
void FortRock::_add_or_inst
(const Instruction * inst)
{
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Operator::eType::OR,
     2, // 入力の数
     this->_state,
     this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName(),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName(),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
} // _add_or_inst

/**
   xor命令をDFGに追加
   @param[in] inst 命令の参照
   @brief b = a0 xor a1
          b <= a0 ^ a1
 */
void FortRock::_add_xor_inst
(const Instruction * inst)
{
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Operator::eType::XOR,
     2, // 入力の数
     this->_state,
     this->_step);

  // 入力
  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName(),
     CDFG_Node::eNode::REG);
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName(),
     CDFG_Node::eNode::REG);

  // 入力の定数対応
  if (!inst->getOperand(0)->hasName())
    a0 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);
  if (!inst->getOperand(1)->hasName())
    a1 = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(1)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
} // _add_xor_inst

/**
   trunc命令をDFGに追加
   @param[in] inst 命令の参照
   @brief b = trunc type a to type2
          b <= a[type2-1:0];
   @note ビット幅を指定して切り詰めた代入
 */
void FortRock::_add_trunc_inst
(const Instruction * inst)
 {
  auto trunc_inst = dynamic_cast<TruncInst*>
    (const_cast<Instruction*>(inst));

  auto elem = std::make_shared<CDFG_TruncElem>
    (trunc_inst->getDestTy()->getPrimitiveSizeInBits(),
     this->_state,
     this->_step);

  // 入力
  auto a = this->_module_gen->get_node
    (inst->getOperand(0)->getName(),
     CDFG_Node::eNode::REG);

  // 入力が定数の場合
  if (!inst->getOperand(0)->hasName())
    a = this->_module_gen->get_node
      (this->_get_value_name(inst->getOperand(0)),
       CDFG_Node::eNode::PARAM);

  // 出力
  auto b = this->_module_gen->get_node
    (inst->getName(),
     CDFG_Node::eNode::REG);

  elem->set_input(a, 0);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);
  ++this->_step;
 } // _add_trunc_inst

/**
   getelemntptr命令をDFGに追加
   @param[in] inst 命令の参照
 */
void FortRock::_add_getelementptr_inst
(const Instruction * inst)
 {
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Operator::eType::GETELEMENTPTR,
     1,
     this->_state,
     this->_step);

  auto getelemptr_inst = dynamic_cast<GetElementPtrInst*>
    (const_cast<Instruction*>(inst));

  auto ptr = getelemptr_inst->getPointerOperand();
  auto array
    = this->_module_gen->get_node(ptr->getName(),
                                  CDFG_Node::eNode::MEM);

  auto addr
    = std::dynamic_pointer_cast<CDFG_Addr>
    (this->_module_gen->get_node
     (getelemptr_inst->getName(),
      CDFG_Node::eNode::ADDR));

  addr->set_ref(array);

  // アドレスの追加
  for (auto ite = getelemptr_inst->idx_begin() + 1; // 最初は無視
       ite != getelemptr_inst->idx_end();
       ++ite) {
    auto value = ite->get();
    auto type = CDFG_Node::eNode::REG;

    if (!value->hasName()) // 定数の場合
      type = CDFG_Node::eNode::PARAM;

    addr->add_addr
      (this->_module_gen->get_node(value->getName(),
                                   type));
  }
  elem->set_input(addr, 0);
  this->_module_gen->add_element(elem);
  ++this->_step;
 } // _add_getelementptr_inst

/**
   プログラムで使用するすべてのレジスタを取得し
   variablesに格納する
   Labelについても列挙し，格納する
 */
void FortRock::_grub_variables
(const Module::FunctionListType::iterator & funct)
{
  BranchInst * binst;
  Value      * value;
  Type       * type;
  std::shared_ptr<CDFG_Node> node;

  for (auto it = inst_begin(*funct); // funct->begin();
       it != inst_end(*funct);       // funct->end();
       ++it) {

    if (!it->use_empty()) {
      // 命令に対応するオペランド数の指定
      int getop = 1;
      switch(it->getOpcode()) {
      case Instruction::Trunc:
      case Instruction::Load: getop = 1;
        break;

      case Instruction::ICmp:
      case Instruction::SRem:
      case Instruction::Mul:
      case Instruction::FMul:
      case Instruction::SDiv:
      case Instruction::Add:
      case Instruction::FAdd:
      case Instruction::Sub:
      case Instruction::FSub:
      case Instruction::Shl:
      case Instruction::LShr:
      case Instruction::AShr:
      case Instruction::PHI:
      case Instruction::And:
      case Instruction::Or:
      case Instruction::Xor: getop = 2;
        break;

      case Instruction::GetElementPtr:
      case Instruction::Select: getop = 3;
        break;

      default:
        errs() << "ERROR ("
               << __func__
               << "):"
               << it->getOpcodeName() << " "
               << it->getOpcode() << " 未定義のオペランド\n";
        getop = 0;
        continue;
      } // if

      if (it->getOpcode() == Instruction::Load) {
        auto vv = dyn_cast<LoadInst>(&*it)->getPointerOperand();
        // getelementptrであれば，add_load_inst()時に取得する
        if (isa<GEPOperator>(vv)) {
          // getelementptr内の定数のインスタンス化
          auto gepope = dyn_cast<GEPOperator>(vv);
          for (auto ite = gepope->idx_begin() + 1; // 最初は無視
               ite != gepope->idx_end();
               ++ite) {
            auto value = ite->get();
            auto type = value->getType();

            auto name = this->_get_value_name(value);

            // 定数の場合
            if (!value->hasName()) {
              node = std::make_shared<CDFG_Parameter>
                (name,
                 type->getPrimitiveSizeInBits(),
                 true, ///< @todo is_signed が常にtrue
                 CDFG_Parameter::eParamType::INTEGER, ///< @todo 常にinteger
                 std::stol(name));
            }
            // 変数の場合
            else {
              node = std::make_shared<CDFG_Reg>
                (name,
                 type->getPrimitiveSizeInBits(),
                 true, ///, @todo is_signed が常にtrue
                 CDFG_Reg::eRegType::REG);
            }

          // 未定義のregの追加
          if (!this->_module_gen->find_node(node))
            this->_module_gen->add_node(node);

          } // for : ite
        }
      } // if : Instruction::Load
      else { // Load 命令以外
        for (auto i=0; i<getop; ++i) {
          value = it->getOperand(i);
          type = value->getType();

          auto name = this->_get_value_name(value);

          // ポインタ
          if (type->isPointerTy()) {
            // ポインタに対する参照の追加
            type = type->getPointerElementType();
            node = std::make_shared<CDFG_Addr>
              (name,
               type->getPrimitiveSizeInBits());
          }
          // 定数
          else if(!value->hasName()) {
            node = std::make_shared<CDFG_Parameter>
              (name,
               type->getPrimitiveSizeInBits(),
               true, ///< @todo is_signedが常にtrue
               CDFG_Parameter::eParamType::INTEGER, ///< @todo 常にinteger
               std::stol(name));
          }
          // 変数
          else {
            node = std::make_shared<CDFG_Reg>
              (name,
               type->getPrimitiveSizeInBits(),
               true, //! @todo is signedが常にtrue
               CDFG_Reg::eRegType::REG);
          }
          // 未定義のregの追加
          if (!this->_module_gen->find_node(node))
            this->_module_gen->add_node(node);

        } // for : i
      } // for : getop
    } // if : use_empty()
    else {
      try {
        switch(it->getOpcode()) {
        case Instruction::Ret:
        case Instruction::Switch:
          break;

        case Instruction::Br:
          {
            binst = dyn_cast<BranchInst>(&*it);
            if (binst->isConditional()) { // 条件付き分岐
              value = binst->getCondition();
              type = value->getType();

              if(type->isPointerTy())
                type = type->getPointerElementType();
            }
            else // 無条件分岐
              continue; // labelは別の場所で確保

            node = std::make_shared<CDFG_Reg>
              (value->getName(),
               type->getPrimitiveSizeInBits(),
               true,
               CDFG_Reg::eRegType::REG);

            if (!this->_module_gen->find_node(node))
              this->_module_gen->add_node(node);
            break;
          } // br

        case Instruction::Store:
          {
            // 未定義のregの追加
            for(auto i=0; i<2; ++i) {
              value = it->getOperand(i);
              type = value->getType();

              auto name = this->_get_value_name(value);

              if (type->isPointerTy()) { // ポインタ
                // ポインタに対する参照の追加
                type = type->getPointerElementType();
                node = std::make_shared<CDFG_Addr>
                  (name,
                   type->getPrimitiveSizeInBits());
              }
              else if(!value->hasName()) { // 定数
                node = std::make_shared<CDFG_Parameter>
                  (name,
                   type->getPrimitiveSizeInBits(),
                   true, //! @todo is signedが常にtrue
                   CDFG_Parameter::eParamType::INTEGER, //! @todo 常にINTGER
                   std::stol(this->_get_value_name(value)));
              }
              else { // 変数
                node = std::make_shared<CDFG_Reg>
                  (name,
                   type->getPrimitiveSizeInBits(),
                   true, //! @todo is signedが常にtrue
                   CDFG_Reg::eRegType::REG);
              }

              if (!this->_module_gen->find_node(node))
                this->_module_gen->add_node(node);

            } // for
            break;
          } // store

        default:
          throw std::string(std::string("ERROR (") + __func__ + "):"
                            + std::string(it->getOpcodeName())
                            + " "
                            + std::to_string(it->getOpcode())
                            + " 未定義のオペランド\n");
          break;
        } // switch
      } // try
      catch(std::string err) {
        errs() << err;
      }
    } // else
  } // for
} // _grub_variables

/**
   グローバル変数のインスタンス化
 */
void FortRock::_grub_global_variables
(const Module & M)
{
  for (auto ite = M.global_begin();
       ite != M.global_end();
       ++ite) {
    std::string name = this->_get_value_name(ite);
    Type * type = ite->getType();
    auto init = ite->getInitializer();

    if (type->isPointerTy())
      type = type->getPointerElementType();

    // 配列の場合
    if (type->isArrayTy()) {
      //! @todo zeroinitializerへの対応
      auto init_array = dyn_cast<ConstantArray>(init);
      auto array      = dyn_cast<ArrayType>(type);
      auto elem_type  = array->getTypeAtIndex((unsigned)0);

      // 初期化子の取得
      //! @todo Integer以外の初期化子への対応
      if (elem_type->isIntegerTy()) {
        //! @todo 1次元配列以外への対応
        std::vector<std::vector<int> > initializer;
        initializer.resize(1);
        initializer[0].reserve(array->getNumElements());

        for (auto i=0;
             i<array->getNumElements();
             ++i) {
          auto val = init->getAggregateElement(i);
          auto type = val->getType();
          auto int_value = dyn_cast<ConstantInt>(val);
          initializer[0].push_back(int_value->getLimitedValue());
        } // for : i

        // 初期化子を用いたメモリの確保
        std::vector<unsigned> length;
        length.reserve(1);
        length.push_back(array->getNumElements());

        auto mem = std::make_shared<CDFG_Array>
          (name,
           CDFG_Array::eDataType::INTEGER,
           elem_type->getPrimitiveSizeInBits(),
           1, // write_ports
           1, // read_ports
           length,
           initializer);

        this->_module_gen->add_node(mem);
      }
    } // type : isArrayTy
  } // for : ite
} // _grub_global_variables

/**
   ステートマシンのラベル(ステート)の列挙
   @param[in] funct CFDの参照
   @note ラベルはNodeとしてインスタンス化され，追加される
   @attention このタイミングでState管理NodeがCModuleに追加される
*/
void FortRock::_grub_labels
(const Module::FunctionListType::iterator & funct)
{
  int num_label
    = std::distance(funct->begin(), funct->end());
  auto label_bit_width
    = this->_get_required_bit_width(num_label + 2 /* + (reset + finish) */);

  // labelの追加
  auto ite = funct->begin();
  auto i = 0;
  for(;ite != funct->end();
      ++ite, ++i) {
    auto label_node = std::make_shared<CDFG_Label>
      (ite->getName(),
       label_bit_width,
       CDFG_Label::eLabelType::LABEL,
       i + 1 /* label parameter ( + reset) */);

    if (!this->_module_gen->find_node(label_node))
      this->_module_gen->add_node(label_node);
  } // for

  // state_nodeの追加
  auto state_node = std::make_shared<CDFG_Reg>
    (this->_CUR_STATE_NAME,
     label_bit_width,
     false,
     CDFG_Reg::eRegType::STATE);

  auto prev_state_node = std::make_shared<CDFG_Reg>
    (this->_PREV_STATE_NAME,
     label_bit_width,
     false,
     CDFG_Reg::eRegType::PREV_STATE);

  this->_module_gen->add_node(state_node);
  this->_module_gen->add_node(prev_state_node);

  // 終了状態ステート(label)の追加
  auto finish_label = std::make_shared<CDFG_Label>
    (this->_FINISH_STATE_NAME,
     label_bit_width,
     CDFG_Label::eLabelType::FINISH,
     i + 1/* label parameter */);

  this->_module_gen->add_node(finish_label);
} // _grub_labels

char FortRock::ID = 0;

// ==================================================
// External Interface declaration
// ==================================================

static RegisterPass<FortRock> X("fortrock", "Fortran to Verilog",
                                false, /* Only looks at CFG */
                                false  /* Analysis Pass */);
