; ModuleID = 'benchmark/random.ll'
source_filename = "random.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.13.0"

@"keys global" = internal constant [3 x i32] [i32 2, i32 29, i32 31]
@"values global" = internal constant [3 x i32] [i32 1, i32 5, i32 5]
@"keys global.1" = internal constant [3 x i32] [i32 2, i32 30, i32 51]
@"values global.2" = internal constant [3 x i32] [i32 1, i32 2, i32 1]
@"keys global.3" = internal constant [6 x i32] [i32 2, i32 11, i32 30, i32 31, i32 32, i32 38]
@"values global.4" = internal constant [6 x i32] [i32 1, i32 1, i32 8, i32 1, i32 3, i32 3]
@"keys global.5" = internal constant [4 x i32] [i32 2, i32 11, i32 30, i32 31]
@"values global.6" = internal constant [4 x i32] [i32 1, i32 1, i32 1, i32 1]
@"keys global.7" = internal constant [1 x i32] [i32 1]
@"values global.8" = internal constant [1 x i32] [i32 1]
@"keys global.9" = internal constant [10 x i32] [i32 2, i32 15, i32 21, i32 29, i32 30, i32 31, i32 36, i32 38, i32 47, i32 54]
@"values global.10" = internal constant [10 x i32] [i32 1, i32 3, i32 1, i32 6, i32 3, i32 6, i32 1, i32 3, i32 3, i32 6]
@"keys global.11" = internal constant [3 x i32] [i32 2, i32 30, i32 51]
@"values global.12" = internal constant [3 x i32] [i32 1, i32 2, i32 1]
@"keys global.13" = internal constant [6 x i32] [i32 2, i32 11, i32 30, i32 31, i32 32, i32 38]
@"values global.14" = internal constant [6 x i32] [i32 1, i32 2, i32 9, i32 3, i32 3, i32 3]
@"keys global.15" = internal constant [4 x i32] [i32 2, i32 11, i32 30, i32 31]
@"values global.16" = internal constant [4 x i32] [i32 1, i32 1, i32 1, i32 1]
@"keys global.17" = internal constant [3 x i32] [i32 1, i32 30, i32 54]
@"values global.18" = internal constant [3 x i32] [i32 1, i32 4, i32 1]

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @__sv_add(i32*, i32*, i32*, i32) #0 {
  %5 = alloca i32*, align 8
  %6 = alloca i32*, align 8
  %7 = alloca i32*, align 8
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store i32* %0, i32** %5, align 8
  store i32* %1, i32** %6, align 8
  store i32* %2, i32** %7, align 8
  store i32 %3, i32* %8, align 4
  store i32 0, i32* %9, align 4
  call void @updateInstrInfo(i32 3, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @"keys global", i32 0, i32 0), i32* getelementptr inbounds ([3 x i32], [3 x i32]* @"values global", i32 0, i32 0))
  br label %10

; <label>:10:                                     ; preds = %30, %4
  %11 = load i32, i32* %9, align 4
  %12 = load i32, i32* %8, align 4
  %13 = icmp slt i32 %11, %12
  call void @updateInstrInfo(i32 3, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @"keys global.1", i32 0, i32 0), i32* getelementptr inbounds ([3 x i32], [3 x i32]* @"values global.2", i32 0, i32 0))
  br i1 %13, label %14, label %33

; <label>:14:                                     ; preds = %10
  %15 = load i32*, i32** %6, align 8
  %16 = load i32, i32* %9, align 4
  %17 = sext i32 %16 to i64
  %18 = getelementptr inbounds i32, i32* %15, i64 %17
  %19 = load i32, i32* %18, align 4
  %20 = load i32*, i32** %7, align 8
  %21 = load i32, i32* %9, align 4
  %22 = sext i32 %21 to i64
  %23 = getelementptr inbounds i32, i32* %20, i64 %22
  %24 = load i32, i32* %23, align 4
  %25 = add nsw i32 %19, %24
  %26 = load i32*, i32** %5, align 8
  %27 = load i32, i32* %9, align 4
  %28 = sext i32 %27 to i64
  %29 = getelementptr inbounds i32, i32* %26, i64 %28
  store i32 %25, i32* %29, align 4
  call void @updateInstrInfo(i32 6, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @"keys global.3", i32 0, i32 0), i32* getelementptr inbounds ([6 x i32], [6 x i32]* @"values global.4", i32 0, i32 0))
  br label %30

; <label>:30:                                     ; preds = %14
  %31 = load i32, i32* %9, align 4
  %32 = add nsw i32 %31, 1
  store i32 %32, i32* %9, align 4
  call void @updateInstrInfo(i32 4, i32* getelementptr inbounds ([4 x i32], [4 x i32]* @"keys global.5", i32 0, i32 0), i32* getelementptr inbounds ([4 x i32], [4 x i32]* @"values global.6", i32 0, i32 0))
  br label %10

