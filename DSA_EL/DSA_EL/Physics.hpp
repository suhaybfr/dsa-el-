#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

namespace Physics
{
    // Improved platform landing detection
    // Returns true if player landed on platform from above
    inline bool landOnPlatform(
        sf::RectangleShape& player,
        float& yVelocity,
        const sf::RectangleShape& platform)
    {
        sf::FloatRect playerBounds = player.getGlobalBounds();
        sf::FloatRect platBounds = platform.getGlobalBounds();

        float playerBottom = playerBounds.position.y + playerBounds.size.y;
        float playerTop = playerBounds.position.y;
        float playerLeft = playerBounds.position.x;
        float playerRight = playerBounds.position.x + playerBounds.size.x;
        float playerCenterX = playerBounds.position.x + playerBounds.size.x / 2.f;

        float platTop = platBounds.position.y;
        float platBottom = platBounds.position.y + platBounds.size.y;
        float platLeft = platBounds.position.x;
        float platRight = platBounds.position.x + platBounds.size.x;

        // Check if player is horizontally aligned with platform
        bool horizontalOverlap = playerRight > platLeft + 5.f && playerLeft < platRight - 5.f;

        // Check if player is falling (or stationary) and coming from above
        bool fallingDown = yVelocity >= 0.f;

        // Check if player's bottom is near platform top (within tolerance)
        float penetration = playerBottom - platTop;
        bool nearTop = penetration > 0.f && penetration < 30.f;

        // Also ensure player wasn't too far below (to avoid snapping from below)
        bool wasAbove = playerBottom - yVelocity * 0.017f <= platTop + 15.f;

        if (horizontalOverlap && fallingDown && nearTop && wasAbove)
        {
            // Snap player to platform top
            player.setPosition({
                player.getPosition().x,
                platTop - playerBounds.size.y
            });

            yVelocity = 0.f;
            return true;
        }

        return false;
    }

    // Check if player is standing on a platform (for grounded state)
    inline bool isOnPlatform(
        const sf::RectangleShape& player,
        const sf::RectangleShape& platform)
    {
        sf::FloatRect playerBounds = player.getGlobalBounds();
        sf::FloatRect platBounds = platform.getGlobalBounds();

        float playerBottom = playerBounds.position.y + playerBounds.size.y;
        float platTop = platBounds.position.y;

        // Player is on platform if bottom is exactly at platform top
        // and horizontally overlapping
        bool onTop = std::abs(playerBottom - platTop) < 2.f;
        bool horizontalOverlap = 
            playerBounds.position.x + playerBounds.size.x > platBounds.position.x + 5.f &&
            playerBounds.position.x < platBounds.position.x + platBounds.size.x - 5.f;

        return onTop && horizontalOverlap;
    }
}
