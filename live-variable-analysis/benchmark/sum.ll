; ModuleID = 'sum.ll'
source_filename = "sum.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: norecurse nounwind readnone ssp uwtable
define i32 @add1(i32 %arg, i32 %arg1) local_unnamed_addr #0 {
bb:
  %tmp = add nsw i32 %arg1, %arg
  ret i32 %tmp
}

; Function Attrs: norecurse nounwind readnone ssp uwtable
define i32 @add2(i32 %arg, i32 %arg1) local_unnamed_addr #0 {
bb:
  %tmp = icmp eq i32 %arg, 1
  %tmp2 = select i1 %tmp, i32 %arg1, i32 0
  %tmp3 = icmp eq i32 %arg, 10
  %tmp4 = add nsw i32 %arg, 10
  %tmp5 = select i1 %tmp3, i32 %tmp4, i32 %tmp2
  %tmp6 = icmp sgt i32 %arg, %arg1
  %tmp7 = mul nsw i32 %arg, 10
  %tmp8 = add nsw i32 %tmp5, %tmp7
  %tmp9 = mul nsw i32 %arg1, 10
  %tmp10 = select i1 %tmp6, i32 %tmp8, i32 %tmp9
  ret i32 %tmp10
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
