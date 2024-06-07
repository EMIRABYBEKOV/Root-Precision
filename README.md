
# Root Precision 2023

## Description

This project focuses on the computation of the square root of 2 (√2). As an irrational number, √2 cannot be expressed as a simple fraction of two integers and has an infinite number of decimal places. The aim of this project is to demonstrate a precise and efficient method for calculating √2 to a high degree of accuracy.

## Project Overview

The main task of this project is to develop a program that efficiently calculates √2 with its numerous decimal places. The program is designed to assist users in performing the calculations and includes various algorithms for division, exponentiation, and matrix application, as well as Karatsuba multiplication. The final result will be presented in a selectable format (decimal or hexadecimal).

## Objectives

- Demonstrate precise calculation methods for √2
- Efficiently compute √2 to a high degree of accuracy
- Implement various mathematical algorithms and present results in multiple formats

## Features

- High-precision calculation of √2
- User-friendly interface for performing calculations
- Support for multiple output formats (decimal, hexadecimal)
- Implementation of advanced algorithms for optimal performance

## Directory Structure

```
INTERPOLATIONAPP/
├── Implementierung/
│   ├── division.h
│   ├── expm.h
│   ├── kmul.h
│   ├── sqrt.h
│   ├── type.h
│   ├── main.c
│   └── Makefile
```

## Getting Started

### Prerequisites

- A C compiler (e.g., `gcc`)
- Make utility

### Building the Project

To build the project, navigate to the `Implementierung` directory and run:

```sh
make
```

This will compile the source files and generate the executable.

## Usage

To use this project, follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/EMIRABYBEKOV/Root-Precision.git
    ```
2. Navigate to the project directory:
    ```sh
    cd rootprecisionapp/Implementierung
    ```
3. Compile the program:
    ```sh
    make
    ```

### Command-Line Options

- `-V<Zahl>`: Specify the implementation to use. Use `-V 0` for the main implementation. If not set, the main implementation will be used by default.
- `-B<Zahl>`: If set, measures and outputs the runtime of the specified implementation.
- `-B<Zahl>`: If set, measures and outputs the runtime of the specified implementation. The optional argument specifies the number of repetitions of the function call.
- `-d<Zahl>`: Output `n` decimal places. This takes precedence over `-h`.
- `-h<Zahl>`: Output `n` hexadecimal places.
- `-h`: Displays a description of all program options and usage examples, then exits.
- `--help`: Displays a description of all program options and usage examples, then exits.

### Examples

- Run the main implementation:
    ```sh
    ./main
    ```
- Use implementation `0`:
    ```sh
    ./main -V0
    ```
- Measure the runtime of implementation `0` with 10 repetitions:
    ```sh
    ./main -B10
    ```
- Output 5 decimal places:
    ```sh
    ./main -d5
    ```
- Output 5 hexadecimal places:
    ```sh
    ./main -h5
    ```
- Display help:
    ```sh
    ./main -h
    ```
    or
    ```sh
    ./main --help
    ```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


