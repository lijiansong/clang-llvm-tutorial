; ModuleID = 'sv_add.ll'
source_filename = "sv_add.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

@c = common global [10 x i32] zeroinitializer, align 16
@a = common global [10 x i32] zeroinitializer, align 16
@b = common global [10 x i32] zeroinitializer, align 16

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
bb:
  %tmp = alloca i32*, align 8
  %tmp1 = alloca i32*, align 8
  %tmp2 = alloca i32*, align 8
  %tmp3 = alloca i32, align 4
  %tmp4 = alloca i32, align 4
  %tmp5 = alloca i32, align 4
  store i32 0, i32* %tmp5, align 4
  store i32* getelementptr inbounds ([10 x i32], [10 x i32]* @c, i32 0, i32 0), i32** %tmp, align 8
  store i32* getelementptr inbounds ([10 x i32], [10 x i32]* @a, i32 0, i32 0), i32** %tmp1, align 8
  store i32* getelementptr inbounds ([10 x i32], [10 x i32]* @b, i32 0, i32 0), i32** %tmp2, align 8
  store i32 10, i32* %tmp3, align 4
  store i32 0, i32* %tmp4, align 4
  br label %bb6

bb6:                                              ; preds = %bb10, %bb
  %tmp7 = load i32, i32* %tmp4, align 4
  %tmp8 = load i32, i32* %tmp3, align 4
  %tmp9 = icmp slt i32 %tmp7, %tmp8
  br i1 %tmp9, label %bb10, label %bb28

bb10:                                             ; preds = %bb6
  %tmp11 = load i32*, i32** %tmp1, align 8
  %tmp12 = load i32, i32* %tmp4, align 4
  %tmp13 = sext i32 %tmp12 to i64
  %tmp14 = getelementptr inbounds i32, i32* %tmp11, i64 %tmp13
  %tmp15 = load i32, i32* %tmp14, align 4
  %tmp16 = load i32*, i32** %tmp2, align 8
  %tmp17 = load i32, i32* %tmp4, align 4
  %tmp18 = sext i32 %tmp17 to i64
  %tmp19 = getelementptr inbounds i32, i32* %tmp16, i64 %tmp18
  %tmp20 = load i32, i32* %tmp19, align 4
  %tmp21 = add nsw i32 %tmp15, %tmp20
  %tmp22 = load i32*, i32** %tmp, align 8
  %tmp23 = load i32, i32* %tmp4, align 4
  %tmp24 = sext i32 %tmp23 to i64
  %tmp25 = getelementptr inbounds i32, i32* %tmp22, i64 %tmp24
  store i32 %tmp21, i32* %tmp25, align 4
  %tmp26 = load i32, i32* %tmp4, align 4
  %tmp27 = add nsw i32 %tmp26, 1
  store i32 %tmp27, i32* %tmp4, align 4
  br label %bb6

bb28:                                             ; preds = %bb6
  ret i32 0
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
