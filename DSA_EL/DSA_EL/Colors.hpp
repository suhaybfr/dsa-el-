#pragma once
#include <SFML/Graphics.hpp>

namespace Colors
{
    // Modern dark theme color palette
    inline const sf::Color Background{ 26, 26, 46 };       // #1a1a2e - Deep blue-black
    inline const sf::Color Primary{ 22, 33, 62 };          // #16213e - Navy
    inline const sf::Color Accent{ 233, 69, 96 };          // #e94560 - Vibrant pink
    inline const sf::Color Secondary{ 15, 52, 96 };        // #0f3460 - Deep blue
    inline const sf::Color Text{ 234, 234, 234 };          // #eaeaea - Off-white
    inline const sf::Color TextDim{ 150, 150, 170 };       // Dimmed text
    inline const sf::Color Success{ 46, 213, 115 };        // Green
    inline const sf::Color Warning{ 255, 193, 7 };         // Amber
    inline const sf::Color Danger{ 255, 71, 87 };          // Red
    
    // UI specific
    inline const sf::Color ButtonNormal{ 40, 40, 70 };
    inline const sf::Color ButtonHover{ 60, 60, 100 };
    inline const sf::Color ButtonPressed{ 80, 50, 90 };
    inline const sf::Color PanelBackground{ 30, 30, 50, 220 };
    inline const sf::Color Overlay{ 0, 0, 0, 150 };

    // Game elements
    inline const sf::Color Player{ 100, 220, 150 };        // Mint green
    inline const sf::Color Enemy{ 255, 100, 120 };         // Coral red
    inline const sf::Color Platform{ 80, 80, 120 };        // Muted purple
    inline const sf::Color Obstacle{ 200, 80, 80 };        // Dusky red
}
