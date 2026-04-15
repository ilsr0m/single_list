# Singly Linked List (C)

A simple and lightweight implementation of a singly linked list in C.

The library provides a generic list container that stores elements of arbitrary type using void*.  
It includes common list operations, searching, filtering, and sorting algorithms.

---

## Features

- Generic container (void*-based)
- Basic operations:
  - create / delete
  - append / prepend / insert
  - remove / clear
- Access:
  - front / back / at
- Algorithms:
  - find / contains
  - filter
  - concat / splice
  - sorting (including experimental bogosort 😄)
- Fully tested with GoogleTest
- Doxygen-ready documentation

---

## Requirements

### Build tools
- CMake (>= 3.10 recommended)
- C compiler (C11 or newer)
- C++ compiler with C++17 support (required for tests)

### Compilers

#### Windows
- MSVC (UCRT toolset recommended)
- Use `build.ps1`

> Git Bash on Windows does not use MSVC by default.  
> Use PowerShell for MSVC builds.

#### Linux
- GCC or Clang
- Use build.sh

---
## Repository Structure

```text
singly-linked-list/
├── CMakeLists.txt 
├── README.md
├── LICENSE
├── include/
│   └── single_list.h
├── src/ 
│   └── single_list.c
├── tests/ 
│   └── ...
├── scripts/ 
│   ├── build.sh
│   └── build.ps1
└── docs/
```

---