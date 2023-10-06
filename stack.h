#ifndef STACK_H
#define STACK_H

#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __stack {
  int size;
  int top;
  char **data;
} Stack;

Stack *create_stack(int size) {
  Stack *s = (Stack *)malloc(sizeof(Stack));
  s->top = -1;
  s->size = size;
  s->data = (char **)malloc(sizeof(char *) * size);
  return s;
}

char *head(Stack *s) { return s->data[s->top]; }

char *pop(Stack *s) {
  assert(s->top >= 0 && "UNDERFLOW !\n");

  s->top -= 1;
  return s->data[s->top + 1];
}

void push(Stack *s, char *v) {
  // assert(s->top < s->size - 1 && "OVERFLOW !\n");

  s->top += 1;
  s->data[s->top] = v;
}

Stack *reverse(Stack *s) {
  Stack *r = create_stack(s->size);
  while (s->top >= 0) {
    push(r, pop(s));
  }

  return r;
}

void print(Stack *s) {
  for (int i = s->top; i >= 0; i--) {
    printf("%s ", s->data[i]);
  }
  printf("\n");
}

#endif // STACK_H
