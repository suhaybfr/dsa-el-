#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>

class ResourceManager
{
public:
    static ResourceManager& getInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    // Load and cache a texture
    sf::Texture* getTexture(const std::string& path)
    {
        auto it = m_textures.find(path);
        if (it != m_textures.end())
            return &it->second;

        sf::Texture tex;
        if (!tex.loadFromFile(path))
        {
            std::cerr << "[ResourceManager] Failed to load texture: " << path << "\n";
            return nullptr;
        }

        m_textures[path] = std::move(tex);
        return &m_textures[path];
    }

    // Load and cache a font
    sf::Font* getFont(const std::string& path)
    {
        auto it = m_fonts.find(path);
        if (it != m_fonts.end())
            return &it->second;

        sf::Font font;
        if (!font.openFromFile(path))
        {
            std::cerr << "[ResourceManager] Failed to load font: " << path << "\n";
            return nullptr;
        }

        m_fonts[path] = std::move(font);
        return &m_fonts[path];
    }

    // Check if texture exists
    bool hasTexture(const std::string& path) const
    {
        return m_textures.find(path) != m_textures.end();
    }

    // Check if font exists
    bool hasFont(const std::string& path) const
    {
        return m_fonts.find(path) != m_fonts.end();
    }

    // Clear all resources
    void clear()
    {
        m_textures.clear();
        m_fonts.clear();
    }

private:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::unordered_map<std::string, sf::Texture> m_textures;
    std::unordered_map<std::string, sf::Font> m_fonts;
};
