#pragma once
#include <SFML/Graphics.hpp>
#include "Queue.hpp"

// Input action types
enum class InputAction
{
    None,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Jump,
    Pause,
    Confirm,
    Cancel,
    Retry
};

// Input event with action and timestamp
struct InputEvent
{
    InputAction action;
    float timestamp;

    InputEvent() : action(InputAction::None), timestamp(0.f) {}
    InputEvent(InputAction a, float t) : action(a), timestamp(t) {}
};

// Centralized input manager using Queue data structure
class InputManager
{
public:
    InputManager() : m_time(0.f) {}

    void update(float dt)
    {
        m_time += dt;

        // Poll keyboard for common actions
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            pushAction(InputAction::MoveUp);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            pushAction(InputAction::MoveDown);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            pushAction(InputAction::MoveLeft);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            pushAction(InputAction::MoveRight);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            pushAction(InputAction::Jump);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
            pushAction(InputAction::Retry);
    }

    void pushAction(InputAction action)
    {
        m_inputQueue.push(InputEvent(action, m_time));
    }

    bool popAction(InputEvent& out)
    {
        return m_inputQueue.pop(out);
    }

    bool hasActions() const
    {
        return !m_inputQueue.empty();
    }

    void clear()
    {
        InputEvent dummy;
        while (!m_inputQueue.empty())
            m_inputQueue.pop(dummy);
    }

    // Check if a key was just pressed (edge detection)
    bool isKeyJustPressed(sf::Keyboard::Key key)
    {
        bool current = sf::Keyboard::isKeyPressed(key);
        bool prev = m_keyStates[static_cast<int>(key)];
        m_keyStates[static_cast<int>(key)] = current;
        return current && !prev;
    }

private:
    Queue<InputEvent> m_inputQueue;
    float m_time;
    bool m_keyStates[256] = { false };
};
