; ModuleID = 'example.c'
source_filename = "example.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1, !dbg !0

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !17 {
entry:
  %retval = alloca i32, align 4
  %a = alloca ptr, align 8
  %b = alloca ptr, align 8
  store i32 0, ptr %retval, align 4
  call void @llvm.dbg.declare(metadata ptr %a, metadata !22, metadata !DIExpression()), !dbg !24
  %call = call noalias ptr @malloc(i64 noundef 4) #5, !dbg !25
  store ptr %call, ptr %a, align 8, !dbg !24
  call void @llvm.dbg.declare(metadata ptr %b, metadata !26, metadata !DIExpression()), !dbg !27
  %call1 = call noalias ptr @malloc(i64 noundef 4) #5, !dbg !28
  store ptr %call1, ptr %b, align 8, !dbg !27
  %call2 = call i32 @rand() #6, !dbg !29
  %rem = srem i32 %call2, 2, !dbg !31
  %tobool = icmp ne i32 %rem, 0, !dbg !31
  br i1 %tobool, label %if.then, label %if.end, !dbg !32

if.then:                                          ; preds = %entry
  %0 = load ptr, ptr %b, align 8, !dbg !33
  store ptr %0, ptr %a, align 8, !dbg !35
  br label %if.end, !dbg !36

if.end:                                           ; preds = %if.then, %entry
  %1 = load ptr, ptr %a, align 8, !dbg !37
  store i32 42, ptr %1, align 4, !dbg !38
  %2 = load ptr, ptr %b, align 8, !dbg !39
  store i32 100, ptr %2, align 4, !dbg !40
  %3 = load ptr, ptr %a, align 8, !dbg !41
  %4 = load i32, ptr %3, align 4, !dbg !42
  %5 = load ptr, ptr %b, align 8, !dbg !43
  %6 = load i32, ptr %5, align 4, !dbg !44
  %call3 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %4, i32 noundef %6), !dbg !45
  ret i32 0, !dbg !46
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #2

; Function Attrs: nounwind
declare i32 @rand() #3

declare i32 @printf(ptr noundef, ...) #4

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind allocsize(0) }
attributes #6 = { nounwind }

!llvm.dbg.cu = !{!7}
!llvm.module.flags = !{!9, !10, !11, !12, !13, !14, !15}
!llvm.ident = !{!16}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(scope: null, file: !2, line: 27, type: !3, isLocal: true, isDefinition: true)
!2 = !DIFile(filename: "example.c", directory: "/work/johan/SVF/example", checksumkind: CSK_MD5, checksum: "993043107f2d81f0bba72c7c5558bc91")
!3 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 56, elements: !5)
!4 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!5 = !{!6}
!6 = !DISubrange(count: 7)
!7 = distinct !DICompileUnit(language: DW_LANG_C11, file: !2, producer: "clang version 16.0.4 (https://github.com/bjjwwang/LLVM-compile a0f79b14ca0360151ea799a9ad1d5932235bbee1)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !8, splitDebugInlining: false, nameTableKind: None)
!8 = !{!0}
!9 = !{i32 7, !"Dwarf Version", i32 5}
!10 = !{i32 2, !"Debug Info Version", i32 3}
!11 = !{i32 1, !"wchar_size", i32 4}
!12 = !{i32 8, !"PIC Level", i32 2}
!13 = !{i32 7, !"PIE Level", i32 2}
!14 = !{i32 7, !"uwtable", i32 2}
!15 = !{i32 7, !"frame-pointer", i32 2}
!16 = !{!"clang version 16.0.4 (https://github.com/bjjwwang/LLVM-compile a0f79b14ca0360151ea799a9ad1d5932235bbee1)"}
!17 = distinct !DISubprogram(name: "main", scope: !2, file: !2, line: 16, type: !18, scopeLine: 16, spFlags: DISPFlagDefinition, unit: !7, retainedNodes: !21)
!18 = !DISubroutineType(types: !19)
!19 = !{!20}
!20 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!21 = !{}
!22 = !DILocalVariable(name: "a", scope: !17, file: !2, line: 17, type: !23)
!23 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !20, size: 64)
!24 = !DILocation(line: 17, column: 10, scope: !17)
!25 = !DILocation(line: 17, column: 14, scope: !17)
!26 = !DILocalVariable(name: "b", scope: !17, file: !2, line: 18, type: !23)
!27 = !DILocation(line: 18, column: 10, scope: !17)
!28 = !DILocation(line: 18, column: 14, scope: !17)
!29 = !DILocation(line: 20, column: 9, scope: !30)
!30 = distinct !DILexicalBlock(scope: !17, file: !2, line: 20, column: 9)
!31 = !DILocation(line: 20, column: 16, scope: !30)
!32 = !DILocation(line: 20, column: 9, scope: !17)
!33 = !DILocation(line: 21, column: 13, scope: !34)
!34 = distinct !DILexicalBlock(scope: !30, file: !2, line: 20, column: 21)
!35 = !DILocation(line: 21, column: 11, scope: !34)
!36 = !DILocation(line: 22, column: 5, scope: !34)
!37 = !DILocation(line: 24, column: 6, scope: !17)
!38 = !DILocation(line: 24, column: 8, scope: !17)
!39 = !DILocation(line: 25, column: 6, scope: !17)
!40 = !DILocation(line: 25, column: 8, scope: !17)
!41 = !DILocation(line: 27, column: 24, scope: !17)
!42 = !DILocation(line: 27, column: 23, scope: !17)
!43 = !DILocation(line: 27, column: 28, scope: !17)
!44 = !DILocation(line: 27, column: 27, scope: !17)
!45 = !DILocation(line: 27, column: 5, scope: !17)
!46 = !DILocation(line: 28, column: 5, scope: !17)
