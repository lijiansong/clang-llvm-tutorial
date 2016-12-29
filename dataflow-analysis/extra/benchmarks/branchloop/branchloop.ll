; ModuleID = 'branchloop.bc'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

; Function Attrs: nounwind
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %retval
  store i32 1, i32* %i, align 4
  store i32 1, i32* %j, align 4
  %0 = load i32* %i, align 4
  %cmp = icmp ne i32 %0, 5
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 0, i32* %i, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  br label %while.cond

while.cond:                                       ; preds = %while.end, %if.end
  %1 = load i32* %i, align 4
  %cmp1 = icmp eq i32 %1, 0
  br i1 %cmp1, label %while.body, label %while.end5

while.body:                                       ; preds = %while.cond
  br label %while.cond2

while.cond2:                                      ; preds = %while.body4, %while.body
  %2 = load i32* %i, align 4
  %cmp3 = icmp eq i32 %2, 0
  br i1 %cmp3, label %while.body4, label %while.end

while.body4:                                      ; preds = %while.cond2
  %3 = load i32* %i, align 4
  %add = add nsw i32 %3, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond2

while.end:                                        ; preds = %while.cond2
  br label %while.cond

while.end5:                                       ; preds = %while.cond
  %4 = load i32* %i, align 4
  ret i32 %4
}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
