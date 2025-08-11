import os
import sys

VAR_NAME = "index"

# Цвета для терминала (ANSI escape codes)
class Colors:
    RED = "\033[31m"
    GREEN = "\033[32m"
    RESET = "\033[0m"

def escape_line(line: str) -> str:
    """Escapes backslashes and quotes in the line for C strings."""
    return line.replace('\\', '\\\\').replace('"', '\\"')

def print_success(message: str):
    print(f"{Colors.GREEN}{message}{Colors.RESET}")

def print_error(message: str):
    print(f"{Colors.RED}{message}{Colors.RESET}")

def generate_header(input_file: str, output_file: str, var_name: str) -> None:
    """
    Generates a C header file with a PROGMEM string
    containing the contents of input_file.
    """
    if not os.path.isfile(input_file):
        print_error(f"Error: input file '{input_file}' not found.")
        sys.exit(1)

    try:
        with open(input_file, "r", encoding="utf-8") as f:
            lines = f.readlines()

        os.makedirs(os.path.dirname(output_file), exist_ok=True)

        with open(output_file, "w", encoding="utf-8") as f:
            f.write('#pragma once\n')
            f.write('#include <pgmspace.h>\n\n')
            f.write('namespace espweb {\n')
            f.write(f'const char {var_name}[] PROGMEM =\n')
            for line in lines:
                escaped = escape_line(line.rstrip('\n'))
                f.write(f'"{escaped}\\n"\n')
            f.write(';\n')
            f.write('}\n')

        print_success(f"Header file successfully generated: {output_file}")

    except Exception as e:
        print_error(f"Error generating header file: {e}")
        sys.exit(1)

def main():
    if len(sys.argv) != 3:
        print_error("Error: input file and output header file path are required.")
        print("Usage: python generate_header.py path_to_input.html path_to_output.h")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    generate_header(input_file, output_file, VAR_NAME)

if __name__ == "__main__":
    main()
