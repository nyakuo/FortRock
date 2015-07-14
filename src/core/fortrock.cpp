#include "fortrock.hpp"

/**
   入力された数を表すために必要なビット幅を計算する
   @param[in] num 表現したい種類の数
   @return 必要なビット幅
   @attention unsignedのビット幅を32ビットと仮定
*/
unsigned
FortRock::_get_required_bit_width
(const unsigned & num) {
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
}

/**
   moduleの名前を取得
   @param[in] M モジュールの参照
   @return モジュールの名前
*/
std::string
FortRock::_get_module_name
(const Module & M) {
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
(const Value * v) {
  std::string ret_str ("");
  auto type = v->getType();

  if (type->isPointerTy())
    type = type->getPointerElementType();

  if (!v->hasName()) { // 定数
    if (type->isIntegerTy()) {
      auto int_value = dyn_cast<ConstantInt>(v);
      ret_str.append(int_value->getValue().toString(10, /* 基数 */
                                                    true)); /* is signed */ //! @todo is signed 対応する
    } // if
  } // if
  else { // 変数
    ret_str.append(v->getName());
  } // else

  return ret_str;
} // _get_value_name

/**
   moduleの入出力を定義
*/
void FortRock::_set_IO
(const Module::FunctionListType::iterator & funct) {
  auto arg_it  = funct->arg_begin();
  auto arg_end = funct->arg_end();

  for(auto num = 1; // 最後の引数(返り値)を知るため
      arg_it != arg_end;
      ++num, ++arg_it) {
    auto type = arg_it->getType();

    if(type->isPointerTy()) // ポインタならポインタの型を取得
      type = type->getPointerElementType();

    //! @todo 浮動小数点対応
    if(type->isIntegerTy()) {
      CDFG_Node::eNode in_out;
      if (num !=  funct->arg_size()) // 最後以外は引数
        in_out = CDFG_Node::eNode::IN;

      else  // 最後の引数は返り値
        in_out = CDFG_Node::eNode::OUT;

      auto node = std::make_shared<CDFG_Node>
        (CDFG_Node(arg_it->getName(),
                   type->getPrimitiveSizeInBits(),
                   true, //! @todo isSigned対応
                   in_out));

      this->_module_gen->add_node(node);
    } // if
  } // for
} // _set_IO

/**
   FortRock passが最初に呼ばれる関数
*/
bool FortRock::runOnModule
(Module &M) {
  this->_module_gen = std::make_shared<CModuleGenerator>
    ("output.v", this->_get_module_name(M));

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
         ++inst) { // Instruction
      this->_parse_instructions(inst);
    }
    this->_step = 0;
    ++this->_state;
  }

  // step信号の追加
  auto step_bit_width = this->_get_required_bit_width
    (this->_module_gen->get_max_step());
  auto step_node = std::make_shared<CDFG_Node>
    (CDFG_Node(this->_STEP_NAME,
               step_bit_width,
               false, /* is signed */
               CDFG_Node::eNode::STEP));

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
(const Instruction * inst) {
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
    case Instruction::SDiv:   this->_add_sdiv_inst(inst);   break;
    default:
      throw std::string(std::string("ERROR:")
                        + std::string(inst->getOpcodeName())
                        + " "
                        + std::to_string(inst->getOpcode())
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
(const Instruction * inst) {
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Element(CDFG_Operator::eType::LOAD,
                  1, /* Num operator input */
                  this->_state,
                  this->_step));

  auto a = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(0)));
  auto b = this->_module_gen->get_node(inst->getName().str());

  elem->set_input(a, 0);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
}

/**
   STORE命令をモジュールのDFGに追加する
   @brief store a b
          b <= a; // latency == 0
*/
void FortRock::_add_store_inst
(const Instruction * inst) {
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Element(CDFG_Operator::eType::STORE,
                  1, /* Number of operator input */
                  this->_state,
                  this->_step));

  auto a = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(0)));
  auto b = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(1)));

  elem->set_input(a, 0);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
}

/**
   ICMP命令をモジュールのDFGに追加する
   @brief b = icmp cond a0 a1
   b = (a0 cond a1)
   @todo conditionへの対応
 */
void FortRock::_add_icmp_inst
(const Instruction * inst) {
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Element(CDFG_Operator::eType::ICMP,
                  2, /* Number of operator input */
                  this->_state,
                  this->_step));

  auto a0 = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(0)));
  auto a1 = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(1)));
  auto b = this->_module_gen->get_node(inst->getName());

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
}

/**
   SELECT命令をモジュールのDFGに追加する
   @brief b = select t/f a0 a1
   b <= (a0 t/f a1)
 */
void FortRock::_add_select_inst
(const Instruction * inst) {
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Element(CDFG_Operator::eType::SELECT,
                  3, /* Number of operator input */
                  this->_state,
                  this->_step));

  auto tf = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(0)));
  auto a0 = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(1)));
  auto a1 = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(2)));
  auto b = this->_module_gen->get_node(inst->getName());

  elem->set_input(tf, 0);
  elem->set_input(a0, 1);
  elem->set_input(a1, 2);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
}

