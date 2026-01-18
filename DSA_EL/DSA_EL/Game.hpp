#pragma once
#include <SFML/Graphics.hpp>

// data structures
#include "DynamicArray.hpp"
#include "Queue.hpp"
#include "Stack.hpp"
#include "LinkedList.hpp"
#include "Block.hpp"
#include "QuadTree.hpp"

// ================= COMMON TYPES =================

struct Command
{
    sf::Vector2f move;
};

enum class GameState
{
    Menu,
    Playing,
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
    void update(float dt);
    void render();

    void updateMenu();
    void updateGame(float dt);
    void updateGameOver();
    void resetGame();

    bool isMouseOver(const sf::RectangleShape& button) const;
    bool isMouseClicked(const sf::RectangleShape& button) const;

private:
    sf::RenderWindow window;
    GameState state;

    sf::Clock deltaClock;
    sf::Clock survivalClock;

    // data structures
    DynamicArray<Block> entities;
    Queue<Command> inputQueue;
    Stack<float> survivalStack;
    LinkedList<int> collisionEvents;
    QuadTree quadTree;

    // enemy
    sf::CircleShape enemy;
    sf::Vector2f enemyVelocity;

    // GUI
    sf::RectangleShape startButton;
    sf::RectangleShape exitButton;
    sf::RectangleShape retryButton;

    bool hasPlayedBefore;
};

// ================= GAME 2 (PLATFORMER) =================
// Exposed as another engine-supported game
void runGame2();
