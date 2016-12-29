; ModuleID = './assign2-testcases-new/funcptr1.bc'
source_filename = "funcptr1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @add(i32 %a, i32 %b) #0 !dbg !8 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %a.addr, metadata !12, metadata !13), !dbg !14
  store i32 %b, i32* %b.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %b.addr, metadata !15, metadata !13), !dbg !16
  %0 = load i32, i32* %a.addr, align 4, !dbg !17
  %1 = load i32, i32* %b.addr, align 4, !dbg !18
  %add = add nsw i32 %0, %1, !dbg !19
  ret i32 %add, !dbg !20
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind uwtable
define i32 @sub(i32 %a, i32 %b) #0 !dbg !21 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %a.addr, metadata !22, metadata !13), !dbg !23
  store i32 %b, i32* %b.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %b.addr, metadata !24, metadata !13), !dbg !25
  %0 = load i32, i32* %a.addr, align 4, !dbg !26
  %1 = load i32, i32* %b.addr, align 4, !dbg !27
  %sub = sub nsw i32 %0, %1, !dbg !28
  ret i32 %sub, !dbg !29
}

; Function Attrs: nounwind uwtable
define i32 @foo(i32 %x) #0 !dbg !30 {
entry:
  %x.addr = alloca i32, align 4
  %a_fptr = alloca i32 (i32, i32)*, align 8
  %s_fptr = alloca i32 (i32, i32)*, align 8
  %t_fptr = alloca i32 (i32, i32)*, align 8
  %op1 = alloca i32, align 4
  %op2 = alloca i32, align 4
  %result = alloca i32, align 4
  store i32 %x, i32* %x.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %x.addr, metadata !33, metadata !13), !dbg !34
  call void @llvm.dbg.declare(metadata i32 (i32, i32)** %a_fptr, metadata !35, metadata !13), !dbg !37
  store i32 (i32, i32)* @add, i32 (i32, i32)** %a_fptr, align 8, !dbg !37
  call void @llvm.dbg.declare(metadata i32 (i32, i32)** %s_fptr, metadata !38, metadata !13), !dbg !39
  store i32 (i32, i32)* @sub, i32 (i32, i32)** %s_fptr, align 8, !dbg !39
  call void @llvm.dbg.declare(metadata i32 (i32, i32)** %t_fptr, metadata !40, metadata !13), !dbg !41
  store i32 (i32, i32)* null, i32 (i32, i32)** %t_fptr, align 8, !dbg !41
  call void @llvm.dbg.declare(metadata i32* %op1, metadata !42, metadata !13), !dbg !43
  store i32 1, i32* %op1, align 4, !dbg !43
  call void @llvm.dbg.declare(metadata i32* %op2, metadata !44, metadata !13), !dbg !45
  store i32 2, i32* %op2, align 4, !dbg !45
  %0 = load i32, i32* %x.addr, align 4, !dbg !46
  %cmp = icmp eq i32 %0, 3, !dbg !48
  br i1 %cmp, label %if.then, label %if.end, !dbg !49

if.then:                                          ; preds = %entry
  %1 = load i32 (i32, i32)*, i32 (i32, i32)** %a_fptr, align 8, !dbg !50
  store i32 (i32, i32)* %1, i32 (i32, i32)** %t_fptr, align 8, !dbg !52
  br label %if.end, !dbg !53

if.end:                                           ; preds = %if.then, %entry
  %2 = load i32 (i32, i32)*, i32 (i32, i32)** %t_fptr, align 8, !dbg !54
  %cmp1 = icmp ne i32 (i32, i32)* %2, null, !dbg !56
  br i1 %cmp1, label %if.then2, label %if.end3, !dbg !57

if.then2:                                         ; preds = %if.end
  call void @llvm.dbg.declare(metadata i32* %result, metadata !58, metadata !13), !dbg !61
  %3 = load i32 (i32, i32)*, i32 (i32, i32)** %t_fptr, align 8, !dbg !62
  %4 = load i32, i32* %op1, align 4, !dbg !63
  %5 = load i32, i32* %op2, align 4, !dbg !64
  %call = call i32 %3(i32 %4, i32 %5), !dbg !62
  store i32 %call, i32* %result, align 4, !dbg !61
  br label %if.end3, !dbg !65

