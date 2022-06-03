#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)

#define MEMSIZE 8192
#define FREE 123456
#define ALLOCATED 654321

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);

struct Header{
    size_t status;
    size_t size;
    struct Header *next;
    struct Header *prev;
};