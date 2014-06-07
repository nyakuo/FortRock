; ModuleID = 'fortran/lcm.f'
target datalayout = "e-p:64:64:64-S128-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f16:16:16-f32:32:32-f64:64:64-f128:128:128-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64--linux-gnu"

module asm "\09.ident\09\22GCC: (Ubuntu/Linaro 4.7.3-12ubuntu1) 4.7.3 LLVM: 3.3\22"

; Function Attrs: nounwind uwtable
define void @lcm_(i32* noalias %i, i32* noalias %j, i32* noalias %ret_lcm) unnamed_addr #0 {
entry:
  %i_addr = alloca i32*, align 8
  %j_addr = alloca i32*, align 8
  %ret_lcm_addr = alloca i32*, align 8
  %ir = alloca i32
  %ir1 = alloca i32
  %ir2 = alloca i32
  %"alloca point" = bitcast i32 0 to i32
  store i32* %i, i32** %i_addr, align 1
  store i32* %j, i32** %j_addr, align 1
  store i32* %ret_lcm, i32** %ret_lcm_addr, align 1
  %0 = load i32** %i_addr, align 8
  %1 = load i32** %j_addr, align 8
  %2 = load i32** %ret_lcm_addr, align 8
  %"ssa point" = bitcast i32 0 to i32
  br label %"2"

"2":                                              ; preds = %entry
  %3 = load i32* %0, align 4
  %4 = load i32* %1, align 4
  %5 = icmp slt i32 %3, %4
  br i1 %5, label %"3", label %"4"

"3":                                              ; preds = %"2"
  %6 = load i32* %1, align 4
  %7 = load i32* %0, align 4
  br label %"5"

"4":                                              ; preds = %"2"
  %8 = load i32* %0, align 4
  %9 = load i32* %1, align 4
  br label %"5"

"5":                                              ; preds = %"4", %"3"
  %10 = phi i32 [ %8, %"4" ], [ %6, %"3" ]
  %11 = phi i32 [ %9, %"4" ], [ %7, %"3" ]
  %12 = srem i32 %10, %11
  br label %"6"

"6":                                              ; preds = %"7", %"5"
  %13 = phi i32 [ %16, %"7" ], [ %12, %"5" ]
  %14 = phi i32 [ %13, %"7" ], [ %11, %"5" ]
  %15 = icmp sle i32 %13, 0
  br i1 %15, label %"8", label %"7"

"7":                                              ; preds = %"6"
  %16 = srem i32 %14, %13
  br label %"6"

"8":                                              ; preds = %"6"
  %17 = load i32* %0, align 4
  %18 = load i32* %1, align 4
  %19 = mul i32 %17, %18
  %20 = sdiv i32 %19, %14
  store i32 %20, i32* %2, align 4
  br label %"9"

"9":                                              ; preds = %"8"
  br label %return

return:                                           ; preds = %"9"
  ret void
}

attributes #0 = { nounwind uwtable }
