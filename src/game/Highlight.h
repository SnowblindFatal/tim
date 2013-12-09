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
    virtual void set(const std::string&, bool);
    virtual void update_rect(const sf::FloatRect&);
    virtual void draw(sf::RenderWindow& win);
	virtual bool checkPoint(sf::Vector2i);
	virtual std::string clicked(const sf::Vector2i&);
	friend class IsNearPoint;
protected:
    sf::RectangleShape rect;
	sf::RectangleShape del1;
	sf::RectangleShape del2;
	bool delete_active;
	bool real; //True when the Highlight has actually been updated with a sf::FloatRect; when it is real.
};

class PlatformHighlight : public Highlight {
public:
	PlatformHighlight();
    void set(const std::string&, bool);
    void update_rect(const sf::FloatRect&);
    void draw(sf::RenderWindow& win);

	//Check wether the clicked position is an active spot. True if it is, also set the corresponding bool and local_mouse.
	bool checkPoint(sf::Vector2i point);
	//Calculate the amount that we wanted to change.
	sf::Vector2i getDelta(const sf::Vector2i point);
	


protected:
	sf::RectangleShape delta_box;
	sf::Vector2i local_mouse;
};

class GravityChangerHighlight : public Highlight {
public:
	GravityChangerHighlight();
	void set(const std::string&, bool);
	void update_rect(const sf::FloatRect&);
	void draw(sf::RenderWindow&);

	bool checkPoint(sf::Vector2i);
	virtual std::string clicked(const sf::Vector2i&);

protected:
	sf::CircleShape	rotate;
	bool rotate_active;
};

class SeesawHighlight : public Highlight {
public:
	SeesawHighlight();
	void set(const std::string&, bool);
	void update_rect(const sf::FloatRect&);
	void draw(sf::RenderWindow&);

	bool checkPoint(sf::Vector2i);
	virtual std::string clicked(const sf::Vector2i&);

protected:
	sf::CircleShape	rotate;
	bool rotate_active;
};

class CatapultHighlight : public Highlight {
public:
	CatapultHighlight();
	void set(const std::string&, bool);
	void update_rect(const sf::FloatRect&);
	void draw(sf::RenderWindow&);

	bool checkPoint(sf::Vector2i);
	virtual std::string clicked(const sf::Vector2i&);

protected:
	sf::CircleShape	rotate;
	bool rotate_active;
};

#endif
