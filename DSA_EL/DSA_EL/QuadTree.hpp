#pragma once
#include <SFML/Graphics.hpp>
#include "LinkedList.hpp"

struct QuadTree
{
    sf::FloatRect boundary;
    LinkedList<int> entities; // store indices of entities
    QuadTree* children[4]{ nullptr, nullptr, nullptr, nullptr };

    QuadTree(sf::FloatRect bounds) : boundary(bounds) {}

    int getQuadrant(const sf::Vector2f& pos)
    {
        float midX = boundary.position.x + boundary.size.x / 2.f;
        float midY = boundary.position.y + boundary.size.y / 2.f;

        if (pos.x < midX && pos.y < midY) return 0; // TL
        if (pos.x >= midX && pos.y < midY) return 1; // TR
        if (pos.x < midX && pos.y >= midY) return 2; // BL
        return 3; // BR
    }

    void insert(int entityIndex, const sf::Vector2f& pos)
    {
        int q = getQuadrant(pos);
        entities.push_front(entityIndex);
    }

    LinkedList<int>& query(const sf::Vector2f&)
    {
        return entities;
    }
};
