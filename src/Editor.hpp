#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "VisualObject.hpp"
#include "VariableDatabase.hpp"
#include "Palette.hpp"

namespace xsmall_hmi {

class Editor {
public:
    Editor();
    ~Editor() = default;
    
    void run();
    
private:
    void handleEvents();
    void update();
    void render();
    
    void handleMouseClick(const sf::Vector2i& mousePos);
    void handleTextEntered(uint32_t unicode);
    
    sf::RenderWindow window_;
    VariableDatabase variableDatabase_;
    Palette palette_;
    
    std::vector<std::unique_ptr<VisualObject>> objects_;
    VisualObject* selectedObject_ = nullptr;
    
    sf::Font font_;
};

} 