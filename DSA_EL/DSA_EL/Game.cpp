#include "Game.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

constexpr float PLAYER_SPEED = 320.f;
constexpr float ENEMY_SPEED = 450.f;
constexpr float WINDOW_WIDTH = 800.f;
constexpr float WINDOW_HEIGHT = 600.f;
constexpr int COLLECTIBLES_TO_WIN = 10;
constexpr float COLLECTIBLE_SPAWN_INTERVAL = 2.5f;

// ---------------- Constructor ----------------
Game::Game()
{
    // Initialize window
    window.create(sf::VideoMode({ (unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT }), "DSA Survival");
    window.setFramerateLimit(60);

    // Initialize random seed
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Load font
    font = ResourceManager::getInstance().getFont("C:/Windows/Fonts/arial.ttf");

    // Initialize player (Square/Block)
    player.shape.setSize({ 30.f, 30.f });
    player.shape.setFillColor(Colors::Player);
    player.shape.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
    player.shape.setOutlineThickness(3.f);
    player.shape.setOutlineColor(sf::Color(100, 255, 200));
    
    std::cout << ">>> SURVIVAL MODE STARTED <<<\n";
    std::cout << "Goal: Collect " << COLLECTIBLES_TO_WIN << " stars.\n";
    std::cout << "[DSA] DynamicArray: Storing collectibles\n";
    std::cout << "[DSA] Queue: Buffering player input (FIFO)\n";
    std::cout << "[DSA] Stack: Managing game states (LIFO)\n";
    std::cout << "[DSA] LinkedList: Tracking score history\n";

    // Initialize enemy
    enemy.setRadius(25.f);
    enemy.setFillColor(Colors::Enemy);
    enemy.setPosition({ 100.f, 100.f });
    enemy.setOutlineThickness(3.f);
    enemy.setOutlineColor(sf::Color(255, 100, 100));

    // Random initial velocity (Avoid cardinal directions)
    float angle;
    do {
        angle = static_cast<float>((std::rand() % 360) * 3.14159 / 180.0);
    } while (std::abs(std::cos(angle)) < 0.3f || std::abs(std::sin(angle)) < 0.3f);
    
    enemyVelocity = { static_cast<float>(std::cos(angle) * ENEMY_SPEED), static_cast<float>(std::sin(angle) * ENEMY_SPEED) };

    // Initialize HUD
    hud = std::make_unique<HUD>(font);

    // Initialize Buttons
    startButton = std::make_unique<Button>(sf::Vector2f(200.f, 60.f), sf::Vector2f(WINDOW_WIDTH/2.f - 100.f, 300.f), "START", font);
    exitButton = std::make_unique<Button>(sf::Vector2f(200.f, 60.f), sf::Vector2f(WINDOW_WIDTH/2.f - 100.f, 400.f), "EXIT", font);
    retryButton = std::make_unique<Button>(sf::Vector2f(200.f, 50.f), sf::Vector2f(WINDOW_WIDTH/2.f - 100.f, 350.f), "RETRY", font);
    menuButton = std::make_unique<Button>(sf::Vector2f(200.f, 50.f), sf::Vector2f(WINDOW_WIDTH/2.f - 100.f, 420.f), "MENU", font);

    // Game state
    state = GameState::Menu;
    isGameOver = false;
    gameWon = false;
    survivalTime = 0.f;
    collectibleSpawnTimer = 0.f;
    collectiblesCollected = 0;

    // Initialize Grid (Visuals)
    grid.setPrimitiveType(sf::PrimitiveType::Lines);
    for (float x = 0; x < WINDOW_WIDTH; x += 50.f)
    {
        sf::Vertex v1; v1.position = sf::Vector2f(x, 0.f); v1.color = sf::Color(30, 30, 50); grid.append(v1);
        sf::Vertex v2; v2.position = sf::Vector2f(x, WINDOW_HEIGHT); v2.color = sf::Color(30, 30, 50); grid.append(v2);
    }
    for (float y = 0; y < WINDOW_HEIGHT; y += 50.f)
    {
        sf::Vertex v1; v1.position = sf::Vector2f(0.f, y); v1.color = sf::Color(30, 30, 50); grid.append(v1);
        sf::Vertex v2; v2.position = sf::Vector2f(WINDOW_WIDTH, y); v2.color = sf::Color(30, 30, 50); grid.append(v2);
    }
}

// ---------------- Collectible Spawning ----------------
void Game::spawnCollectible()
{
    sf::ConvexShape star;
    star.setPointCount(10); // 5-point star
    for (int i = 0; i < 10; ++i)
    {
        float angle = static_cast<float>(i * 2 * 3.14159f / 10 - 3.14159f / 2);
        float r = (i % 2 == 0) ? 12.f : 6.f; // Outer/Inner radius
        star.setPoint(i, { std::cos(angle) * r, std::sin(angle) * r });
    }
    star.setFillColor(Colors::Warning);
    
    float x = static_cast<float>(std::rand() % (int)(WINDOW_WIDTH - 40.f)) + 20.f;
    float y = static_cast<float>(std::rand() % (int)(WINDOW_HEIGHT - 40.f)) + 20.f;
    
    star.setPosition({ x, y });
    star.setOutlineThickness(2.f);
    star.setOutlineColor(sf::Color(255, 220, 100));
    
    collectibles.push_back(star);
}

// ---------------- Main Loop ----------------
void Game::run()
{
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        
        if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyEvent->code == sf::Keyboard::Key::Escape)
            {
                if (state == GameState::Playing) 
                {
                    stateStack.push(state);  // [DSA] Stack: Push current state
                    state = GameState::Paused;
                    std::cout << "[DSA] Stack: Pushed Playing state (paused)\n";
                }
                else if (state == GameState::Paused)
                {
                    GameState prev;
                    if (stateStack.pop(prev))  // [DSA] Stack: Pop previous state
                    {
                        state = prev;
                        std::cout << "[DSA] Stack: Popped to previous state\n";
                    }
                }
                else if (state == GameState::Menu)
                {
                    window.close();
                }
            }
        }
    }
}

