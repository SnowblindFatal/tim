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
	virtual ~Highlight() {}
    virtual void set(std::string, bool);
    virtual void update_rect(sf::FloatRect);
    virtual void draw(sf::RenderWindow& win);
	virtual bool checkPoint(sf::Vector2i) {return false;}
	virtual sf::Vector2i getDelta(sf::Vector2i) {return sf::Vector2i(0,0);}
protected:
    sf::RectangleShape rect;
};

class PlatformHighlight : public Highlight {
public:
	PlatformHighlight();
    void set(std::string, bool);
    void update_rect(sf::FloatRect);
    void draw(sf::RenderWindow& win);

	//Check wether the clicked position is an active spot. True if it is, also set the corresponding bool and local_mouse.
	bool checkPoint(sf::Vector2i point);
	//Calculate the amount that we wanted to change.
	sf::Vector2i getDelta(sf::Vector2i point);
	


protected:
	sf::RectangleShape delta_height;
	sf::RectangleShape delta_width;
	bool height_active;
	bool width_active;
	sf::Vector2i local_mouse;
};

#endif
