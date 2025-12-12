#include "Editor.hpp"
#include <iostream>

namespace xsmall_hmi {

Editor::Editor() 
    : window_(sf::VideoMode(sf::Vector2u(1200, 800)), "XSmall-HMI Editor", sf::Style::Close) {
    
    window_.setFramerateLimit(60);
    
    variableDatabase_.setVariable("sensor_value", 50.0f); 
    auto sensorText = std::make_unique<TextObject>("sensor_text");
    sensorText->setPosition(sf::Vector2f(250, 50));
    sensorText->setSize(sf::Vector2f(200, 30));
    sensorText->setText("Sensor Value: ");
    sensorText->setVariableBinding("sensor_value");
    objects_.push_back(std::move(sensorText));
    
    auto sensorButton = std::make_unique<ButtonObject>("sensor_button");
    sensorButton->setPosition(sf::Vector2f(250, 100));
    sensorButton->setSize(sf::Vector2f(150, 40));
    sensorButton->setText("Random Sensor");
    sensorButton->setCallback([this]() {
        float randomValue = 20.0f + rand() % 60;
        variableDatabase_.setVariable("sensor_value", randomValue);
    });
    objects_.push_back(std::move(sensorButton));
    
    auto graph = std::make_unique<HistoryGraphObject>("sensor_graph");
    graph->setPosition(sf::Vector2f(250, 200));
    graph->setSize(sf::Vector2f(400, 200));
    graph->setVariableBinding("sensor_value");
    graph->addValue(50.0f); 
    objects_.push_back(std::move(graph));
}

void Editor::run() {
    while (window_.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Editor::handleEvents() {
    while (auto event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window_.close();
        } else if (auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePress->button == sf::Mouse::Button::Left) {
                handleMouseClick(sf::Vector2i(mousePress->position.x, mousePress->position.y));
            }
        } else if (auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
            handleTextEntered(textEvent->unicode);
        }
    }
}

void Editor::update() {
    for (auto& obj : objects_) {
        obj->update(variableDatabase_);
    }
}

void Editor::render() {
    window_.clear(sf::Color(255, 255, 255));

    sf::RectangleShape workspace;
    workspace.setPosition(sf::Vector2f(200, 0));
    workspace.setSize(sf::Vector2f(1000, 800));
    workspace.setFillColor(sf::Color(250, 250, 250));
    workspace.setOutlineColor(sf::Color(200, 200, 200));
    workspace.setOutlineThickness(2.0f);
    window_.draw(workspace);
    
    for (const auto& obj : objects_) {
        obj->draw(window_);
    }
    
    palette_.draw(window_);
    
    window_.display();
}

void Editor::handleMouseClick(const sf::Vector2i& mousePos) {
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), 
                          static_cast<float>(mousePos.y));
    
    if (mousePos.x < 200) {
        palette_.handleClick(mousePosF);
        return;
    }
    
    for (auto& obj : objects_) {
        if (auto* input = dynamic_cast<InputFieldObject*>(obj.get())) {
            input->setActive(false);
        }
    }
    
    for (auto& obj : objects_) {
        if (auto* button = dynamic_cast<ButtonObject*>(obj.get())) {
            if (button->contains(mousePosF)) {
                button->onClick();
                return;
            }
        }
    }
    
    bool clickedOnInputField = false;
    for (auto& obj : objects_) {
        if (auto* input = dynamic_cast<InputFieldObject*>(obj.get())) {
            if (input->contains(mousePosF)) {
                input->setActive(true); 
                clickedOnInputField = true;
                break;
            }
        }
    }
    
    if (clickedOnInputField) {
        return;
    }
    
    auto tool = palette_.getCurrentTool();
    
    switch (tool) {
        case Palette::Tool::Select:
            selectedObject_ = nullptr;
            for (auto it = objects_.rbegin(); it != objects_.rend(); ++it) {
                if ((*it)->contains(mousePosF)) {
                    selectedObject_ = it->get();
                    break;
                }
            }
            break;
            
        case Palette::Tool::Rectangle: {
            auto rect = std::make_unique<RectangleObject>("rect_" + std::to_string(objects_.size()));
            rect->setPosition(mousePosF);
            rect->setSize(sf::Vector2f(100, 60));
            rect->setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
            objects_.push_back(std::move(rect));
            break;
        }
            
        case Palette::Tool::Line: {
            auto line = std::make_unique<LineObject>("line_" + std::to_string(objects_.size()));
            line->setPoints(mousePosF, mousePosF + sf::Vector2f(100, 100));
            objects_.push_back(std::move(line));
            break;
        }
            
        case Palette::Tool::Polyline: {
            auto polyline = std::make_unique<PolylineObject>("poly_" + std::to_string(objects_.size()));
            polyline->setPosition(mousePosF);
            polyline->setSize(sf::Vector2f(200, 150));
            polyline->addPoint(sf::Vector2f(0, 0));
            polyline->addPoint(sf::Vector2f(50, 100));
            polyline->addPoint(sf::Vector2f(100, 50));
            polyline->addPoint(sf::Vector2f(150, 100));
            polyline->addPoint(sf::Vector2f(200, 0));
            
            objects_.push_back(std::move(polyline));
            break;
        }
            
        case Palette::Tool::Text: {
            auto text = std::make_unique<TextObject>("text_" + std::to_string(objects_.size()));
            text->setPosition(mousePosF);
            text->setSize(sf::Vector2f(200, 30));
            text->setText("Sensor: ");
            text->setVariableBinding("sensor_value");
            
            objects_.push_back(std::move(text));
            break;
        }
            
        case Palette::Tool::Button: {
            auto button = std::make_unique<ButtonObject>("btn_" + std::to_string(objects_.size()));
            button->setPosition(mousePosF);
            button->setSize(sf::Vector2f(120, 40));
            button->setText("Random Sensor");
            button->setCallback([this]() {
                float randomValue = 20.0f + rand() % 60;
                variableDatabase_.setVariable("sensor_value", randomValue);
                std::cout << "Sensor value: " << randomValue << std::endl;
            });
            
            objects_.push_back(std::move(button));
            break;
        }
            
        case Palette::Tool::InputField: {
            auto input = std::make_unique<InputFieldObject>("input_" + std::to_string(objects_.size()));
            input->setPosition(mousePosF);
            input->setSize(sf::Vector2f(200, 30));
            input->setActive(true); // New input field is active by default
            objects_.push_back(std::move(input));
            break;
        }
            
        case Palette::Tool::HistoryGraph: {
            auto graph = std::make_unique<HistoryGraphObject>("graph_" + std::to_string(objects_.size()));
            graph->setPosition(mousePosF);
            graph->setSize(sf::Vector2f(300, 150));
            graph->setVariableBinding("sensor_value");
            graph->addValue(50.0f);
            objects_.push_back(std::move(graph));
            break;
        }
            
        case Palette::Tool::Image: {
            auto image = std::make_unique<ImageObject>("img_" + std::to_string(objects_.size()));
            image->setPosition(mousePosF);
            image->setSize(sf::Vector2f(200, 150));
            image->loadFromFile("test_image.png");
            objects_.push_back(std::move(image));
            break;
        }
    }
    
    if (tool != Palette::Tool::Select) {
        static int objectCount = 0;
        objectCount++;
        variableDatabase_.setVariable("object_count", objectCount);
        
        std::cout << "Total objects: " << objectCount << std::endl;
    }
}

void Editor::handleTextEntered(uint32_t unicode) {
    for (auto& obj : objects_) {
        if (auto* input = dynamic_cast<InputFieldObject*>(obj.get())) {
            if (input->isActive()) {
                input->handleTextEntered(unicode);
            }
        }
    }
}

} // namespace xsmall_hmi