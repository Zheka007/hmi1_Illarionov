#include "VisualObject.hpp"
#include "VariableDatabase.hpp"
#include <iostream>

namespace xsmall_hmi {

VisualObject::VisualObject(ObjectType type, const std::string& id)
    : type_(type), id_(id), position_(0, 0), size_(100, 50), 
      color_(sf::Color::White) {
    
    if (!font_.openFromFile("arial.ttf")) {
    }
}

void VisualObject::update(const VariableDatabase& db) {
    if (!boundVariable_.empty()) {
        if (auto value = db.getVariableAs<std::string>(boundVariable_)) {
            text_ = *value;
        }
    }
}

bool VisualObject::contains(const sf::Vector2f& point) const {
    return getBounds().contains(point);
}

void VisualObject::setPosition(const sf::Vector2f& pos) { position_ = pos; }
void VisualObject::setSize(const sf::Vector2f& sz) { size_ = sz; }
void VisualObject::setColor(const sf::Color& color) { color_ = color; }
void VisualObject::setText(const std::string& text) { text_ = text; }
void VisualObject::setVariableBinding(const std::string& varName) { boundVariable_ = varName; }

sf::FloatRect VisualObject::getBounds() const {
    return sf::FloatRect(position_, size_);
}

RectangleObject::RectangleObject(const std::string& id)
    : VisualObject(ObjectType::Rectangle, id) {
    color_ = sf::Color(200, 200, 200);
}

void RectangleObject::draw(sf::RenderWindow& window) const {
    sf::RectangleShape shape;
    shape.setPosition(position_);
    shape.setSize(size_);
    shape.setFillColor(color_);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.0f);
    window.draw(shape);
}

TextObject::TextObject(const std::string& id)
    : VisualObject(ObjectType::Text, id) {
    color_ = sf::Color::Transparent;
}

void TextObject::draw(sf::RenderWindow& window) const {
    if (!text_.empty()) {
        sf::Text text(font_, text_, 20);
        text.setPosition(position_);
        text.setFillColor(sf::Color::Black);
        window.draw(text);
    }
}

void TextObject::update(const VariableDatabase& db) {
    VisualObject::update(db);
}

LineObject::LineObject(const std::string& id)
    : VisualObject(ObjectType::Line, id),
      startPoint_(0, 0), endPoint_(100, 100) {
    color_ = sf::Color::Black;
}

void LineObject::draw(sf::RenderWindow& window) const {
    sf::Vertex line[] = {
        sf::Vertex{startPoint_, color_},
        sf::Vertex{endPoint_, color_}
    };
    window.draw(line, 2, sf::PrimitiveType::Lines);
}

void LineObject::setPoints(const sf::Vector2f& start, const sf::Vector2f& end) {
    startPoint_ = start;
    endPoint_ = end;
}

PolylineObject::PolylineObject(const std::string& id)
    : VisualObject(ObjectType::Polyline, id) {
    color_ = sf::Color::Blue;
}

void PolylineObject::draw(sf::RenderWindow& window) const {
    if (points_.size() < 2) return;
    
    std::vector<sf::Vertex> vertices;
    for (size_t i = 0; i < points_.size() - 1; ++i) {
        vertices.emplace_back(sf::Vertex{points_[i], color_});
        vertices.emplace_back(sf::Vertex{points_[i + 1], color_});
    }
    window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::Lines);
}

void PolylineObject::addPoint(const sf::Vector2f& point) {
    points_.push_back(point);
}

ButtonObject::ButtonObject(const std::string& id)
    : VisualObject(ObjectType::Button, id) {
    color_ = sf::Color(100, 150, 200);
}

void ButtonObject::draw(sf::RenderWindow& window) const {
    sf::RectangleShape shape;
    shape.setPosition(position_);
    shape.setSize(size_);
    shape.setFillColor(isPressed_ ? sf::Color(80, 130, 180) : color_);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.0f);
    window.draw(shape);
    
    if (!text_.empty()) {
        sf::Text btnText(font_, text_, 16);
        btnText.setPosition(position_ + sf::Vector2f(10, 10));
        btnText.setFillColor(sf::Color::White);
        window.draw(btnText);
    }
}

bool ButtonObject::contains(const sf::Vector2f& point) const {
    return getBounds().contains(point);
}

void ButtonObject::setCallback(Callback callback) {
    callback_ = std::move(callback);
}

