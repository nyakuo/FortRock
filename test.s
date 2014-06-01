; ModuleID = 'test.f'
target datalayout = "e-p:64:64:64-S128-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f16:16:16-f32:32:32-f64:64:64-f128:128:128-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64--linux-gnu"

module asm "\09.ident\09\22GCC: (Ubuntu/Linaro 4.7.3-12ubuntu1) 4.7.3 LLVM: 3.3\22"

; Function Attrs: nounwind uwtable
define void @lcm_(float* noalias nocapture %ret_lcm) unnamed_addr #0 {
"8":
  store float 1.400000e+02, float* %ret_lcm, align 4, !tbaa !0
  ret void
}

attributes #0 = { nounwind uwtable }

!0 = metadata !{metadata !"alias set 15: real(kind=4)", metadata !1}
!1 = metadata !{metadata !1}
