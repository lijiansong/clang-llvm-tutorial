; ModuleID = 'foo.ll'
source_filename = "foo.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: norecurse nounwind readnone ssp uwtable
define i32 @sum(i32 %arg, i32 %arg1) local_unnamed_addr #0 {
bb:
  %tmp = add nsw i32 %arg1, %arg
  %tmp2 = icmp eq i32 %tmp, 10
  %tmp3 = select i1 %tmp2, i32 %arg, i32 %arg1
  %tmp4 = add nsw i32 %tmp3, %tmp
  %tmp5 = icmp sgt i32 %arg1, 0
  br i1 %tmp5, label %bb6, label %bb19

bb6:                                              ; preds = %bb
  %tmp7 = add i32 %tmp3, %arg
  %tmp8 = shl i32 %arg1, 1
  %tmp9 = add i32 %tmp7, %tmp8
  %tmp10 = add i32 %arg1, -1
  %tmp11 = zext i32 %tmp10 to i33
  %tmp12 = add i32 %arg1, -2
  %tmp13 = zext i32 %tmp12 to i33
  %tmp14 = mul i33 %tmp11, %tmp13
  %tmp15 = lshr i33 %tmp14, 1
  %tmp16 = trunc i33 %tmp15 to i32
  %tmp17 = add i32 %tmp9, %tmp16
  %tmp18 = add i32 %tmp17, -1
  br label %bb19

bb19:                                             ; preds = %bb6, %bb
  %tmp20 = phi i32 [ %tmp4, %bb ], [ %tmp18, %bb6 ]
  ret i32 %tmp20
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define i32 @main() local_unnamed_addr #0 {
bb:
  ret i32 0
}

attributes #0 = { norecurse nounwind readnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