if.end3:                                          ; preds = %if.then2, %if.end
  ret i32 0, !dbg !66
}

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!5, !6}
!llvm.ident = !{!7}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 4.0.0 (trunk 280809) (llvm/trunk 280808)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3)
!1 = !DIFile(filename: "funcptr1.c", directory: "/home/json-lee/workdir/compile/llvm/homework/assignment2/llvm-assignment/assign2-testcases-new")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64, align: 64)
!5 = !{i32 2, !"Dwarf Version", i32 4}
!6 = !{i32 2, !"Debug Info Version", i32 3}
!7 = !{!"clang version 4.0.0 (trunk 280809) (llvm/trunk 280808)"}
!8 = distinct !DISubprogram(name: "add", scope: !1, file: !1, line: 2, type: !9, isLocal: false, isDefinition: true, scopeLine: 2, flags: DIFlagPrototyped, isOptimized: false, unit: !0, variables: !2)
!9 = !DISubroutineType(types: !10)
!10 = !{!11, !11, !11}
!11 = !DIBasicType(name: "int", size: 32, align: 32, encoding: DW_ATE_signed)
!12 = !DILocalVariable(name: "a", arg: 1, scope: !8, file: !1, line: 2, type: !11)
!13 = !DIExpression()
!14 = !DILocation(line: 2, column: 13, scope: !8)
!15 = !DILocalVariable(name: "b", arg: 2, scope: !8, file: !1, line: 2, type: !11)
!16 = !DILocation(line: 2, column: 20, scope: !8)
!17 = !DILocation(line: 3, column: 11, scope: !8)
!18 = !DILocation(line: 3, column: 13, scope: !8)
!19 = !DILocation(line: 3, column: 12, scope: !8)
!20 = !DILocation(line: 3, column: 4, scope: !8)
!21 = distinct !DISubprogram(name: "sub", scope: !1, file: !1, line: 6, type: !9, isLocal: false, isDefinition: true, scopeLine: 6, flags: DIFlagPrototyped, isOptimized: false, unit: !0, variables: !2)
!22 = !DILocalVariable(name: "a", arg: 1, scope: !21, file: !1, line: 6, type: !11)
!23 = !DILocation(line: 6, column: 13, scope: !21)
!24 = !DILocalVariable(name: "b", arg: 2, scope: !21, file: !1, line: 6, type: !11)
!25 = !DILocation(line: 6, column: 20, scope: !21)
!26 = !DILocation(line: 7, column: 11, scope: !21)
!27 = !DILocation(line: 7, column: 13, scope: !21)
!28 = !DILocation(line: 7, column: 12, scope: !21)
!29 = !DILocation(line: 7, column: 4, scope: !21)
!30 = distinct !DISubprogram(name: "foo", scope: !1, file: !1, line: 10, type: !31, isLocal: false, isDefinition: true, scopeLine: 10, flags: DIFlagPrototyped, isOptimized: false, unit: !0, variables: !2)
!31 = !DISubroutineType(types: !32)
!32 = !{!11, !11}
!33 = !DILocalVariable(name: "x", arg: 1, scope: !30, file: !1, line: 10, type: !11)
!34 = !DILocation(line: 10, column: 13, scope: !30)
!35 = !DILocalVariable(name: "a_fptr", scope: !30, file: !1, line: 11, type: !36)
!36 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !9, size: 64, align: 64)
!37 = !DILocation(line: 11, column: 11, scope: !30)
!38 = !DILocalVariable(name: "s_fptr", scope: !30, file: !1, line: 12, type: !36)
!39 = !DILocation(line: 12, column: 11, scope: !30)
!40 = !DILocalVariable(name: "t_fptr", scope: !30, file: !1, line: 13, type: !36)
!41 = !DILocation(line: 13, column: 11, scope: !30)
!42 = !DILocalVariable(name: "op1", scope: !30, file: !1, line: 15, type: !11)
!43 = !DILocation(line: 15, column: 9, scope: !30)
!44 = !DILocalVariable(name: "op2", scope: !30, file: !1, line: 15, type: !11)
!45 = !DILocation(line: 15, column: 16, scope: !30)
!46 = !DILocation(line: 17, column: 9, scope: !47)
!47 = distinct !DILexicalBlock(scope: !30, file: !1, line: 17, column: 9)
!48 = !DILocation(line: 17, column: 11, scope: !47)
!49 = !DILocation(line: 17, column: 9, scope: !30)
!50 = !DILocation(line: 18, column: 17, scope: !51)
!51 = distinct !DILexicalBlock(scope: !47, file: !1, line: 17, column: 17)
!52 = !DILocation(line: 18, column: 15, scope: !51)
!53 = !DILocation(line: 19, column: 5, scope: !51)
!54 = !DILocation(line: 21, column: 9, scope: !55)
!55 = distinct !DILexicalBlock(scope: !30, file: !1, line: 21, column: 9)
!56 = !DILocation(line: 21, column: 16, scope: !55)
!57 = !DILocation(line: 21, column: 9, scope: !30)
!58 = !DILocalVariable(name: "result", scope: !59, file: !1, line: 22, type: !60)
!59 = distinct !DILexicalBlock(scope: !55, file: !1, line: 21, column: 25)
!60 = !DIBasicType(name: "unsigned int", size: 32, align: 32, encoding: DW_ATE_unsigned)
!61 = !DILocation(line: 22, column: 17, scope: !59)
!62 = !DILocation(line: 22, column: 26, scope: !59)
!63 = !DILocation(line: 22, column: 33, scope: !59)
!64 = !DILocation(line: 22, column: 38, scope: !59)
!65 = !DILocation(line: 23, column: 5, scope: !59)
!66 = !DILocation(line: 24, column: 4, scope: !30)