; <label>:33:                                     ; preds = %10
  call void @updateInstrInfo(i32 1, i32* getelementptr inbounds ([1 x i32], [1 x i32]* @"keys global.7", i32 0, i32 0), i32* getelementptr inbounds ([1 x i32], [1 x i32]* @"values global.8", i32 0, i32 0))
  call void @printOutInstrInfo()
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32*, align 8
  %4 = alloca i32*, align 8
  %5 = alloca i32*, align 8
  %6 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %7 = call i64 @time(i64* null)
  %8 = trunc i64 %7 to i32
  call void @srand(i32 %8)
  %9 = call i32 @rand()
  %10 = srem i32 %9, 1024
  store i32 %10, i32* %2, align 4
  %11 = load i32, i32* %2, align 4
  %12 = sext i32 %11 to i64
  %13 = mul i64 %12, 4
  %14 = call i8* @malloc(i64 %13) #3
  %15 = bitcast i8* %14 to i32*
  store i32* %15, i32** %3, align 8
  %16 = load i32, i32* %2, align 4
  %17 = sext i32 %16 to i64
  %18 = mul i64 %17, 4
  %19 = call i8* @malloc(i64 %18) #3
  %20 = bitcast i8* %19 to i32*
  store i32* %20, i32** %4, align 8
  %21 = load i32, i32* %2, align 4
  %22 = sext i32 %21 to i64
  %23 = mul i64 %22, 4
  %24 = call i8* @malloc(i64 %23) #3
  %25 = bitcast i8* %24 to i32*
  store i32* %25, i32** %5, align 8
  store i32 0, i32* %6, align 4
  call void @updateInstrInfo(i32 10, i32* getelementptr inbounds ([10 x i32], [10 x i32]* @"keys global.9", i32 0, i32 0), i32* getelementptr inbounds ([10 x i32], [10 x i32]* @"values global.10", i32 0, i32 0))
  br label %26

; <label>:26:                                     ; preds = %48, %0
  %27 = load i32, i32* %6, align 4
  %28 = load i32, i32* %2, align 4
  %29 = icmp slt i32 %27, %28
  call void @updateInstrInfo(i32 3, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @"keys global.11", i32 0, i32 0), i32* getelementptr inbounds ([3 x i32], [3 x i32]* @"values global.12", i32 0, i32 0))
  br i1 %29, label %30, label %51

; <label>:30:                                     ; preds = %26
  %31 = load i32, i32* %6, align 4
  %32 = load i32*, i32** %3, align 8
  %33 = load i32, i32* %6, align 4
  %34 = sext i32 %33 to i64
  %35 = getelementptr inbounds i32, i32* %32, i64 %34
  store i32 %31, i32* %35, align 4
  %36 = load i32, i32* %6, align 4
  %37 = add nsw i32 %36, 1
  %38 = load i32*, i32** %4, align 8
  %39 = load i32, i32* %6, align 4
  %40 = sext i32 %39 to i64
  %41 = getelementptr inbounds i32, i32* %38, i64 %40
  store i32 %37, i32* %41, align 4
  %42 = load i32, i32* %6, align 4
  %43 = add nsw i32 %42, 2
  %44 = load i32*, i32** %5, align 8
  %45 = load i32, i32* %6, align 4
  %46 = sext i32 %45 to i64
  %47 = getelementptr inbounds i32, i32* %44, i64 %46
  store i32 %43, i32* %47, align 4
  call void @updateInstrInfo(i32 6, i32* getelementptr inbounds ([6 x i32], [6 x i32]* @"keys global.13", i32 0, i32 0), i32* getelementptr inbounds ([6 x i32], [6 x i32]* @"values global.14", i32 0, i32 0))
  br label %48

; <label>:48:                                     ; preds = %30
  %49 = load i32, i32* %6, align 4
  %50 = add nsw i32 %49, 1
  store i32 %50, i32* %6, align 4
  call void @updateInstrInfo(i32 4, i32* getelementptr inbounds ([4 x i32], [4 x i32]* @"keys global.15", i32 0, i32 0), i32* getelementptr inbounds ([4 x i32], [4 x i32]* @"values global.16", i32 0, i32 0))
  br label %26

; <label>:51:                                     ; preds = %26
  %52 = load i32*, i32** %5, align 8
  %53 = load i32*, i32** %3, align 8
  %54 = load i32*, i32** %4, align 8
  %55 = load i32, i32* %2, align 4
  call void @__sv_add(i32* %52, i32* %53, i32* %54, i32 %55)
  call void @updateInstrInfo(i32 3, i32* getelementptr inbounds ([3 x i32], [3 x i32]* @"keys global.17", i32 0, i32 0), i32* getelementptr inbounds ([3 x i32], [3 x i32]* @"values global.18", i32 0, i32 0))
  call void @printOutInstrInfo()
  ret i32 0
}

declare void @srand(i32) #1

declare i64 @time(i64*) #1

declare i32 @rand() #1

; Function Attrs: allocsize(0)
declare i8* @malloc(i64) #2

declare void @updateInstrInfo(i32, i32*, i32*)

declare void @printOutInstrInfo()

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { allocsize(0) "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { allocsize(0) }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