/**
   SREM命令を module の DFG に追加
   @brief b = srem a0 a1
         演算器の信号線による接続
   @todo 使用した演算器にフラグを立てる
         div命令と演算器の共有
   @note dividerのfractionalに接続
 */
void FortRock::_add_srem_inst
(const Instruction * inst) {
  auto srem = this->_module_gen->get_operator
    (CDFG_Operator::eType::SREM);

  auto elem = std::make_shared<CDFG_Element>(srem);
  elem->set_state(this->_state);
  elem->set_step(this->_step);

  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName());
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName());
  auto b = this->_module_gen->get_node
    (inst->getName());

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 1); // 余剰に接続

  this->_module_gen->add_element(elem);

  this->_step += srem->get_latency() + 2;
}

/**
   SDIV命令を module の DFG に追加
   @brief b = sdiv a0 a1
         演算器の信号線による接続
   @todo 使用した演算器にフラグを立てる
         rem命令と演算器の共有
   @note dividerのquotientに接続
 */
void FortRock::_add_sdiv_inst
(const Instruction * inst) {
  auto sdiv = this->_module_gen->get_operator
    (CDFG_Operator::eType::SDIV);

  auto elem = std::make_shared<CDFG_Element>(sdiv);
  elem->set_state(this->_state);
  elem->set_step(this->_step);

  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName());
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName());
  auto b = this->_module_gen->get_node
    (inst->getName());

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  this->_step += sdiv->get_latency() + 2;
}

/**
   MUL命令をモジュールの DFG に追加
   @brief b = mul a0 a1
         演算器の信号線による接続

   @todo 使用した演算器にフラグを立てる
 */
void FortRock::_add_mul_inst
(const Instruction * inst) {
  auto mul = this->_module_gen->get_operator
    (CDFG_Operator::eType::MUL);

  auto elem = std::make_shared<CDFG_Element>(mul);
  elem->set_state(this->_state);
  elem->set_step(this->_step);

  auto a0 = this->_module_gen->get_node
    (inst->getOperand(0)->getName());
  auto a1 = this->_module_gen->get_node
    (inst->getOperand(1)->getName());
  auto b = this->_module_gen->get_node
    (inst->getName());

  elem->set_input(a0, 0);
  elem->set_input(a1, 1);
  elem->set_output(b, 0);

  this->_module_gen->add_element(elem);

  this->_step += mul->get_latency() + 2;
}

/**
   BR命令をモジュールのDFGに追加する
   @brief br cond ltrue, lfalse
   state <= (cond) ? ltrue : lfalse;
 */
void FortRock::_add_br_inst
(const Instruction * inst) {
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Element(CDFG_Operator::eType::BR,
                  3, /* Number of operator input */
                  this->_state,
                  this->_step));
  auto tf = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(0)));
  auto lfalse = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(1)));
  auto ltrue = this->_module_gen->get_node
    (this->_get_value_name(inst->getOperand(2)));
  auto state = this->_module_gen->get_node(CDFG_Node::eNode::STATE);

  elem->set_input(tf, 0);
  elem->set_input(ltrue, 1);
  elem->set_input(lfalse, 2);
  elem->set_output(state, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
}

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
(const Instruction * inst) {
  auto phinode = dynamic_cast<PHINode*>(const_cast<Instruction*>(inst));
  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Element(CDFG_Operator::eType::PHI,
                  phinode->getNumIncomingValues() << 1, /* Number of operator input */
                  this->_state,
                  this->_step));

  for (auto i = 0;
       i < phinode->getNumIncomingValues();
       ++i) {
    auto label_name = phinode->getIncomingBlock(i)->getName();
    auto prev_label = this->_module_gen->get_node(label_name);

    auto value_name = phinode->getIncomingValue(i)->getName();
    auto in = this->_module_gen->get_node(value_name);

    elem->set_input(prev_label, i << 1);
    elem->set_input(in, (i << 1) + 1);
  }

  auto destination_node = this->_module_gen->get_node(inst->getName());

  elem->set_output(destination_node, 0);

  this->_module_gen->add_element(elem);

  ++this->_step;
}

/**
   RET命令をモジュールのDFGに追加する
 */
void FortRock::_add_ret_inst
(const Instruction * inst) {
  auto finish_state_label
    = this->_module_gen->get_node
    (CDFG_Node::eNode::FINISH_LABEL);

  auto elem = std::make_shared<CDFG_Element>
    (CDFG_Operator::eType::RET,
     1, /* Number of operator input */
     this->_state,
     this->_step);

  elem->set_input(finish_state_label, 0);

  this->_module_gen->add_element(elem);

  this->_step = 0;
}

/**
 * プログラムで使用するすべてのレジスタを取得し
 * variablesに格納する
 * Labelについても列挙し，格納する
 */
