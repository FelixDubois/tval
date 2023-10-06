#include "lexer.h"
#include "stack.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define PRECISION 12
#define PI "3.141592653589793238462643383279502884197"
#define E "2.7182818284590452353602874713526624977572"

#define MAX_INPUT 9999

bool streq(char *s1, char *s2) { return strcmp(s1, s2) == 0; }

bool is_number(char *c) {
  int i = 0;
  int point_count = 0;
  while (c[i] != '\0') {
    if (!isdigit(c[i]) && c[i] != '.')
      return false;

    if (c[i] == '.') {
      point_count++;
    }

    i++;
  }

  if(point_count >= 2){
    fprintf(stderr, "ERROR : To much points for a float ! : \"%s\"\n", c);
    exit(1);
  }

  return true;
}

bool is_function(char *c) {
  static char *functions[] = {
        "log","log10","log2",
        "exp",
        "tanh",
        "atan", "tan","sin", "cos",
        "sqrt"
    };

    for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); i++) {
        if (streq(c, functions[i])) {
            return true;
        }
    }

    return false;
}

bool is_operator(char *c) {
  return streq(c, "+") || streq(c, "-") || streq(c, "*") || streq(c, "/") ||
         streq(c, "^");
}

int operator_order(char *op) {
  if (streq(op, "-") || streq(op, "+")) {
    return 1;
  }

  if (streq(op, "/") || streq(op, "*")) {
    return 2;
  }

  if (streq(op, "^")) {
    return 3;
  }

  fprintf(stderr, "ERROR : Unknown operator : \"%s\"\n", op);
  exit(1);
}

Stack *SYA(Stack *tokens) {
  Stack *out = create_stack(tokens->top);
  Stack *op = create_stack(tokens->top);

  while (tokens->top >= 0) {
    char *v = pop(tokens);

    if (is_number(v)) {
      push(out, v);
    } else if (is_function(v)) {
      push(op, v);
    } else if (is_operator(v)) {
      char *o2 = head(op);
      while (op->top >= 0 && !streq(o2, "(") && is_operator(o2) &&
             (operator_order(o2) > operator_order(v) ||
              (operator_order(o2) == operator_order(v) && !streq(v, "^")))) {
        push(out, pop(op));
      }

      push(op, v);
    } else if (streq(v, "(")) {
      push(op, v);
    } else if (streq(v, ")")) {

      while (!streq(head(op), "(")) {
        if (op->top < 0) {
          fprintf(stderr, "ERROR : Problem with parenthesis\n");
          exit(1);
        }
        push(out, pop(op));
      }

      if (!streq(head(op), "(")) {
        fprintf(stderr, "ERROR : Problem with parenthesis\n");
        exit(1);
      }

      pop(op);
      if (is_function(head(op))) {
        push(out, pop(op));
      }
    } else {
      fprintf(stderr,
              "ERROR : Unknown token (not implemented yet ?) : \"%s\"\n", v);
      exit(1);
    }
  }

  while (op->top >= 0) {
    push(out, pop(op));
  }

  // Reverse the stack
  Stack *out_r = create_stack(out->top + 1);
  while (out->top >= 0) {
    push(out_r, pop(out));
  }

  return out_r;
}

long double evaluate_sya(Stack *out_r) {
  long double result = 0.0;
  Stack *s = create_stack(out_r->size);
  while (out_r->top >= 0) {    
    if (is_number(head(out_r))) {
      push(s, pop(out_r));
    } else if (is_operator(head(out_r))) {
      if (s->top < 1) {
        fprintf(stderr, "ERROR : Can\'t perform expression\n");
        exit(1);
      }

      char *op = pop(out_r);
      long double a = strtold(pop(s), NULL);
      long double b = strtold(pop(s), NULL);

      switch (*op)
      {
        case '+':
          result = a + b;
          break;
        case '-':
          result = a - b;
          break;
        case '*':
          result = a * b;
          break;
        case '/':
          result = a / b;
          break;
        case '^':
          result = powl(a, b);;
          break;
        default:
          fprintf(stderr, "ERROR : Unknown operator : \"%s\"\n", op);
          exit(1);
      }

      char buff[MAX_INPUT];
      snprintf(buff, sizeof(buff), "%.*Lf", PRECISION + 1, result);
      push(s, buff);
    }
  }

  return result;
}

Stack *inside_par(Stack *o) {
  Stack *ip = create_stack(o->size);

  int open_parenthesis = 0;
  int close_parenthesis = 0;
  while (!streq(head(o), ")") || open_parenthesis > close_parenthesis) {
    if(streq(head(o), "(")) open_parenthesis += 1;
    if(streq(head(o), ")")) close_parenthesis += 1;
    push(ip, pop(o));
  }
  pop(o);

  return reverse(ip);
}

long double evaluate(Stack *tokens) {
  long double result = 0.0f;

  Stack *s = create_stack(tokens->size);
  while (tokens->top >= 0) {
    char *t = pop(tokens);
    if (streq(t, "(")) {
      Stack *ip = inside_par(tokens);
      long double calc = evaluate(ip);
    
      char buff[MAX_INPUT];
      snprintf(buff, sizeof(buff), "%.*Lf", PRECISION + 1, calc);
      push(tokens, buff);

    } else if (is_function(t)) {

      if (!streq(head(tokens), "(")) {
        fprintf(stderr, "ERROR : Missig parenthesis !\n");
        exit(1);
      }

      pop(tokens); // Remove the function parenthesis
      long double a = evaluate(inside_par(tokens));

      long double fn_res = 0.0;

      if (streq(t, "log")) {
        fn_res = logl(a);
      } else if (streq(t, "log10")) {
        fn_res = log10l(a);
      } else if (streq(t, "log2")) {
        fn_res = log2l(a);
      } else if (streq(t, "exp")) {
        fn_res = expl(a);
      } else if (streq(t, "tanh")) {
        fn_res = tanhl(a);
      } else if (streq(t, "atan")) {
        fn_res = atanl(a);
      } else if (streq(t, "sin")) {
        fn_res = sinl(a);
      } else if (streq(t, "cos")) {
        fn_res = cosl(a);
      } else if (streq(t, "tan")) {
        fn_res = tanl(a);
      } else if (streq(t, "sqrt")){
        fn_res = sqrtl(a);
      } else {
        fprintf(stderr, "ERROR : Unknown function : %s", t);
        exit(1);
      }

      char fn_buff[MAX_INPUT];
      snprintf(fn_buff, sizeof(fn_buff), "%.*Lf", PRECISION + 1, fn_res);
      push(tokens, fn_buff);

    } else if (streq(t, "pi") || streq(t, "pie")) {
      push(s, PI);
    } else if (streq(t, "e")) {
      push(s, E);
    } else {
      push(s, t);
    }
  }

  if (s->top == 0) {
    return strtold(pop(s), NULL);
  }

  Stack *sya = SYA(s);

  result = evaluate_sya(sya);

  return result;
}

long double calculate(char * cmd){
  Tokens tokens = tokenize(cmd);
  Stack *s = create_stack(tokens.count);

  for (int i = 0; i < tokens.count; i++) {
    push(s, tokens.tokens[i].value);
  }

  Stack *s_r = reverse(s);
  return evaluate(s_r);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "%s", "Wrong usage : tval \"5 * exp(2) + pi\"\n");
    exit(1);
  }

  char *cmd = argv[1];

  long double result = calculate(cmd);

  printf("%.*Lf\n", PRECISION, result);
}
