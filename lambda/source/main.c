#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/animal.h"

void write_function(char *filename, char *returntype, char *parametertype,
                    char *source) {
  FILE *file = fopen(filename, "w");
  fprintf(
      file,
      "#include <math.h>\n#include <string.h>\n#include <stdbool.h>\n#include "
      "\"include/animal.h\"\n\n%s lambda0(%s "
      "a){%s}",
      returntype, parametertype, source);
  fclose(file);
}
struct Lambdalib {
  char *libfile;
  char *sourcefile;
  void *handle;
  char *symbol;
  void *function;
  char *return_type;
  char *parameter_type;
};
typedef struct Lambdalib Lambdalib;

typedef struct SystemFailure {
  int return_value;
  int lerrno;
} SystemFailure;

typedef union Status_Lambda {
  SystemFailure systemfailure;
  Lambdalib lambdalib;
} Status_Lambda;

typedef struct StatusLambda {
  bool success;
  union Status_Lambda status_lambda;
} StatusLambda;

StatusLambda makelambdalib(char *return_type, char *parameter_type,
                           char *source) {
  write_function("lambda.c", return_type, parameter_type, source);
  int status = system("cc -shared -fPIC lambda.c -o liblambda.so -lm");
  void *lambdalib = dlopen("./liblambda.so", RTLD_NOW);
  int lerrno = errno;
  void *function = dlsym(lambdalib, "lambda0");
  if (function == 0) {
    printf("function is NULL\n");
  }
  Lambdalib lambda_data = {
      .libfile = "./liblambda.so",
      .sourcefile = "lambda.c",
      .handle = lambdalib,
      .symbol = "lambda0",
      .function = function,
      .return_type = return_type,
      .parameter_type = parameter_type,
  };
  if (status == 0) {
    StatusLambda statuslambda = {
        .success = true,
        .status_lambda =
            {
                .lambdalib = lambda_data,
            },
    };
    return statuslambda;
  } else {
    StatusLambda statuslambda = {
        .success = false,
        .status_lambda =
            {
                .systemfailure =
                    {
                        .return_value = status,
                        .lerrno = lerrno,
                    },
            },
    };
    return statuslambda;
  }
}

Animal animals[] = {
    {
        "Animalia",
        "Chordata",
        "Mammalia",
        "Carnivora",
        "Felidae",
        "Felis",
        "F. catus",
        "Felis catus",
    },
    {
        "Fungi",
        "Ascomycota",
        "Saccharomycetes",
        "Saccharomycetales",
        "Saccharomycetaceae",
        "Saccharomyces",
        "S. cerevisiae",
        "Saccharomyces cerevisiae",
    },
    {
        "Animalia",
        "Chordata",
        "Mammalia",
        "Carnivora",
        "Canidae",
        "Canis",
        "C. familiaris",
        "Canis familiaris",
    },
    {
        "Animalia",
        "Arthropoda",
        "Insecta",
        "Blattodea",
        "Ectobiidae",
        "Blattella",
        "B. germanica",
        "Blattella germanica",
    },
};
int main(int argc, char **argv) {

  double res[100];
  char *return_type;
  char *parameter_type;
  char *function_body;

  if (argc == 4) {
    return_type = argv[1];
    parameter_type = argv[2];
    function_body = argv[3];
  } else {
    function_body = "return sin(a)+1;";
    parameter_type = "double";
    return_type = "double";
    printf("Choosing the default transformation: %s (%s) %s\n", return_type,
           parameter_type, function_body);
  }
  StatusLambda statuslambda =
      makelambdalib(return_type, parameter_type, function_body);
  if (statuslambda.success == false) {
    printf("failed to compile lambda \n");
    exit(1);
  }
  Lambdalib lambdalib = statuslambda.status_lambda.lambdalib;
  if (0 == strcmp(lambdalib.return_type, "double")) {

    // For demonstration we use this function pointer. And I know this is not a
    // lambda, but if you squint really hard.
    double (*lambda)(double) = lambdalib.function;
    for (int i = 0; i < 100; i++) {
      double a = (double)i;
      res[i] = lambda(a);
    }
    for (int i = 0; i < 100; i++) {
      printf("%f ", res[i]);
    }
    printf("\n");
  } else if (0 == strcmp(lambdalib.return_type, "bool")) {
    // For demonstration we use this function pointer. And I know this is not a
    // lambda, but if you squint really hard.
    bool (*lambda)(Animal) = lambdalib.function;
    printf("Matching animals: \n");
    for (int i = 0; i < 4; i++) {
      if (lambda(animals[i])) {
        printf("%s\n", animals[i].Binomial_name);
      }
    }
    printf("End of matching animals\n");
  } else {
    printf("failed comparisons, parameter_type is:  %s\n",
           lambdalib.parameter_type);
  }
  dlclose(lambdalib.handle);
  // statuslambda =
  //     makelambdalib("char * ", "char * ",
  //                   "return \"Hello, World! from a newly loaded
  //                   function!\";");
  // if (statuslambda.success) {
  //   char *(*fun)(char *) = statuslambda.status_lambda.lambdalib.function;
  //   printf("%s\n", fun(NULL));
  // } else {
  //   printf("creating sharedlib failed.\n");
  // }
}
