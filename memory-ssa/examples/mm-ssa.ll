MemorySSA for function: sum

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @sum(i32* %arg, i32* %arg1, i32* %arg2, i32 %arg3) #0 {
bb:
  %tmp = alloca i32*, align 8
  %tmp4 = alloca i32*, align 8
  %tmp5 = alloca i32*, align 8
  %tmp6 = alloca i32, align 4
  %tmp7 = alloca i32, align 4
; 1 = MemoryDef(liveOnEntry)
  store i32* %arg, i32** %tmp, align 8
; 2 = MemoryDef(1)
  store i32* %arg1, i32** %tmp4, align 8
; 3 = MemoryDef(2)
  store i32* %arg2, i32** %tmp5, align 8
; 4 = MemoryDef(3)
  store i32 %arg3, i32* %tmp6, align 4
; 5 = MemoryDef(4)
  store i32 0, i32* %tmp7, align 4
  br label %bb8

bb8:                                              ; preds = %bb28, %bb
; 8 = MemoryPhi({bb,5},{bb28,7})
; MemoryUse(8)
  %tmp9 = load i32, i32* %tmp7, align 4
; MemoryUse(8)
  %tmp10 = load i32, i32* %tmp6, align 4
  %tmp11 = icmp slt i32 %tmp9, %tmp10
  br i1 %tmp11, label %bb12, label %bb31

bb12:                                             ; preds = %bb8
; MemoryUse(8)
  %tmp13 = load i32*, i32** %tmp, align 8
; MemoryUse(8)
  %tmp14 = load i32, i32* %tmp7, align 4
  %tmp15 = sext i32 %tmp14 to i64
  %tmp16 = getelementptr inbounds i32, i32* %tmp13, i64 %tmp15
; MemoryUse(8)
  %tmp17 = load i32, i32* %tmp16, align 4
; MemoryUse(8)
  %tmp18 = load i32*, i32** %tmp4, align 8
; MemoryUse(8)
  %tmp19 = load i32, i32* %tmp7, align 4
  %tmp20 = sext i32 %tmp19 to i64
  %tmp21 = getelementptr inbounds i32, i32* %tmp18, i64 %tmp20
; MemoryUse(8)
  %tmp22 = load i32, i32* %tmp21, align 4
  %tmp23 = add nsw i32 %tmp17, %tmp22
; MemoryUse(8)
  %tmp24 = load i32*, i32** %tmp5, align 8
; MemoryUse(8)
  %tmp25 = load i32, i32* %tmp7, align 4
  %tmp26 = sext i32 %tmp25 to i64
  %tmp27 = getelementptr inbounds i32, i32* %tmp24, i64 %tmp26
; 6 = MemoryDef(8)
  store i32 %tmp23, i32* %tmp27, align 4
  br label %bb28

bb28:                                             ; preds = %bb12
; MemoryUse(6)
  %tmp29 = load i32, i32* %tmp7, align 4
  %tmp30 = add nsw i32 %tmp29, 1
; 7 = MemoryDef(6)
  store i32 %tmp30, i32* %tmp7, align 4
  br label %bb8

bb31:                                             ; preds = %bb8
  ret void
}
