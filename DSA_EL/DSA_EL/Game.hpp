#pragma once
#include <SFML/Graphics.hpp>

// Data structures
#include "DynamicArray.hpp"
#include "Queue.hpp"
#include "Stack.hpp"
#include "LinkedList.hpp"
#include "QuadTree.hpp"
#include "SceneNode.hpp"

// Engine systems
#include "Block.hpp"
#include "ResourceManager.hpp"
#include "Colors.hpp"
#include "UI.hpp"
#include "Particles.hpp"
#include "InputManager.hpp"



struct Command
{
    sf::Vector2f move;
};

enum class GameState
{
    Menu,
    Playing,
    Paused,
    GameOver
};

// ================= GAME 1 (SURVIVAL) =================

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void spawnCollectible();

private:
    sf::RenderWindow window;
    
    // Resources
    sf::Font* font;

    // Data structures (DSA Demonstration)
    Block player; // Square
    DynamicArray<sf::ConvexShape> collectibles; // Stars - Dynamic resizing array
    Queue<Command> inputQueue;                   // FIFO input processing
    Stack<GameState> stateStack;                 // LIFO pause/resume
    LinkedList<int> scoreHistory;                // Score tracking linked list
    
    // Particle system
    ParticleSystem particles;

    // Enemy
    sf::CircleShape enemy;
    sf::Vector2f enemyVelocity;

    // UI
    std::unique_ptr<HUD> hud;
    
    // Menu UI (Added for proper menu support)
    std::unique_ptr<Button> startButton;
    std::unique_ptr<Button> exitButton;
    std::unique_ptr<Button> retryButton;
    std::unique_ptr<Button> menuButton;
    
    // Background Grid (Cached for performance)
    sf::VertexArray grid;

    // Game state
    GameState state;
    bool isGameOver;
    bool gameWon;
    float survivalTime;
    float collectibleSpawnTimer;
    int collectiblesCollected;
};

// ================= GAME 2 (DASH) =================
void runGame2();
