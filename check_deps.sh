#!/bin/bash
echo "Checking dependencies..."

# Verificar Qt
if command -v qmake6 &> /dev/null; then
    echo "✓ Qt6 found"
elif command -v qmake-qt6 &> /dev/null; then
    echo "✓ Qt6 found"
elif command -v qmake &> /dev/null; then
    echo "✓ Qt found (checking version...)"
    qmake --version | head -1
else
    echo "✗ Qt not found!"
    exit 1
fi

# Verificar compilador
if command -v g++ &> /dev/null; then
    echo "✓ g++ found: $(g++ --version | head -1)"
elif command -v clang++ &> /dev/null; then
    echo "✓ clang++ found: $(clang++ --version | head -1)"
else
    echo "✗ C++ compiler not found!"
    exit 1
fi

# Verificar CMake
if command -v cmake &> /dev/null; then
    echo "✓ CMake found: $(cmake --version | head -1)"
else
    echo "✗ CMake not found!"
    exit 1
fi

# Verificar MySQL
if mysql_config --version &> /dev/null; then
    echo "✓ MySQL client found"
else
    echo "⚠ MySQL client not found (some features disabled)"
fi

echo "All dependencies checked!"
