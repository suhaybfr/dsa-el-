#include "Game.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Menu.hpp"


constexpr float PLAYER_SPEED = 250.f;
constexpr float ENEMY_SPEED = 1550.f;

// ---------------- Constructor ----------------
Game::Game()
    : window(sf::VideoMode({ 800, 600 }), "DSA Game Engine"),
    state(GameState::Menu),
    quadTree(sf::FloatRect({ 0.f, 0.f }, { 800.f, 600.f })),
    hasPlayedBefore(false)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // ---- GUI buttons ----
    startButton.setSize({ 220.f, 60.f });
    startButton.setPosition({ 290.f, 220.f });
    startButton.setFillColor(sf::Color::White);

    exitButton.setSize({ 220.f, 60.f });
    exitButton.setPosition({ 290.f, 310.f });
    exitButton.setFillColor(sf::Color::White);

    retryButton.setSize({ 220.f, 60.f });
    retryButton.setPosition({ 290.f, 270.f });
    retryButton.setFillColor(sf::Color::White);

    std::cout << "MENU\nClick TOP box to START\nClick BOTTOM box to EXIT\n";
}

// ---------------- Main loop ----------------
void Game::run()
{
    while (window.isOpen())
    {
        float dt = deltaClock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

// ---------------- Events ----------------
void Game::processEvents()
{
    while (auto e = window.pollEvent())
    {
        if (e->is<sf::Event::Closed>())
            window.close();
    }
}

// ---------------- Update dispatcher ----------------
void Game::update(float dt)
{
    if (state == GameState::Menu)
        updateMenu();
    else if (state == GameState::Playing)
        updateGame(dt);
    else
        updateGameOver();
}

// ---------------- Mouse helpers ----------------
bool Game::isMouseOver(const sf::RectangleShape& button) const
{
    sf::Vector2f mouse =
        window.mapPixelToCoords(
            sf::Mouse::getPosition(window));

    return button.getGlobalBounds().contains(mouse);
}

bool Game::isMouseClicked(const sf::RectangleShape& button) const
{
    return isMouseOver(button) &&
        sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

// ================= MENU =================
void Game::updateMenu()
{
    startButton.setFillColor(
        isMouseOver(startButton) ? sf::Color(180, 180, 180)
        : sf::Color::White);

    exitButton.setFillColor(
        isMouseOver(exitButton) ? sf::Color(180, 180, 180)
        : sf::Color::White);

    static bool printed = false;
    if (!printed)
    {
        if (hasPlayedBefore)
            std::cout << "MENU\nClick TOP box to CONTINUE\n";
        else
            std::cout << "MENU\nClick TOP box to START\n";

        std::cout << "Click BOTTOM box to EXIT\n";
        printed = true;
    }

    if (isMouseClicked(startButton))
    {
        resetGame();
        state = GameState::Playing;
        printed = false;
    }

    if (isMouseClicked(exitButton))
    {
        window.close();
    }
}

// ================= GAME =================
void Game::updateGame(float dt)
{
    // ---- INPUT ----
    sf::Vector2f move(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) move.y -= PLAYER_SPEED * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) move.y += PLAYER_SPEED * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) move.x -= PLAYER_SPEED * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) move.x += PLAYER_SPEED * dt;

    inputQueue.push({ move });

    Command cmd;
    while (!inputQueue.empty())
        if (inputQueue.pop(cmd))
            entities[0].shape.move(cmd.move);

    // ---- PLAYER BOUNDS ----
    sf::Vector2f p = entities[0].shape.getPosition();
    auto size = entities[0].shape.getSize();

    if (p.x < 0) p.x = 0;
    if (p.y < 0) p.y = 0;
    if (p.x + size.x > 800) p.x = 800 - size.x;
    if (p.y + size.y > 600) p.y = 600 - size.y;

    entities[0].shape.setPosition(p);

    // ---- ENEMY ----
    enemy.move(enemyVelocity * dt);

    sf::Vector2f e = enemy.getPosition();
    float r = enemy.getRadius();

    if (e.x <= 0 || e.x + 2 * r >= 800) enemyVelocity.x *= -1;
    if (e.y <= 0 || e.y + 2 * r >= 600) enemyVelocity.y *= -1;

    // ---- COLLISION ----
    collisionEvents.clear();
    quadTree.entities.clear();

    sf::Vector2f center = p + size / 2.f;
    quadTree.insert(0, center);

    if (entities[0].shape.getGlobalBounds()
        .findIntersection(enemy.getGlobalBounds())
        .has_value())
    {
        collisionEvents.push_front(
            quadTree.getQuadrant(center));
    }

    if (collisionEvents.getHead())
    {
        float survived =
            survivalClock.getElapsedTime().asSeconds();

        survivalStack.push(survived);
        hasPlayedBefore = true;

        std::cout << "GAME OVER\nSurvived: "
            << survived << "s\n";

        state = GameState::GameOver;
    }
}

// ================= GAME OVER =================
void Game::updateGameOver()
{
    retryButton.setFillColor(
        isMouseOver(retryButton) ? sf::Color(180, 180, 180)
        : sf::Color::White);

    if (isMouseClicked(retryButton))
    {
        state = GameState::Menu;
    }
}

// ================= RESET =================
void Game::resetGame()
{
    entities = DynamicArray<Block>();

    entities.push_back(Block(
        { 50.f, 50.f },
        { 30.f, 275.f },
        sf::Color::Green
    ));

    enemy.setRadius(18.f);
    enemy.setFillColor(sf::Color::Cyan);
    enemy.setPosition({ 400.f, 300.f });

    float a =
        static_cast<float>(std::rand()) / RAND_MAX * 6.28318f;

    enemyVelocity = {
        std::cos(a) * ENEMY_SPEED,
        std::sin(a) * ENEMY_SPEED
    };

    survivalClock.restart();
}

// ================= RENDER =================
void Game::render()
{
    window.clear(sf::Color::Black);

    if (state == GameState::Menu)
    {
        window.draw(startButton);
        window.draw(exitButton);
    }
    else if (state == GameState::Playing)
    {
        window.draw(entities[0].shape);
        window.draw(enemy);
    }
    else
    {
        window.draw(retryButton);
    }

    window.display();
}
