#include "sqrt.h"
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Definition of a macro, used to check if the paramter has a argument or not
// (used for optional arguments)
#define OPT_ARG_EXISTS                                                         \
  ((optarg == NULL && optind < argc && argv[optind][0] != '-')                 \
       ? (bool)(optarg = argv[optind++])                                       \
       : (optarg != NULL))

const char *usage_msg = "Usage: %s [options] -d X      Gib das Ergebnis mit X "
                        "dezimalen Nachkommastellen aus.\n"
                        "   or: %s -h X                Gib das Ergebnis mit X "
                        "hexadezimalen Nachkommastellen aus.\n"
                        "   or: %s -h                  Eine Beschreibung aller "
                        "Optionen des Programms.\n";

const char *help_msg =
    "Optional arguments:\n"
    "  -V N     Welche Implementierung ausgeführt werden soll (default: N = 0 "
    "(Hauptimplementierung))\n"
    "  -B N     Messung der Laufzeit. Optionales Argument gibt die "
    "Wiederholungen an. (default: N = 1)\n"
    "  -d N     Ausgabe von n dezimalen Nachkommastellen\n"
    "  -h N     Ausgabe von n hexadezimalen Nachkommastellen\n"
    "  -h       Eine Beschreibung aller Optionen des Programms. (das hier)\n"
    "  --help   Eine Beschreibung aller Optionen des Programms. (das hier)\n";

/**
 * @brief This function print_usage prints a text containing information on how
 * to use the program. Output is on stderr.
 * @param progname
 */
void print_usage(const char *progname) {
  fprintf(stderr, usage_msg, progname, progname, progname);
}

/**
 * @brief This function print_help prints the usage information at first and
 * then the help text which contains information about what parameters are
 * allowed. Both texts are printed on stderr.
 * @param progname
 */
void print_help(const char *progname) {
  print_usage(progname);
  fprintf(stderr, "\n%s", help_msg);
}

/**
 * @brief This is the starting point of the program.
 * @param argc argument count
 * @param argv[] array of arguments
 */
int main(int argc, char *argv[]) {
  const char *progname = argv[0];

  // If the argument count is one, there's no argument passed since the name of
  // the program itself is argument one.
  if (argc == 1) {
    print_usage(progname);
    // Prettier for return 1 alias failure
    return EXIT_FAILURE;
  }

  // Declare default values
  __uint8_t impl = 0;
  bool perf = false;
  bool set = false;
  int perfLoops = 1;
  size_t s = 1;
  int form = 0;

  int opt;
  // Declare long options which are options for the program but with 2 hyphen in
  // front. NOTE: it's only a forward to the short form
  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {NULL, 0, 0, '\0'}};
  // Check if the next optional argument is indeed on of the valid ones
  // x:  -> The parameter x must have a argument
  // x:: -> The parameter x may have a argument (optional argument)
  // x   -> The parameter x must have zero arguments
  while ((opt = getopt_long(argc, argv, "V:B::h::d:", long_options, NULL)) !=
         -1) {
    switch (opt) {
    case 'V': // Set which implementation to use
      // Scan it's argument and parse it to a char
      sscanf(optarg, "%hhu", &impl);
      break;
    case 'B': // Set performance measurement to true
      // It's tricky to check wheter the parameter has an argument or not, so
      // there's a macro for checking.
      if (OPT_ARG_EXISTS) { // Set the number of loops for performance
                            // measurement
        sscanf(optarg, "%d", &perfLoops);
      }
      perf = true;
      break;
    case 'd': // Set number of decimal places
      sscanf(optarg, "%zu", &s);
      form = 0;
      set = true;
      break;
    case 'h': // Set number of hexadecimal places
      // Since the parameter h can either print the help message or select the
      // number of hexadecimal places, the check for an optional argument has to
      // be done at first
      if (OPT_ARG_EXISTS) {
        if (!set) {
          sscanf(optarg, "%zu", &s);
          form = 2;
        }
      } else {
        print_help(progname);
        return EXIT_SUCCESS;
      }
      break;
    default: // If there's a non-valid parameter passed, return a message on how
             // to use the program
      print_usage(progname);
      return EXIT_FAILURE;
      break;
    }
  }

  // Check for <= 0 inputs (make no sense)
  if ((long)s <= 0) {
    fprintf(stderr, "Die Eingabe muss größer 0 sein.\n");
    return EXIT_FAILURE;
  }

  // Some information metrics for the user (depending on the input)
  printf("Implementierung: %d\n", impl);
  printf("Performance (ja/nein): %d\n", perf);
  printf("Wie viele Performanztestwiederholungen: %d\n", perfLoops);
  printf("Eingabe: %ld\n", s);
  printf("\n");

  double time = 0.0;
  struct bignum *a = NULL;
  // Switch for different implementation methods
  switch (impl) {
  case 0:
    // For performance testing, iterate perfLoops many times
    // Default for perfLoops is 1, so if performance testing options isn't
    // passed, everything works fine too
    for (int i = 0; i < perfLoops; i++) {
      // Start timer
      clock_t start = clock();

      // Calculate the decimal places
      struct bignum r = sqrt2(s);
      a = &r;

      // End timer
      clock_t end = clock();
      // Add timer value to overall time and at the end just divide it by the
      // number of tries
      // NOTE: / 1000 is needed for getting the number of milliseconds
      time += (double)(end - start) / (CLOCKS_PER_SEC / 1000);
    }
    break;
  case 1:
    // For performance testing, iterate perfLoops many times
    // Default for perfLoops is 1, so if performance testing options isn't
    // passed, everything works fine too
    for (int i = 0; i < perfLoops; i++) {
      // Start timer
      clock_t start = clock();

      // Calculate the decimal places
      struct bignum r = sqrt2_V1(s);
      a = &r;

      // End timer
      clock_t end = clock();
      // Add timer value to overall time and at the end just divide it by the
      // number of tries
      // NOTE: / 1000 is needed for getting the number of milliseconds
      time += (double)(end - start) / (CLOCKS_PER_SEC / 1000);
    }
    break;
  default:
    fprintf(stderr, "Die Eingabe ist außerhalb des Bereichs ([0, 1]).\n");
    return EXIT_FAILURE;
    break;
  }

  // Print decimal places
  printf("Ergebnis: ");
  switch (form) {
  case 0: // Decimal
    print_bignum(a, 0, s);
    break;
  case 1: // Binary
    print_bignum(a, 1, s);
    break;
  case 2: // Hexa
    print_bignum(a, 2, s);
    break;
  default:
    fprintf(stderr, "Die Eingabe ist außerhalb des Bereichs ([0, 2])");
    return EXIT_FAILURE;
    break;
  }
  printf("\n");

  // If performance flag has been set, print performance data
  if (perf) {
    printf("Durchschnittliche Performanz: %f Millisekunden\n",
           (time / perfLoops));
  }

  // Exit with success if everything worked fine
  return EXIT_SUCCESS;
}