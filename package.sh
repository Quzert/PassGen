#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="$(dirname "$0")"
PREFIX="${PREFIX:-/usr/local}"
BUILD_DIR="$PROJECT_DIR/build-package"
PACKAGE_DIR="$BUILD_DIR/package"
PACKAGE_NAME="passgen-$(date +%Y%m%d).tar.gz"

echo "Создание пакета passgen..."
echo ""

# Очистка старой сборки для пакета
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"

# Сборка проекта с установкой в временную директорию
echo "Сборка проекта..."
cmake -S "$PROJECT_DIR" -B "$BUILD_DIR" -DCMAKE_INSTALL_PREFIX="$PREFIX"
cmake --build "$BUILD_DIR"

# Установка в DESTDIR
echo "Установка в временную директорию..."
DESTDIR="$PACKAGE_DIR" cmake --install "$BUILD_DIR"

# Упаковка в архив
echo "Упаковка архива..."
cd "$BUILD_DIR"
tar -czf "$PACKAGE_NAME" package/
cd - > /dev/null

# Перемещение архива в корень проекта
mv "$BUILD_DIR/$PACKAGE_NAME" "$PROJECT_DIR/"

# Вывод информации
PACKAGE_PATH="$PROJECT_DIR/$PACKAGE_NAME"
PACKAGE_SIZE=$(du -h "$PACKAGE_PATH" | cut -f1)

echo ""
echo "✓ Пакет создан успешно!"
echo ""
echo "Файл:       $PACKAGE_NAME"
echo "Размер:     $PACKAGE_SIZE"
echo "Путь:       $PACKAGE_PATH"
echo ""
echo "Установка пакета:"
echo "  tar -xzf $PACKAGE_NAME -C /"
echo "  # или в кастомное место:"
echo "  tar -xzf $PACKAGE_NAME --transform='s,^package/usr/local,/opt/passgen,' -C /"
echo ""
echo "После установки программа будет доступна как:"
echo "  passgen --help"
