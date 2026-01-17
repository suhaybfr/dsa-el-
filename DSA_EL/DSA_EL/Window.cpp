#include "Window.hpp"

Window::Window()
    : m_window(sf::VideoMode({ 800, 600 }), "SFML 3 Engine")
{
}


bool Window::isOpen() const {
    return m_window.isOpen();
}

void Window::pollEvents() {
    while (auto event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            m_window.close();
    }
}

void Window::clear() {
    m_window.clear();
}

void Window::display() {
    m_window.display();
}

sf::RenderWindow& Window::native() {
    return m_window;
}
