#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H
#include <string>
#include <SFML/Graphics.hpp>

/*Highlighting implementation
 All you need to do is remember to
 update_rect() with a bounding box of the SFML implementation of a GameObject
 and to draw() the highlight.
 */
class Highlight {
public:
    Highlight();
    void set(std::string, bool);
    void update_rect(sf::FloatRect);
    void draw(sf::RenderWindow& win);
protected:
    sf::RectangleShape rect;
};

#endif