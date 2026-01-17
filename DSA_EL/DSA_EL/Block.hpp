#pragma once
#include <SFML/Graphics.hpp>

struct Block
{
    sf::RectangleShape shape;
    sf::Vector2f velocity;

    // ? DEFAULT CONSTRUCTOR (REQUIRED)
    Block()
    {
        shape.setSize({ 0.f, 0.f });
        shape.setPosition({ 0.f, 0.f });
        shape.setFillColor(sf::Color::White);
        velocity = { 0.f, 0.f };
    }

    // PARAMETERIZED CONSTRUCTOR
    Block(sf::Vector2f size, sf::Vector2f position, sf::Color color)
    {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(color);
        velocity = { 0.f, 0.f };
    }
};
