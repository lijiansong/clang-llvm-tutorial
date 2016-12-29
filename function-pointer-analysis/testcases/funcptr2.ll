; ModuleID = 'funcptr2.bc'
source_filename = "funcptr2.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @add(i32 %a, i32 %b) #0 !dbg !6 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %a.addr, metadata !10, metadata !11), !dbg !12
  store i32 %b, i32* %b.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %b.addr, metadata !13, metadata !11), !dbg !14
  %0 = load i32, i32* %a.addr, align 4, !dbg !15
  %1 = load i32, i32* %b.addr, align 4, !dbg !16
  %add = add nsw i32 %0, %1, !dbg !17
  ret i32 %add, !dbg !18
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind uwtable
define i32 @sub(i32 %a, i32 %b) #0 !dbg !19 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %a.addr, metadata !20, metadata !11), !dbg !21
  store i32 %b, i32* %b.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %b.addr, metadata !22, metadata !11), !dbg !23
  %0 = load i32, i32* %a.addr, align 4, !dbg !24
  %1 = load i32, i32* %b.addr, align 4, !dbg !25
  %sub = sub nsw i32 %0, %1, !dbg !26
  ret i32 %sub, !dbg !27
}

; Function Attrs: nounwind uwtable
define i32 @foo(i32 %a, i32 %b, i32 (i32, i32)* %a_fptr) #0 !dbg !28 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %a_fptr.addr = alloca i32 (i32, i32)*, align 8
  store i32 %a, i32* %a.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %a.addr, metadata !32, metadata !11), !dbg !33
  store i32 %b, i32* %b.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %b.addr, metadata !34, metadata !11), !dbg !35
  store i32 (i32, i32)* %a_fptr, i32 (i32, i32)** %a_fptr.addr, align 8
  call void @llvm.dbg.declare(metadata i32 (i32, i32)** %a_fptr.addr, metadata !36, metadata !11), !dbg !37
  %0 = load i32 (i32, i32)*, i32 (i32, i32)** %a_fptr.addr, align 8, !dbg !38
  %1 = load i32, i32* %a.addr, align 4, !dbg !39
  %2 = load i32, i32* %b.addr, align 4, !dbg !40
  %call = call i32 %0(i32 %1, i32 %2), !dbg !38
  ret i32 %call, !dbg !41
}

; Function Attrs: nounwind uwtable
define i32 @moo(i8 signext %x, i32 %op1, i32 %op2) #0 !dbg !42 {
entry:
  %x.addr = alloca i8, align 1
  %op1.addr = alloca i32, align 4
  %op2.addr = alloca i32, align 4
  %a_fptr = alloca i32 (i32, i32)*, align 8
  %s_fptr = alloca i32 (i32, i32)*, align 8
  %t_fptr = alloca i32 (i32, i32)*, align 8
  %result = alloca i32, align 4
  store i8 %x, i8* %x.addr, align 1
  call void @llvm.dbg.declare(metadata i8* %x.addr, metadata !46, metadata !11), !dbg !47
  store i32 %op1, i32* %op1.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %op1.addr, metadata !48, metadata !11), !dbg !49
  store i32 %op2, i32* %op2.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %op2.addr, metadata !50, metadata !11), !dbg !51
  call void @llvm.dbg.declare(metadata i32 (i32, i32)** %a_fptr, metadata !52, metadata !11), !dbg !53
  store i32 (i32, i32)* @add, i32 (i32, i32)** %a_fptr, align 8, !dbg !53
  call void @llvm.dbg.declare(metadata i32 (i32, i32)** %s_fptr, metadata !54, metadata !11), !dbg !55
  store i32 (i32, i32)* @sub, i32 (i32, i32)** %s_fptr, align 8, !dbg !55
  call void @llvm.dbg.declare(metadata i32 (i32, i32)** %t_fptr, metadata !56, metadata !11), !dbg !57
  store i32 (i32, i32)* null, i32 (i32, i32)** %t_fptr, align 8, !dbg !57
  %0 = load i8, i8* %x.addr, align 1, !dbg !58
  %conv = sext i8 %0 to i32, !dbg !58
  %cmp = icmp eq i32 %conv, 43, !dbg !60
  br i1 %cmp, label %if.then, label %if.end, !dbg !61

