# Tenpiler - тензорный компилятор

Это учебный проект 4-го семестра по реализации синтаксического анализатора простого "арифметического"языка алгоритмом "shift-reduce". Курс "Введение в тензорные компиляторы"

Автор:
-   Шелонин Арсений, Б01-411

## Зависимости

| Зависимость           | Минимальная версия    | Назначение                                    |
|-----------------------|-----------------------|-----------------------------------------------|
| **CMake**             | 3.12                  | Сборка проекта и зависимостей                 |
| **GCC / Clang**       | GCC 10 / Clang 11     | Компиляция C++20 кода                         |
| **FLeX**              | 2.6.4                 | Токенизация исходного кода                    |

### Установка зависимостей (Ubuntu/Debian)

#### 1. Системные зависимости
```zsh
sudo apt update
sudo apt install -y \
    build-essential \
    git \
    wget \
    snap
```

#### 2. CMake (version 4.2.3)

```zsh
sudo snap install cmake --classic
echo 'export PATH="/snap/bin:$PATH"' >> ~/.zshrc # или ~/.bashrc
source ~/.zshrc # или ~/.bashrc
```

#### 3. FLeX
```zsh
sudo apt install flex libfl-dev
```

## Сборка проекта

```zsh
git clone https://github.com/71frukt/SLR_Parser.git
cd SLR_Parser

cmake -B build -DCMAKE_BUILD_TYPE=Release # -DSANITIZE=ON включение санитайзеров
cmake --build build -j$(nproc)
```

## Запуск
```zsh
./build/slr_parser
```

| Флаги                    |   Действие                 |
|--------------------------|----------------------------|
| `-h`, `--help`           |   Вывести помощь           |
| `-f`, `--source_file`    | Задать имя входного файла  |