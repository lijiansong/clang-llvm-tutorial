; ModuleID = 'must-pointer.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %i = alloca i32*, align 8
  %j = alloca i32, align 4
  %k = alloca i32*, align 8
  store i32 0, i32* %retval
  %0 = load i32** %i, align 8
  %cmp = icmp ne i32* %0, %j
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = load i32** %i, align 8
  %add.ptr = getelementptr inbounds i32* %1, i64 4
  store i32* %add.ptr, i32** %k, align 8
  br label %if.end

if.else:                                          ; preds = %entry
  %2 = load i32** %i, align 8
  %add.ptr1 = getelementptr inbounds i32* %2, i64 8
  store i32* %add.ptr1, i32** %k, align 8
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %3 = load i32** %i, align 8
  %4 = load i32* %3, align 4
  ret i32 %4
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
