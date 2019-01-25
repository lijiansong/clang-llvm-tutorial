; ModuleID = 'add.ll'
source_filename = "add.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
bb:
  %tmp = alloca i32*, align 8
  %tmp1 = alloca i32*, align 8
  %tmp2 = alloca i32*, align 8
  %tmp3 = alloca i32, align 4
  %tmp4 = alloca i32, align 4
  %tmp5 = alloca i32, align 4
  %tmp6 = alloca [10 x i32], align 16
  %tmp7 = alloca [10 x i32], align 16
  %tmp8 = alloca [10 x i32], align 16
  store i32 0, i32* %tmp5, align 4
  %tmp9 = getelementptr inbounds [10 x i32], [10 x i32]* %tmp8, i32 0, i32 0
  %tmp10 = getelementptr inbounds [10 x i32], [10 x i32]* %tmp6, i32 0, i32 0
  %tmp11 = getelementptr inbounds [10 x i32], [10 x i32]* %tmp7, i32 0, i32 0
  store i32* %tmp9, i32** %tmp, align 8
  store i32* %tmp10, i32** %tmp1, align 8
  store i32* %tmp11, i32** %tmp2, align 8
  store i32 10, i32* %tmp3, align 4
  store i32 0, i32* %tmp4, align 4
  br label %bb12

bb12:                                             ; preds = %bb16, %bb
  %tmp13 = load i32, i32* %tmp4, align 4
  %tmp14 = load i32, i32* %tmp3, align 4
  %tmp15 = icmp slt i32 %tmp13, %tmp14
  br i1 %tmp15, label %bb16, label %bb34

bb16:                                             ; preds = %bb12
  %tmp17 = load i32*, i32** %tmp1, align 8
  %tmp18 = load i32, i32* %tmp4, align 4
  %tmp19 = sext i32 %tmp18 to i64
  %tmp20 = getelementptr inbounds i32, i32* %tmp17, i64 %tmp19
  %tmp21 = load i32, i32* %tmp20, align 4
  %tmp22 = load i32*, i32** %tmp2, align 8
  %tmp23 = load i32, i32* %tmp4, align 4
  %tmp24 = sext i32 %tmp23 to i64
  %tmp25 = getelementptr inbounds i32, i32* %tmp22, i64 %tmp24
  %tmp26 = load i32, i32* %tmp25, align 4
  %tmp27 = add nsw i32 %tmp21, %tmp26
  %tmp28 = load i32*, i32** %tmp, align 8
  %tmp29 = load i32, i32* %tmp4, align 4
  %tmp30 = sext i32 %tmp29 to i64
  %tmp31 = getelementptr inbounds i32, i32* %tmp28, i64 %tmp30
  store i32 %tmp27, i32* %tmp31, align 4
  %tmp32 = load i32, i32* %tmp4, align 4
  %tmp33 = add nsw i32 %tmp32, 1
  store i32 %tmp33, i32* %tmp4, align 4
  br label %bb12

bb34:                                             ; preds = %bb12
  ret i32 0
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
