#include "Game.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

Game::Game()
    : window(sf::VideoMode({ 800, 600 }), "DSA Game Engine"),
    state(GameState::Menu),
    quadTree(sf::FloatRect({ 0.f, 0.f }, { 800.f, 600.f }))
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "MENU\nPress ENTER to start\n";
}

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

void Game::processEvents()
{
    while (auto e = window.pollEvent())
    {
        if (e->is<sf::Event::Closed>())
            window.close();
    }
}

void Game::update(float dt)
{
    if (state == GameState::Menu)
        updateMenu();
    else if (state == GameState::Playing)
        updateGame(dt);
    else
        updateGameOver();
}

/* ================= MENU ================= */

void Game::updateMenu()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
    {
        resetGame();
        state = GameState::Playing;
    }
}

/* ================= GAME ================= */

void Game::updateGame(float dt)
{
    // ---- INPUT ----
    sf::Vector2f move(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) move.y -= 160.f * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) move.y += 160.f * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) move.x -= 160.f * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) move.x += 160.f * dt;

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
        collisionEvents.push_front(quadTree.getQuadrant(center));
    }

    if (collisionEvents.getHead())
    {
        float survived = survivalClock.getElapsedTime().asSeconds();
        survivalStack.push(survived);

        std::cout << "GAME OVER\nSurvived: "
            << survived << "s\n";
        std::cout << "Press R to retry\n";

        state = GameState::GameOver;
    }
}

/* ================= GAME OVER ================= */

void Game::updateGameOver()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
    {
        std::cout << "MENU\nPress ENTER to start\n";
        state = GameState::Menu;
    }
}

/* ================= RESET ================= */

void Game::resetGame()
{
    // RESET entity container (no clear())
    entities = DynamicArray<Block>();

    // Player
    entities.push_back(Block(
        { 50.f, 50.f },
        { 30.f, 275.f },
        sf::Color::Green
    ));

    // Enemy
    enemy.setRadius(18.f);
    enemy.setFillColor(sf::Color::Cyan);
    enemy.setPosition({ 400.f, 300.f });

    float a = static_cast<float>(std::rand()) / RAND_MAX * 6.28318f;
    enemyVelocity = {
        std::cos(a) * 320.f,
        std::sin(a) * 320.f
    };

    survivalClock.restart();
}


/* ================= RENDER ================= */

void Game::render()
{
    window.clear(sf::Color::Black);

    if (state == GameState::Playing)
    {
        window.draw(entities[0].shape);
        window.draw(enemy);
    }

    window.display();
}
