#include "lexer.h"
#include "stack.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_PRECISION 12
#define PI "3.141592653589793238462643383279502884197"
#define E "2.7182818284590452353602874713526624977572"
#define GR "1.61803398874989484820458683436563811"

#define MAX_PRECISION 100

void exit_err(char *msg, void *p, ...)
{
  fprintf(stderr, msg, p);
  exit(1);
}

bool streq(char *s1, char *s2) { return strcmp(s1, s2) == 0; }

bool is_number(char *c)
{
  if (c == NULL)
    return false;

  int i = 0;
  int point_count = 0;
  while (c[i] != '\0')
  {
    if (!isdigit(c[i]) && c[i] != '.')
      return false;

    if (c[i] == '.')
    {
      point_count++;
    }

    i++;
  }

  if (point_count >= 2)
  {
    fprintf(stderr, "ERROR : To much points for a float ! : \"%s\"\n", c);
    exit(1);
  }

  return true;
}

bool is_function(char *c)
{
  if (c == NULL)
    return false;

  static char *functions[] = {"log", "log10", "log2", "exp", "tanh",
                              "atan", "tan", "sin", "cos", "sqrt"};

  for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); i++)
  {
    if (streq(c, functions[i]))
    {
      return true;
    }
  }

  return false;
}

bool is_operator(char *c)
{
  if (c == NULL)
    return false;

  return streq(c, "+") || streq(c, "-") || streq(c, "*") || streq(c, "/") ||
         streq(c, "^") || streq(c, "(") || streq(c, ")");
}

int operator_order(char *op)
{
  if (streq(op, "-") || streq(op, "+"))
  {
    return 1;
  }

  if (streq(op, "/") || streq(op, "*"))
  {
    return 2;
  }

  if (streq(op, "^"))
  {
    return 3;
  }

  if (streq(op, "(") || streq(op, ")"))
  {
    return 4;
  }

  exit_err("ERROR : Unknown operator : \"%s\"\n", op);
  return -1;
}

Stack *SYA(Tokens tokens)
{
  Stack *out = create_stack(tokens.count);
  Stack *op = create_stack(tokens.count);

  for (int i = 0; i < tokens.count; i++)
  {
    char *t = tokens.tokens[i].value;
    if (is_number(t))
    {
      push(out, t);
    }
    else if (streq(t, "pi") || streq(t, "pie"))
    {
      push(out, PI);
    }
    else if (streq(t, "e"))
    {
      push(out, E);
    }
    else if (streq(t, "gr"))
    {
      push(out, GR);
    }
    else if (is_function(t))
    {
      push(op, t);
    }
    else if (streq(t, ","))
    {
      while (!streq(head(op), "("))
      {
        push(out, pop(op));
      }
    }
    else if (streq(t, "("))
    {
      push(op, t);
    }
    else if (streq(t, ")"))
    {
      while (!streq(head(op), "("))
      {
        push(out, pop(op));
      }
      pop(op);
      if (is_function(head(op)))
      {
        push(out, pop(op));
      }
    }
    else if (is_operator(t))
    {
      char *o2 = head(op);
      if (o2 == NULL)
      {
        push(op, t);
        continue;
      }
      while (is_operator(o2) && !streq(o2, "(") &&
             (operator_order(o2) > operator_order(t) ||
              (operator_order(o2) == operator_order(t) && !streq(t, "^"))))
      {
        push(out, pop(op));
        o2 = head(op);
      }

      push(op, t);
    }
  }

  while (op->top >= 0)
  {
    push(out, pop(op));
  }

  return out;
}

void push_result(Stack *s, long double result, int precision)
{
  char buff[MAX_PRECISION];
  snprintf(buff, sizeof(buff), "%.*Lf", precision + 1, result);

  char *v = malloc(sizeof(char) * MAX_PRECISION);
  strcpy(v, buff);
  push(s, v);
}

