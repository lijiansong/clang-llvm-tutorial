; ModuleID = 'sum.ll'
source_filename = "sum.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
bb:
  %tmp = alloca i32, align 4
  %tmp1 = alloca i32, align 4
  %tmp2 = alloca i32, align 4
  %tmp3 = alloca i32, align 4
  %tmp4 = alloca i32, align 4
  %tmp5 = alloca i32, align 4
  %tmp6 = alloca i32, align 4
  %tmp7 = alloca i32, align 4
  %tmp8 = alloca i32, align 4
  store i32 0, i32* %tmp5, align 4
  store i32 100, i32* %tmp6, align 4
  store i32 0, i32* %tmp7, align 4
  store i32 0, i32* %tmp8, align 4
  br label %bb9

bb9:                                              ; preds = %bb23, %bb
  %tmp10 = load i32, i32* %tmp8, align 4
  %tmp11 = load i32, i32* %tmp6, align 4
  %tmp12 = icmp slt i32 %tmp10, %tmp11
  br i1 %tmp12, label %bb13, label %bb26

bb13:                                             ; preds = %bb9
  %tmp14 = load i32, i32* %tmp8, align 4
  %tmp15 = load i32, i32* %tmp7, align 4
  %tmp16 = add nsw i32 %tmp15, %tmp14
  store i32 %tmp16, i32* %tmp7, align 4
  %tmp17 = load i32, i32* %tmp7, align 4
  %tmp18 = icmp slt i32 %tmp17, 50
  br i1 %tmp18, label %bb19, label %bb22

bb19:                                             ; preds = %bb13
  %tmp20 = load i32, i32* %tmp7, align 4
  %tmp21 = add nsw i32 %tmp20, 1
  store i32 %tmp21, i32* %tmp7, align 4
  br label %bb22

bb22:                                             ; preds = %bb19, %bb13
  br label %bb23

bb23:                                             ; preds = %bb22
  %tmp24 = load i32, i32* %tmp8, align 4
  %tmp25 = add nsw i32 %tmp24, 1
  store i32 %tmp25, i32* %tmp8, align 4
  br label %bb9

bb26:                                             ; preds = %bb9
  %tmp27 = load i32, i32* %tmp6, align 4
  %tmp28 = load i32, i32* %tmp7, align 4
  store i32 %tmp27, i32* %tmp3, align 4
  store i32 %tmp28, i32* %tmp4, align 4
  %tmp29 = load i32, i32* %tmp3, align 4
  %tmp30 = load i32, i32* %tmp4, align 4
  %tmp31 = add nsw i32 %tmp29, %tmp30
  %tmp32 = load i32, i32* %tmp6, align 4
  %tmp33 = load i32, i32* %tmp7, align 4
  store i32 %tmp32, i32* %tmp, align 4
  store i32 %tmp33, i32* %tmp1, align 4
  store i32 0, i32* %tmp2, align 4
  %tmp34 = load i32, i32* %tmp, align 4
  %tmp35 = icmp eq i32 %tmp34, 1
  br i1 %tmp35, label %bb36, label %bb38

bb36:                                             ; preds = %bb26
  %tmp37 = load i32, i32* %tmp1, align 4
  store i32 %tmp37, i32* %tmp2, align 4
  br label %bb38

bb38:                                             ; preds = %bb36, %bb26
  %tmp39 = load i32, i32* %tmp, align 4
  %tmp40 = icmp eq i32 %tmp39, 10
  br i1 %tmp40, label %bb41, label %bb44

bb41:                                             ; preds = %bb38
  %tmp42 = load i32, i32* %tmp, align 4
  %tmp43 = add nsw i32 %tmp42, 10
  store i32 %tmp43, i32* %tmp2, align 4
  br label %bb44

bb44:                                             ; preds = %bb41, %bb38
  %tmp45 = load i32, i32* %tmp, align 4
  %tmp46 = load i32, i32* %tmp1, align 4
  %tmp47 = icmp sgt i32 %tmp45, %tmp46
  br i1 %tmp47, label %bb48, label %bb53

bb48:                                             ; preds = %bb44
  %tmp49 = load i32, i32* %tmp, align 4
  %tmp50 = mul nsw i32 %tmp49, 10
  %tmp51 = load i32, i32* %tmp2, align 4
  %tmp52 = add nsw i32 %tmp51, %tmp50
  store i32 %tmp52, i32* %tmp2, align 4
  br label %bb56

bb53:                                             ; preds = %bb44
  %tmp54 = load i32, i32* %tmp1, align 4
  %tmp55 = mul nsw i32 %tmp54, 10
  store i32 %tmp55, i32* %tmp2, align 4
  br label %bb56

bb56:                                             ; preds = %bb53, %bb48
  %tmp57 = load i32, i32* %tmp2, align 4
  ret i32 0
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
