; ModuleID = 'alias.ll'
source_filename = "alias.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

; Function Attrs: norecurse nounwind ssp uwtable
define i32 @sum(i32* nocapture readonly %arg, i32* nocapture readonly %arg1, i32* nocapture %arg2, i32 %arg3) local_unnamed_addr #0 {
bb:
  %tmp = getelementptr inbounds i32, i32* %arg, i64 1
  %tmp4 = getelementptr inbounds i32, i32* %arg1, i64 1
  %tmp5 = getelementptr inbounds i32, i32* %arg2, i64 1
  %tmp6 = icmp sgt i32 %arg3, 1
  br i1 %tmp6, label %bb7, label %bb81

bb7:                                              ; preds = %bb
  %tmp8 = zext i32 %arg3 to i64
  %tmp9 = add nsw i64 %tmp8, -1
  %tmp10 = icmp ult i64 %tmp9, 8
  br i1 %tmp10, label %bb11, label %bb35

bb11:                                             ; preds = %bb79, %bb35, %bb7
  %tmp12 = phi i64 [ 1, %bb35 ], [ 1, %bb7 ], [ %tmp55, %bb79 ]
  %tmp13 = sub nsw i64 %tmp8, %tmp12
  %tmp14 = add nsw i64 %tmp8, -1
  %tmp15 = sub nsw i64 %tmp14, %tmp12
  %tmp16 = and i64 %tmp13, 3
  %tmp17 = icmp eq i64 %tmp16, 0
  br i1 %tmp17, label %bb31, label %bb18

bb18:                                             ; preds = %bb11
  br label %bb19

bb19:                                             ; preds = %bb19, %bb18
  %tmp20 = phi i64 [ %tmp28, %bb19 ], [ %tmp12, %bb18 ]
  %tmp21 = phi i64 [ %tmp29, %bb19 ], [ %tmp16, %bb18 ]
  %tmp22 = getelementptr inbounds i32, i32* %tmp, i64 %tmp20
  %tmp23 = load i32, i32* %tmp22, align 4, !tbaa !3
  %tmp24 = getelementptr inbounds i32, i32* %tmp4, i64 %tmp20
  %tmp25 = load i32, i32* %tmp24, align 4, !tbaa !3
  %tmp26 = add nsw i32 %tmp25, %tmp23
  %tmp27 = getelementptr inbounds i32, i32* %tmp5, i64 %tmp20
  store i32 %tmp26, i32* %tmp27, align 4, !tbaa !3
  %tmp28 = add nuw nsw i64 %tmp20, 1
  %tmp29 = add i64 %tmp21, -1
  %tmp30 = icmp eq i64 %tmp29, 0
  br i1 %tmp30, label %bb31, label %bb19, !llvm.loop !7

bb31:                                             ; preds = %bb19, %bb11
  %tmp32 = phi i64 [ %tmp12, %bb11 ], [ %tmp28, %bb19 ]
  %tmp33 = icmp ult i64 %tmp15, 3
  br i1 %tmp33, label %bb81, label %bb34

bb34:                                             ; preds = %bb31
  br label %bb82

bb35:                                             ; preds = %bb7
  %tmp36 = getelementptr i32, i32* %arg2, i64 2
  %tmp37 = add nuw nsw i64 %tmp8, 1
  %tmp38 = getelementptr i32, i32* %arg2, i64 %tmp37
  %tmp39 = getelementptr i32, i32* %arg, i64 2
  %tmp40 = getelementptr i32, i32* %arg, i64 %tmp37
  %tmp41 = getelementptr i32, i32* %arg1, i64 2
  %tmp42 = getelementptr i32, i32* %arg1, i64 %tmp37
  %tmp43 = icmp ult i32* %tmp36, %tmp40
  %tmp44 = icmp ult i32* %tmp39, %tmp38
  %tmp45 = and i1 %tmp43, %tmp44
  %tmp46 = icmp ult i32* %tmp36, %tmp42
  %tmp47 = icmp ult i32* %tmp41, %tmp38
  %tmp48 = and i1 %tmp46, %tmp47
  %tmp49 = or i1 %tmp45, %tmp48
  br i1 %tmp49, label %bb11, label %bb50

