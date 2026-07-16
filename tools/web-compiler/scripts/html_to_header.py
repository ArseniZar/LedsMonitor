from enum import Enum
import os
import re
import shutil
import subprocess
import sys

class Color(Enum):
    RED = "\033[31m"
    GREEN = "\033[32m"
    YELLOW = "\033[33m"
    BLUE = "\033[34m"
    CYAN = "\033[36m"
    BOLD = "\033[1m"
    RESET = "\033[0m"
    WHITE = "\033[37m"
    GRAY = "\033[90m"

    @classmethod
    def paint(cls, text, *colors):
        color_prefix = "".join([c.value for c in colors])
        return f"{color_prefix}{text}{cls.RESET.value}"


def print_main_banner():
    print("\n\n\n")
    print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.GREEN))
    print(Color.paint("│                                                            │", Color.GREEN))
    print(Color.paint("│                     HTML_TO_HEADER.PY                      │", Color.GREEN))
    print(Color.paint("│                                                            │", Color.GREEN))
    print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.GREEN))


def step1_check_arguments():
    print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.GRAY))
    print(Color.paint("│                                                            │", Color.GRAY))
    print(Color.paint("│ ", Color.GRAY) + Color.paint("                   STEP 1: CHECKING ARGUMENTS             ", Color.WHITE, Color.BOLD) + Color.paint(" │", Color.GRAY))
    print(Color.paint("│                                                            │", Color.GRAY))
    print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.GRAY))

    if len(sys.argv) != 4:
        print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("│ ", Color.RED) + Color.paint("         ERROR: INVALID NUMBER OF ARGUMENTS!               ", Color.RED, Color.BOLD) + Color.paint("│", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.RED))
        
        print(f" Arguments passed: {Color.paint(len(sys.argv) - 1, Color.YELLOW)}")
        print(f" Required:         {Color.paint('3', Color.GREEN, Color.BOLD)}")
        print("\n Usage:")
        print(Color.paint("   python html_to_header.py <input_file> <output_dir> <output_filename>", Color.CYAN))
        print("\n Example:")
        print("   python html_to_header.py index.html . index_html.h\n")
        print("\n" + Color.paint("—" * 62, Color.RED) + "\n")
        print(Color.paint("##############################################################", Color.BLUE))
        print("\n\n\n")
        sys.exit(1)

    input_file = sys.argv[1]       
    output_dir = sys.argv[2]     
    output_filename = sys.argv[3]  

    if not os.path.exists(input_file) or not os.path.isfile(input_file):
        print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("│ ", Color.RED) + Color.paint("         ERROR: SOURCE HTML FILE DOES NOT EXIST!           ", Color.RED, Color.BOLD) + Color.paint("│", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.RED))
        print(f" Passed path: {Color.paint(input_file, Color.YELLOW)}")
        print(f" Status:      {Color.paint('NOT FOUND on disk', Color.RED)}")
        print(f" Expected:    A valid path to an existing HTML file\n")
        print("\n" + Color.paint("—" * 62, Color.RED) + "\n")
        print(Color.paint("##############################################################", Color.BLUE))
        print("\n\n\n")
        sys.exit(1)

    if not input_file.lower().endswith('.html'):
        print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("│ ", Color.RED) + Color.paint("         ERROR: INPUT FILE MUST HAVE .HTML EXTENSION!      ", Color.RED, Color.BOLD) + Color.paint("│", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.RED))
        print(f" Passed file: {Color.paint(input_file, Color.YELLOW)}")
        print(f" Expected format: {Color.paint('*.html', Color.GREEN, Color.BOLD)}\n")
        print("\n" + Color.paint("—" * 62, Color.RED) + "\n")
        print(Color.paint("##############################################################", Color.BLUE))
        print("\n\n\n")
        sys.exit(1)

    if not os.path.exists(output_dir):
        print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("│ ", Color.RED) + Color.paint("         ERROR: OUTPUT DIRECTORY DOES NOT EXIST!           ", Color.RED, Color.BOLD) + Color.paint("│", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.RED))
        print(f" Passed path: {Color.paint(output_dir, Color.YELLOW)}")
        print(f" Status:      {Color.paint('NOT FOUND on disk', Color.RED)}")
        print(f" Expected:    An existing folder (e.g. '.', './src', './include')\n")
        print(Color.paint("##############################################################", Color.BLUE))
        print("\n\n\n")
        sys.exit(1)

    if not os.path.isdir(output_dir):
        print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("│ ", Color.RED) + Color.paint("         ERROR: OUTPUT PATH IS NOT A DIRECTORY!            ", Color.RED, Color.BOLD) + Color.paint("│", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.RED))
        print(f" Passed path: {Color.paint(output_dir, Color.YELLOW)}")
        print(f" Expected:    A valid folder, not a file.\n")
        print(Color.paint("##############################################################", Color.BLUE))
        print("\n\n\n")
        sys.exit(1)

    print(f" [✓] Input file:  {Color.paint(input_file, Color.CYAN)}")
    print(f" [✓] Output dir:  {Color.paint(output_dir, Color.CYAN)}")
    print(f" [✓] Output file: {Color.paint(output_filename, Color.CYAN)}")
    print("\n" + Color.paint("—" * 62, Color.GREEN) + "\n")
    
    return input_file, output_dir, output_filename


