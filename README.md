# DSA-EL  
*A simple engine-style game project built with SFML 3 and custom data structures*

---

## 📌 Overview

This project is a **minimal game engine–style implementation** developed using **SFML 3** and **custom data structures** as part of a Data Structures & Algorithms learning exercise.

The goal of the project is to demonstrate:
- Practical usage of core data structures
- Separation of game logic from the entry point
- A basic state-driven game loop (Menu → Playing → Game Over)

---

## 🛠️ SFML Setup (Visual Studio)

To set up **SFML 3** with Visual Studio, please refer to one of the following resources:

- 📺 **YouTube Tutorial**:  
  https://www.youtube.com/watch?v=qvg8BXXWpCE

- 🌐 **Official SFML Documentation**:  
  https://www.sfml-dev.org/tutorials/3.0/getting-started/visual-studio/

> ⚠️ SFML binaries are **not included** in this repository and must be set up separately.

---

## ✨ Features

- Custom **Dynamic Array** (entity storage)
- **Queue** for input handling
- **Stack** for survival-time history
- **Linked List** for collision events
- **QuadTree-based spatial partitioning** for collision filtering
- **State-based game loop** (Menu, Playing, Game Over)
- Rendering using **SFML 3**

---

## 🎮 Controls

| Key | Action |
|----|-------|
| `W / A / S / D` | Move player |
| `Enter` | Start game |
| `R` | Retry after game over |

---

## 📁 Project Structure

```text
main.cpp        → Entry point  
Game.hpp/.cpp   → Core game loop & logic  
DataStructures/ → Custom data structure implementations  
