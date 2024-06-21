void
arrprint(int *arr, int len)
{
    int max = 0;
    for (int i = 0; i < len; ++i) 
        if (abs(arr[i]) > max)
            max = abs(arr[i]);
    int digits = (int) log10(max + 1) + 2;
    printf("[");
    for (int i = 0; i < len; ++i) {
        printf("%*d", digits, arr[i]);
        if (i < len - 1)
            printf(",");
    }
    printf("]");
}

void
arr2dprint(int *arr, int width, int height)
{
    int max = 0;
    for (int i = 0; i < width * height; ++i) 
        if (abs(arr[i]) > max)
            max = abs(arr[i]);
    int digits = (int) log10(max + 1) + 2;
    printf("[");
    for (int i = 0; i < width * height; ++i) {
        if (i % width == 0 && i > 0)
            printf("\n");
        if (i > 0)
            printf(" ");
        printf("%*d", digits, arr[i]);
        if (i < width * height - 1)
            printf(",");
    }
    printf("]");
}
