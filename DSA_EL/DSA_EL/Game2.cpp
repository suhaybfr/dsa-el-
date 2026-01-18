#include <SFML/Graphics.hpp>
#include <iostream>

#include "DynamicArray.hpp"
#include "Queue.hpp"
#include "Physics.hpp"

enum class Game2State
{
    Menu,
    Playing,
    GameOver
};

struct PlatformCommand
{
    sf::Vector2f move;
};

bool isMouseOver(
    const sf::RenderWindow& window,
    const sf::RectangleShape& box)
{
    sf::Vector2f mouse =
        window.mapPixelToCoords(sf::Mouse::getPosition(window));
    return box.getGlobalBounds().contains(mouse);
}

void runGame2()
{
    constexpr float W = 800.f;
    constexpr float H = 600.f;

    sf::RenderWindow window(
        sf::VideoMode({ 800, 600 }),
        "Game2 – Platformer"
    );

    sf::Clock clock;
    sf::Clock gameTimer;

    Game2State state = Game2State::Menu;

    // ================= MENU RECTANGLES =================
    sf::RectangleShape startBox({ 300.f, 80.f });
    startBox.setPosition({ 250.f, 220.f });
    startBox.setFillColor(sf::Color::White);

    sf::RectangleShape exitBox({ 300.f, 80.f });
    exitBox.setPosition({ 250.f, 320.f });
    exitBox.setFillColor(sf::Color::White);

    sf::RectangleShape continueBox({ 300.f, 80.f });
    continueBox.setPosition({ 250.f, 270.f });
    continueBox.setFillColor(sf::Color::White);

    bool printedMenu = false;
    bool printedGameOver = false;

    // ================= PLAYER =================
    sf::RectangleShape player({ 40.f, 40.f });
    player.setFillColor(sf::Color::Green);

    // ================= PLATFORMS =================
    DynamicArray<sf::RectangleShape> platforms;

    sf::RectangleShape ground({ 800.f, 40.f });
    ground.setPosition({ 0.f, 560.f });
    platforms.push_back(ground);

    sf::RectangleShape p1({ 200.f, 20.f });
    p1.setPosition({ 150.f, 450.f });
    platforms.push_back(p1);

    sf::RectangleShape p2({ 200.f, 20.f });
    p2.setPosition({ 420.f, 360.f });
    platforms.push_back(p2);

    sf::RectangleShape goal({ 150.f, 20.f });
    goal.setPosition({ 300.f, 260.f });
    platforms.push_back(goal);

    // ================= OBSTACLE =================
    sf::RectangleShape obstacle({ 20.f, 20.f });
    obstacle.setFillColor(sf::Color::Red);
    obstacle.setPosition({
        platforms[1].getPosition().x + 90.f,
        platforms[1].getPosition().y - 20.f
        });

    // ================= INPUT =================
    Queue<PlatformCommand> inputQueue;

    float yVelocity = 0.f;
    bool onGround = true;

    const float gravity = 2800.f;
    const float jumpVelocity = -900.f;
    const float moveSpeed = 260.f;

    auto resetGame = [&]()
        {
            player.setPosition({ 50.f, 520.f });
            yVelocity = 0.f;
            onGround = true;
            gameTimer.restart();
        };

    // ================= GAME LOOP =================
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        while (auto e = window.pollEvent())
        {
            if (e->is<sf::Event::Closed>())
                window.close();
        }

        // ================= MENU =================
        if (state == Game2State::Menu)
        {
            if (!printedMenu)
            {
                std::cout << "\nMENU\n";
                std::cout << "Top box  -> START\n";
                std::cout << "Bottom box -> EXIT\n";
                printedMenu = true;
            }

            startBox.setFillColor(
                isMouseOver(window, startBox)
                ? sf::Color(180, 180, 180)
                : sf::Color::White);

            exitBox.setFillColor(
                isMouseOver(window, exitBox)
                ? sf::Color(180, 180, 180)
                : sf::Color::White);

            if (isMouseOver(window, startBox) &&
                sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                resetGame();
                state = Game2State::Playing;
                printedMenu = false;
            }

            if (isMouseOver(window, exitBox) &&
                sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                resetGame();
                state = Game2State::Playing;
                printedMenu = false;
            }
        }

        // ================= PLAYING =================
        else if (state == Game2State::Playing)
        {
            sf::Vector2f move(0.f, 0.f);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                move.x -= moveSpeed * dt;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                move.x += moveSpeed * dt;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && onGround)
            {
                yVelocity = jumpVelocity;
                onGround = false;
            }

            inputQueue.push({ move });

            PlatformCommand cmd;
            while (!inputQueue.empty())
                if (inputQueue.pop(cmd))
                    player.move(cmd.move);

            yVelocity += gravity * dt;
            player.move({ 0.f, yVelocity * dt });

            onGround = false;
            bool landedOnGoal = false;

            for (int i = 0; i < platforms.size(); i++)
            {
                if (player.getGlobalBounds()
                    .findIntersection(platforms[i].getGlobalBounds())
                    .has_value())
                {
                    if (Physics::landOnPlatform(player, yVelocity, platforms[i]))
                    {
                        onGround = true;
                        if (i == 3)
                            landedOnGoal = true;
                    }
                }
            }

            if (player.getGlobalBounds()
                .findIntersection(obstacle.getGlobalBounds())
                .has_value())
            {
                state = Game2State::GameOver;
            }

            sf::Vector2f pos = player.getPosition();
            if (pos.x < 0) pos.x = 0;
            if (pos.x + player.getSize().x > W)
                pos.x = W - player.getSize().x;
            if (pos.y > 520.f)
            {
                pos.y = 520.f;
                yVelocity = 0.f;
                onGround = true;
            }
            player.setPosition(pos);

            if (landedOnGoal)
            {
                std::cout << "LEVEL COMPLETE in "
                    << gameTimer.getElapsedTime().asSeconds()
                    << " seconds\n";
                state = Game2State::Menu;
            }
        }

        // ================= GAME OVER =================
        else
        {
            if (!printedGameOver)
            {
                std::cout << "\nGAME OVER\n";
                std::cout << "Click box to CONTINUE\n";
                printedGameOver = true;
            }

            continueBox.setFillColor(
                isMouseOver(window, continueBox)
                ? sf::Color(180, 180, 180)
                : sf::Color::White);

            if (isMouseOver(window, continueBox) &&
                sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                state = Game2State::Menu;
                printedGameOver = false;
            }
        }

        // ================= RENDER =================
        window.clear(sf::Color::Black);

        if (state == Game2State::Menu)
        {
            window.draw(startBox);
            window.draw(exitBox);
        }
        else if (state == Game2State::Playing)
        {
            window.draw(player);
            for (int i = 0; i < platforms.size(); i++)
                window.draw(platforms[i]);
            window.draw(obstacle);
        }
        else
        {
            window.draw(continueBox);
        }

        window.display();
    }
}
