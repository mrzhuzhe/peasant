; ModuleID = 'simd.c'
source_filename = "simd.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"%f\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 10000, ptr %2, align 4
  %5 = call ptr @llvm.stacksave.p0()
  store ptr %5, ptr %3, align 8
  %6 = alloca float, i64 10000, align 16
  store i32 0, ptr %4, align 4
  br label %7

7:                                                ; preds = %17, %0
  %8 = load i32, ptr %4, align 4
  %9 = icmp slt i32 %8, 10000
  br i1 %9, label %10, label %20

10:                                               ; preds = %7
  %11 = load i32, ptr %4, align 4
  %12 = sitofp i32 %11 to float
  %13 = fadd float %12, 1.000000e+00
  %14 = load i32, ptr %4, align 4
  %15 = sext i32 %14 to i64
  %16 = getelementptr inbounds float, ptr %6, i64 %15
  store float %13, ptr %16, align 4
  br label %17

17:                                               ; preds = %10
  %18 = load i32, ptr %4, align 4
  %19 = add nsw i32 %18, 1
  store i32 %19, ptr %4, align 4
  br label %7, !llvm.loop !6

20:                                               ; preds = %7
  %21 = getelementptr inbounds float, ptr %6, i64 3
  %22 = load float, ptr %21, align 4
  %23 = fpext float %22 to double
  %24 = call i32 (ptr, ...) @printf(ptr noundef @.str, double noundef %23)
  store i32 0, ptr %1, align 4
  %25 = load ptr, ptr %3, align 8
  call void @llvm.stackrestore.p0(ptr %25)
  %26 = load i32, ptr %1, align 4
  ret i32 %26
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare ptr @llvm.stacksave.p0() #1

declare i32 @printf(ptr noundef, ...) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.stackrestore.p0(ptr) #1

attributes #0 = { nofree nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 22.0.0git (https://github.com/llvm/llvm-project.git 08c1e9e80a3dcda4112751db0d8bd8828e1dc6bb)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
