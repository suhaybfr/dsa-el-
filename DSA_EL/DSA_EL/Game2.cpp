#include "Game.hpp"
#include "Physics.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

// ================= GEOMETRY DASH STYLE GAME =================
// Auto-scrolling, one-button jump, avoid spikes, collect orbs!

namespace
{
    constexpr float WINDOW_WIDTH = 800.f;
    constexpr float WINDOW_HEIGHT = 600.f;
    constexpr float GRAVITY = 2200.f;        // Smoother gravity
    constexpr float JUMP_FORCE = -750.f;      // Balanced jump
    constexpr float GROUND_Y = 480.f;        // Ground level
    constexpr float PLAYER_SIZE = 40.f;
    constexpr float SCROLL_SPEED_START = 350.f;
    constexpr float SCROLL_SPEED_MAX = 550.f;

    // Renamed to avoid conflict with GameState in Game.hp
    enum class DashState
    {
        Menu,
        Playing,
        Crashed,
        Paused
    };

    struct Obstacle
    {
        sf::ConvexShape shape;  // Triangle for spikes
        float x = 0.f;
        bool isSpike = false;           // true = spike, false = block/platform
        bool passed = false;
    };

    struct Orb
    {
        sf::CircleShape shape;
        float x = 0.f;
        bool collected = false;
    };
}

static bool isMouseOverBox(const sf::RenderWindow& window, const sf::RectangleShape& box)
{
    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    return box.getGlobalBounds().contains(mouse);
}

