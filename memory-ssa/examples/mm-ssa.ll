MemorySSA for function: sum

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @sum(i32*, i32*, i32*, i32) #0 {
  %5 = alloca i32*, align 8
  %6 = alloca i32*, align 8
  %7 = alloca i32*, align 8
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
; 1 = MemoryDef(liveOnEntry)
  store i32* %0, i32** %5, align 8
; 2 = MemoryDef(1)
  store i32* %1, i32** %6, align 8
; 3 = MemoryDef(2)
  store i32* %2, i32** %7, align 8
; 4 = MemoryDef(3)
  store i32 %3, i32* %8, align 4
; 5 = MemoryDef(4)
  store i32 0, i32* %9, align 4
  br label %10

; <label>:10:                                     ; preds = %30, %4
; 8 = MemoryPhi({%4,5},{%30,7})
; MemoryUse(8)
  %11 = load i32, i32* %9, align 4
; MemoryUse(8)
  %12 = load i32, i32* %8, align 4
  %13 = icmp slt i32 %11, %12
  br i1 %13, label %14, label %33

; <label>:14:                                     ; preds = %10
; MemoryUse(8)
  %15 = load i32*, i32** %5, align 8
; MemoryUse(8)
  %16 = load i32, i32* %9, align 4
  %17 = sext i32 %16 to i64
  %18 = getelementptr inbounds i32, i32* %15, i64 %17
; MemoryUse(8)
  %19 = load i32, i32* %18, align 4
; MemoryUse(8)
  %20 = load i32*, i32** %6, align 8
; MemoryUse(8)
  %21 = load i32, i32* %9, align 4
  %22 = sext i32 %21 to i64
  %23 = getelementptr inbounds i32, i32* %20, i64 %22
; MemoryUse(8)
  %24 = load i32, i32* %23, align 4
  %25 = add nsw i32 %19, %24
; MemoryUse(8)
  %26 = load i32*, i32** %7, align 8
; MemoryUse(8)
  %27 = load i32, i32* %9, align 4
  %28 = sext i32 %27 to i64
  %29 = getelementptr inbounds i32, i32* %26, i64 %28
; 6 = MemoryDef(8)
  store i32 %25, i32* %29, align 4
  br label %30

; <label>:30:                                     ; preds = %14
; MemoryUse(6)
  %31 = load i32, i32* %9, align 4
  %32 = add nsw i32 %31, 1
; 7 = MemoryDef(6)
  store i32 %32, i32* %9, align 4
  br label %10

; <label>:33:                                     ; preds = %10
  ret void
}