def step2_check_utilities():
    print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.GRAY))
    print(Color.paint("│                                                            │", Color.GRAY))
    print(Color.paint("│ ", Color.GRAY) + Color.paint("           STEP 2: CHECKING SYSTEM UTILITIES               ", Color.WHITE, Color.BOLD) + Color.paint("│", Color.GRAY))
    print(Color.paint("│                                                            │", Color.GRAY))
    print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.GRAY))

    xxd_path = shutil.which("xxd")

    if xxd_path is None:
        print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("│ ", Color.RED) + Color.paint("         ERROR: 'xxd' UTILITY WAS NOT FOUND!               ", Color.RED, Color.BOLD) + Color.paint("│", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.RED))
        
        print(" Please install 'xxd' on your system to proceed:")
        print(f"   - {Color.paint('Linux (Ubuntu/Debian)', Color.CYAN)}: sudo apt install xxd (or vim-common)")
        print(f"   - {Color.paint('macOS', Color.CYAN)}: xxd is built-in, check your PATH")
        print(f"   - {Color.paint('Windows', Color.CYAN)}: install via Git Bash, MSYS2 or wsl\n")
        print(Color.paint("##############################################################", Color.BLUE))
        print("\n\n\n")
        sys.exit(1)

    print(f" [✓] Found 'xxd' at: {Color.paint(xxd_path, Color.CYAN)}")
    print("\n" + Color.paint("—" * 62, Color.GREEN) + "\n")


