; RUN: llc -mtriple=riscv32 -mattr=+zbkb -global-isel -global-isel-abort=1 \
; RUN:   -verify-machineinstrs < %s | FileCheck %s
; RUN: llc -mtriple=riscv64 -mattr=+zbkb -global-isel -global-isel-abort=1 \
; RUN:   -verify-machineinstrs < %s | FileCheck %s

; The combiner erases the dead constant feeding the debug value. The debug
; value must not be left on a generic vreg with no def after selection.

define zeroext i8 @bitreverse_zero() !dbg !5 {
; CHECK-LABEL: bitreverse_zero:
; CHECK: #DEBUG_VALUE: bitreverse_zero:r <- undef
; CHECK: li a0, 0
  %r = call i8 @llvm.bitreverse.i8(i8 0), !dbg !10
    #dbg_value(i8 %r, !8, !DIExpression(), !10)
  ret i8 %r, !dbg !10
}

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "llvm", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2)
!1 = !DIFile(filename: "test.ll", directory: "/")
!2 = !{}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!5 = distinct !DISubprogram(name: "bitreverse_zero", scope: null, file: !1, line: 1, type: !6, scopeLine: 1, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !7)
!6 = !DISubroutineType(types: !2)
!7 = !{!8}
!8 = !DILocalVariable(name: "r", scope: !5, file: !1, line: 1, type: !9)
!9 = !DIBasicType(name: "ty8", size: 8, encoding: DW_ATE_unsigned)
!10 = !DILocation(line: 1, column: 1, scope: !5)