long double evaluate_sya(Stack *out_r, int precision)
{
  long double result = 0.0;
  Stack *s = create_stack(out_r->size);
  while (out_r->top >= 0)
  {
    if (is_number(head(out_r)))
    {
      push(s, pop(out_r));
    }
    else if (is_operator(head(out_r)))
    {
      if (s->top < 1)
      {
        fprintf(stderr, "ERROR : Can\'t perform expression\n");
        exit(1);
      }

      char *op = pop(out_r);

      long double a = strtold(pop(s), NULL);
      long double b = strtold(pop(s), NULL);

      switch (*op)
      {
      case '+':
        result = b + a;
        break;
      case '-':
        result = b - a;
        break;
      case '*':
        result = b * a;
        break;
      case '/':
        if (a == 0)
        {
          fprintf(stderr, "ERROR : Division by zero\n");
          exit(1);
        }
        result = b / a;
        break;
      case '^':
        result = powl(b, a);
        break;
      default:
        fprintf(stderr, "ERROR : Unknown operator : \"%s\"\n", op);
        exit(1);
      }
      push_result(s, result, precision);
    }
    else if (is_function(head(out_r)))
    {
      char *f = pop(out_r);
      if (s->top < 0)
      {
        fprintf(stderr, "ERROR : Can\'t perform expression\n");
        exit(1);
      }

      long double a = strtold(pop(s), NULL);

      if (streq(f, "log") || streq(f, "log10") || streq(f, "log2"))
      {
        if (a <= 0)
        {
          fprintf(stderr, "ERROR : Logarithm of non-positive number\n");
          exit(1);
        }
        result = (streq(f, "log") ? logl(a) : (streq(f, "log10") ? log10l(a) : log2l(a)));
      }
      else if (streq(f, "exp"))
      {
        result = expl(a);
      }
      else if (streq(f, "tanh"))
      {
        result = tanhl(a);
      }
      else if (streq(f, "atan"))
      {
        result = atanl(a);
      }
      else if (streq(f, "tan"))
      {
        result = tanl(a);
      }
      else if (streq(f, "sin"))
      {
        result = sinl(a);
      }
      else if (streq(f, "cos"))
      {
        result = cosl(a);
      }
      else if (streq(f, "sqrt"))
      {
        if (a < 0)
        {
          fprintf(stderr, "ERROR : Square root of negative number\n");
          exit(1);
        }
        result = sqrtl(a);
      }
      else
      {
        fprintf(stderr, "ERROR : Unknown function : \"%s\"\n", f);
        exit(1);
      }

      push_result(s, result, precision);
    }
  }

  if (s->top != 0)
  {
    fprintf(stderr, "ERROR : Malformed expression\n");
    exit(1);
  }

  return result;
}

long double calculate(char *cmd, int precision)
{
  Tokens tokens = tokenize(cmd);

  Stack *s = reverse(SYA(tokens));

  return evaluate_sya(s, precision);
}

void print_help()
{
  printf("Help for tval\n");
  printf("Usage : tval [OPTIONS] \"EXPRESSION\"\n");
  printf("Options :\n");
  printf("\t-h, --help : Print this help\n");
  printf("\t-p PRECISION : Set the precision of the result (default : %d)\n", DEFAULT_PRECISION);
  printf("\t-s : Print the result in the scientific format\n");
}

int main(int argc, char **argv)
{

  if (argc < 2)
  {
    printf("Error : Wrong usage of tval\nType --help to have more info\n");
    exit(1);
  }

  int precision = DEFAULT_PRECISION;

  bool scientific_notation = false;
  for (int c = 0; c < argc; c++)
  {
    if (streq(argv[c], "-h") || streq(argv[c], "--help"))
    {
      print_help();
      return 0;
    }
    else if (streq(argv[c], "-p"))
    {
      assert(argc > c + 1);

      precision = atoi(argv[c + 1]);
      if (precision > MAX_PRECISION)
        precision = MAX_PRECISION;

      c++;
    }
    else if (streq(argv[c], "-s"))
    {
      scientific_notation = true;
    }
    else if (c > 0 && c != argc - 1)
    {
      printf("Error : Wrong usage of parameters\nType --help to have more info\n");
      exit(1);
    }
  }

  char *cmd = argv[argc - 1];

  long double result = calculate(cmd, precision);

  if (scientific_notation)
  {
    printf("%.*Le\n", precision, result);
  }
  else
  {
    printf("%.*Lf\n", precision, result);
  }

  return 0;
}