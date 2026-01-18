# DSA-EL  
*A simple engine-style game project built with SFML 3 and custom data structures*

---

## 📌 Overview

This project is a **game engine–style implementation** developed using **SFML 3** and **custom data structures** as part of a Data Structures & Algorithms learning exercise.

The goal of the project is to demonstrate:
- Practical usage of core data structures
- Separation of game logic from the entry point
- A console-based menu to select between different game modes:
  1.  **Survival Game**: A top-down survival experience.
  2.  **Platformer Game**: A side-scrolling platformer with physics.

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
- **State-based game loop**
- Rendering using **SFML 3**

---

## 🎮 Controls

### General
| Key | Action |
|----|-------|
| `Enter` | Confirm / Start |

### Survival Mode
| Key | Action |
|----|-------|
| `W / A / S / D` | Move player |
| `R` | Retry after game over |

### Platformer Mode
- **Space**: Jump
- **A / D**: Move Left/Right
*(Basic platforming controls)*

---

## 📁 Project Structure

```text
DSA_EL.cpp      → Entry point (Console Menu)
Game.hpp/.cpp   → Survival Game Logic
Game2.cpp       → Platformer Game Logic
*.hpp           → Custom Data Structures (DynamicArray, LinkedList, etc) and Helpers
```
