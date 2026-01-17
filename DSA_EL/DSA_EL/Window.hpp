#pragma once
#include <SFML/Graphics.hpp>

class Window {
public:
    Window();
    bool isOpen() const;

    void pollEvents();
    void clear();
    void display();

    sf::RenderWindow& native();

private:
    sf::RenderWindow m_window;
};
