#ifndef MAIN_HPP
# define MAIN_HPP

# include <iostream>
# include <dlfcn.h>
# include "IGraphism.hpp"
# include "IEntity.hpp"
# include <list>

extern void *BINARY_LIB;

class Game;

IGraphism   *createEngine(
  unsigned int width,
  unsigned int height
);

IEntity     *createEntity(
  unsigned int x,
  unsigned int y,
  eType type,
  eDirection direction,
  eTexture texture
);

void        deleteEngine(IGraphism *engine);
void        deleteEntity(IEntity *entity);
void        dlerror_wrapper(void);


#endif
