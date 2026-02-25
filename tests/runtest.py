import subprocess
from pathlib import Path
import sys

def main():
    executable = Path("./build/slr_parser")
    tests_root = Path("tests")

    if not executable.exists():
        print(f"Ошибка: Исполняемый файл '{executable}' не найден.")
        sys.exit(1)

    if not tests_root.exists():
        print(f"Ошибка: Директория с тестами '{tests_root}' не найдена.")
        sys.exit(1)

    for input_path in tests_root.rglob("*"):

        if input_path.is_file() and input_path.parent.name == "input":
            
            output_dir = input_path.parent.parent / "output"
            
            output_path = output_dir / input_path.name

            output_dir.mkdir(parents=True, exist_ok=True)

            cmd = [
                str(executable),
                "--input_file" , str(input_path),
                "--output_file", str(output_path)
            ]

            print(f"Запуск: {' '.join(cmd)}")

            try:
                result = subprocess.run(cmd, capture_output=True, text=True)
                
                if result.returncode != 0:
                    print(f"  [ОШИБКА] Код возврата: {result.returncode}")
                    if result.stderr:
                        print(f"  stderr: {result.stderr.strip()}")
                    if result.stdout:
                        print(f"  stdout: {result.stdout.strip()}")
                        
            except OSError as e:
                print(f"  [КРИТИЧЕСКАЯ ОШИБКА] Не удалось запустить процесс: {e}")

if __name__ == "__main__":
    main()