def step3_generate_header(input_file, output_dir, output_filename):
    print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.GRAY))
    print(Color.paint("│                                                            │", Color.GRAY))
    print(Color.paint("│ ", Color.GRAY) + Color.paint("         STEP 3: INITIALIZING HEADER & RUNNING XXD         ", Color.WHITE, Color.BOLD) + Color.paint("│", Color.GRAY))
    print(Color.paint("│                                                            │", Color.GRAY))
    print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.GRAY))

    full_output_path = os.path.join(output_dir, output_filename)
    filename_base = os.path.basename(input_file)
    clean_var_name = "".join([c if c.isalnum() else "_" for c in filename_base])
    
    if os.path.exists(full_output_path):
        print(f" [!] File '{Color.paint(output_filename, Color.YELLOW)}' already exists in '{output_dir}'.")
        print(f"     Status: {Color.paint('Overwriting existing file...\n', Color.YELLOW)}")
    else:
        print(f" [+] File '{Color.paint(output_filename, Color.GREEN)}' does not exist in '{output_dir}'.")
        print(f"     Status: {Color.paint('Creating a new file...\n', Color.GREEN)}")

    try:
        print(f" [~] Running: {Color.paint(f'xxd -i {input_file}', Color.CYAN)}")
        
        result = subprocess.run(
            ["xxd", "-i", input_file],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,  
            text=True,
            check=True
        )

        xxd_raw_output = result.stdout

        match_arr = re.search(r"unsigned char\s+(\w+)\[\]", xxd_raw_output)
        match_len = re.search(r"unsigned int\s+(\w+)\s*=", xxd_raw_output)
        
        if match_arr:
            old_name = match_arr.group(1)
            xxd_raw_output = re.sub(rf"\b{old_name}\b", clean_var_name, xxd_raw_output)
            
            if match_len:
                old_len_name = match_len.group(1)
                new_len_name = f"{clean_var_name}_len"
                xxd_raw_output = re.sub(rf"\b{old_len_name}\b", new_len_name, xxd_raw_output)

        modified_payload = xxd_raw_output.replace(
            "unsigned char", 
            "inline constexpr unsigned char"
        )
        modified_payload = modified_payload.replace("[] =", "[] PROGMEM =")
        modified_payload = modified_payload.replace(
            "unsigned int", 
            "inline constexpr unsigned int"
        )

        with open(full_output_path, "w", encoding="utf-8") as out:
            out.write("#pragma once\n")
            out.write("#include <pgmspace.h>\n\n")
            out.write(modified_payload)
            out.write("\n")  
            
        print(f"\n [✓] C++ header successfully generated at:")
        print(f"     {Color.paint(full_output_path, Color.GREEN)}")

    except subprocess.CalledProcessError as e:
        xxd_error_message = e.stderr.strip() if e.stderr else "Unknown xxd error"
        print(Color.paint("\n╭────────────────────────────────────────────────────────────╮", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("│ ", Color.RED) + Color.paint("         ERROR: XXD UTILITY FAILED TO RUN!                 ", Color.RED, Color.BOLD) + Color.paint("│", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.RED))
        print(f" Command executed: {Color.paint(f'xxd -i {input_file}', Color.YELLOW)}")
        print(f" Exit code:       {Color.paint(e.returncode, Color.YELLOW)}")
        print(f" System message:   {Color.paint(xxd_error_message, Color.RED, Color.BOLD)}\n")
        print(Color.paint("##############################################################", Color.BLUE))
        print("\n\n\n")
        sys.exit(1)

    except Exception as e:
        print(Color.paint("\n╭────────────────────────────────────────────────────────────╮", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("│ ", Color.RED) + Color.paint("         ERROR: FAILED TO WRITE HEADER FILE!               ", Color.RED, Color.BOLD) + Color.paint("│", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.RED))
        print(f" Target path: {Color.paint(full_output_path, Color.YELLOW)}")
        print(f" Reason:      {Color.paint(str(e), Color.YELLOW)}\n")
        print(Color.paint("##############################################################", Color.BLUE))
        print("\n\n\n")
        sys.exit(1)

    return full_output_path


def print_success_banner(full_output_path):
    print("\n" + Color.paint("—" * 62, Color.GREEN) + "\n")
    print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.GREEN))
    print(Color.paint("│                                                            │", Color.GREEN))
    print(Color.paint("│ ", Color.GREEN) + Color.paint("                         SUCCESS                           ", Color.GREEN, Color.BOLD) + Color.paint("│", Color.GREEN))
    print(Color.paint("│                                                            │", Color.GREEN))
    print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.GREEN))
    print(f" Final output target: {Color.paint(full_output_path, Color.GREEN, Color.BOLD)}\n")
    print(Color.paint("##############################################################", Color.BLUE))
    print("\n\n\n")


def main():
    print_main_banner()
    input_file, output_dir, output_filename = step1_check_arguments()
    step2_check_utilities()
    full_output_path = step3_generate_header(input_file, output_dir, output_filename)
    print_success_banner(full_output_path)


if __name__ == "__main__":
    main()