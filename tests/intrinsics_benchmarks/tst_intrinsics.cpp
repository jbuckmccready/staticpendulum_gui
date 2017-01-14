#include "immintrin.h"
#include "xmmintrin.h"
#include <QDebug>
#include <QString>
#include <QtTest>
#include <cmath>
#include <iostream>

const std::size_t DOUBLE_COUNT = 1'000'000;
const std::size_t INNER_COUNT = 20;

class IntrinsicsBenchmarks : public QObject {
  Q_OBJECT

public:
  IntrinsicsBenchmarks();
  double *a;
  double *b;
  double *c;
  double *a_8;
  double *b_8;
  double *c_8;
  double *a_16;
  double *b_16;
  double *c_16;
  double *a_32;
  double *b_32;
  double *c_32;

private Q_SLOTS:
  void init();
  void cleanup();
  void double_test();
  void double8_test();
  void sse_double_test();
  void avx_double_test();
};

IntrinsicsBenchmarks::IntrinsicsBenchmarks() {}

void IntrinsicsBenchmarks::init() {
  // default aligned
  auto createBuf = [] { return malloc(sizeof(double) * DOUBLE_COUNT); };

  a = static_cast<double *>(createBuf());
  b = static_cast<double *>(createBuf());
  c = static_cast<double *>(createBuf());
  for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
    a[i] = i + 1;
  }
  for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
    b[i] = i + 1;
  }

  // 8 byte aligned
  auto create8AlignedBuf = [] {
    return _mm_malloc(sizeof(double) * DOUBLE_COUNT, 8);
  };

  a_8 = static_cast<double *>(create8AlignedBuf());
  b_8 = static_cast<double *>(create8AlignedBuf());
  c_8 = static_cast<double *>(create8AlignedBuf());
  for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
    a_8[i] = i + 1;
  }
  for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
    b_8[i] = i + 1;
  }

  // 16 byte aligned
  auto create16AlignedBuf = [] {
    return _mm_malloc(sizeof(double) * DOUBLE_COUNT, 16);
  };

  a_16 = static_cast<double *>(create16AlignedBuf());
  b_16 = static_cast<double *>(create16AlignedBuf());
  c_16 = static_cast<double *>(create16AlignedBuf());
  for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
    a_16[i] = i + 1;
  }
  for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
    b_16[i] = i + 1;
  }

  // 32 byte aligned
  auto create32AlignedBuf = [] {
    return _mm_malloc(sizeof(double) * DOUBLE_COUNT, 32);
  };

  a_32 = static_cast<double *>(create32AlignedBuf());
  b_32 = static_cast<double *>(create32AlignedBuf());
  c_32 = static_cast<double *>(create32AlignedBuf());
  for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
    a_32[i] = i + 1;
  }
  for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
    b_32[i] = i + 1;
  }
}

void IntrinsicsBenchmarks::cleanup() {
  free(a);
  _mm_free(a_8);
  _mm_free(b_16);
  _mm_free(c_32);
}

void IntrinsicsBenchmarks::double_test() {
  QBENCHMARK {
    for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
      for (std::size_t j = 0; j < INNER_COUNT; ++j) {
        c[i] = std::sqrt(std::sqrt(a[i])) * b[i] * b[i] * b[i] + 1.25;
      }
    }
  }
  std::cout << c[DOUBLE_COUNT-1] << '\n';
}

void IntrinsicsBenchmarks::double8_test() {
  QBENCHMARK {
    for (std::size_t i = 0; i < DOUBLE_COUNT; ++i) {
      for (std::size_t j = 0; j < INNER_COUNT; ++j) {
        c_8[i] = std::sqrt(std::sqrt(a_8[i])) * b_8[i] * b_8[i] * b_8[i] + 1.25;
      }
    }
  }
  std::cout << c_8[DOUBLE_COUNT-1] << '\n';
}

void IntrinsicsBenchmarks::sse_double_test() {
  const std::size_t d_stride = 2;
  const std::size_t iters_m128d = DOUBLE_COUNT / d_stride;
  QBENCHMARK {
    double *c_ptr = c_16;
    __m128d *a_128d_ptr = reinterpret_cast<__m128d *>(a_16);
    __m128d *b_128d_ptr = reinterpret_cast<__m128d *>(b_16);
    __m128d *c_128d_ptr = reinterpret_cast<__m128d *>(c_16);
    const __m128d k = _mm_set1_pd(1.25);
    for (std::size_t i = 0; i < iters_m128d;
         ++i, c_ptr += d_stride, ++a_128d_ptr, ++b_128d_ptr, ++c_128d_ptr) {
      for (std::size_t j = 0; j < INNER_COUNT; ++j) {
        __m128d tmp = _mm_sqrt_pd(*a_128d_ptr);
        tmp = _mm_sqrt_pd(tmp);
        tmp = _mm_mul_pd(*b_128d_ptr, tmp);
        tmp = _mm_mul_pd(*b_128d_ptr, tmp);
        tmp = _mm_mul_pd(*b_128d_ptr, tmp);
        tmp = _mm_add_pd(k, tmp);
        _mm_store_pd(c_ptr, tmp);
      }
    }
  }
  std::cout << c_16[DOUBLE_COUNT-1] << '\n';
}

void IntrinsicsBenchmarks::avx_double_test() {
  const std::size_t d_stride = 4;
  const std::size_t iters_m256d = DOUBLE_COUNT / d_stride;

  QBENCHMARK {
    double *c_ptr = c_32;
    __m256d *a_256d_ptr = reinterpret_cast<__m256d *>(a_32);
    __m256d *b_256d_ptr = reinterpret_cast<__m256d *>(b_32);
    __m256d *c_256d_ptr = reinterpret_cast<__m256d *>(c_32);
    const __m256d k = _mm256_set1_pd(1.25);
    for (std::size_t i = 0; i < iters_m256d;
         ++i, c_ptr += d_stride, ++a_256d_ptr, ++b_256d_ptr, ++c_256d_ptr) {
      for (std::size_t j = 0; j < INNER_COUNT; ++j) {
        __m256d tmp = _mm256_sqrt_pd(*a_256d_ptr);
        tmp = _mm256_sqrt_pd(tmp);
        tmp = _mm256_mul_pd(*b_256d_ptr, tmp);
        tmp = _mm256_mul_pd(*b_256d_ptr, tmp);
        tmp = _mm256_fmadd_pd(*b_256d_ptr, tmp, k);
        _mm256_store_pd(c_ptr, tmp);
      }
    }
  }
  std::cout << c_32[DOUBLE_COUNT-1] << '\n';
}

QTEST_APPLESS_MAIN(IntrinsicsBenchmarks)

#include "tst_intrinsics.moc"
