// RUN: %clang_cc1 -triple x86_64-linux-gnu -fexperimental-overflow-behavior-types -emit-llvm -o - %s | FileCheck %s --check-prefix=X86_64
// RUN: %clang_cc1 -triple x86_64-windows-msvc -fexperimental-overflow-behavior-types -emit-llvm -o - %s | FileCheck %s --check-prefix=WIN64
// RUN: %clang_cc1 -triple i386-apple-darwin9 -fexperimental-overflow-behavior-types -emit-llvm -o - %s | FileCheck %s --check-prefix=I386

// Overflow behavior types should be passed and returned exactly like their
// underlying types.

struct pair {
  unsigned __ob_wrap x;
  unsigned __ob_wrap y;
};

// X86_64-LABEL: define{{.*}} i32 @pass_pair(i64 %p.coerce)
// WIN64-LABEL: define{{.*}} i32 @pass_pair(i64 %p.coerce)
// I386-LABEL: define{{.*}} i32 @pass_pair(i32 %p.0, i32 %p.1)
unsigned pass_pair(struct pair p) { return p.x; }

// X86_64-LABEL: define{{.*}} i64 @ret_pair()
// WIN64-LABEL: define{{.*}} i64 @ret_pair()
// I386-LABEL: define{{.*}} i64 @ret_pair()
struct pair ret_pair(void) {
  struct pair p = {1, 2};
  return p;
}

struct single {
  int __ob_trap x;
};

// X86_64-LABEL: define{{.*}} i32 @ret_single()
// WIN64-LABEL: define{{.*}} i32 @ret_single()
// I386-LABEL: define{{.*}} i32 @ret_single()
struct single ret_single(void) {
  struct single s = {1};
  return s;
}

struct mixed {
  long long __ob_wrap a;
  double b;
};

// X86_64-LABEL: define{{.*}} double @pass_mixed(i64 %m.coerce0, double %m.coerce1)
// WIN64-LABEL: define{{.*}} double @pass_mixed(ptr {{.*}}dereferenceable(16) %m)
// I386-LABEL: define{{.*}} double @pass_mixed(i64 %m.0, double %m.1)
double pass_mixed(struct mixed m) { return m.b; }

// X86_64-LABEL: define{{.*}} signext i8 @pass_bitint8(i8 noundef signext %x)
// WIN64-LABEL: define{{.*}} i8 @pass_bitint8(i8 noundef %x)
// I386-LABEL: define{{.*}} signext i8 @pass_bitint8(i8 noundef signext %x)
_BitInt(8) __ob_wrap pass_bitint8(_BitInt(8) __ob_wrap x) { return x; }

// X86_64-LABEL: define{{.*}} void @pass_bitint200(ptr {{.*}}sret({{.*}}) align 8 %agg.result, ptr noundef byval({{.*}}) align 8 %0)
// WIN64-LABEL: define{{.*}} void @pass_bitint200(ptr {{.*}}sret({{.*}}) align 8 %agg.result, ptr {{.*}}dereferenceable(32) %0)
// I386-LABEL: define{{.*}} void @pass_bitint200(ptr {{.*}}sret({{.*}}) align 4 %agg.result, ptr {{.*}}dereferenceable(28) %0)
_BitInt(200) __ob_wrap pass_bitint200(_BitInt(200) __ob_wrap x) { return x; }

#ifdef __x86_64__
struct int128_struct {
  __int128 __ob_wrap a;
};

// X86_64-LABEL: define{{.*}} i128 @pass_int128_struct(i128 %s.coerce)
// WIN64-LABEL: define{{.*}} <2 x i64> @pass_int128_struct(ptr {{.*}}dereferenceable(16) %s)
__int128 pass_int128_struct(struct int128_struct s) { return s.a; }

// X86_64-LABEL: define{{.*}} i128 @pass_int128(i128 noundef %x)
// WIN64-LABEL: define{{.*}} <2 x i64> @pass_int128(ptr {{.*}}dereferenceable(16) %0)
__int128 __ob_wrap pass_int128(__int128 __ob_wrap x) { return x; }
#endif
