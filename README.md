# ImagoRef Mobile

Starter mobile shell for the ImagoRef ecosystem on `QML + C++`.

Current scope:

- mobile-first стартовый экран;
- выбор аккаунта;
- выбор синхронизированной доски;
- сохранён фирменный стиль темы `Imago`;
- редактирование пока не реализовано и остаётся на desktop-версии.

Build locally:

```bash
cmake -S . -B build
cmake --build build
./build/ImagoRefMobile.app/Contents/MacOS/ImagoRefMobile
```

```bash
cd /Users/sergejasin/Documents/ImagoRef-mobile && cmake -S . -B build && cmake --build build && ./build/ImagoRefMobile.app/Contents/MacOS/ImagoRefMobile
```
