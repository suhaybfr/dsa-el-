#include "Game.hpp"
#include <iostream>

// Main menu GUI for game selection
void runMainMenu()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "DSA Game Engine");
    window.setFramerateLimit(60);

    // Load font
    sf::Font* mainFont = ResourceManager::getInstance().getFont("C:/Windows/Fonts/arial.ttf");

    // UI Buttons
    float centerX = 400.f;

    std::unique_ptr<Button> survivalButton = std::make_unique<Button>(
        sf::Vector2f(280.f, 65.f),
        sf::Vector2f(centerX - 140.f, 220.f),
        "SURVIVAL",
        mainFont
    );

    std::unique_ptr<Button> platformerButton = std::make_unique<Button>(
        sf::Vector2f(280.f, 65.f),
        sf::Vector2f(centerX - 140.f, 310.f),
        "DASH",
        mainFont
    );

    std::unique_ptr<Button> exitButton = std::make_unique<Button>(
        sf::Vector2f(180.f, 50.f),
        sf::Vector2f(centerX - 90.f, 420.f),
        "EXIT",
        mainFont
    );

    int selectedGame = 0; // 0 = none, 1 = survival, 2 = platformer
    bool wasMousePressed = true;  // Start true to prevent immediate click

    while (window.isOpen())
    {
        // Events
        while (auto e = window.pollEvent())
        {
            if (e->is<sf::Event::Closed>())
                window.close();
        }

        // Update buttons
        survivalButton->update(window);
        platformerButton->update(window);
        exitButton->update(window);

        bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        
        // Only trigger on mouse release (click complete)
        if (!mousePressed && wasMousePressed)
        {
            sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (survivalButton->getShape().getGlobalBounds().contains(mouse))
            {
                selectedGame = 1;
                window.close();
            }
            else if (platformerButton->getShape().getGlobalBounds().contains(mouse))
            {
                selectedGame = 2;
                window.close();
            }
            else if (exitButton->getShape().getGlobalBounds().contains(mouse))
            {
                selectedGame = 0;
                window.close();
            }
        }
        wasMousePressed = mousePressed;

        // Render
        window.clear(Colors::Background);

        // Background grid
        sf::RectangleShape line;
        line.setFillColor(sf::Color(40, 40, 60, 100));
        for (float x = 0; x < 800.f; x += 50.f)
        {
            line.setSize({ 1.f, 600.f });
            line.setPosition({ x, 0.f });
            window.draw(line);
        }
        for (float y = 0; y < 600.f; y += 50.f)
        {
            line.setSize({ 800.f, 1.f });
            line.setPosition({ 0.f, y });
            window.draw(line);
        }

        // Title
        if (mainFont)
        {
            sf::Text title(*mainFont, "GAME ENGINE", 52);
            title.setFillColor(Colors::Accent);
            sf::FloatRect bounds = title.getLocalBounds();
            title.setPosition({
                (800.f - bounds.size.x) / 2.f,
                60.f
            });
            window.draw(title);

            sf::Text subtitle(*mainFont, "Select a game", 20);
            subtitle.setFillColor(Colors::TextDim);
            bounds = subtitle.getLocalBounds();
            subtitle.setPosition({
                (800.f - bounds.size.x) / 2.f,
                130.f
            });
            window.draw(subtitle);
        }

        // Draw buttons
        survivalButton->draw(window);
        platformerButton->draw(window);
        exitButton->draw(window);

        // Footer
        if (mainFont)
        {
            sf::Text footer(*mainFont, "DSA Project - SFML 3.0", 14);
            footer.setFillColor(sf::Color(80, 80, 100));
            sf::FloatRect bounds = footer.getLocalBounds();
            footer.setPosition({
                (800.f - bounds.size.x) / 2.f,
                560.f
            });
            window.draw(footer);
        }

        window.display();
    }

    // Launch selected game
    if (selectedGame == 1)
    {
        Game game;
        game.run();
    }
    else if (selectedGame == 2)
    {
        runGame2();
    }
}

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    std::cout << "===== DSA GAME ENGINE =====\n";
    std::cout << "Select a game from the menu!\n\n";

    runMainMenu();

    return 0;
}
