# DSA Engine Demo

A C++ project exploring Data Structures and Algorithms (DSA) through interactive games using SFML.

## Overview

This project demonstrates the practical application of various custom-implemented data structures in a game development context. The application launches a console menu where users can choose between two different game modes:

1.  **Survival Game**
2.  **Platformer Game**

## Features

### Custom Data Structures
The engine relies on handcrafted implementations of core data structures to manage game entities and logic. You can find these implementations in the header files:
-   **Dynamic Array** (`DynamicArray.hpp`): Resizable array implementation.
-   **Linked List** (`LinkedList.hpp`): Singly linked list.
-   **QuadTree** (`QuadTree.hpp`): Spatial partitioning data structure used for efficient collision detection and querying.
-   **Stack** (`Stack.hpp`): LIFO data structure.
-   **Queue** (`Queue.hpp`): FIFO data structure.

### Game Modes
-   **Survival Game**: A top-down survival experience where you face waves of enemies.
-   **Platformer Game**: A 2D side-scrolling platformer featuring physics interactions and level navigation.

## Dependencies

-   **C++ 17** (or later)
-   **SFML** (Simple and Fast Multimedia Library): Used for rendering, window management, and input handling.

## Building and Running

This project is configured for **Visual Studio** on Windows.

1.  Clone the repository:
    ```bash
    git clone https://github.com/suhaybfr/dsa-el-.git
    ```
2.  Open the solution file (`DSA_EL.slnx` or `DSA_EL.vcxproj`) in Visual Studio.
3.  Ensure the SFML binaries and headers are correctly linked.
    -   *Note: If the SFML DLLs are missing from the output directory, you may need to copy them there.*
4.  Build the solution (Ctrl+Shift+B).
5.  Run the application (F5).
6.  Follow the on-screen console prompts to select a game mode.

## Author
[suhaybfr](https://github.com/suhaybfr)
