#pragma once
#include <SFML/Graphics.hpp>
#include "DynamicArray.hpp"
#include <cstdlib>
#include <cstdint>
#include <cmath>

// Simple particle for visual effects
struct Particle
{
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float lifetime;
    float maxLifetime;
    sf::Color startColor;
    sf::Color endColor;

    Particle() : velocity(0.f, 0.f), lifetime(0.f), maxLifetime(1.f), 
        startColor(sf::Color::White), endColor(sf::Color::Transparent)
    {
        shape.setRadius(3.f);
        shape.setPosition({0.f, 0.f});
        shape.setFillColor(sf::Color::White);
    }

    Particle(sf::Vector2f pos, sf::Vector2f vel, float life, sf::Color start, sf::Color end)
        : velocity(vel), lifetime(life), maxLifetime(life), startColor(start), endColor(end)
    {
        shape.setRadius(3.f);
        shape.setPosition(pos);
        shape.setFillColor(start);
    }

    bool update(float dt)
    {
        lifetime -= dt;
        if (lifetime <= 0.f)
            return false;

        shape.move(velocity * dt);
        velocity.y += 200.f * dt; // gravity

        // Fade color
        float t = 1.f - (lifetime / maxLifetime);
        std::uint8_t r = static_cast<std::uint8_t>(startColor.r + t * (endColor.r - startColor.r));
        std::uint8_t g = static_cast<std::uint8_t>(startColor.g + t * (endColor.g - startColor.g));
        std::uint8_t b = static_cast<std::uint8_t>(startColor.b + t * (endColor.b - startColor.b));
        std::uint8_t a = static_cast<std::uint8_t>(255 * (1.f - t));
        shape.setFillColor(sf::Color(r, g, b, a));

        return true;
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(shape);
    }
};

// Particle system using DynamicArray
class ParticleSystem
{
public:
    ParticleSystem() = default;

    void emit(sf::Vector2f position, int count, sf::Color color)
    {
        for (int i = 0; i < count; ++i)
        {
            float angle = static_cast<float>(std::rand()) / RAND_MAX * 6.28318f;
            float speed = 50.f + static_cast<float>(std::rand()) / RAND_MAX * 150.f;
            sf::Vector2f vel(std::cos(angle) * speed, std::sin(angle) * speed - 100.f);
            
            float life = 0.5f + static_cast<float>(std::rand()) / RAND_MAX * 0.5f;
            
            sf::Color endColor(color.r / 2, color.g / 2, color.b / 2, 0);
            m_particles.push_back(Particle(position, vel, life, color, endColor));
        }
    }

    void update(float dt)
    {
        // Create new array with alive particles only
        DynamicArray<Particle> alive;
        for (int i = 0; i < m_particles.size(); ++i)
        {
            if (m_particles[i].update(dt))
                alive.push_back(m_particles[i]);
        }
        m_particles = alive;
    }

    void draw(sf::RenderWindow& window)
    {
        for (int i = 0; i < m_particles.size(); ++i)
        {
            m_particles[i].draw(window);
        }
    }

    void clear()
    {
        m_particles = DynamicArray<Particle>();
    }

private:
    DynamicArray<Particle> m_particles;
};
