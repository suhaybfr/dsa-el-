# DSA-EL 🎮
*A SFML 3 game engine demo showcasing 6 custom data structures*

---

## 📌 Overview

A **game engine–style project** built for a Data Structures & Algorithms course. Features two complete games demonstrating practical usage of custom data structures.

### 🎮 Game Modes

| Mode | Description |
|------|-------------|
| **Survival** | Top-down arena - collect 10 stars while dodging a bouncing enemy |
| **Dash** | Geometry Dash-style auto-scrolling platformer with spikes and orbs |

---

## 🧱 Data Structures Used

| Structure | Implementation |
|-----------|----------------|
| **DynamicArray** | Entity storage, platforms, particles, obstacles |
| **Queue** | Input command buffering (FIFO) |
| **Stack** | Game state management, pause/resume (LIFO) |
| **LinkedList** | Score history tracking |
| **QuadTree** | Spatial partitioning for collision detection |
| **SceneNode** | UI hierarchy (buttons, panels, labels) |

---

## 🛠️ Engine Systems

- **ResourceManager** - Texture & font caching
- **InputManager** - Centralized input handling
- **UI System** - Buttons, Panels, Labels, HUD
- **ParticleSystem** - Visual effects
- **Physics** - Platform collision & gravity

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| `W/A/S/D` | Move (Survival Mode) |
| `Space` / `Click` | Jump (Dash Mode) |
| `ESC` | Pause/Resume |

---

## 🚀 Setup

1. **SFML 3 Required** - Not included. Follow the [official guide](https://www.sfml-dev.org/tutorials/3.0/getting-started/visual-studio/) or [this tutorial](https://www.youtube.com/watch?v=qvg8BXXWpCE)
2. Open [DSA_EL.slnx](cci:7://file:///c:/vss/DSA_EL/DSA_EL.slnx:0:0-0:0) in Visual Studio
3. Build & Run

---

## 📁 Structure
# DSA-EL 🎮
*A SFML 3 game engine demo showcasing 6 custom data structures*

---

## 📌 Overview

A **game engine–style project** built for a Data Structures & Algorithms course. Features two complete games demonstrating practical usage of custom data structures.

### 🎮 Game Modes

| Mode | Description |
|------|-------------|
| **Survival** | Top-down arena - collect 10 stars while dodging a bouncing enemy |
| **Dash** | Geometry Dash-style auto-scrolling platformer with spikes and orbs |

---

## 🧱 Data Structures Used

| Structure | Implementation |
|-----------|----------------|
| **DynamicArray** | Entity storage, platforms, particles, obstacles |
| **Queue** | Input command buffering (FIFO) |
| **Stack** | Game state management, pause/resume (LIFO) |
| **LinkedList** | Score history tracking |
| **QuadTree** | Spatial partitioning for collision detection |
| **SceneNode** | UI hierarchy (buttons, panels, labels) |

---

## 🛠️ Engine Systems

- **ResourceManager** - Texture & font caching
- **InputManager** - Centralized input handling
- **UI System** - Buttons, Panels, Labels, HUD
- **ParticleSystem** - Visual effects
- **Physics** - Platform collision & gravity

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| `W/A/S/D` | Move (Survival Mode) |
| `Space` / `Click` | Jump (Dash Mode) |
| `ESC` | Pause/Resume |

---

## 🚀 Setup

1. **SFML 3 Required** - Not included. Follow the [official guide](https://www.sfml-dev.org/tutorials/3.0/getting-started/visual-studio/) or [this tutorial](https://www.youtube.com/watch?v=qvg8BXXWpCE)
2. Open [DSA_EL.slnx](cci:7://file:///c:/vss/DSA_EL/DSA_EL.slnx:0:0-0:0) in Visual Studio
3. Build & Run

---

## 📁 Structure
| File | Description |
|------|-------------|
| [DSA_EL.cpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/DSA_EL.cpp:0:0-0:0) | Entry point + console menu |
| `Game.cpp/hpp` | Survival game logic |
| [Game2.cpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/Game2.cpp:0:0-0:0) | Dash game logic |
| [DynamicArray.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/DynamicArray.hpp:0:0-0:0) | Custom dynamic array |
| [Queue.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/Queue.hpp:0:0-0:0) | Circular queue |
| [Stack.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/Stack.hpp:0:0-0:0) | Stack implementation |
| [LinkedList.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/LinkedList.hpp:0:0-0:0) | Singly linked list |
| [QuadTree.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/QuadTree.hpp:0:0-0:0) | Spatial partitioning |
| [SceneNode.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/SceneNode.hpp:0:0-0:0) | Tree hierarchy |
| [UI.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/UI.hpp:0:0-0:0) | Button, Panel, Label, HUD |
| [Particles.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/Particles.hpp:0:0-0:0) | Particle effects |
| [ResourceManager.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/ResourceManager.hpp:0:0-0:0) | Asset caching |
| [InputManager.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/InputManager.hpp:0:0-0:0) | Input handling |
| [Colors.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/Colors.hpp:0:0-0:0) | Color palette |
| [Physics.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/Physics.hpp:0:0-0:0) | Collision helpers |
| [Block.hpp](cci:7://file:///c:/vss/DSA_EL/DSA_EL/Block.hpp:0:0-0:0) | Game entity struct |


