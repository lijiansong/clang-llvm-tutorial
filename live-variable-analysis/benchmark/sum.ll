; ModuleID = 'sum.ll'
source_filename = "sum.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @add1(i32 %arg, i32 %arg1) #0 {
bb:
  %tmp = alloca i32, align 4
  %tmp2 = alloca i32, align 4
  store i32 %arg, i32* %tmp, align 4
  store i32 %arg1, i32* %tmp2, align 4
  %tmp3 = load i32, i32* %tmp, align 4
  %tmp4 = load i32, i32* %tmp2, align 4
  %tmp5 = add nsw i32 %tmp3, %tmp4
  ret i32 %tmp5
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @add2(i32 %arg, i32 %arg1) #0 {
bb:
  %tmp = alloca i32, align 4
  %tmp2 = alloca i32, align 4
  %tmp3 = alloca i32, align 4
  store i32 %arg, i32* %tmp, align 4
  store i32 %arg1, i32* %tmp2, align 4
  store i32 0, i32* %tmp3, align 4
  %tmp4 = load i32, i32* %tmp, align 4
  %tmp5 = icmp eq i32 %tmp4, 1
  br i1 %tmp5, label %bb6, label %bb8

bb6:                                              ; preds = %bb
  %tmp7 = load i32, i32* %tmp2, align 4
  store i32 %tmp7, i32* %tmp3, align 4
  br label %bb8

bb8:                                              ; preds = %bb6, %bb
  %tmp9 = load i32, i32* %tmp, align 4
  %tmp10 = icmp eq i32 %tmp9, 10
  br i1 %tmp10, label %bb11, label %bb14

bb11:                                             ; preds = %bb8
  %tmp12 = load i32, i32* %tmp, align 4
  %tmp13 = add nsw i32 %tmp12, 10
  store i32 %tmp13, i32* %tmp3, align 4
  br label %bb14

bb14:                                             ; preds = %bb11, %bb8
  %tmp15 = load i32, i32* %tmp, align 4
  %tmp16 = load i32, i32* %tmp2, align 4
  %tmp17 = icmp sgt i32 %tmp15, %tmp16
  br i1 %tmp17, label %bb18, label %bb23

bb18:                                             ; preds = %bb14
  %tmp19 = load i32, i32* %tmp, align 4
  %tmp20 = mul nsw i32 %tmp19, 10
  %tmp21 = load i32, i32* %tmp3, align 4
  %tmp22 = add nsw i32 %tmp21, %tmp20
  store i32 %tmp22, i32* %tmp3, align 4
  br label %bb26

bb23:                                             ; preds = %bb14
  %tmp24 = load i32, i32* %tmp2, align 4
  %tmp25 = mul nsw i32 %tmp24, 10
  store i32 %tmp25, i32* %tmp3, align 4
  br label %bb26

bb26:                                             ; preds = %bb23, %bb18
  %tmp27 = load i32, i32* %tmp3, align 4
  ret i32 %tmp27
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
bb:
  %tmp = alloca i32, align 4
  %tmp1 = alloca i32, align 4
  %tmp2 = alloca i32, align 4
  %tmp3 = alloca i32, align 4
  store i32 0, i32* %tmp, align 4
  store i32 100, i32* %tmp1, align 4
  store i32 0, i32* %tmp2, align 4
  store i32 0, i32* %tmp3, align 4
  br label %bb4

bb4:                                              ; preds = %bb18, %bb
  %tmp5 = load i32, i32* %tmp3, align 4
  %tmp6 = load i32, i32* %tmp1, align 4
  %tmp7 = icmp slt i32 %tmp5, %tmp6
  br i1 %tmp7, label %bb8, label %bb21

bb8:                                              ; preds = %bb4
  %tmp9 = load i32, i32* %tmp3, align 4
  %tmp10 = load i32, i32* %tmp2, align 4
  %tmp11 = add nsw i32 %tmp10, %tmp9
  store i32 %tmp11, i32* %tmp2, align 4
  %tmp12 = load i32, i32* %tmp2, align 4
  %tmp13 = icmp slt i32 %tmp12, 50
  br i1 %tmp13, label %bb14, label %bb17

bb14:                                             ; preds = %bb8
  %tmp15 = load i32, i32* %tmp2, align 4
  %tmp16 = add nsw i32 %tmp15, 1
  store i32 %tmp16, i32* %tmp2, align 4
  br label %bb17

bb17:                                             ; preds = %bb14, %bb8
  br label %bb18

bb18:                                             ; preds = %bb17
  %tmp19 = load i32, i32* %tmp3, align 4
  %tmp20 = add nsw i32 %tmp19, 1
  store i32 %tmp20, i32* %tmp3, align 4
  br label %bb4

bb21:                                             ; preds = %bb4
  %tmp22 = load i32, i32* %tmp1, align 4
  %tmp23 = load i32, i32* %tmp2, align 4
  %tmp24 = call i32 @add1(i32 %tmp22, i32 %tmp23)
  %tmp25 = load i32, i32* %tmp1, align 4
  %tmp26 = load i32, i32* %tmp2, align 4
  %tmp27 = call i32 @add2(i32 %tmp25, i32 %tmp26)
  ret i32 0
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
