# 🔧 MIL Compiler

> A high-performance compiler for the MIL (Minimal Intermediate Language) programming language, written in C++20.

[![C++](https://img.shields.io/badge/C%2B%2B-20-blue?logo=cplusplus)](https://isocpp.org/)
[![Make](https://img.shields.io/badge/Make-Supported-064687)](https://www.gnu.org/software/make/)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)

## 📋 Requirements

| Tool | Version | Purpose |
|------|---------|---------|
| **C++ Compiler** | C++20 | Compilation (GCC 9.0+, Clang 10.0+, or MSVC 2019+) |
| **Make** | Any | Build execution |
| **NASM** | Any | Assembly to machine code |
| **ld** | Any | Linker (Linux) |
| **GoLink** | Any | Linker (Windows, optional) |

### ⚙️ Installation

<details>
<summary><b>Ubuntu/Debian</b></summary>

```bash
sudo apt-get install build-essential nasm
```
</details>

<details>
<summary><b>macOS</b></summary>

```bash
brew install nasm
# Xcode command-line tools should already be installed
```
</details>

<details>
<summary><b>Windows (MSYS2/Git Bash)</b></summary>

1. Install [MSYS2](https://www.msys2.org/)
2. Open MSYS2 terminal and run:
   ```bash
   pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-nasm
   ```
3. (Optional) Install **GoLink** for linking and add to PATH
</details>

## 🚀 Quick Start

### Build Instructions

```bash
# Clone the repository
git clone <repository-url> && cd MIL

# Build the compiler
make

# View available targets
make help
```

The compiled executable will be at `build/compiler`.

## 💻 Usage

Run the compiler with a MIL source file:

```bash
./build/compiler <path-to-file.mil>
```

You can also copy the executable elsewhere:
```bash
cp build/compiler /usr/local/bin/
compiler <file.mil>
```

### Example:
```bash
./build/compiler test.mil
```

**Note:** The compiler generates assembly and executables in a `target/` directory. This directory is created automatically for each compilation.

## ✅ How To Test (Current Project Stage)

The compiler generates an executable (`prog`) in the `target/` directory. For now, validation is done via:

### Linux
```bash
./build/compiler test.mil
./target/prog
echo $?
```

### Windows
```bash
.\build\compiler.exe test.mil
.\target\prog.exe
echo $LASTEXITCODE
```

The exit code indicates the result of the last arithmetic expression computed.

## 📁 Project Structure

```
MIL/
├── 📄 main.cpp                 # Entry point
├── 📂 src/
│   ├── scanner.cpp             # Lexical analyzer (tokenization)
│   ├── parser.cpp              # Syntax analyzer (AST generation)
│   └── codegen.cpp             # Code generation (AST → assembly)
├── 📂 include/
│   ├── scanner.h               # Scanner interface
│   ├── parser.h                # Parser interface
│   ├── types.h                 # Token type definitions
│   └── ast/
│       ├── expr_node.h         # Expression AST nodes
│       └── stmt_node.h         # Statement AST nodes
├── 📂 runtime/
│   └── linux/
│       └── print_int_linux.asm # Linux runtime for print
├── 📂 docs/                    # Language documentation
├── 📂 target/                  # Generated assembly & executables (auto-created)
├── 📄 Makefile                 # Build configuration
├── 🧪 test.mil                 # Example MIL program
└── 📝 README.md                # This file
```

## ✨ Features

- **Lexical Analysis** - Tokenizes MIL source code using a DFA-based scanner
- **Syntax Analysis** - Builds an Abstract Syntax Tree (AST) via recursive descent parser
- **Code Generation** - Generates x86-64 assembly output
- **Cross-Platform** - Supports Linux, macOS, and Windows

## 🧹 Clean Build Artifacts

```bash
# Remove build directory
make clean

# Also remove generated target files
rm -rf target/
```

## 📝 Input File Requirements

- Files must use the `.mil` extension
- Must conform to MIL language syntax (see `docs/grammar.md`)
- Integers must be 32-bit signed values (no leading zeros)

## 🐛 Troubleshooting

| Issue | Solution |
|-------|----------|
| `make: command not found` | Install GNU Make via your package manager |
| `g++: command not found` | Install a C++20 compiler (GCC 9+, Clang 10+, or MSVC) |
| `nasm: command not found` | Install NASM for assembly compilation |
| `Build fails on link` | Ensure `ld` (Linux) or appropriate linker is installed |

## 📚 Documentation

Learn more about the MIL language:
- [Grammar](docs/grammar.md) - Language syntax rules
- [Keywords](docs/keywords.txt) - Reserved keywords
- [Regex Patterns](docs/regex.md) - Token pattern definitions
- [Alphabet](docs/alphabet.txt) - Character set

## 📜 License

This project is provided as-is for educational and personal use.

## 👤 Author

**Ahmed** - Student Developer

Feel free to use, modify, and learn from this project!