void FortRock::_grub_variables
(const Module::FunctionListType::iterator &funct) {
  BranchInst * binst;
  Value      * value;
  Type       * type;
  std::shared_ptr<CDFG_Node> node;

  for (auto it = inst_begin(*funct); //funct->begin();
       it != inst_end(*funct);   // funct->end();
       ++it) {

    if (!it->use_empty()) {
      // 命令に対応するオペランド数の指定
      int getop = 1;
      switch(it->getOpcode()) {
      case Instruction::Load:   getop = 1; break;
      case Instruction::ICmp:   getop = 2; break;
      case Instruction::PHI:    getop = 2; break;
      case Instruction::Select: getop = 3; break;
      case Instruction::SRem:   getop = 2; break;
      case Instruction::Mul:    getop = 2; break;
      case Instruction::SDiv:   getop = 2; break;
      default:
        errs() << "ERROR:"
               << it->getOpcodeName() << " "
               << it->getOpcode() << " 未定義のオペランド\n";
        getop = 0;
        break;
      } // if

      // 未定義のregの追加
      for(auto i=0; i<getop; ++i) {
        value = it->getOperand(i);
        type = value->getType();

        if(type->isPointerTy())
          type = type->getPointerElementType();

        auto name = this->_get_value_name(value);

        if(!value->hasName()) { // 定数
          node = std::make_shared<CDFG_Node>
            (name,
             type->getPrimitiveSizeInBits(),
             true, //! @todo is signedが常にtrue
             CDFG_Node::eNode::PARAM,
             std::stol(this->_get_value_name(value)));
        }
        else { // 変数
          node = std::make_shared<CDFG_Node>
            (CDFG_Node(name,
                       type->getPrimitiveSizeInBits(),
                       true, //! @todo is signedが常にtrue
                       CDFG_Node::eNode::REG));
        }
        if (!this->_module_gen->find_node(node))
          this->_module_gen->add_node(node);
      } // for
    } // if
    else {
      try {
        switch(it->getOpcode()) {
        case Instruction::Ret:
          break;

        case Instruction::Br:
          binst = dyn_cast<BranchInst>(&*it);
          value = binst->getCondition();
          type = value->getType();

          if(type->isPointerTy())
            type = type->getPointerElementType();

          node = std::make_shared<CDFG_Node>
            (value->getName(),
             type->getPrimitiveSizeInBits(),
             true,
             CDFG_Node::eNode::REG);

          if (!this->_module_gen->find_node(node))
            this->_module_gen->add_node(node);
          break;

        case Instruction::Store:
          for(auto i=0; i<2; ++i) {
            value = it->getOperand(i);
            type = value->getType();

            if(type->isPointerTy())
              type = type->getPointerElementType();

            node = std::make_shared<CDFG_Node>
              (CDFG_Node(value->getName(),
                         type->getPrimitiveSizeInBits(),
                         true,
                         CDFG_Node::eNode::REG));

            if (!this->_module_gen->find_node(node))
              this->_module_gen->add_node(node);
          }
          break;

        default:
          throw std::string(std::string("ERROR:")
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
   ステートマシンのラベル(ステート)の列挙
   @param[in] funct CFDの参照
   @note ラベルはNodeとしてインスタンス化され，追加される
   @attention このタイミングでState管理NodeがCModuleに追加される
*/
void FortRock::_grub_labels
(const Module::FunctionListType::iterator & funct) {
  int num_label
    = std::distance(funct->begin(), funct->end());
  auto label_bit_width
    = this->_get_required_bit_width(num_label + 2 /* + (reset + finish) */);

  // labelの追加
  auto ite = funct->begin();
  auto i = 0;
  for(;ite != funct->end();
      ++ite, ++i) {
    auto label_node = std::make_shared<CDFG_Node>
      (CDFG_Node(ite->getName(),
                 label_bit_width,
                 false,
                 CDFG_Node::eNode::LABEL,
                 i + 1 /* label parameter ( + reset) */));

    if (!this->_module_gen->find_node(label_node))
      this->_module_gen->add_node(label_node);
  } // for

  // state_nodeの追加
  auto state_node = std::make_shared<CDFG_Node>
    (CDFG_Node(this->_CUR_STATE_NAME,
               label_bit_width,
               false,
               CDFG_Node::eNode::STATE));
  auto prev_state_node = std::make_shared<CDFG_Node>
    (CDFG_Node(this->_PREV_STATE_NAME,
               label_bit_width,
               false,
               CDFG_Node::eNode::PREV_STATE));

  this->_module_gen->add_node(state_node);
  this->_module_gen->add_node(prev_state_node);

  // 終了状態ステート(label)の追加
  auto finish_label = std::make_shared<CDFG_Node>
    (CDFG_Node(this->_FINISH_STATE_NAME,
               label_bit_width,
               false,
               CDFG_Node::eNode::FINISH_LABEL,
               i + 1/* label parameter */));

  this->_module_gen->add_node(finish_label);
} // _grub_labels

char FortRock::ID = 0;

// ==================================================
// External Interface declaration
// ==================================================

static RegisterPass<FortRock> X("fortrock", "Fortran to Verilog",
                                false, /* Only looks at CFG */
                                false  /* Analysis Pass */);
