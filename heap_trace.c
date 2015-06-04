#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define __USE_GNU
#include <dlfcn.h>

#define FUNC_LEN 50

int resolve_function_name(void *(*)(), char *, size_t);
void print_function_name(void *(*ip)());

static void init_hook(void);

static void* malloc_hook(size_t, const void *);
static void* realloc_hook(void *, size_t, const void *);
static void free_hook(void *, const void *);

static void *(*old_malloc_hook)(size_t, const void *);
static void *(*old_realloc_hook)(void *, size_t, const void *);
static void (*old_free_hook)(void *, const void *);

int resolve_function_name(void *(*ip)(), char *dst, size_t n)
{
    int r;
    Dl_info dlinfo;

    r = dladdr((void *)ip, &dlinfo);
    if (r == 0 || dlinfo.dli_sname == NULL)
        return 0;

    strncpy(dst, dlinfo.dli_sname, n);
    return 1;
}

void print_function_call(char *function, void *(*caller)(), size_t size, void *pointer)
{
    int r;
    char pfunc[FUNC_LEN];
    r = resolve_function_name(caller, pfunc, sizeof(pfunc)-1);
    if (r)
        fprintf(stderr, "%s: %s(%zi) = %p\n", pfunc, function, size, pointer);
    else
        fprintf(stderr, "%p: %s(%zi) = %p\n", caller, function, size, pointer);
}


static void init_hook(void)
{
    old_malloc_hook = __malloc_hook;
    __malloc_hook = malloc_hook;

    old_realloc_hook = __realloc_hook;
    __realloc_hook = realloc_hook;

    old_free_hook = __free_hook;
    __free_hook = free_hook;
}
void (*__MALLOC_HOOK_VOLATILE __malloc_initialize_hook)(void) = init_hook;

static void* malloc_hook(size_t size, const void *caller)
{
    void *p = NULL;

    __malloc_hook = old_malloc_hook;

    p = malloc(size);
    print_function_call("malloc", caller, size, p);

    old_malloc_hook = __malloc_hook;
    __malloc_hook = malloc_hook;
    return p;
}

static void* realloc_hook(void *ptr, size_t size, const void *caller)
{
    void *p = NULL;

    __realloc_hook = old_realloc_hook;

    p = realloc(ptr, size);
    print_function_call("realloc", caller, size, p);

    old_realloc_hook = __realloc_hook;
    __realloc_hook = realloc_hook;
    return p;
}

static void free_hook(void *ptr, const void *caller)
{
    int r;
    char pfunc[FUNC_LEN];

    __free_hook = old_free_hook;

    free(ptr);

    r = resolve_function_name(caller, pfunc, sizeof(pfunc)-1);
    if (r)
        fprintf(stderr, "%s: free(%p)\n", pfunc, ptr);
    else
        fprintf(stderr, "%p: free(%p)\n", caller, ptr);

    old_free_hook = __free_hook;
    __free_hook = free_hook;
    return;
}