void ButtonObject::onClick() {
    isPressed_ = !isPressed_;
    if (callback_) {
        callback_();
    }
}

InputFieldObject::InputFieldObject(const std::string& id)
    : VisualObject(ObjectType::InputField, id) {
    color_ = sf::Color::White;
}

void InputFieldObject::draw(sf::RenderWindow& window) const {
    sf::RectangleShape shape;
    shape.setPosition(position_);
    shape.setSize(size_);
    shape.setFillColor(isActive_ ? sf::Color(240, 240, 255) : color_);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.0f);
    window.draw(shape);
    
    std::string displayText = inputText_ + (isActive_ ? "|" : "");
    sf::Text fieldText(font_, displayText, 16);
    fieldText.setPosition(position_ + sf::Vector2f(5, 5));
    fieldText.setFillColor(sf::Color::Black);
    window.draw(fieldText);
}

void InputFieldObject::handleTextEntered(uint32_t unicode) {
    if (!isActive_) return;
    
    if (unicode == 8) {
        if (!inputText_.empty()) inputText_.pop_back();
    } else if (unicode >= 32 && unicode < 127) {
        inputText_ += static_cast<char>(unicode);
    }
}

void InputFieldObject::setActive(bool active) {
    isActive_ = active;
}

HistoryGraphObject::HistoryGraphObject(const std::string& id)
    : VisualObject(ObjectType::HistoryGraph, id) {
    color_ = sf::Color(150, 200, 150);
    for (int i = 0; i < 10; ++i) {
        values_.push_back(30 + rand() % 50);
    }
}

void HistoryGraphObject::draw(sf::RenderWindow& window) const {
    sf::RectangleShape background;
    background.setPosition(position_);
    background.setSize(size_);
    background.setFillColor(sf::Color(240, 240, 240));
    background.setOutlineColor(sf::Color(180, 180, 180));
    background.setOutlineThickness(2.0f);
    window.draw(background);
    
    if (values_.empty()) return;
    
    float barWidth = size_.x / values_.size();
    for (size_t i = 0; i < values_.size(); ++i) {
        float barHeight = (values_[i] / maxValue_) * size_.y;
        
        sf::RectangleShape bar;
        bar.setPosition(sf::Vector2f(position_.x + i * barWidth, 
                                     position_.y + size_.y - barHeight));
        bar.setSize(sf::Vector2f(barWidth - 2, barHeight));
        bar.setFillColor(color_);
        window.draw(bar);
    }
}

void HistoryGraphObject::update(const VariableDatabase& db) {
    VisualObject::update(db);
    if (!boundVariable_.empty()) {
        if (auto value = db.getVariableAs<float>(boundVariable_)) {
            addValue(*value);
        }
    }
}

bool HistoryGraphObject::contains(const sf::Vector2f& point) const {
    return getBounds().contains(point);
}

void HistoryGraphObject::addValue(float value) {
    values_.push_back(value);
    if (values_.size() > 20) {
        values_.erase(values_.begin());
    }
    if (value > maxValue_) {
        maxValue_ = value * 1.1f;
    }
}

ImageObject::ImageObject(const std::string& id)
    : VisualObject(ObjectType::Image, id) {
    color_ = sf::Color(200, 200, 200);
}

void ImageObject::draw(sf::RenderWindow& window) const {
    if (textureLoaded_) {
        sf::Sprite sprite(texture_);
        sprite.setPosition(position_);
        sprite.setScale(sf::Vector2f(size_.x / texture_.getSize().x, 
                                      size_.y / texture_.getSize().y));
        window.draw(sprite);
    } else {
        sf::RectangleShape placeholder;
        placeholder.setPosition(position_);
        placeholder.setSize(size_);
        placeholder.setFillColor(color_);
        placeholder.setOutlineColor(sf::Color::Black);
        placeholder.setOutlineThickness(2.0f);
        window.draw(placeholder);
        
        sf::Text text(font_, "Image", 20);
        text.setPosition(position_ + sf::Vector2f(10, 10));
        text.setFillColor(sf::Color::Black);
        window.draw(text);
    }
}

bool ImageObject::contains(const sf::Vector2f& point) const {
    return getBounds().contains(point);
}

bool ImageObject::loadFromFile(const std::string& filename) {
    if (texture_.loadFromFile(filename)) {
        textureLoaded_ = true;
        return true;
    }
    return false;
}

} // namespace xsmall_hmi