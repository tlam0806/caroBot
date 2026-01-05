
# Caro Bot (Gomoku) — C++ / SFML 3

An interactive Gomoku (Caro) game with an Bot opponent written in modern C++ and rendered with SFML 3.

> **Gameplay:** 
> 

https://github.com/user-attachments/assets/553c3b4c-817b-4b37-a395-3f6e7922e6b4


> **Platform:** macOS (tested)

---

## Features
- Play **Human vs Bot**
- Visual UI: move highlight, rollback button, player generation labels
- Portable asset loading (font under `assets/`)

---

## Bot Overview

The bot determine which player is winning by a heuristic evaluation function and searches few moves ahead using minimax algorithm (+ alpha-beta pruning) to find optimal one.
Key ideas implemented / explored:
- Board evaluation based on patterns (open/closed lines, threats, etc.)
- Search depth control and move ordering
- A genetic algorithm is used to optimize heuristic evaluation weights via self-play, evolving successive generations of agents based on game outcomes.

## Evaluation
The bot was evaluated through informal human-vs-bot playtesting against multiple players. 
In these sessions, the bot consistently outperformed human opponents.

These results are based on informal testing and are not intended as a statistically rigorous benchmark.

---

## Project Structure
```txt
assets/                 # runtime assets (e.g., Jetbrain.ttf)
src/                    # C++ source code
CMakeLists.txt          # build config