void runGame2()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "DSA Dash");
    window.setFramerateLimit(60);

    sf::Font* mainFont = ResourceManager::getInstance().getFont("C:/Windows/Fonts/arial.ttf");

    DashState state = DashState::Menu;
    Stack<DashState> stateStack;

    sf::Clock deltaClock;
    sf::Clock gameClock;

    // Player - rotating cube like Geometry Dash
    sf::RectangleShape player;
    player.setSize({ PLAYER_SIZE, PLAYER_SIZE });
    player.setOrigin({ PLAYER_SIZE / 2.f, PLAYER_SIZE / 2.f });  // Center origin for rotation
    player.setFillColor(Colors::Player);
    player.setOutlineThickness(3.f);
    player.setOutlineColor(sf::Color(100, 255, 200));

    float playerY = GROUND_Y - PLAYER_SIZE / 2.f;
    float yVelocity = 0.f;
    float rotation = 0.f;
    bool isGrounded = true;
    bool isDead = false;

    int score = 0;
    int highScore = 0;
    int attempts = 1;  // Start at 1
    bool isFirstRun = true;
    float distance = 0.f;
    float scrollSpeed = SCROLL_SPEED_START;

    // Ground
    sf::RectangleShape ground;
    ground.setSize({ WINDOW_WIDTH, WINDOW_HEIGHT - GROUND_Y });
    ground.setPosition({ 0.f, GROUND_Y });
    ground.setFillColor(Colors::Platform);

    // Obstacles and orbs
    DynamicArray<Obstacle> obstacles;
    DynamicArray<Orb> orbs;

    // Particle system
    ParticleSystem particles;

    auto spawnSpike = [&](float x) {
        Obstacle obs;
        obs.shape.setPointCount(3);
        obs.shape.setPoint(0, sf::Vector2f(0.f, 0.f));
        obs.shape.setPoint(1, sf::Vector2f(20.f, -40.f));
        obs.shape.setPoint(2, sf::Vector2f(40.f, 0.f));
        obs.shape.setPosition({ x, GROUND_Y });
        obs.shape.setFillColor(Colors::Danger);
        obs.shape.setOutlineThickness(2.f);
        obs.shape.setOutlineColor(sf::Color(255, 100, 100));
        obs.x = x;
        obs.isSpike = true;
        obs.passed = false;
        obstacles.push_back(obs);
    };

    auto spawnBlock = [&](float x, float height) {
        Obstacle obs;
        obs.shape.setPointCount(4);
        obs.shape.setPoint(0, sf::Vector2f(0.f, 0.f));
        obs.shape.setPoint(1, sf::Vector2f(50.f, 0.f));
        obs.shape.setPoint(2, sf::Vector2f(50.f, -height));
        obs.shape.setPoint(3, sf::Vector2f(0.f, -height));
        obs.shape.setPosition({ x, GROUND_Y });
        obs.shape.setFillColor(sf::Color(60, 60, 80));
        obs.shape.setOutlineThickness(2.f);
        obs.shape.setOutlineColor(Colors::Secondary);
        obs.x = x;
        obs.isSpike = false;
        obs.passed = false;
        obstacles.push_back(obs);
    };

    auto spawnOrb = [&](float x, float y) {
        Orb orb;
        orb.shape.setRadius(15.f);
        orb.shape.setPosition({ x, y });
        orb.shape.setFillColor(Colors::Warning);
        orb.shape.setOutlineThickness(2.f);
        orb.shape.setOutlineColor(sf::Color(255, 220, 100));
        orb.x = x;
        orb.collected = false;
        orbs.push_back(orb);
    };

    float nextObstacleX = 600.f;

    auto resetGame = [&]() {
        playerY = GROUND_Y - PLAYER_SIZE / 2.f;
        yVelocity = 0.f;
        rotation = 0.f;
        isGrounded = true;
        isDead = false;
        score = 0;
        distance = 0.f;
        scrollSpeed = SCROLL_SPEED_START;
        
        // Only increment attempts after first death
        if (!isFirstRun)
            attempts++;
        isFirstRun = false;

        obstacles = DynamicArray<Obstacle>();
        orbs = DynamicArray<Orb>();
        nextObstacleX = 600.f;

        // Spawn initial obstacles
        for (int i = 0; i < 5; ++i)
        {
            int type = std::rand() % 4;
            if (type == 0 || type == 1)
            {
                spawnSpike(nextObstacleX);
                if (std::rand() % 2 == 0) // Double spike sometimes
                    spawnSpike(nextObstacleX + 45.f);
            }
            else if (type == 2)
            {
                spawnBlock(nextObstacleX, 50.f + (std::rand() % 30));
            }
            else
            {
                spawnSpike(nextObstacleX);
                spawnOrb(nextObstacleX + 20.f, GROUND_Y - 100.f);
            }
            nextObstacleX += 200.f + (std::rand() % 150);
        }

        particles.clear();
        gameClock.restart();
        std::cout << ">>> DASH GAME STARTED (Attempt " << attempts << ") <<<\n";
        std::cout << "[DSA] DynamicArray: Storing obstacles (" << obstacles.size() << " items)\n";
        std::cout << "[DSA] DynamicArray: Storing orbs\n";
        std::cout << "[DSA] Stack: Managing pause/resume states (LIFO)\n";
    };

    // UI Buttons
    auto playButton = std::make_unique<Button>(
        sf::Vector2f(240.f, 60.f),
        sf::Vector2f(WINDOW_WIDTH / 2.f - 120.f, 300.f),
        "PLAY",
        mainFont
    );

    auto backButton = std::make_unique<Button>(
        sf::Vector2f(180.f, 50.f),
        sf::Vector2f(WINDOW_WIDTH / 2.f - 90.f, 390.f),
        "BACK",
        mainFont
    );

    auto retryButton = std::make_unique<Button>(
        sf::Vector2f(220.f, 55.f),
        sf::Vector2f(WINDOW_WIDTH / 2.f - 110.f, 320.f),
        "RETRY",
        mainFont
    );

    auto menuButton = std::make_unique<Button>(
        sf::Vector2f(220.f, 55.f),
        sf::Vector2f(WINDOW_WIDTH / 2.f - 110.f, 400.f),
        "MENU",
        mainFont
    );

    auto resumeButton = std::make_unique<Button>(
        sf::Vector2f(220.f, 55.f),
        sf::Vector2f(WINDOW_WIDTH / 2.f - 110.f, 280.f),
        "RESUME",
        mainFont
    );

    bool wasMousePressed = true;
    bool wasSpacePressed = false;

    while (window.isOpen())
    {
        float dt = deltaClock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;

        // Events
        while (auto e = window.pollEvent())
        {
            if (e->is<sf::Event::Closed>())
                window.close();

            if (auto* keyEvent = e->getIf<sf::Event::KeyPressed>())
            {
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    if (state == DashState::Playing)
                    {
                        stateStack.push(state);
                        state = DashState::Paused;
                        std::cout << "[DSA] Stack: Pushed Playing state (paused)\n";
                    }
                    else if (state == DashState::Paused)
                    {
                        DashState prev;
                        if (stateStack.pop(prev))
                        {
                            state = prev;
                            std::cout << "[DSA] Stack: Popped to previous state\n";
                        }
                    }
                }
            }
        }

        bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        bool mouseClicked = !mousePressed && wasMousePressed;
        wasMousePressed = mousePressed;

        bool spacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
                           sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
                           sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
        bool spaceTapped = spacePressed && !wasSpacePressed;
        wasSpacePressed = spacePressed;

        // =================== UPDATE ===================
        if (state == DashState::Menu)
        {
            playButton->update(window);
            backButton->update(window);

            if (mouseClicked)
            {
                if (isMouseOverBox(window, playButton->getShape()))
                {
                    resetGame();
                    state = DashState::Playing;
                }
                else if (isMouseOverBox(window, backButton->getShape()))
                {
                    window.close();
                }
            }
        }
        else if (state == DashState::Playing)
        {
            // Jump on tap (space or click)
            if ((spaceTapped || mouseClicked) && isGrounded)
            {
                yVelocity = JUMP_FORCE;
                isGrounded = false;
                particles.emit({ 100.f + PLAYER_SIZE / 2.f, playerY + PLAYER_SIZE / 2.f }, 8, Colors::Player);
            }

            // Gravity
            yVelocity += GRAVITY * dt;
            playerY += yVelocity * dt;

            // Ground collision
            if (playerY >= GROUND_Y - PLAYER_SIZE / 2.f)
            {
                playerY = GROUND_Y - PLAYER_SIZE / 2.f;
                yVelocity = 0.f;
                isGrounded = true;
                
                // Snap rotation to nearest 90 degrees when landing
                rotation = std::round(rotation / 90.f) * 90.f;
            }

            // Rotate while in air
            if (!isGrounded)
            {
                rotation += 400.f * dt;  // Spin!
            }

            // Increase speed over time
            scrollSpeed = std::min(SCROLL_SPEED_MAX, SCROLL_SPEED_START + distance * 0.02f);
            distance += scrollSpeed * dt;

            // Move obstacles and orbs
            for (int i = 0; i < obstacles.size(); ++i)
            {
                obstacles[i].shape.move({ -scrollSpeed * dt, 0.f });
                obstacles[i].x -= scrollSpeed * dt;
            }
            for (int i = 0; i < orbs.size(); ++i)
            {
                orbs[i].shape.move({ -scrollSpeed * dt, 0.f });
                orbs[i].x -= scrollSpeed * dt;
            }

            // Spawn new obstacles
            float rightmost = 0.f;
            for (int i = 0; i < obstacles.size(); ++i)
            {
                if (obstacles[i].x > rightmost)
                    rightmost = obstacles[i].x;
            }

            while (rightmost < WINDOW_WIDTH + 400.f)
            {
                float gap = 180.f + (std::rand() % 120);
                float newX = rightmost + gap;

                int type = std::rand() % 5;
                if (type <= 1)
                {
                    spawnSpike(newX);
                    if (std::rand() % 3 == 0)
                        spawnSpike(newX + 45.f);  // Double spike
                }
                else if (type == 2)
                {
                    spawnBlock(newX, 40.f + (std::rand() % 40));
                }
                else if (type == 3)
                {
                    spawnSpike(newX);
                    spawnOrb(newX + 20.f, GROUND_Y - 90.f - (std::rand() % 40));
                }
                else
                {
                    // Triple spike challenge
                    spawnSpike(newX);
                    spawnSpike(newX + 45.f);
                    spawnSpike(newX + 90.f);
                }

                rightmost = newX + 50.f;
            }

            // Remove off-screen obstacles
            DynamicArray<Obstacle> visibleObs;
            for (int i = 0; i < obstacles.size(); ++i)
            {
                if (obstacles[i].x > -100.f)
                {
                    // Score for passing obstacles
                    if (!obstacles[i].passed && obstacles[i].x < 80.f)
                    {
                        obstacles[i].passed = true;
                        if (obstacles[i].isSpike)
                            score += 1;
                    }
                    visibleObs.push_back(obstacles[i]);
                }
            }
            obstacles = visibleObs;

            // Remove collected/off-screen orbs
            DynamicArray<Orb> visibleOrbs;
            for (int i = 0; i < orbs.size(); ++i)
            {
                if (orbs[i].x > -50.f && !orbs[i].collected)
                    visibleOrbs.push_back(orbs[i]);
            }
            orbs = visibleOrbs;

            // Collision detection
            sf::FloatRect playerHitbox(
                { 100.f - PLAYER_SIZE / 2.f + 5.f, playerY - PLAYER_SIZE / 2.f + 5.f },
                { PLAYER_SIZE - 10.f, PLAYER_SIZE - 10.f }
            );

            // Check spike collisions
            for (int i = 0; i < obstacles.size(); ++i)
            {
                if (obstacles[i].isSpike)
                {
                    sf::FloatRect spikeBox(
                        { obstacles[i].x + 8.f, GROUND_Y - 35.f },
                        { 24.f, 35.f }
                    );

                    if (playerHitbox.findIntersection(spikeBox).has_value())
                    {
                        isDead = true;
                        if (score > highScore)
                        {
                            highScore = score;
                            std::cout << ">>> NEW HIGH SCORE: " << highScore << " <<<\n";
                        }
                        particles.emit({ 100.f, playerY }, 40, Colors::Danger);
                        state = DashState::Crashed;
                        std::cout << ">>> CRASHED! Score: " << score << ", Distance: " << (int)(distance/10.f) << "m <<<\n";
                    }
                }
                else
                {
                    sf::FloatRect blockBox = obstacles[i].shape.getGlobalBounds();
                    
                    if (playerHitbox.findIntersection(blockBox).has_value())
                    {
                        float playerRight = 100.f + PLAYER_SIZE / 2.f - 5.f;
                        if (playerRight > obstacles[i].x + 10.f)
                        {
                            isDead = true;
                            if (score > highScore)
                            {
                                highScore = score;
                                std::cout << ">>> NEW HIGH SCORE: " << highScore << " <<<\n";
                            }
                            particles.emit({ 100.f, playerY }, 40, Colors::Danger);
                            state = DashState::Crashed;
                            std::cout << ">>> CRASHED! Score: " << score << ", Distance: " << (int)(distance/10.f) << "m <<<\n";
                        }
                    }
                }
            }

            // Collect orbs
            for (int i = 0; i < orbs.size(); ++i)
            {
                if (!orbs[i].collected)
                {
                    sf::FloatRect orbBox = orbs[i].shape.getGlobalBounds();
                    if (playerHitbox.findIntersection(orbBox).has_value())
                    {
                        orbs[i].collected = true;
                        score += 5;
                        particles.emit(orbs[i].shape.getPosition() + sf::Vector2f(15.f, 15.f), 15, Colors::Warning);
                    }
                }
            }

            player.setPosition({ 100.f, playerY });
            player.setRotation(sf::degrees(rotation));
        }

        particles.update(dt);

        // =================== RENDER ===================
        window.clear(sf::Color(20, 20, 35));

        // Background
        static float bgOffset = 0.f;
        if (state == DashState::Playing)
            bgOffset += scrollSpeed * dt;

        sf::RectangleShape gridLine;
        gridLine.setFillColor(sf::Color(35, 35, 55));
        
        for (float x = -std::fmod(bgOffset, 80.f); x < WINDOW_WIDTH; x += 80.f)
        {
            gridLine.setSize({ 2.f, GROUND_Y });
            gridLine.setPosition({ x, 0.f });
            window.draw(gridLine);
        }
        
        for (float y = 80.f; y < GROUND_Y; y += 80.f)
        {
            gridLine.setSize({ WINDOW_WIDTH, 2.f });
            gridLine.setPosition({ 0.f, y });
            window.draw(gridLine);
        }

        window.draw(ground);
        
        // Ground line
        sf::RectangleShape groundLine;
        groundLine.setSize({ WINDOW_WIDTH, 4.f });
        groundLine.setPosition({ 0.f, GROUND_Y });
        groundLine.setFillColor(Colors::Accent);
        window.draw(groundLine);

        if (state == DashState::Menu)
        {
            if (mainFont)
            {
                sf::Text title(*mainFont, "DASH", 80);
                title.setFillColor(Colors::Accent);
                sf::FloatRect bounds = title.getLocalBounds();
                title.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 80.f });
                window.draw(title);

                sf::Text subtitle(*mainFont, "Tap or Space to Jump", 24);
                subtitle.setFillColor(Colors::Text);
                bounds = subtitle.getLocalBounds();
                subtitle.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 180.f });
                window.draw(subtitle);

                if (highScore > 0)
                {
                    char buf[32];
                    snprintf(buf, sizeof(buf), "Best: %d", highScore);
                    sf::Text bestText(*mainFont, buf, 24);
                    bestText.setFillColor(Colors::Warning);
                    bounds = bestText.getLocalBounds();
                    bestText.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 260.f });
                    window.draw(bestText);
                }
            }

            playButton->draw(window);
            backButton->draw(window);
        }
        else if (state == DashState::Playing || state == DashState::Paused)
        {
            for (int i = 0; i < obstacles.size(); ++i)
                window.draw(obstacles[i].shape);

            for (int i = 0; i < orbs.size(); ++i)
            {
                if (!orbs[i].collected)
                {
                    sf::CircleShape glow;
                    glow.setRadius(orbs[i].shape.getRadius() + 6.f);
                    glow.setPosition(orbs[i].shape.getPosition() - sf::Vector2f(6.f, 6.f));
                    glow.setFillColor(sf::Color(255, 200, 50, 50));
                    window.draw(glow);
                    window.draw(orbs[i].shape);
                }
            }

            for (int i = 3; i >= 1; --i)
            {
                sf::RectangleShape trail;
                trail.setSize({ PLAYER_SIZE - i * 4.f, PLAYER_SIZE - i * 4.f });
                trail.setOrigin({ (PLAYER_SIZE - i * 4.f) / 2.f, (PLAYER_SIZE - i * 4.f) / 2.f });
                trail.setPosition({ 100.f - i * 15.f, playerY });
                trail.setRotation(sf::degrees(rotation - i * 15.f));
                trail.setFillColor(sf::Color(Colors::Player.r, Colors::Player.g, Colors::Player.b, 
                                             static_cast<std::uint8_t>(80 - i * 20)));
                window.draw(trail);
            }

            window.draw(player);

            if (mainFont)
            {
                char buf[32];
                snprintf(buf, sizeof(buf), "%d", score);
                sf::Text scoreText(*mainFont, buf, 36);
                scoreText.setFillColor(Colors::Text);
                sf::FloatRect bounds = scoreText.getLocalBounds();
                scoreText.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 20.f });
                window.draw(scoreText);

                snprintf(buf, sizeof(buf), "%.0fm", distance / 10.f);
                sf::Text distText(*mainFont, buf, 18);
                distText.setFillColor(Colors::TextDim);
                bounds = distText.getLocalBounds();
                distText.setPosition({ WINDOW_WIDTH - bounds.size.x - 20.f, 20.f });
                window.draw(distText);
            }
        }
        else if (state == DashState::Crashed)
        {
            // Draw faded game elements
            for (int i = 0; i < obstacles.size(); ++i)
                window.draw(obstacles[i].shape);
            window.draw(ground);
            window.draw(groundLine);

            sf::RectangleShape overlay;
            overlay.setSize({ WINDOW_WIDTH, WINDOW_HEIGHT });
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);

            if (mainFont)
            {
                sf::Text crashText(*mainFont, "CRASH!", 56);
                crashText.setFillColor(Colors::Danger);
                sf::FloatRect bounds = crashText.getLocalBounds();
                crashText.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 100.f });
                window.draw(crashText);

                char buf[64];
                snprintf(buf, sizeof(buf), "Score: %d", score);
                sf::Text scoreText(*mainFont, buf, 32);
                scoreText.setFillColor(Colors::Warning);
                bounds = scoreText.getLocalBounds();
                scoreText.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 190.f });
                window.draw(scoreText);

                snprintf(buf, sizeof(buf), "Distance: %.0fm", distance / 10.f);
                sf::Text distText(*mainFont, buf, 20);
                distText.setFillColor(Colors::Text);
                bounds = distText.getLocalBounds();
                distText.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 240.f });
                window.draw(distText);

                snprintf(buf, sizeof(buf), "Attempt #%d", attempts);
                sf::Text attText(*mainFont, buf, 18);
                attText.setFillColor(Colors::TextDim);
                bounds = attText.getLocalBounds();
                attText.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 280.f });
                window.draw(attText);

                if (score >= highScore && score > 0)
                {
                    sf::Text newBest(*mainFont, "NEW BEST!", 24);
                    newBest.setFillColor(Colors::Success);
                    bounds = newBest.getLocalBounds();
                    newBest.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 156.f });
                    window.draw(newBest);
                }
            }

            retryButton->draw(window);
            menuButton->draw(window);

            // Handle input in render loop for simplicity (as requested to revert to old structure)
            if (mouseClicked)
            {
                if (isMouseOverBox(window, retryButton->getShape()))
                {
                    resetGame();
                    state = DashState::Playing;
                }
                else if (isMouseOverBox(window, menuButton->getShape()))
                {
                    state = DashState::Menu;
                }
            }
        }
        else if (state == DashState::Paused)
        {
             // Paused state drawing
            sf::RectangleShape overlay;
            overlay.setSize({ WINDOW_WIDTH, WINDOW_HEIGHT });
            overlay.setFillColor(Colors::Overlay);
            window.draw(overlay);

            sf::RectangleShape panel;
            panel.setSize({ 280.f, 240.f });
            panel.setPosition({ WINDOW_WIDTH / 2.f - 140.f, 180.f });
            panel.setFillColor(Colors::PanelBackground);
            panel.setOutlineThickness(2.f);
            panel.setOutlineColor(Colors::Accent);
            window.draw(panel);

            if (mainFont)
            {
                sf::Text pauseText(*mainFont, "PAUSED", 36);
                pauseText.setFillColor(Colors::Accent);
                sf::FloatRect bounds = pauseText.getLocalBounds();
                pauseText.setPosition({ (WINDOW_WIDTH - bounds.size.x) / 2.f, 200.f });
                window.draw(pauseText);
            }

            resumeButton->draw(window);
            menuButton->draw(window);

            if (mouseClicked)
            {
                if (isMouseOverBox(window, resumeButton->getShape()))
                {
                    DashState prev;
                    if (stateStack.pop(prev))
                        state = prev;
                }
                else if (isMouseOverBox(window, menuButton->getShape()))
                {
                    state = DashState::Menu;
                }
            }
        }

        window.display();
    }
}
