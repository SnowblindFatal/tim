

#ifndef LEVEL_H
#define LEVEL_H
#include <list>
#include <utility>
#include "GameObject.h"
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace {
    b2Vec2 default_gravity(0.0f, 9.8f);
    float32 timestep = 1.0f/60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
}

//Copyright: Allen Jordan, Xabier Koetxea, under GNU licence.
class DebugDraw : public b2Draw
{
public:
    DebugDraw(sf::RenderWindow &window);
    virtual ~DebugDraw();
    
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    void DrawTransform(const b2Transform& xf);
    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
    void DrawString(int x, int y, const char* string);
    void DrawAABB(b2AABB* aabb, const b2Color& color);
    sf::Color B2SFColor(const b2Color &color, int alpha);
    void DrawMouseJoint(b2Vec2& p1, b2Vec2& p2, const b2Color &boxColor, const b2Color &lineColor);
    
private:
    sf::RenderWindow *window;
    size_t RATIO = 10;
};


class LevelData
{
public:
	LevelData(sf::RenderWindow& _App) : phys_world(default_gravity), level_loaded(false), App(_App), DebugDrawInstance(_App) {
        phys_world.SetDebugDraw(&DebugDrawInstance);
        DebugDrawInstance.SetFlags(b2Draw::e_shapeBit);
    }
    ~LevelData() {
        for (auto &iter : levelobjects) {
            delete iter;
        }
    }
	void loadlevel() {
        //Load a demo level:
        //NOTE: The sprites and bodies do not match yet.
        
        
        
        //A static block:
        std::pair<float,float> statsize(10.0f, 3.0f);
        
        //B2D:
        b2BodyDef blockbodydef;
        blockbodydef.position.Set(12.0f, 40.0f);
        b2Body* statblock = phys_world.CreateBody(&blockbodydef);
        b2PolygonShape blockbox;
        blockbox.SetAsBox(statsize.first, statsize.second);
        statblock->CreateFixture(&blockbox, 0.0f);
        //SFML:
        sf::RectangleShape statblock_sprite;
        sf::Vector2f statspritesize(statsize.first * 20, statsize.second * 20);
        statblock_sprite.setSize(statspritesize);
        //statblock_sprite.setOrigin(100.0f/2, 30.0f/2);
        statblock_sprite.setFillColor(sf::Color::White);
        
        
        //A moving block:
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(22.5f, 20.0f);
        b2Body* dynblock = phys_world.CreateBody(&bodyDef);
        b2PolygonShape dynblockbox;
        dynblockbox.SetAsBox(1.0f, 1.0f);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynblockbox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        dynblock->CreateFixture(&fixtureDef);
        //SFML:
        sf::RectangleShape dynblock_sprite;
        sf::Vector2f dynspritesize(20.0f, 20.0f); //Here, the Box2D size is half of the box2D side size.
        dynblock_sprite.setSize(dynspritesize);
        //dynblock_sprite.setOrigin(10.0f/2, 10.0f/2);
        dynblock_sprite.setFillColor(sf::Color::Red);
        
        
        //Must figure out a better way to store sprites and GameObjects, this is just for the demo
        sprites.push_back(statblock_sprite);
        sprites.push_back(dynblock_sprite);
        auto sprite_it = sprites.begin();
        levelobjects.push_back(new GameObject("Static", statblock, &(*sprite_it)));
        sprite_it++;
        levelobjects.push_back(new GameObject("Dynamic", dynblock, &(*sprite_it)));
        
        level_loaded = true;
	}
    
    
	void draw(bool debug=false) {
        if (debug)
            phys_world.DrawDebugData();
            
            
        for (auto &iter : levelobjects) {
            iter->update_sprite();
            App.draw(*(iter->sprite_ptr));
        }
    }
    
    //One Box2D step:
	void simulate() {
        phys_world.Step(timestep, velocityIterations, positionIterations);
    }
    
	bool loaded(void) const {
        return level_loaded;
    }
private:
	
	b2World phys_world;
    DebugDraw DebugDrawInstance;
    sf::RenderWindow& App;
    
    bool level_loaded;
	std::list<sf::RectangleShape> sprites;      //Just for the demo
    
    std::list<GameObject* > levelobjects;		//The level itself
	std::list<GameObject* > playerobjects;		//The objects that the player has placed

};
#endif //LEVEL_H