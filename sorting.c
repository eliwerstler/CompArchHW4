#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>

// Avoid making changes to this function skeleton, apart from data type changes if required
// In this starter code we have used uint32_t, feel free to change it to any other data type if required
void sort_array(uint32_t *arr, size_t size) {
    if (size <= 1) return;

    uint32_t *out = malloc(size * sizeof(uint32_t));
    if (!out) exit(1);

    size_t count[256];

    // 4 byte passes
    for (unsigned shift = 0; shift < 32; shift += 8) {

        // reset count
        for (int i = 0; i < 256; i++) count[i] = 0;

        // count byte values
        for (size_t i = 0; i < size; i++)
            count[(arr[i] >> shift) & 0xFF]++;

        // prefix sums
        size_t p = 0;
        for (int i = 0; i < 256; i++) {
            size_t c = count[i];
            count[i] = p;
            p += c;
        }

        // place in output
        for (size_t i = 0; i < size; i++) {
            unsigned b = (arr[i] >> shift) & 0xFF;
            out[count[b]++] = arr[i];
        }

        // copy back
        memcpy(arr, out, size * sizeof(uint32_t));
    }

    free(out);
}

int main() {
    size_t size = 10; // temp for compile

    //Initialise the array

    uint32_t *sorted_arr = malloc(size * sizeof(uint32_t)); // Allocate memory for the sorted array
    
    // Sort the copied array
    sort_array(sorted_arr, size);

    // Print the sorted array

    return 0;
}

       
