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
DSA_EL/ ├── DSA_EL.cpp # Entry point + console menu ├── Game.cpp/hpp # Survival game logic ├── Game2.cpp # Dash game logic ├── DynamicArray.hpp # Custom dynamic array ├── Queue.hpp # Circular queue ├── Stack.hpp # Stack implementation ├── LinkedList.hpp # Singly linked list ├── QuadTree.hpp # Spatial partitioning ├── SceneNode.hpp # Tree hierarchy ├── UI.hpp # Button, Panel, Label, HUD ├── Particles.hpp # Particle effects ├── ResourceManager.hpp # Asset caching ├── InputManager.hpp # Input handling ├── Colors.hpp # Color palette ├── Physics.hpp # Collision helpers └── Block.hpp # Game entity struct

