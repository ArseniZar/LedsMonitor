import os
import subprocess

BASE_DIR = os.path.abspath(os.path.dirname(__file__))

VITE_BUILD_CMD = ["npm", "run", "build"]
INPUT_FILE = os.path.join(BASE_DIR, "dist", "index.html")
OUTPUT_FILE = os.path.join(BASE_DIR, "..", "include", "index_html.h")
VAR_NAME = "index_html"

def run_vite_build():
    print("🔧 Запуск сборки Vite...")
    result = subprocess.run(VITE_BUILD_CMD, cwd=BASE_DIR, capture_output=True, text=True)
    if result.returncode != 0:
        print("❌ Сборка не удалась!")
        print(result.stderr)
        exit(1)
    print("✅ Сборка завершена успешно.")

def generate_header(input_file: str, output_file: str, var_name: str):
    if not os.path.exists(input_file):
        print(f"❌ Ошибка: файл {input_file} не найден.")
        exit(1)

    with open(input_file, "r", encoding="utf-8") as f:
        content = f.read()

    content = content.replace('\\', '\\\\').replace('"', '\\"')

    lines = [content[i:i+80] for i in range(0, len(content), 80)]

    os.makedirs(os.path.dirname(output_file), exist_ok=True)

    with open(output_file, "w", encoding="utf-8") as f:
        f.write('#include <pgmspace.h>\n\n')
        f.write(f'const char {var_name}[] PROGMEM =\n')
        for line in lines:
            f.write(f'"{line}"\n')
        f.write(';\n')

    print(f"✅ Заголовочный файл сгенерирован: {output_file}")

if __name__ == "__main__":
    run_vite_build()
    generate_header(INPUT_FILE, OUTPUT_FILE, VAR_NAME)
