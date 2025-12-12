#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace xsmall_hmi {

class Palette {
public:
    enum class Tool {
        Select,
        Rectangle,
        Line,
        Polyline,
        Text,
        Button,
        InputField,
        HistoryGraph,
        Image
    };
    
    Palette();
    
    void draw(sf::RenderWindow& window);
    Tool handleClick(const sf::Vector2f& mousePos);
    Tool getCurrentTool() const { return currentTool_; }
    
private:
    Tool currentTool_ = Tool::Select;
    sf::RectangleShape paletteBackground_;
    std::vector<std::pair<sf::FloatRect, Tool>> toolButtons_;
};

} // namespace xsmall_hmi