if.then:                                          ; preds = %entry
  %1 = load i32 (i32, i32)*, i32 (i32, i32)** %a_fptr, align 8, !dbg !62
  store i32 (i32, i32)* %1, i32 (i32, i32)** %t_fptr, align 8, !dbg !64
  br label %if.end, !dbg !65

if.end:                                           ; preds = %if.then, %entry
  %2 = load i8, i8* %x.addr, align 1, !dbg !66
  %conv2 = sext i8 %2 to i32, !dbg !66
  %cmp3 = icmp eq i32 %conv2, 45, !dbg !68
  br i1 %cmp3, label %if.then5, label %if.end6, !dbg !69

if.then5:                                         ; preds = %if.end
  %3 = load i32 (i32, i32)*, i32 (i32, i32)** %s_fptr, align 8, !dbg !70
  store i32 (i32, i32)* %3, i32 (i32, i32)** %t_fptr, align 8, !dbg !72
  br label %if.end6, !dbg !73

if.end6:                                          ; preds = %if.then5, %if.end
  call void @llvm.dbg.declare(metadata i32* %result, metadata !74, metadata !11), !dbg !76
  %4 = load i32, i32* %op1.addr, align 4, !dbg !77
  %5 = load i32, i32* %op2.addr, align 4, !dbg !78
  %6 = load i32 (i32, i32)*, i32 (i32, i32)** %t_fptr, align 8, !dbg !79
  %call = call i32 @foo(i32 %4, i32 %5, i32 (i32, i32)* %6), !dbg !80
  store i32 %call, i32* %result, align 4, !dbg !76
  ret i32 0, !dbg !81
}

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4}
!llvm.ident = !{!5}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 4.0.0 (trunk 280809) (llvm/trunk 280808)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "funcptr2.c", directory: "/home/json-lee/workdir/compile/llvm/homework/assignment2/llvm-assignment/assign2-testcases-new")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{!"clang version 4.0.0 (trunk 280809) (llvm/trunk 280808)"}
!6 = distinct !DISubprogram(name: "add", scope: !1, file: !1, line: 1, type: !7, isLocal: false, isDefinition: true, scopeLine: 1, flags: DIFlagPrototyped, isOptimized: false, unit: !0, variables: !2)
!7 = !DISubroutineType(types: !8)
!8 = !{!9, !9, !9}
!9 = !DIBasicType(name: "int", size: 32, align: 32, encoding: DW_ATE_signed)
!10 = !DILocalVariable(name: "a", arg: 1, scope: !6, file: !1, line: 1, type: !9)
!11 = !DIExpression()
!12 = !DILocation(line: 1, column: 13, scope: !6)
!13 = !DILocalVariable(name: "b", arg: 2, scope: !6, file: !1, line: 1, type: !9)
!14 = !DILocation(line: 1, column: 20, scope: !6)
!15 = !DILocation(line: 2, column: 11, scope: !6)
!16 = !DILocation(line: 2, column: 13, scope: !6)
!17 = !DILocation(line: 2, column: 12, scope: !6)
!18 = !DILocation(line: 2, column: 4, scope: !6)
!19 = distinct !DISubprogram(name: "sub", scope: !1, file: !1, line: 5, type: !7, isLocal: false, isDefinition: true, scopeLine: 5, flags: DIFlagPrototyped, isOptimized: false, unit: !0, variables: !2)
!20 = !DILocalVariable(name: "a", arg: 1, scope: !19, file: !1, line: 5, type: !9)
!21 = !DILocation(line: 5, column: 13, scope: !19)
!22 = !DILocalVariable(name: "b", arg: 2, scope: !19, file: !1, line: 5, type: !9)
!23 = !DILocation(line: 5, column: 20, scope: !19)
!24 = !DILocation(line: 6, column: 11, scope: !19)
!25 = !DILocation(line: 6, column: 13, scope: !19)
!26 = !DILocation(line: 6, column: 12, scope: !19)
!27 = !DILocation(line: 6, column: 4, scope: !19)
!28 = distinct !DISubprogram(name: "foo", scope: !1, file: !1, line: 9, type: !29, isLocal: false, isDefinition: true, scopeLine: 9, flags: DIFlagPrototyped, isOptimized: false, unit: !0, variables: !2)
!29 = !DISubroutineType(types: !30)
!30 = !{!9, !9, !9, !31}
!31 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 64, align: 64)
!32 = !DILocalVariable(name: "a", arg: 1, scope: !28, file: !1, line: 9, type: !9)
!33 = !DILocation(line: 9, column: 13, scope: !28)
!34 = !DILocalVariable(name: "b", arg: 2, scope: !28, file: !1, line: 9, type: !9)
!35 = !DILocation(line: 9, column: 20, scope: !28)
!36 = !DILocalVariable(name: "a_fptr", arg: 3, scope: !28, file: !1, line: 9, type: !31)
!37 = !DILocation(line: 9, column: 29, scope: !28)
!38 = !DILocation(line: 10, column: 12, scope: !28)
!39 = !DILocation(line: 10, column: 19, scope: !28)
!40 = !DILocation(line: 10, column: 22, scope: !28)
!41 = !DILocation(line: 10, column: 5, scope: !28)
!42 = distinct !DISubprogram(name: "moo", scope: !1, file: !1, line: 14, type: !43, isLocal: false, isDefinition: true, scopeLine: 14, flags: DIFlagPrototyped, isOptimized: false, unit: !0, variables: !2)
!43 = !DISubroutineType(types: !44)
!44 = !{!9, !45, !9, !9}
!45 = !DIBasicType(name: "char", size: 8, align: 8, encoding: DW_ATE_signed_char)
!46 = !DILocalVariable(name: "x", arg: 1, scope: !42, file: !1, line: 14, type: !45)
!47 = !DILocation(line: 14, column: 14, scope: !42)
!48 = !DILocalVariable(name: "op1", arg: 2, scope: !42, file: !1, line: 14, type: !9)
!49 = !DILocation(line: 14, column: 21, scope: !42)
!50 = !DILocalVariable(name: "op2", arg: 3, scope: !42, file: !1, line: 14, type: !9)
!51 = !DILocation(line: 14, column: 30, scope: !42)
!52 = !DILocalVariable(name: "a_fptr", scope: !42, file: !1, line: 15, type: !31)
!53 = !DILocation(line: 15, column: 11, scope: !42)
!54 = !DILocalVariable(name: "s_fptr", scope: !42, file: !1, line: 16, type: !31)
!55 = !DILocation(line: 16, column: 11, scope: !42)
!56 = !DILocalVariable(name: "t_fptr", scope: !42, file: !1, line: 17, type: !31)
!57 = !DILocation(line: 17, column: 11, scope: !42)
!58 = !DILocation(line: 19, column: 9, scope: !59)
!59 = distinct !DILexicalBlock(scope: !42, file: !1, line: 19, column: 9)
!60 = !DILocation(line: 19, column: 11, scope: !59)
!61 = !DILocation(line: 19, column: 9, scope: !42)
!62 = !DILocation(line: 20, column: 17, scope: !63)
!63 = distinct !DILexicalBlock(scope: !59, file: !1, line: 19, column: 19)
!64 = !DILocation(line: 20, column: 15, scope: !63)
!65 = !DILocation(line: 21, column: 5, scope: !63)
!66 = !DILocation(line: 22, column: 9, scope: !67)
!67 = distinct !DILexicalBlock(scope: !42, file: !1, line: 22, column: 9)
!68 = !DILocation(line: 22, column: 11, scope: !67)
!69 = !DILocation(line: 22, column: 9, scope: !42)
!70 = !DILocation(line: 23, column: 17, scope: !71)
!71 = distinct !DILexicalBlock(scope: !67, file: !1, line: 22, column: 19)
!72 = !DILocation(line: 23, column: 15, scope: !71)
!73 = !DILocation(line: 24, column: 5, scope: !71)
!74 = !DILocalVariable(name: "result", scope: !42, file: !1, line: 26, type: !75)
!75 = !DIBasicType(name: "unsigned int", size: 32, align: 32, encoding: DW_ATE_unsigned)
!76 = !DILocation(line: 26, column: 14, scope: !42)
!77 = !DILocation(line: 26, column: 27, scope: !42)
!78 = !DILocation(line: 26, column: 32, scope: !42)
!79 = !DILocation(line: 26, column: 37, scope: !42)
!80 = !DILocation(line: 26, column: 23, scope: !42)
!81 = !DILocation(line: 27, column: 5, scope: !42)
