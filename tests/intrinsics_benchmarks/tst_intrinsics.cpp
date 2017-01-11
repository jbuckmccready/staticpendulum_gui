#include "immintrin.h"
#include "xmmintrin.h"
#include <QDebug>
#include <QString>
#include <QtTest>
#include <cmath>
#include <iostream>

const std::size_t DOUBLE_COUNT = 10'000'000;

class IntrinsicsBenchmarks : public QObject {
  Q_OBJECT

public:
  IntrinsicsBenchmarks();

private Q_SLOTS:
  void double_test();
  void sse_double_test();
};

IntrinsicsBenchmarks::IntrinsicsBenchmarks() {}

void IntrinsicsBenchmarks::double_test() {
  double *a = static_cast<double *>(malloc(sizeof(double) * DOUBLE_COUNT));
  double *b = static_cast<double *>(malloc(sizeof(double) * DOUBLE_COUNT));
  double *c = static_cast<double *>(malloc(sizeof(double) * DOUBLE_COUNT));
  for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
    a[i] = i + 1;
  }
  for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
    b[i] = i + 1;
  }
  QBENCHMARK {
    for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
      c[i] = std::sqrt(std::sqrt(a[i])) * b[i] * b[i] * b[i];
    }
  }
  free(a);
  free(b);
  free(c);
}

void IntrinsicsBenchmarks::sse_double_test() {
  auto createAlignedBuf = [] {
    return _mm_malloc(sizeof(double) * DOUBLE_COUNT, 16);
  };
  double *const a = static_cast<double *>(createAlignedBuf());
  __m128d *const mm_a = reinterpret_cast<__m128d *>(a);

  double *const b = static_cast<double *>(createAlignedBuf());
  __m128d *const mm_b = reinterpret_cast<__m128d *>(b);

  double *const c = static_cast<double *>(createAlignedBuf());
  __m128d *const mm_c = reinterpret_cast<__m128d *>(c);

  for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
    a[i] = i + 1;
  }
  for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
    b[i] = i + 1;
  }
  const std::size_t mm_iters = DOUBLE_COUNT / 2;
  QBENCHMARK {
    __m128d *mm_a_ptr = mm_a;
    __m128d *mm_b_ptr = mm_b;
    double *c_ptr = c;
    __m128d *mm_c_ptr = mm_c;
    for (std::size_t i = 0; i < mm_iters;
         ++i, c_ptr += 2, ++mm_a_ptr, ++mm_b_ptr, ++mm_c_ptr) {
      _mm_store_pd(c_ptr, _mm_sqrt_pd(*mm_a_ptr));
      _mm_store_pd(c_ptr, _mm_sqrt_pd(*mm_c_ptr));
      _mm_store_pd(c_ptr, _mm_mul_pd(*mm_b_ptr, *mm_c_ptr));
      _mm_store_pd(c_ptr, _mm_mul_pd(*mm_b_ptr, *mm_c_ptr));
      _mm_store_pd(c_ptr, _mm_mul_pd(*mm_b_ptr, *mm_c_ptr));
    }
  }
  _mm_free(a);
  _mm_free(b);
  _mm_free(c);
}

QTEST_APPLESS_MAIN(IntrinsicsBenchmarks)

#include "tst_intrinsics.moc"
