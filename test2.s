; ModuleID = 'test2.f'
target datalayout = "e-p:64:64:64-S128-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f16:16:16-f32:32:32-f64:64:64-f128:128:128-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64"
target triple = "x86_64--linux-gnu"

module asm "\09.ident\09\22GCC: (Ubuntu/Linaro 4.7.3-12ubuntu1) 4.7.3 LLVM: 3.3\22"

%0 = type { [256 x float], [256 x float] }

@__BLNK__ = common unnamed_addr global %0 zeroinitializer, align 32

; Function Attrs: nounwind uwtable
define void @matmulkernel_(i32* noalias nocapture %n, i32* noalias nocapture %m, i32* noalias nocapture %k, [0 x float]* noalias nocapture %a, [0 x float]* noalias nocapture %b, [0 x float]* noalias nocapture %c) unnamed_addr #0 {
entry:
  %0 = load i32* %n, align 4, !tbaa !0
  %1 = sext i32 %0 to i64
  %2 = icmp slt i64 %1, 0
  %3 = select i1 %2, i64 0, i64 %1
  %4 = load i32* %k, align 4, !tbaa !0
  %5 = sext i32 %4 to i64
  %not = xor i64 %3, -1
  %6 = icmp slt i64 %5, 0
  %7 = select i1 %6, i64 0, i64 %5
  %8 = tail call i32 bitcast (i32 (...)* @blockidxx_ to i32 ()*)() #1
  %9 = shl nsw i32 %8, 4
  %10 = tail call i32 bitcast (i32 (...)* @blockidxy_ to i32 ()*)() #1
  %11 = shl nsw i32 %10, 4
  %12 = tail call i32 bitcast (i32 (...)* @threadidxx_ to i32 ()*)() #1
  %13 = add nsw i32 %12, 1
  %14 = tail call i32 bitcast (i32 (...)* @threadidxy_ to i32 ()*)() #1
  %15 = add nsw i32 %14, 1
  %16 = icmp slt i32 %4, 1
  br i1 %16, label %entry.8_crit_edge, label %"3"

entry.8_crit_edge:                                ; preds = %entry
  %.pre = add nsw i32 %15, %11
  %.pre5 = sext i32 %.pre to i64
  %.pre7 = add nsw i32 %13, %9
  %.pre9 = sext i32 %.pre7 to i64
  %.pre11 = add i64 %.pre9, %not
  br label %"8"

"3":                                              ; preds = %entry
  %not1 = xor i64 %7, -1
  %17 = add nsw i32 %4, -1
  %18 = lshr i32 %17, 4
  %19 = sext i32 %15 to i64
  %20 = shl nsw i64 %19, 4
  %21 = sext i32 %13 to i64
  %22 = add i64 %21, -17
  %23 = add i64 %22, %20
  %24 = add nsw i32 %13, %9
  %25 = sext i32 %24 to i64
  %26 = add i64 %25, %not
  %27 = getelementptr inbounds %0* @__BLNK__, i64 0, i32 0, i64 %23
  %28 = add nsw i32 %15, %11
  %29 = sext i32 %28 to i64
  %30 = mul nsw i64 %29, %7
  %31 = add nsw i64 %30, %not1
  %32 = getelementptr inbounds %0* @__BLNK__, i64 0, i32 1, i64 %23
  %33 = add i64 %20, -17
  br label %"4"

"4":                                              ; preds = %"7", %"3"
  %indvars.iv3 = phi i64 [ %indvars.iv.next4, %"7" ], [ 0, %"3" ]
  %34 = phi float [ %54, %"7" ], [ 0.000000e+00, %"3" ]
  %35 = phi i32 [ %56, %"7" ], [ %18, %"3" ]
  %36 = add nsw i64 %indvars.iv3, %19
  %37 = mul nsw i64 %36, %3
  %38 = add i64 %26, %37
  %39 = getelementptr inbounds [0 x float]* %a, i64 0, i64 %38
  %40 = load float* %39, align 4, !tbaa !2
  store float %40, float* %27, align 4, !tbaa !2
  %41 = add nsw i64 %indvars.iv3, %21
  %42 = add nsw i64 %31, %41
  %43 = getelementptr inbounds [0 x float]* %b, i64 0, i64 %42
  %44 = load float* %43, align 4, !tbaa !2
  store float %44, float* %32, align 4, !tbaa !2
  tail call void bitcast (void (...)* @syncthreads_ to void ()*)() #1
  br label %"5"

"5":                                              ; preds = %"4", %"5"
  %indvars.iv = phi i64 [ 1, %"4" ], [ %indvars.iv.next, %"5" ]
  %45 = phi float [ %34, %"4" ], [ %54, %"5" ]
  %46 = shl nsw i64 %indvars.iv, 4
  %47 = add i64 %22, %46
  %48 = getelementptr inbounds %0* @__BLNK__, i64 0, i32 0, i64 %47
  %49 = load float* %48, align 4, !tbaa !2
  %50 = add i64 %33, %indvars.iv
  %51 = getelementptr inbounds %0* @__BLNK__, i64 0, i32 1, i64 %50
  %52 = load float* %51, align 4, !tbaa !2
  %53 = fmul float %49, %52
  %54 = fadd float %45, %53
  %indvars.iv.next = add i64 %indvars.iv, 1
  %lftr.wideiv = trunc i64 %indvars.iv.next to i32
  %exitcond = icmp eq i32 %lftr.wideiv, 17
  br i1 %exitcond, label %"6", label %"5"

"6":                                              ; preds = %"5"
  tail call void bitcast (void (...)* @syncthreads_ to void ()*)() #1
  %55 = icmp eq i32 %35, 0
  br i1 %55, label %"8", label %"7"

"7":                                              ; preds = %"6"
  %indvars.iv.next4 = add i64 %indvars.iv3, 16
  %56 = add i32 %35, -1
  br label %"4"

"8":                                              ; preds = %"6", %entry.8_crit_edge
  %.pre-phi12 = phi i64 [ %.pre11, %entry.8_crit_edge ], [ %26, %"6" ]
  %.pre-phi6 = phi i64 [ %.pre5, %entry.8_crit_edge ], [ %29, %"6" ]
  %57 = phi float [ 0.000000e+00, %entry.8_crit_edge ], [ %54, %"6" ]
  %58 = mul nsw i64 %.pre-phi6, %3
  %59 = add i64 %.pre-phi12, %58
  %60 = getelementptr inbounds [0 x float]* %c, i64 0, i64 %59
  store float %57, float* %60, align 4, !tbaa !2
  ret void
}

declare i32 @blockidxx_(...)

declare i32 @blockidxy_(...)

declare i32 @threadidxx_(...)

declare i32 @threadidxy_(...)

declare void @syncthreads_(...)

attributes #0 = { nounwind uwtable }
attributes #1 = { nounwind }

!0 = metadata !{metadata !"alias set 7: integer(kind=4)", metadata !1}
!1 = metadata !{metadata !1}
!2 = metadata !{metadata !"alias set 15: real(kind=4)", metadata !1}
