#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>

#ifdef RADIX16
#define RADIX_BITS 16
#else
#define RADIX_BITS 8
#endif
#define RADIX_BUCKETS (1u << RADIX_BITS)
#define RADIX_PASSES (32 / RADIX_BITS)

// Avoid making changes to this function skeleton, apart from data type changes if required
// In this starter code we have used uint32_t, feel free to change it to any other data type if required
void sort_array(uint32_t *arr, size_t size) {
    if (size <= 1) return;

    uint32_t *out = malloc(size * sizeof(uint32_t));
    if (!out) exit(1);

    size_t count[RADIX_BUCKETS];
    // size_t count[256];
    uint32_t *src = arr;
    uint32_t *dst = out;

    // 4 byte passes
    for (unsigned shift = 0; shift < 32; shift += RADIX_BITS) {

        // reset count
        for (int i = 0; i < RADIX_BUCKETS; i++) count[i] = 0;
        // for (int i = 0; i < 256; i++) count[i] = 0;

        // count byte values
        for (size_t i = 0; i < size; i++)
            count[(src[i] >> shift) & (RADIX_BUCKETS - 1)]++;
            // count[(arr[i] >> shift) & 0xFF]++;

        // prefix sums
        size_t p = 0;
        for (int i = 0; i < RADIX_BUCKETS; i++) {
            size_t c = count[i];
            count[i] = p;
            p += c;
        }

        // place in output
        for (size_t i = 0; i < size; i++) {
            unsigned b = (src[i] >> shift) & (RADIX_BUCKETS - 1);
            dst[count[b]++] = src[i];
            // unsigned b = (arr[i] >> shift) & 0xFF;
            // out[count[b]++] = arr[i];
        }

        // swap buffers (ping-pong)
        uint32_t *tmp = src;
        src = dst;
        dst = tmp;
        // memcpy(arr, out, size * sizeof(uint32_t));
    }

    if (src != arr) memcpy(arr, src, size * sizeof(uint32_t));

    free(out);
}
#ifndef TESTING
int main(void) {
    const char *in_path = "input.bin";
    const char *out_path = "output.bin";

    // Read
    FILE *in = fopen(in_path, "rb");
    if (!in) return 1;
    if (fseek(in, 0, SEEK_END) != 0) {
        fclose(in);
        return 1;
    }
    long bytes = ftell(in);
    if (bytes < 0) {
        fclose(in);
        return 1;
    }
    size_t size = (size_t)(bytes / sizeof(uint32_t));
    if (fseek(in, 0, SEEK_SET) != 0) {
        fclose(in);
        return 1;
    }

    uint32_t *arr = malloc(size * sizeof(uint32_t));
    if (!arr) {
        fclose(in);
        return 1;
    }
    size_t read = fread(arr, sizeof(uint32_t), size, in);
    fclose(in);
    if (read != size) {
        free(arr);
        return 1;
    }

    sort_array(arr, size);

    // write
    FILE *out = fopen(out_path, "wb");
    if (!out) {
        free(arr);
        return 1;
    }
    size_t wrote = fwrite(arr, sizeof(uint32_t), size, out);
    fclose(out);
    free(arr);
    return wrote == size ? 0 : 1;
}
#endif
