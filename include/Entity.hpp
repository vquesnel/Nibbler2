#ifndef ENTITY_HPP
# define ENTITY_HPP

# include "IEntity.hpp"


class Entity : public IEntity  {
  private:
    Entity(void);
    Entity(Entity const &src);

    Entity  &operator=(Entity const &rhs);

    unsigned int          _posX;
    unsigned int          _posY;
    eType                 _type;
    eDirection            _direction;
    eTexture              _texture;

  public:
    Entity(unsigned int x, unsigned int y, eType type, eDirection direction, eTexture texture);
    ~Entity(void);

    void                  randomizePosition(void);
    unsigned int          getPosX(void) const;
    unsigned int          getPosY(void) const;
    eType                 getType(void) const;
    eDirection            getDirection(void) const;
    eTexture              getTexture(void) const;
    void                  setPosX(unsigned int x);
    void                  setPosY(unsigned int y);
    void                  setType(eType type);
    void                  setDirection(eDirection direction);
    void                  setTexture(eTexture texture);
};

extern "C" {
  Entity                  *createEntity(unsigned int x, unsigned int y, eType type, eDirection direction, eTexture texture);
  void                     deleteEntity(Entity *entity);
}

#endif
