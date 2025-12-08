sorting project

files
- sorting.c: radix sort implementation with a small sample main
- tests/test_sorting.c: tiny assert-based checks for ordering and duplicates
- Makefile: builds and runs everything

notes
- default build uses 16-bit digits (2 passes) via -DRADIX16; remove that flag for 8-bit digits (4 passes)
- single memcpy happens at the end only if needed (ping pong)
- main reads input.bin, sorts, writes output.bin
- simulator.py compiles sorting.c to ./sorter and feeds input.bin/output.bin

commands
- make sorter              # build (16-bit digits by default)
- make RADIX_FLAGS= sorter # build with 8-bit digits (note the underscore)
- make run                 # run ./sorter on input.bin -> output.bin
- make RADIX_FLAGS= run    # run using an 8-bit build in one step
- make test                # run unit tests
- make clean               # remove binaries

old sort_array
```
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
```
