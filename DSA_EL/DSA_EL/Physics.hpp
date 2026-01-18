#pragma once
#include <SFML/Graphics.hpp>

namespace Physics
{
    inline bool landOnPlatform(
        sf::RectangleShape& player,
        float& yVelocity,
        const sf::RectangleShape& platform)
    {
        sf::FloatRect p = player.getGlobalBounds();
        sf::FloatRect plat = platform.getGlobalBounds();

        float playerBottom = p.position.y + p.size.y;
        float platformTop = plat.position.y;

        if (playerBottom <= platformTop + 20.f && yVelocity >= 0.f)
        {
            player.setPosition({
                player.getPosition().x,
                platformTop - p.size.y
                });

            yVelocity = 0.f;
            return true;
        }
        return false;
    }
}
