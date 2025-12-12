#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <cstdint>

namespace xsmall_hmi {

class VariableDatabase;

enum class ObjectType {
    Rectangle,
    Line,
    Polyline,
    Text,
    Button,
    InputField,
    HistoryGraph,
    Image
};

class VisualObject {
public:
    VisualObject(ObjectType type, const std::string& id);
    virtual ~VisualObject() = default;
    
    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual void update(const VariableDatabase& db);
    virtual bool contains(const sf::Vector2f& point) const;
    
    void setPosition(const sf::Vector2f& pos);
    void setSize(const sf::Vector2f& size);
    void setColor(const sf::Color& color);
    void setText(const std::string& text);
    void setVariableBinding(const std::string& varName);
    
    const std::string& getId() const { return id_; }
    ObjectType getType() const { return type_; }
    sf::FloatRect getBounds() const;
    
protected:
    ObjectType type_;
    std::string id_;
    sf::Vector2f position_;
    sf::Vector2f size_;
    sf::Color color_;
    std::string text_;
    std::string boundVariable_;
    sf::Font font_;
};

class RectangleObject : public VisualObject {
public:
    RectangleObject(const std::string& id);
    void draw(sf::RenderWindow& window) const override;
};

class TextObject : public VisualObject {
public:
    TextObject(const std::string& id);
    void draw(sf::RenderWindow& window) const override;
    void update(const VariableDatabase& db) override;
};

class LineObject : public VisualObject {
public:
    LineObject(const std::string& id);
    void draw(sf::RenderWindow& window) const override;
    void setPoints(const sf::Vector2f& start, const sf::Vector2f& end);
    
private:
    sf::Vector2f startPoint_;
    sf::Vector2f endPoint_;
};

class PolylineObject : public VisualObject {
public:
    PolylineObject(const std::string& id);
    void draw(sf::RenderWindow& window) const override;
    void addPoint(const sf::Vector2f& point);
    
private:
    std::vector<sf::Vector2f> points_;
};

class ButtonObject : public VisualObject {
public:
    using Callback = std::function<void()>;
    
    ButtonObject(const std::string& id);
    void draw(sf::RenderWindow& window) const override;
    bool contains(const sf::Vector2f& point) const override;
    void setCallback(Callback callback);
    void onClick();
    
private:
    Callback callback_;
    bool isPressed_ = false;
};

class InputFieldObject : public VisualObject {
public:
    InputFieldObject(const std::string& id);
    void draw(sf::RenderWindow& window) const override;
    void handleTextEntered(uint32_t unicode);
    void setActive(bool active);
    bool isActive() const { return isActive_; }
    
private:
    bool isActive_ = false;
    std::string inputText_;
};

class HistoryGraphObject : public VisualObject {
public:
    HistoryGraphObject(const std::string& id);
    void draw(sf::RenderWindow& window) const override;
    void update(const VariableDatabase& db) override;
    bool contains(const sf::Vector2f& point) const override;
    void addValue(float value);
    
private:
    std::vector<float> values_;
    float maxValue_ = 100.0f;
};

class ImageObject : public VisualObject {
public:
    ImageObject(const std::string& id);
    void draw(sf::RenderWindow& window) const override;
    bool contains(const sf::Vector2f& point) const override;
    bool loadFromFile(const std::string& filename);
    
private:
    sf::Texture texture_;
    bool textureLoaded_ = false;
};

} // namespace xsmall_hmi