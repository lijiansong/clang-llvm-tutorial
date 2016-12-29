; ModuleID = 'funcptr0.c'
source_filename = "funcptr0.c"
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
define i32 @foo() #0 !dbg !19 {
entry:
  %a_fptr = alloca i32 (i32, i32)*, align 8
  %op1 = alloca i32, align 4
  %op2 = alloca i32, align 4
  %result = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32 (i32, i32)** %a_fptr, metadata !22, metadata !11), !dbg !24
  store i32 (i32, i32)* @add, i32 (i32, i32)** %a_fptr, align 8, !dbg !24
  call void @llvm.dbg.declare(metadata i32* %op1, metadata !25, metadata !11), !dbg !26
  store i32 1, i32* %op1, align 4, !dbg !26
  call void @llvm.dbg.declare(metadata i32* %op2, metadata !27, metadata !11), !dbg !28
  store i32 2, i32* %op2, align 4, !dbg !28
  call void @llvm.dbg.declare(metadata i32* %result, metadata !29, metadata !11), !dbg !31
  %0 = load i32 (i32, i32)*, i32 (i32, i32)** %a_fptr, align 8, !dbg !32
  %1 = load i32, i32* %op1, align 4, !dbg !33
  %2 = load i32, i32* %op2, align 4, !dbg !34
  %call = call i32 %0(i32 %1, i32 %2), !dbg !32
  store i32 %call, i32* %result, align 4, !dbg !31
  ret i32 0, !dbg !35
}

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4}
!llvm.ident = !{!5}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 4.0.0 (trunk 280809) (llvm/trunk 280808)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "funcptr0.c", directory: "/home/json-lee/workdir/compile/llvm/homework/assignment2/llvm-assignment/assign2-testcases-new")
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
!19 = distinct !DISubprogram(name: "foo", scope: !1, file: !1, line: 5, type: !20, isLocal: false, isDefinition: true, scopeLine: 5, isOptimized: false, unit: !0, variables: !2)
!20 = !DISubroutineType(types: !21)
!21 = !{!9}
!22 = !DILocalVariable(name: "a_fptr", scope: !19, file: !1, line: 6, type: !23)
!23 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 64, align: 64)
!24 = !DILocation(line: 6, column: 11, scope: !19)
!25 = !DILocalVariable(name: "op1", scope: !19, file: !1, line: 8, type: !9)
!26 = !DILocation(line: 8, column: 9, scope: !19)
!27 = !DILocalVariable(name: "op2", scope: !19, file: !1, line: 8, type: !9)
!28 = !DILocation(line: 8, column: 18, scope: !19)
!29 = !DILocalVariable(name: "result", scope: !19, file: !1, line: 10, type: !30)
!30 = !DIBasicType(name: "unsigned int", size: 32, align: 32, encoding: DW_ATE_unsigned)
!31 = !DILocation(line: 10, column: 14, scope: !19)
!32 = !DILocation(line: 10, column: 23, scope: !19)
!33 = !DILocation(line: 10, column: 30, scope: !19)
!34 = !DILocation(line: 10, column: 35, scope: !19)
!35 = !DILocation(line: 11, column: 5, scope: !19)