bb50:                                             ; preds = %bb35
  %tmp51 = add i32 %arg3, 7
  %tmp52 = and i32 %tmp51, 7
  %tmp53 = zext i32 %tmp52 to i64
  %tmp54 = sub nsw i64 %tmp9, %tmp53
  %tmp55 = add nsw i64 %tmp54, 1
  br label %bb56

bb56:                                             ; preds = %bb56, %bb50
  %tmp57 = phi i64 [ 0, %bb50 ], [ %tmp77, %bb56 ]
  %tmp58 = or i64 %tmp57, 1
  %tmp59 = getelementptr inbounds i32, i32* %tmp, i64 %tmp58
  %tmp60 = bitcast i32* %tmp59 to <4 x i32>*
  %tmp61 = load <4 x i32>, <4 x i32>* %tmp60, align 4, !tbaa !3, !alias.scope !9
  %tmp62 = getelementptr i32, i32* %tmp59, i64 4
  %tmp63 = bitcast i32* %tmp62 to <4 x i32>*
  %tmp64 = load <4 x i32>, <4 x i32>* %tmp63, align 4, !tbaa !3, !alias.scope !9
  %tmp65 = getelementptr inbounds i32, i32* %tmp4, i64 %tmp58
  %tmp66 = bitcast i32* %tmp65 to <4 x i32>*
  %tmp67 = load <4 x i32>, <4 x i32>* %tmp66, align 4, !tbaa !3, !alias.scope !12
  %tmp68 = getelementptr i32, i32* %tmp65, i64 4
  %tmp69 = bitcast i32* %tmp68 to <4 x i32>*
  %tmp70 = load <4 x i32>, <4 x i32>* %tmp69, align 4, !tbaa !3, !alias.scope !12
  %tmp71 = add nsw <4 x i32> %tmp67, %tmp61
  %tmp72 = add nsw <4 x i32> %tmp70, %tmp64
  %tmp73 = getelementptr inbounds i32, i32* %tmp5, i64 %tmp58
  %tmp74 = bitcast i32* %tmp73 to <4 x i32>*
  store <4 x i32> %tmp71, <4 x i32>* %tmp74, align 4, !tbaa !3, !alias.scope !14, !noalias !16
  %tmp75 = getelementptr i32, i32* %tmp73, i64 4
  %tmp76 = bitcast i32* %tmp75 to <4 x i32>*
  store <4 x i32> %tmp72, <4 x i32>* %tmp76, align 4, !tbaa !3, !alias.scope !14, !noalias !16
  %tmp77 = add i64 %tmp57, 8
  %tmp78 = icmp eq i64 %tmp77, %tmp54
  br i1 %tmp78, label %bb79, label %bb56, !llvm.loop !17

bb79:                                             ; preds = %bb56
  %tmp80 = icmp eq i32 %tmp52, 0
  br i1 %tmp80, label %bb81, label %bb11

bb81:                                             ; preds = %bb82, %bb79, %bb31, %bb
  switch i32 %arg3, label %bb117 [
    i32 0, label %bb121
    i32 1, label %bb113
    i32 2, label %bb115
  ]

