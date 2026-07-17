from enum import Enum
import subprocess
import platform
import os
import shutil
from SCons.Script import Import # type: ignore

Import("env") # type: ignore

if env.IsIntegrationDump(): # type: ignore
   Return() # type: ignore


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
    print("\n\n")
    print(Color.paint("##############################################################", Color.BLUE))
    print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.CYAN))
    print(Color.paint("│                                                            │", Color.CYAN))
    print(Color.paint("│                      TASK WEB-COMPILER                     │", Color.CYAN))
    print(Color.paint("│                                                            │", Color.CYAN))
    print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.CYAN))


def step1_check_docker():
    print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.GRAY))
    print(Color.paint("│                                                            │", Color.GRAY))
    print(Color.paint("│ ", Color.GRAY) + Color.paint("               STEP 1: CHECKING DOCKER                     ", Color.WHITE, Color.BOLD) + Color.paint("│", Color.GRAY))
    print(Color.paint("│                                                            │", Color.GRAY))
    print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.GRAY))

    docker_path = shutil.which("docker")
    if docker_path is None:
        print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("│ ", Color.RED) + Color.paint("         ERROR: DOCKER IS NOT INSTALLED!                   ", Color.RED, Color.BOLD) + Color.paint("│", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.RED))
        print(" Docker CLI was not found in your system PATH.")
        print(f" Please install Docker Desktop or Docker Engine to proceed:")
        print(f"   - {Color.paint('Official Link', Color.CYAN)}: https://www.docker.com/products/docker-desktop/")
        print("\n" + Color.paint("—" * 62, Color.RED) + "\n")
        print(Color.paint("##############################################################", Color.BLUE))
        print("\n\n")
        env.Exit(1)  # type: ignore

    print(f" [✓] Found 'docker' CLI at: {Color.paint(docker_path, Color.GREEN)}")

    try:
        subprocess.run(
            ["docker", "info"], 
            stdout=subprocess.DEVNULL, 
            stderr=subprocess.DEVNULL, 
            check=True
        )
        print(f" [✓] Docker Daemon is:      {Color.paint('RUNNING & ACTIVE', Color.GREEN, Color.BOLD)}")
        print("\n" + Color.paint("—" * 62, Color.GREEN) + "\n")
    except (subprocess.CalledProcessError, FileNotFoundError):
        print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("│ ", Color.RED) + Color.paint("         ERROR: DOCKER DAEMON IS NOT RUNNING!              ", Color.RED, Color.BOLD) + Color.paint("│", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.RED))
        print(" Docker CLI is installed, but the background service is offline.")
        print(f" Please {Color.paint('start Docker', Color.YELLOW, Color.BOLD)}")
        print("\n" + Color.paint("—" * 62, Color.RED) + "\n")
        print(Color.paint("##############################################################", Color.BLUE))
        print("\n\n")
        env.Exit(1) # type: ignore


def step2_run_compiler(app_dir, output_dir, scripts_dir, script_path, image_name, output_file):
    print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.GRAY))
    print(Color.paint("│                                                            │", Color.GRAY))
    print(Color.paint("│ ", Color.GRAY) + Color.paint("               STEP 2: RUNNING COMPILER                    ", Color.WHITE, Color.BOLD) + Color.paint("│", Color.GRAY))
    print(Color.paint("│                                                            │", Color.GRAY))
    print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.GRAY))

    try:
        subprocess.run([
            "docker", "build", 
            "-t", image_name, 
            "-f", f"{scripts_dir}/Dockerfile", 
            app_dir
        ], check=True)
    except subprocess.CalledProcessError as e:
        print(Color.paint("╭────────────────────────────────────────────────────────────╮", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("│ ", Color.RED) + Color.paint("         ERROR: DOCKER IMAGE BUILD FAILED!                 ", Color.RED, Color.BOLD) + Color.paint("│", Color.RED))
        print(Color.paint("│                                                            │", Color.RED))
        print(Color.paint("╰────────────────────────────────────────────────────────────╯\n", Color.RED))
        print(f" Build command returned exit code: {Color.paint(e.returncode, Color.YELLOW)}")
        print("\n" + Color.paint("—" * 62, Color.RED) + "\n")
        print(Color.paint("##############################################################", Color.BLUE))
        print("\n\n")
        env.Exit(1) # type: ignore

    cmd = ["docker", "run", "--rm"]
    
    if platform.system() != "Windows":
        try:
            uid = subprocess.check_output(['id', '-u']).decode().strip()
            gid = subprocess.check_output(['id', '-g']).decode().strip()
            cmd.extend(["-u", f"{uid}:{gid}"])
        except Exception:
            pass

    cmd.extend([
        "-v", f"{app_dir}:/input",
        "-v", f"{output_dir}:/output",
        "-v", f"{script_path}:/scripts/script.py",
        image_name,
        "python", "/scripts/script.py", 
        "/input/index.html", 
        "/output", 
        output_file
    ])

    try:
        subprocess.run(cmd, check=True)
    except subprocess.CalledProcessError:
        env.Exit(1) # type: ignore


def main():
    root = env.subst("$PROJECT_DIR") # type: ignore
    app_dir = os.path.abspath(f"{root}/configApp")
    output_dir = os.path.abspath(f"{root}/include/webserver")
    scripts_dir = os.path.abspath(f"{root}/tools/web-compiler")
    
    image_name = "lumvia-adminapp-build"
    output_file = "index.h"
    script_path = os.path.abspath(f"{scripts_dir}/scripts/html_to_header.py")

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    print_main_banner()
    step1_check_docker()
    step2_run_compiler(app_dir, output_dir, scripts_dir, script_path, image_name, output_file)


main()