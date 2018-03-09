
#ifndef GAME_HPP
# define GAME_HPP

# include "main.hpp"
# include "Player.hpp"
# include "Timer.hpp"

class Game {
  private:
    Game(void);

    Player                        *_player;
    std::list <IEntity *>         _food;
    std::list <IEntity *>         _walls;
    IGraphism                     *_engine;

  public:
    ~Game(void);

    static Game                   &singleton(void);

    std::list <IEntity *>         mergeEntities(void) const;
    std::list <IEntity *>         _map;
    void                          initMap(unsigned int width, unsigned int height);
    void                          *getBinaryLib(void) const;
    IGraphism                     *getEngine(void) const;
    void                          setEngine(IGraphism  *engine);
    std::list <IEntity *>         getFood(void) const;
    void                          refresh(void);
    void                          start(unsigned int with, unsigned int height, int mode);
    void                          pause(void) const;
    void                          save(void) const;
    void                          quit(void) const;
    void                          initFood(unsigned int x, unsigned int y);
    void                          initMode(int mode);
    void                          switchEngine(eHook engine);
};

#endif
