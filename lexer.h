#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
  char *value;
} Token;

typedef struct {
  Token *tokens;
  int count;
} Tokens;

bool is_token(char c) {
  return c == '(' || c == ')' || c == '_' || c == '*' || c == '+' || c == '-' ||
         c == '/' || c == '^';
}

char *append(char *ori, char c) {
  int size = strlen(ori);
  char *str = (char *)malloc(size + 2);
  strcpy(str, ori);
  str[size] = c;
  str[size + 1] = '\0';

  return str;
}

Tokens append_tokens(Tokens tokens, Token *t) {

  Token *nt = (Token *)malloc((tokens.count + 1) * sizeof(Token));
  for (int i = 0; i < tokens.count; i++) {
    nt[i] = tokens.tokens[i];
  }

  nt[tokens.count] = *t;

  tokens.tokens = nt;
  tokens.count += 1;

  t->value = (char *)"";
  return tokens;
}

Tokens tokenize(char *cmd) {
  Tokens tokens = {NULL, 0};

  bool last_was_known_token = false;
  Token *t = (Token *)malloc(sizeof(Token));
  t->value = (char *)"";

  while (*cmd != '\0') {
    if (isspace(*cmd)) {
      cmd += 1;
      continue;
    }
    if (is_token(*cmd)) {

      if (!last_was_known_token && strlen(t->value) > 0) {
        tokens = append_tokens(tokens, t);
      };

      t->value = append((char *)"", *cmd);
      tokens = append_tokens(tokens, t);

      last_was_known_token = true;
    } else {
      t->value = append(t->value, *cmd);
      last_was_known_token = false;
    }

    cmd += 1;
  }

  if (strlen(t->value) > 0) {
    tokens = append_tokens(tokens, t);
  }

  return tokens;
}

#endif // LEXER_H
