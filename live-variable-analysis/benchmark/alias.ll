; ModuleID = 'alias.ll'
source_filename = "alias.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @sum(i32* %arg, i32* %arg1, i32* %arg2, i32 %arg3) #0 {
bb:
  %tmp = alloca i32, align 4
  %tmp4 = alloca i32*, align 8
  %tmp5 = alloca i32*, align 8
  %tmp6 = alloca i32*, align 8
  %tmp7 = alloca i32, align 4
  %tmp8 = alloca i32*, align 8
  %tmp9 = alloca i32*, align 8
  %tmp10 = alloca i32*, align 8
  %tmp11 = alloca i32, align 4
  store i32* %arg, i32** %tmp4, align 8
  store i32* %arg1, i32** %tmp5, align 8
  store i32* %arg2, i32** %tmp6, align 8
  store i32 %arg3, i32* %tmp7, align 4
  %tmp12 = load i32*, i32** %tmp4, align 8
  %tmp13 = getelementptr inbounds i32, i32* %tmp12, i64 1
  store i32* %tmp13, i32** %tmp8, align 8
  %tmp14 = load i32*, i32** %tmp5, align 8
  %tmp15 = getelementptr inbounds i32, i32* %tmp14, i64 1
  store i32* %tmp15, i32** %tmp9, align 8
  %tmp16 = load i32*, i32** %tmp6, align 8
  %tmp17 = getelementptr inbounds i32, i32* %tmp16, i64 1
  store i32* %tmp17, i32** %tmp10, align 8
  store i32 1, i32* %tmp11, align 4
  br label %bb18

bb18:                                             ; preds = %bb38, %bb
  %tmp19 = load i32, i32* %tmp11, align 4
  %tmp20 = load i32, i32* %tmp7, align 4
  %tmp21 = icmp slt i32 %tmp19, %tmp20
  br i1 %tmp21, label %bb22, label %bb41

bb22:                                             ; preds = %bb18
  %tmp23 = load i32*, i32** %tmp8, align 8
  %tmp24 = load i32, i32* %tmp11, align 4
  %tmp25 = sext i32 %tmp24 to i64
  %tmp26 = getelementptr inbounds i32, i32* %tmp23, i64 %tmp25
  %tmp27 = load i32, i32* %tmp26, align 4
  %tmp28 = load i32*, i32** %tmp9, align 8
  %tmp29 = load i32, i32* %tmp11, align 4
  %tmp30 = sext i32 %tmp29 to i64
  %tmp31 = getelementptr inbounds i32, i32* %tmp28, i64 %tmp30
  %tmp32 = load i32, i32* %tmp31, align 4
  %tmp33 = add nsw i32 %tmp27, %tmp32
  %tmp34 = load i32*, i32** %tmp10, align 8
  %tmp35 = load i32, i32* %tmp11, align 4
  %tmp36 = sext i32 %tmp35 to i64
  %tmp37 = getelementptr inbounds i32, i32* %tmp34, i64 %tmp36
  store i32 %tmp33, i32* %tmp37, align 4
  br label %bb38

bb38:                                             ; preds = %bb22
  %tmp39 = load i32, i32* %tmp11, align 4
  %tmp40 = add nsw i32 %tmp39, 1
  store i32 %tmp40, i32* %tmp11, align 4
  br label %bb18

bb41:                                             ; preds = %bb18
  %tmp42 = load i32, i32* %tmp7, align 4
  switch i32 %tmp42, label %bb55 [
    i32 0, label %bb43
    i32 1, label %bb47
    i32 2, label %bb51
  ]

bb43:                                             ; preds = %bb41
  %tmp44 = load i32*, i32** %tmp10, align 8
  %tmp45 = getelementptr inbounds i32, i32* %tmp44, i64 0
  %tmp46 = load i32, i32* %tmp45, align 4
  store i32 %tmp46, i32* %tmp, align 4
  br label %bb62

bb47:                                             ; preds = %bb41
  %tmp48 = load i32*, i32** %tmp10, align 8
  %tmp49 = getelementptr inbounds i32, i32* %tmp48, i64 1
  %tmp50 = load i32, i32* %tmp49, align 4
  store i32 %tmp50, i32* %tmp, align 4
  br label %bb62

bb51:                                             ; preds = %bb41
  %tmp52 = load i32*, i32** %tmp10, align 8
  %tmp53 = getelementptr inbounds i32, i32* %tmp52, i64 2
  %tmp54 = load i32, i32* %tmp53, align 4
  store i32 %tmp54, i32* %tmp, align 4
  br label %bb62

bb55:                                             ; preds = %bb41
  %tmp56 = load i32*, i32** %tmp10, align 8
  %tmp57 = load i32, i32* %tmp7, align 4
  %tmp58 = sub nsw i32 %tmp57, 1
  %tmp59 = sext i32 %tmp58 to i64
  %tmp60 = getelementptr inbounds i32, i32* %tmp56, i64 %tmp59
  %tmp61 = load i32, i32* %tmp60, align 4
  store i32 %tmp61, i32* %tmp, align 4
  br label %bb62

bb62:                                             ; preds = %bb55, %bb51, %bb47, %bb43
  %tmp63 = load i32, i32* %tmp, align 4
  ret i32 %tmp63
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