void Game::update(sf::Time deltaTime)
{
    if (state == GameState::Menu)
    {
        startButton->update(window);
        exitButton->update(window);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            
            if (startButton->isHovered())
            {
                state = GameState::Playing;
                // Reset game if needed
                if (isGameOver || gameWon)
                {
                    isGameOver = false;
                    gameWon = false;
                    survivalTime = 0.f;
                    collectiblesCollected = 0;
                    collectibles.clear();
                    player.shape.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
                    
                    // Reset enemy
                    enemy.setPosition({ 100.f, 100.f });
                    
                    float angle;
                    do {
                        angle = static_cast<float>((std::rand() % 360) * 3.14159 / 180.0);
                    } while (std::abs(std::cos(angle)) < 0.3f || std::abs(std::sin(angle)) < 0.3f);
                    
                    enemyVelocity = { static_cast<float>(std::cos(angle) * ENEMY_SPEED), static_cast<float>(std::sin(angle) * ENEMY_SPEED) };
                }
            }
            if (exitButton->isHovered())
            {
                window.close();
            }
        }
        return;
    }

    // Restart/Menu handling
    if (isGameOver || gameWon)
    {
        retryButton->update(window);
        menuButton->update(window);

        bool restart = false;
        bool menu = false;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if (retryButton->isHovered()) restart = true;
            if (menuButton->isHovered()) menu = true;
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) restart = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) menu = true;

        if (restart)
        {
            // Restart
            isGameOver = false;
            gameWon = false;
            survivalTime = 0.f;
            collectiblesCollected = 0;
            collectibles.clear();
            player.shape.setPosition({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f });
            
            enemy.setPosition({ 100.f, 100.f });
            
            float angle;
            do {
                angle = static_cast<float>((std::rand() % 360) * 3.14159 / 180.0);
            } while (std::abs(std::cos(angle)) < 0.3f || std::abs(std::sin(angle)) < 0.3f);
            
            enemyVelocity = { static_cast<float>(std::cos(angle) * ENEMY_SPEED), static_cast<float>(std::sin(angle) * ENEMY_SPEED) };
        }
        
        if (menu)
        {
            state = GameState::Menu;
        }
            
        return;
    }

    float dt = deltaTime.asSeconds();
    survivalTime += dt;

    // ---- INPUT HANDLING ----
    sf::Vector2f move(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) move.y -= PLAYER_SPEED * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) move.y += PLAYER_SPEED * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) move.x -= PLAYER_SPEED * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) move.x += PLAYER_SPEED * dt;

    inputQueue.push({ move });  // [DSA] Queue: Push input command

    Command cmd;
    while (!inputQueue.empty())
        if (inputQueue.pop(cmd))  // [DSA] Queue: Pop and apply in FIFO order
            player.shape.move(cmd.move);

    // ---- PLAYER BOUNDS ----
    sf::Vector2f p = player.shape.getPosition();
    auto size = player.shape.getSize();

    if (p.x < 0) p.x = 0;
    if (p.y < 0) p.y = 0;
    if (p.x + size.x > WINDOW_WIDTH) p.x = WINDOW_WIDTH - size.x;
    if (p.y + size.y > WINDOW_HEIGHT) p.y = WINDOW_HEIGHT - size.y;

    player.shape.setPosition(p);

    // ---- ENEMY PHYSICS (Simple Reflection) ----
    enemy.move(enemyVelocity * dt);

    sf::Vector2f e = enemy.getPosition();
    float r = enemy.getRadius();

    if (e.x <= 0 || e.x + 2 * r >= WINDOW_WIDTH)
    {
        enemyVelocity.x *= -1;
    }
    if (e.y <= 0 || e.y + 2 * r >= WINDOW_HEIGHT)
    {
        enemyVelocity.y *= -1;
    }

    // ---- SPAWN COLLECTIBLES ----
    collectibleSpawnTimer += dt;
    
    int visibleStars = 0;
    for (int i = 0; i < collectibles.size(); ++i)
    {
        if (collectibles[i].getPosition().x >= 0)
            visibleStars++;
    }
    
    if (collectibleSpawnTimer >= COLLECTIBLE_SPAWN_INTERVAL && visibleStars < 5)
    {
        collectibleSpawnTimer = 0.f;
        spawnCollectible();
    }

    // ---- COLLECT ITEMS ----
    sf::FloatRect playerBounds = player.shape.getGlobalBounds();
    for (int i = 0; i < collectibles.size(); ++i)
    {
        if (collectibles[i].getPosition().x >= 0 &&
            playerBounds.findIntersection(collectibles[i].getGlobalBounds()).has_value())
        {
            collectibles[i].setPosition({ -100.f, -100.f });
            collectiblesCollected++;
            particles.emit(player.shape.getPosition() + sf::Vector2f(15.f, 15.f), 10, Colors::Warning);
            
            std::cout << "[GAME] Collected! Total: " << collectiblesCollected << "/" << COLLECTIBLES_TO_WIN << "\n";
            std::cout << "[DSA] DynamicArray size: " << collectibles.size() << " items\n";
            
            if (collectiblesCollected >= COLLECTIBLES_TO_WIN)
            {
                gameWon = true;
                particles.emit({ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f }, 50, Colors::Success);
                std::cout << ">>> YOU WON! Time: " << survivalTime << "s <<<\n";
            }
        }
    }

    // Check collision with enemy
    sf::Vector2f playerCenter = player.shape.getPosition() + sf::Vector2f(15.f, 15.f);
    sf::Vector2f enemyCenter = enemy.getPosition() + sf::Vector2f(25.f, 25.f);
    float dist = std::sqrt(std::pow(playerCenter.x - enemyCenter.x, 2) + std::pow(playerCenter.y - enemyCenter.y, 2));

    if (dist < 15.f + 25.f) // Square radius approx
    {
        isGameOver = true;
        particles.emit(playerCenter, 30, Colors::Danger);
        
        // [DSA] LinkedList: Track score history
        scoreHistory.push_front(collectiblesCollected);
        std::cout << ">>> GAME OVER! Time: " << survivalTime << "s <<<\n";
        std::cout << "[DSA] LinkedList: Score " << collectiblesCollected << " added to history\n";
    }

    hud->update(survivalTime, collectiblesCollected);
    particles.update(dt);
}

