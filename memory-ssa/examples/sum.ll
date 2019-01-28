; ModuleID = 'sum.ll'
source_filename = "sum.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @sum(i32* %arg, i32* %arg1, i32* %arg2, i32 %arg3) #0 {
bb:
  %tmp = alloca i32*, align 8
  %tmp4 = alloca i32*, align 8
  %tmp5 = alloca i32*, align 8
  %tmp6 = alloca i32, align 4
  %tmp7 = alloca i32, align 4
  store i32* %arg, i32** %tmp, align 8
  store i32* %arg1, i32** %tmp4, align 8
  store i32* %arg2, i32** %tmp5, align 8
  store i32 %arg3, i32* %tmp6, align 4
  store i32 0, i32* %tmp7, align 4
  br label %bb8

bb8:                                              ; preds = %bb28, %bb
  %tmp9 = load i32, i32* %tmp7, align 4
  %tmp10 = load i32, i32* %tmp6, align 4
  %tmp11 = icmp slt i32 %tmp9, %tmp10
  br i1 %tmp11, label %bb12, label %bb31

bb12:                                             ; preds = %bb8
  %tmp13 = load i32*, i32** %tmp, align 8
  %tmp14 = load i32, i32* %tmp7, align 4
  %tmp15 = sext i32 %tmp14 to i64
  %tmp16 = getelementptr inbounds i32, i32* %tmp13, i64 %tmp15
  %tmp17 = load i32, i32* %tmp16, align 4
  %tmp18 = load i32*, i32** %tmp4, align 8
  %tmp19 = load i32, i32* %tmp7, align 4
  %tmp20 = sext i32 %tmp19 to i64
  %tmp21 = getelementptr inbounds i32, i32* %tmp18, i64 %tmp20
  %tmp22 = load i32, i32* %tmp21, align 4
  %tmp23 = add nsw i32 %tmp17, %tmp22
  %tmp24 = load i32*, i32** %tmp5, align 8
  %tmp25 = load i32, i32* %tmp7, align 4
  %tmp26 = sext i32 %tmp25 to i64
  %tmp27 = getelementptr inbounds i32, i32* %tmp24, i64 %tmp26
  store i32 %tmp23, i32* %tmp27, align 4
  br label %bb28

bb28:                                             ; preds = %bb12
  %tmp29 = load i32, i32* %tmp7, align 4
  %tmp30 = add nsw i32 %tmp29, 1
  store i32 %tmp30, i32* %tmp7, align 4
  br label %bb8

bb31:                                             ; preds = %bb8
  ret void
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
