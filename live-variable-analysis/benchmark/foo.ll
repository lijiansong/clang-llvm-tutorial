; ModuleID = 'foo.ll'
source_filename = "foo.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
bb:
  %tmp = alloca i32, align 4
  %tmp1 = alloca i32, align 4
  %tmp2 = alloca i32, align 4
  %tmp3 = alloca i32, align 4
  store i32 1, i32* %tmp, align 4
  store i32 2, i32* %tmp1, align 4
  %tmp4 = load i32, i32* %tmp, align 4
  %tmp5 = load i32, i32* %tmp1, align 4
  %tmp6 = add nsw i32 %tmp4, %tmp5
  store i32 %tmp6, i32* %tmp2, align 4
  %tmp7 = load i32, i32* %tmp2, align 4
  %tmp8 = icmp eq i32 %tmp7, 10
  br i1 %tmp8, label %bb9, label %bb13

bb9:                                              ; preds = %bb
  %tmp10 = load i32, i32* %tmp, align 4
  %tmp11 = load i32, i32* %tmp2, align 4
  %tmp12 = add nsw i32 %tmp11, %tmp10
  store i32 %tmp12, i32* %tmp2, align 4
  br label %bb17

bb13:                                             ; preds = %bb
  %tmp14 = load i32, i32* %tmp1, align 4
  %tmp15 = load i32, i32* %tmp2, align 4
  %tmp16 = add nsw i32 %tmp15, %tmp14
  store i32 %tmp16, i32* %tmp2, align 4
  br label %bb17

bb17:                                             ; preds = %bb13, %bb9
  store i32 0, i32* %tmp3, align 4
  br label %bb18

bb18:                                             ; preds = %bb22, %bb17
  %tmp19 = load i32, i32* %tmp3, align 4
  %tmp20 = load i32, i32* %tmp1, align 4
  %tmp21 = icmp slt i32 %tmp19, %tmp20
  br i1 %tmp21, label %bb22, label %bb28

bb22:                                             ; preds = %bb18
  %tmp23 = load i32, i32* %tmp3, align 4
  %tmp24 = load i32, i32* %tmp2, align 4
  %tmp25 = add nsw i32 %tmp24, %tmp23
  store i32 %tmp25, i32* %tmp2, align 4
  %tmp26 = load i32, i32* %tmp3, align 4
  %tmp27 = add nsw i32 %tmp26, 1
  store i32 %tmp27, i32* %tmp3, align 4
  br label %bb18

bb28:                                             ; preds = %bb18
  %tmp29 = load i32, i32* %tmp2, align 4
  ret i32 0
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