bb82:                                             ; preds = %bb82, %bb34
  %tmp83 = phi i64 [ %tmp32, %bb34 ], [ %tmp111, %bb82 ]
  %tmp84 = getelementptr inbounds i32, i32* %tmp, i64 %tmp83
  %tmp85 = load i32, i32* %tmp84, align 4, !tbaa !3
  %tmp86 = getelementptr inbounds i32, i32* %tmp4, i64 %tmp83
  %tmp87 = load i32, i32* %tmp86, align 4, !tbaa !3
  %tmp88 = add nsw i32 %tmp87, %tmp85
  %tmp89 = getelementptr inbounds i32, i32* %tmp5, i64 %tmp83
  store i32 %tmp88, i32* %tmp89, align 4, !tbaa !3
  %tmp90 = add nuw nsw i64 %tmp83, 1
  %tmp91 = getelementptr inbounds i32, i32* %tmp, i64 %tmp90
  %tmp92 = load i32, i32* %tmp91, align 4, !tbaa !3
  %tmp93 = getelementptr inbounds i32, i32* %tmp4, i64 %tmp90
  %tmp94 = load i32, i32* %tmp93, align 4, !tbaa !3
  %tmp95 = add nsw i32 %tmp94, %tmp92
  %tmp96 = getelementptr inbounds i32, i32* %tmp5, i64 %tmp90
  store i32 %tmp95, i32* %tmp96, align 4, !tbaa !3
  %tmp97 = add nsw i64 %tmp83, 2
  %tmp98 = getelementptr inbounds i32, i32* %tmp, i64 %tmp97
  %tmp99 = load i32, i32* %tmp98, align 4, !tbaa !3
  %tmp100 = getelementptr inbounds i32, i32* %tmp4, i64 %tmp97
  %tmp101 = load i32, i32* %tmp100, align 4, !tbaa !3
  %tmp102 = add nsw i32 %tmp101, %tmp99
  %tmp103 = getelementptr inbounds i32, i32* %tmp5, i64 %tmp97
  store i32 %tmp102, i32* %tmp103, align 4, !tbaa !3
  %tmp104 = add nsw i64 %tmp83, 3
  %tmp105 = getelementptr inbounds i32, i32* %tmp, i64 %tmp104
  %tmp106 = load i32, i32* %tmp105, align 4, !tbaa !3
  %tmp107 = getelementptr inbounds i32, i32* %tmp4, i64 %tmp104
  %tmp108 = load i32, i32* %tmp107, align 4, !tbaa !3
  %tmp109 = add nsw i32 %tmp108, %tmp106
  %tmp110 = getelementptr inbounds i32, i32* %tmp5, i64 %tmp104
  store i32 %tmp109, i32* %tmp110, align 4, !tbaa !3
  %tmp111 = add nsw i64 %tmp83, 4
  %tmp112 = icmp eq i64 %tmp111, %tmp8
  br i1 %tmp112, label %bb81, label %bb82, !llvm.loop !20

bb113:                                            ; preds = %bb81
  %tmp114 = getelementptr inbounds i32, i32* %arg2, i64 2
  br label %bb121

bb115:                                            ; preds = %bb81
  %tmp116 = getelementptr inbounds i32, i32* %arg2, i64 3
  br label %bb121

bb117:                                            ; preds = %bb81
  %tmp118 = add nsw i32 %arg3, -1
  %tmp119 = sext i32 %tmp118 to i64
  %tmp120 = getelementptr inbounds i32, i32* %tmp5, i64 %tmp119
  br label %bb121

bb121:                                            ; preds = %bb117, %bb115, %bb113, %bb81
  %tmp122 = phi i32* [ %tmp120, %bb117 ], [ %tmp116, %bb115 ], [ %tmp114, %bb113 ], [ %tmp5, %bb81 ]
  %tmp123 = load i32, i32* %tmp122, align 4, !tbaa !3
  ret i32 %tmp123
}

attributes #0 = { norecurse nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = distinct !{!7, !8}
!8 = !{!"llvm.loop.unroll.disable"}
!9 = !{!10}
!10 = distinct !{!10, !11}
!11 = distinct !{!11, !"LVerDomain"}
!12 = !{!13}
!13 = distinct !{!13, !11}
!14 = !{!15}
!15 = distinct !{!15, !11}
!16 = !{!10, !13}
!17 = distinct !{!17, !18, !19}
!18 = !{!"llvm.loop.vectorize.width", i32 1}
!19 = !{!"llvm.loop.interleave.count", i32 1}
!20 = distinct !{!20, !18, !19}
