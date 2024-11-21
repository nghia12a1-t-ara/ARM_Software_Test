#include <arm_math.h>
#include <stdint.h>
#include "SYSTICK_Base.h"

void add_arrays(uint8_t * a, uint8_t * b, uint16_t * result, uint32_t length)
{
    for (int i = 0; i < length; i++) {
        result[i] = a[i] + b[i];
    }
}

void add_arrays_simd(int8_t* a, int8_t* b, int8_t* result)
{
    // Sử dụng lệnh SIMD để cộng 4 phần tử 8-bit cùng lúc
    int32_t vecA = *((int32_t*)a);     // Load 4 byte từ mảng A
    int32_t vecB = *((int32_t*)b);     // Load 4 byte từ mảng B
    int32_t vecResult = __SADD8(vecA, vecB); // SIMD cộng 4 phần tử 8-bit
    *((int32_t*)result) = vecResult;   // Lưu kết quả 4 byte
}

void start_measure_time()
{
	SYSTICK->RVR = 0xFFFFFF;
	SYSTICK->CVR = 0;
	SYSTICK->CSR = SYSTICK_CSR_CLKSOURCE_MASK | SYSTICK_CSR_ENABLE_MASK;
}
uint32_t stop_measure_time()
{
	SYSTICK->CSR = 0;
    return 0xFFFFFF - SYSTICK->CVR;
}

volatile int8_t result[4] = {0};

int main() {
    volatile int8_t A[] = {1, 2, 3, 4};
    volatile int8_t B[] = {5, 6, 7, 8};
    volatile uint32_t time_with_simd, time_without_simd;

    start_measure_time();
    add_arrays(A, B, result, 4);
    time_without_simd = stop_measure_time();

    start_measure_time();
    add_arrays_simd(A, B, result);
    time_with_simd = stop_measure_time();

    (void)result;
    (void)time_without_simd;
    (void)time_with_simd;
}
