#include "Palette.hpp"
#include <SFML/Graphics.hpp>

namespace xsmall_hmi {

Palette::Palette() {
    paletteBackground_.setSize(sf::Vector2f(200, 600));
    paletteBackground_.setFillColor(sf::Color(240, 240, 240));
    paletteBackground_.setOutlineColor(sf::Color(180, 180, 180));
    paletteBackground_.setOutlineThickness(2.0f);
    
    float y = 10.0f;
    std::vector<std::pair<std::string, Tool>> tools = {
        {"Select", Tool::Select},
        {"Rectangle", Tool::Rectangle},
        {"Line", Tool::Line},
        {"Polyline", Tool::Polyline},
        {"Text", Tool::Text},
        {"Button", Tool::Button},
        {"Input Field", Tool::InputField},
        {"History Graph", Tool::HistoryGraph},
        {"Image", Tool::Image}
    };
    
    for (const auto& [name, tool] : tools) {
        sf::FloatRect rect(sf::Vector2f(10, y), sf::Vector2f(180, 40));
        toolButtons_.emplace_back(rect, tool);
        y += 50;
    }
}

void Palette::draw(sf::RenderWindow& window) {
    window.draw(paletteBackground_);
    
    sf::Font font;
    if (font.openFromFile("arial.ttf")) {
        sf::Text toolText(font, "", 16);
        toolText.setFillColor(sf::Color::Black);
        
        for (const auto& [rect, tool] : toolButtons_) {
            sf::RectangleShape button;
            button.setPosition(rect.position);
            button.setSize(rect.size);
            
            bool isCurrent = (tool == currentTool_);
            button.setFillColor(isCurrent ? sf::Color(200, 220, 255) : sf::Color(220, 220, 220));
            button.setOutlineColor(sf::Color(150, 150, 150));
            button.setOutlineThickness(1.0f);
            
            window.draw(button);
            
            std::string name;
            switch (tool) {
                case Tool::Select: name = "Select"; break;
                case Tool::Rectangle: name = "Rectangle"; break;
                case Tool::Line: name = "Line"; break;
                case Tool::Polyline: name = "Polyline"; break;
                case Tool::Text: name = "Text"; break;
                case Tool::Button: name = "Button"; break;
                case Tool::InputField: name = "Input Field"; break;
                case Tool::HistoryGraph: name = "History Graph"; break;
                case Tool::Image: name = "Image"; break;
            }
            
            toolText.setString(name);
            toolText.setPosition(rect.position + sf::Vector2f(10, 10));
            window.draw(toolText);
        }
    }
}

Palette::Tool Palette::handleClick(const sf::Vector2f& mousePos) {
    for (const auto& [rect, tool] : toolButtons_) {
        if (rect.contains(mousePos)) {
            currentTool_ = tool;
            return tool;
        }
    }
    return currentTool_;
}

} // namespace xsmall_hmi