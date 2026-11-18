# Лабораторна робота №2 (ТПП) — Варіант 18
**Виконавець:** Надійко Роман Олександрович

## Склад проєкту
- `task1_graph.cpp` — побудова графіка функції $y = \frac{x}{(x+1)^2}$ на інтервалі $0 \le x \le 4$ засобами WinAPI / GDI.
- `task2_scene.cpp` — морська композиція з класами `Ship`, `Bird`, `Cloud`, `Sun` та обробкою повідомлень `WM_PAINT`, `WM_LBUTTONDOWN`, `WM_KEYDOWN`, `WM_SIZE`, `WM_DESTROY`.
- `Makefile` — крос-компіляція у GitHub Codespaces (`x86_64-w64-mingw32-g++ -mwindows -municode -lgdi32 -luser32`).
- `.github/workflows/build-winapi.yml` — автоматична збірка `.exe` файлів у GitHub Actions.

## Збірка у GitHub Codespaces
```bash
sudo apt-get update && sudo apt-get install -y g++-mingw-w64-x86-64
make all
```
