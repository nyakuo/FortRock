; ModuleID = 'fortran/lcm.f'
target datalayout = "e-p:64:64:64-S128-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f16:16:16-f32:32:32-f64:64:64-f128:128:128-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64--linux-gnu"

module asm "\09.ident\09\22GCC: (Ubuntu/Linaro 4.7.3-12ubuntu1) 4.7.3 LLVM: 3.3\22"

; Function Attrs: nounwind uwtable
define void @lcm_(i32* noalias nocapture %i, i32* noalias nocapture %j, i32* noalias nocapture %ret_lcm) unnamed_addr #0 {
entry:
  %0 = load i32* %i, align 4, !tbaa !0
  %1 = load i32* %j, align 4, !tbaa !0
  %2 = icmp slt i32 %0, %1
  %. = select i1 %2, i32 %1, i32 %0
  %.1 = select i1 %2, i32 %0, i32 %1
  %3 = srem i32 %., %.1
  %4 = icmp slt i32 %3, 1
  br i1 %4, label %"8", label %"7"

"7":                                              ; preds = %entry, %"7"
  %5 = phi i32 [ %6, %"7" ], [ %.1, %entry ]
  %6 = phi i32 [ %7, %"7" ], [ %3, %entry ]
  %7 = srem i32 %5, %6
  %8 = icmp slt i32 %7, 1
  br i1 %8, label %"8", label %"7"

"8":                                              ; preds = %"7", %entry
  %.lcssa = phi i32 [ %.1, %entry ], [ %6, %"7" ]
  %9 = mul nsw i32 %1, %0
  %10 = sdiv i32 %9, %.lcssa
  store i32 %10, i32* %ret_lcm, align 4, !tbaa !0
  ret void
}

attributes #0 = { nounwind uwtable }

!0 = metadata !{metadata !"alias set 7: integer(kind=4)", metadata !1}
!1 = metadata !{metadata !1}
