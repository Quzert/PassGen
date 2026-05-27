
# PassGen

Утилита для генерации криптографически стойких паролей на основе мастер-пароля и названия сайта.

## Возможности

- 🔐 Генерация паролей с использованием libsodium
- 📋 Встроенные пресеты (буквы, цифры, спецсимволы)
- 🎯 Кастомные алфавиты
- 🚀 Быстрая и простая установка

## Установка

### Требования

- CMake 3.16+
- C++17 компилятор
- libsodium

### Из исходников

```bash
# Сборка
bash build.sh

# Установка
cd build
sudo make install
```

### Из готового пакета

```bash
# Создание пакета
bash package.sh

# Установка пакета
sudo tar -xzf passgen-*.tar.gz -C /
```

## Использование

```bash
# Генерация пароля
passgen <мастер_пароль> <название_сайта>

# Пример
passgen mypass example.com
```

### Опции

```bash
-l, --length <длина>        Длина пароля (по умолчанию: 16)
-s, --require-special       Требовать спецсимвол
-A, --preset <name>         Использовать пресет (default, digits, lowercase и т.д.)
-f, --file <path>           Загрузить алфавит из файла
-a, --alphabet <string>     Использовать буквенный набор напрямую
--list                      Показать доступные пресеты
```

### Примеры

```bash
# С кастомной длиной
passgen mypass example.com -l 32

# С пресетом (только цифры)
passgen mypass example.com -A digits

# Со спецсимволом (обязательно)
passgen mypass example.com -s

# Со своим алфавитом
passgen mypass example.com -a "abcdef123"
```

## Пресеты

- `default` - буквы + цифры + спецсимволы
- `digits` - только цифры
- `lowercase` - только строчные буквы
- `lowercase-digits` - строчные буквы + цифры
- `no-special` - буквы и цифры (без спецсимволов)

Посмотреть все пресеты:

```bash
passgen --list
```
