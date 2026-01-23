#pragma once
#include <SFML/Graphics.hpp>
#include "Colors.hpp"
#include "SceneNode.hpp"
#include <string>
#include <functional>
#include <memory>

// ==================== BUTTON ====================
class Button : public SceneNode
{
public:
    Button(sf::Vector2f size, sf::Vector2f position, const std::string& label, sf::Font* font = nullptr)
        : m_label(label), m_font(font), m_isHovered(false), m_isPressed(false), m_text(nullptr)
    {
        m_shape.setSize(size);
        m_shape.setPosition(position);
        m_shape.setFillColor(Colors::ButtonNormal);
        m_shape.setOutlineThickness(2.f);
        m_shape.setOutlineColor(Colors::Accent);

        if (m_font)
        {
            m_text = std::make_unique<sf::Text>(*m_font, label, 20);
            m_text->setFillColor(Colors::Text);
            centerText();
        }
    }

    void setCallback(std::function<void()> callback)
    {
        m_callback = callback;
    }

    void update(const sf::RenderWindow& window)
    {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        m_isHovered = m_shape.getGlobalBounds().contains(mouse);

        if (m_isHovered)
        {
            m_shape.setFillColor(Colors::ButtonHover);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                m_shape.setFillColor(Colors::ButtonPressed);
                if (!m_isPressed)
                {
                    m_isPressed = true;
                }
            }
            else if (m_isPressed)
            {
                // Button was released while hovered - trigger callback
                m_isPressed = false;
                if (m_callback)
                    m_callback();
            }
        }
        else
        {
            m_shape.setFillColor(Colors::ButtonNormal);
            m_isPressed = false;
        }
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(m_shape);
        if (m_text)
            window.draw(*m_text);
    }

    bool isHovered() const { return m_isHovered; }
    sf::RectangleShape& getShape() { return m_shape; }

private:
    void centerText()
    {
        if (!m_text) return;
        sf::FloatRect textBounds = m_text->getLocalBounds();
        sf::FloatRect shapeBounds = m_shape.getGlobalBounds();
        m_text->setPosition({
            shapeBounds.position.x + (shapeBounds.size.x - textBounds.size.x) / 2.f - textBounds.position.x,
            shapeBounds.position.y + (shapeBounds.size.y - textBounds.size.y) / 2.f - textBounds.position.y
        });
    }

    sf::RectangleShape m_shape;
    std::unique_ptr<sf::Text> m_text;
    std::string m_label;
    sf::Font* m_font;
    bool m_isHovered;
    bool m_isPressed;
    std::function<void()> m_callback;
};

// ==================== PANEL ====================
class Panel : public SceneNode
{
public:
    Panel(sf::Vector2f size, sf::Vector2f position, sf::Color color = Colors::PanelBackground)
    {
        m_shape.setSize(size);
        m_shape.setPosition(position);
        m_shape.setFillColor(color);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(m_shape);
    }

    sf::RectangleShape& getShape() { return m_shape; }

private:
    sf::RectangleShape m_shape;
};

// ==================== LABEL ====================
class Label : public SceneNode
{
public:
    Label(const std::string& text, sf::Font* font, unsigned int size, sf::Vector2f position)
        : m_font(font), m_text(nullptr), m_position(position)
    {
        if (m_font)
        {
            m_text = std::make_unique<sf::Text>(*m_font, text, size);
            m_text->setPosition(position);
            m_text->setFillColor(Colors::Text);
        }
    }

    void setText(const std::string& text)
    {
        if (m_text)
            m_text->setString(text);
    }

    void setColor(sf::Color color)
    {
        if (m_text)
            m_text->setFillColor(color);
    }

    void centerHorizontally(float windowWidth)
    {
        if (!m_text) return;
        sf::FloatRect bounds = m_text->getLocalBounds();
        m_text->setPosition({
            (windowWidth - bounds.size.x) / 2.f - bounds.position.x,
            m_text->getPosition().y
        });
    }

    void draw(sf::RenderWindow& window)
    {
        if (m_text)
            window.draw(*m_text);
    }

    sf::Text* getText() { return m_text.get(); }

private:
    std::unique_ptr<sf::Text> m_text;
    sf::Font* m_font;
    sf::Vector2f m_position;
};

// ==================== HUD ====================
class HUD
{
public:
    HUD(sf::Font* font) : m_font(font), m_score(0), m_time(0.f),
        m_scoreText(nullptr), m_timeText(nullptr)
    {
        if (m_font)
        {
            m_scoreText = std::make_unique<sf::Text>(*m_font, "Score: 0", 18);
            m_scoreText->setPosition({ 10.f, 10.f });
            m_scoreText->setFillColor(Colors::Text);

            m_timeText = std::make_unique<sf::Text>(*m_font, "Time: 0.0s", 18);
            m_timeText->setPosition({ 10.f, 35.f });
            m_timeText->setFillColor(Colors::TextDim);
        }
    }

    void setScore(int score)
    {
        m_score = score;
        if (m_scoreText)
            m_scoreText->setString("Score: " + std::to_string(m_score));
    }

    void setTime(float time)
    {
        m_time = time;
        if (m_timeText)
        {
            char buf[32];
            snprintf(buf, sizeof(buf), "Time: %.1fs", m_time);
            m_timeText->setString(buf);
        }
    }

    void update(float time, int score)
    {
        setTime(time);
        setScore(score);
    }

    void draw(sf::RenderWindow& window)
    {
        if (m_scoreText)
            window.draw(*m_scoreText);
        if (m_timeText)
            window.draw(*m_timeText);
    }

private:
    sf::Font* m_font;
    std::unique_ptr<sf::Text> m_scoreText;
    std::unique_ptr<sf::Text> m_timeText;
    int m_score;
    float m_time;
};
