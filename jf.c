#include <string.h>

#include "jf.h"
#include "list.h"

struct jf_value {
  int tag;
  union {
    long i32;
    double f64;
    struct {
      bool marked;
      union {
        struct jf_string str;
        struct jf_function fun;
        struct jf_cfunction cfun;
      } u;
    } *obj;
  } u;
};

struct jf_function {};

struct jf_cfunction {};

struct jf_string {
  struct jf_string *next;
  uint32_t hash;
  size_t len;
  char buf[1];
};

struct table {
  size_t len;
  struct jf_string **heads;
};

struct jf_runtime {
  struct table table;

  unsigned int cap;
  unsigned int len;
  struct list_head ctx;
  struct jf_value *base;
  struct jf_value *avail;

  struct list_head errs;

  void *(*alloc)(size_t);
  void *(*realloc)(void *, size_t);
  void (*free)(void*);
};

struct jf_context {
  struct list_head node;
  struct jf_runtime *rt;
  struct jf_value *base;
  struct jf_value *top;
};

void jf_init(struct jf_runtime *rt, int backlog) {
  memset(rt, 0, sizeof *rt);
  int cap = backlog * JF_STACK;
  rt->base = (struct jf_value *)malloc(sizeof(struct jf_value) * cap);
  if (unlikely(!rt->base))
    return; // TODO: should we panic here?
  rt->cap = cap;
  rt->avail = rt->base;
  rt->alloc = malloc;
  rt->realloc = realloc;
  rt->free = free;
  list_head_init(&rt->ctx);
}

void jf_throw(struct jf_context *ctx, const char *err) {}

void *jf_alloc(struct jf_context *ctx, size_t size) {
  void *p = ctx->rt->alloc(size);
  if (unlikely(!p)) {
    jf_throw(ctx, "out of memory");
    return NULL;
  }
  return p;
}

struct jf_context *jf_open(struct jf_runtime *rt) {
  struct jf_context *ctx;
  // if the runtime stack is full, extend it to 1.5 its current size.
  if (rt->len >= rt->cap || rt->avail == rt->base + rt->len) {
    int cap = rt->cap + rt->cap / 2;
    rt->base = rt->realloc(rt->base, cap * sizeof(struct jf_value));
    if (unlikely(!rt->base))
      return NULL;
    rt->avail = rt->base + rt->len;
    rt->cap = cap;
  }
  ctx = (struct jf_context *)rt->avail;
  ctx->base = rt->avail + sizeof(*ctx);
  ctx->top = ctx->base;
  ctx->rt = rt;
  rt->avail = rt->base + rt->len;
  rt->len++;
  list_add(&rt->ctx, &ctx->node);
  return ctx;
}

void jf_close(struct jf_context *ctx) {
  struct jf_runtime *rt = ctx->rt;
  rt->avail = (struct jf_value *)ctx;
  rt->len--;
  list_del(&ctx->node);
}

#ifdef JF_TEST

int main(void)
{
  struct jf_runtime rt;
  struct jf_context *c1, *c2;
  jf_init(&rt, 1);
  c1 = jf_open(&rt);
  assert(c1 != NULL);
  c2 = jf_open(&rt);
  assert(c2 != NULL);
  jf_close(c1);
  jf_close(c2);
  return 0;
}


#endif