void Game::render()
{
    window.clear(Colors::Background);

    // Grid (Cached)
    window.draw(grid);

    if (state == GameState::Menu)
    {
        if (font)
        {
            sf::Text title(*font, "SURVIVAL", 60);
            title.setFillColor(Colors::Accent);
            sf::FloatRect bounds = title.getLocalBounds();
            title.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 150.f });
            window.draw(title);
        }
        startButton->draw(window);
        exitButton->draw(window);
        
        // Show last score if returning from game? Optional polish.
        
        window.display();
        return;
    }

    // Draw Entities (Playing State)
    if (!isGameOver)
    {
        sf::RectangleShape glow = player.shape;
        glow.setSize(glow.getSize() + sf::Vector2f(10.f, 10.f));
        glow.setPosition(glow.getPosition() - sf::Vector2f(5.f, 5.f));
        glow.setFillColor(sf::Color(100, 200, 255, 50));
        window.draw(glow);
        window.draw(player.shape);
    }

    sf::CircleShape enemyGlow = enemy;
    enemyGlow.setRadius(enemy.getRadius() + 5.f);
    enemyGlow.setPosition(enemy.getPosition() - sf::Vector2f(5.f, 5.f));
    enemyGlow.setFillColor(sf::Color(255, 50, 50, 50));
    window.draw(enemyGlow);
    window.draw(enemy);

    for (int i = 0; i < collectibles.size(); ++i)
    {
        if (collectibles[i].getPosition().x >= 0)
        {
            sf::ConvexShape glow = collectibles[i];
            glow.setScale({ 1.2f, 1.2f }); // Glow for stars
            glow.setFillColor(sf::Color(255, 220, 100, 50));
            window.draw(glow);
            window.draw(collectibles[i]);
        }
    }
    
    particles.draw(window);
    hud->draw(window);

    if (isGameOver)
    {
        sf::RectangleShape overlay({ WINDOW_WIDTH, WINDOW_HEIGHT });
        overlay.setFillColor(Colors::Overlay);
        window.draw(overlay);

        if (font)
        {
            sf::Text text(*font, "GAME OVER", 50);
            text.setFillColor(Colors::Danger);
            sf::FloatRect bounds = text.getLocalBounds();
            text.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 200.f });
            window.draw(text);
        }
        retryButton->draw(window);
        menuButton->draw(window);
    }

    if (gameWon)
    {
        sf::RectangleShape overlay({ WINDOW_WIDTH, WINDOW_HEIGHT });
        overlay.setFillColor(Colors::Overlay);
        window.draw(overlay);

        if (font)
        {
            sf::Text text(*font, "YOU WON!", 50);
            text.setFillColor(Colors::Success);
            sf::FloatRect bounds = text.getLocalBounds();
            text.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 200.f });
            window.draw(text);
            
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "Time: %.2fs", survivalTime);
            sf::Text timeText(*font, buffer, 30);
            timeText.setFillColor(Colors::Text);
            bounds = timeText.getLocalBounds();
            timeText.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 280.f });
            window.draw(timeText);
        }
        retryButton->draw(window);
        menuButton->draw(window);
    }

    // Pause Screen
    if (state == GameState::Paused)
    {
        sf::RectangleShape overlay({ WINDOW_WIDTH, WINDOW_HEIGHT });
        overlay.setFillColor(Colors::Overlay);
        window.draw(overlay);

        sf::RectangleShape panel({ 300.f, 180.f });
        panel.setPosition({ WINDOW_WIDTH / 2.f - 150.f, WINDOW_HEIGHT / 2.f - 90.f });
        panel.setFillColor(Colors::PanelBackground);
        panel.setOutlineThickness(2.f);
        panel.setOutlineColor(Colors::Accent);
        window.draw(panel);

        if (font)
        {
            sf::Text pauseText(*font, "PAUSED", 40);
            pauseText.setFillColor(Colors::Accent);
            sf::FloatRect bounds = pauseText.getLocalBounds();
            pauseText.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, WINDOW_HEIGHT / 2.f - 60.f });
            window.draw(pauseText);

            sf::Text hint(*font, "Press ESC to Resume", 18);
            hint.setFillColor(Colors::TextDim);
            bounds = hint.getLocalBounds();
            hint.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, WINDOW_HEIGHT / 2.f + 20.f });
            window.draw(hint);
        }
    }

    window.display();
}
