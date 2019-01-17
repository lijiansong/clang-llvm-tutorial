; ModuleID = 'foo.ll'
source_filename = "foo.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @sum(i32 %arg, i32 %arg1) #0 {
bb:
  %tmp = alloca i32, align 4
  %tmp2 = alloca i32, align 4
  %tmp3 = alloca i32, align 4
  %tmp4 = alloca i32, align 4
  store i32 %arg, i32* %tmp, align 4
  store i32 %arg1, i32* %tmp2, align 4
  %tmp5 = load i32, i32* %tmp, align 4
  %tmp6 = load i32, i32* %tmp2, align 4
  %tmp7 = add nsw i32 %tmp5, %tmp6
  store i32 %tmp7, i32* %tmp3, align 4
  %tmp8 = load i32, i32* %tmp3, align 4
  %tmp9 = icmp eq i32 %tmp8, 10
  br i1 %tmp9, label %bb10, label %bb14

bb10:                                             ; preds = %bb
  %tmp11 = load i32, i32* %tmp, align 4
  %tmp12 = load i32, i32* %tmp3, align 4
  %tmp13 = add nsw i32 %tmp12, %tmp11
  store i32 %tmp13, i32* %tmp3, align 4
  br label %bb18

bb14:                                             ; preds = %bb
  %tmp15 = load i32, i32* %tmp2, align 4
  %tmp16 = load i32, i32* %tmp3, align 4
  %tmp17 = add nsw i32 %tmp16, %tmp15
  store i32 %tmp17, i32* %tmp3, align 4
  br label %bb18

bb18:                                             ; preds = %bb14, %bb10
  store i32 0, i32* %tmp4, align 4
  br label %bb19

bb19:                                             ; preds = %bb27, %bb18
  %tmp20 = load i32, i32* %tmp4, align 4
  %tmp21 = load i32, i32* %tmp2, align 4
  %tmp22 = icmp slt i32 %tmp20, %tmp21
  br i1 %tmp22, label %bb23, label %bb30

bb23:                                             ; preds = %bb19
  %tmp24 = load i32, i32* %tmp4, align 4
  %tmp25 = load i32, i32* %tmp3, align 4
  %tmp26 = add nsw i32 %tmp25, %tmp24
  store i32 %tmp26, i32* %tmp3, align 4
  br label %bb27

bb27:                                             ; preds = %bb23
  %tmp28 = load i32, i32* %tmp4, align 4
  %tmp29 = add nsw i32 %tmp28, 1
  store i32 %tmp29, i32* %tmp4, align 4
  br label %bb19

bb30:                                             ; preds = %bb19
  %tmp31 = load i32, i32* %tmp3, align 4
  ret i32 %tmp31
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
bb:
  %tmp = call i32 @sum(i32 1, i32 2)
  ret i32 0
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
