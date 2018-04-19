#include "Game.hpp"

Game::Game(void) {
    this->_player = new Player(4, 0);
    this->coreAudio = createCoreAudio();
    this->coreAudio->play(Theme);
    return;
}

Game::~Game(void) {
    std::list<IEntity *>::iterator itPos = this->_freePos.begin();
    while (itPos != this->_freePos.end()) {
        deleteEntity(*itPos);
        itPos++;
    }
    std::list<IEntity *>::iterator itWalls = this->_walls.begin();
    while (itWalls != this->_walls.end()) {
        deleteEntity(*itWalls);
        itWalls++;
    }
    std::list<IEntity *>::iterator itFire = this->_fire.begin();
    while (itFire != this->_fire.end()) {
        deleteEntity(*itFire);
        itFire++;
    }
    std::list<IEntity *>::iterator itFood = this->_food.begin();
    while (itFood != this->_food.end()) {
        deleteEntity(*itFood);
        itFood++;
    }
    delete this->_player;
    dlclose(BINARY_LIB);
    dlclose(BINARY_AUDIO);
}

Game   &Game::singleton(void) {
  static Game game;
  return game;
}

// GETTERS & SETTERS

IGraphism    *Game::getEngine() const
  { return this->_engine; }

void   Game::setEngine(IGraphism  *engine)
{
  this->_engine = engine;
  return;
}

std::list <IEntity *>  &Game::getFood(void) { return this->_food; }
std::list <IEntity *>  &Game::getBonus(void) { return this->_bonus; }
std::list <IEntity *>  &Game::getFreePos(void) { return this->_freePos; }
std::list <IEntity *>  &Game::getWalls(void) { return this->_walls; }
std::list <IEntity *>  &Game::getFire(void) { return this->_fire; }

std::list <IEntity *>  Game::mergeEntities(void) const {
    std::list<IEntity *> tmp;
    tmp = this->_player->getSnake();
    tmp.insert(tmp.end(), this->_food.begin(), this->_food.end());
    tmp.insert(tmp.end(), this->_walls.begin(), this->_walls.end());
    tmp.insert(tmp.end(), this->_fire.begin(), this->_fire.end());
    tmp.insert(tmp.end(), this->_bonus.begin(), this->_bonus.end());
  return tmp;
}

void  Game::initFood(void) {
    std::list<IEntity *>::iterator it =
        this->_freePos.begin();
    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<int> choose(
        0,
        static_cast<int>(this->_freePos.size() - 1)
    );
    std::advance(it, choose(engine));
    IEntity *food = createEntity( (*it)->getPosX(), (*it)->getPosY(), Food, NoDir, tFood );
    if (this->_food.size()) {
        deleteEntity(this->_food.front());
        this->_food.pop_back();
    }
    this->listAdd(this->_food, food);
    this->listErase(this->_freePos, (*it)->getPosX(), (*it)->getPosY());
    return;
}

void  Game::initFire(void) {
    std::list<IEntity *>::iterator it =
        this->_freePos.begin();
    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<int> choose(
        0,
        static_cast<int>(this->_freePos.size() - 1)
    );
    std::advance(it, choose(engine));
    IEntity *fire = createEntity( (*it)->getPosX(), (*it)->getPosY(), Fire, NoDir, tFire );
    if (this->_fire.size()) {
        this->listAdd(this->_freePos, createEntity(this->_fire.front()->getPosX(), this->_fire.front()->getPosY(), Free, NoDir, None));
        deleteEntity(this->_fire.front());
        this->_fire.pop_back();
    }
    this->listAdd(this->_fire, fire);
    this->listErase(this->_freePos, (*it)->getPosX(), (*it)->getPosY());
    return;
}
void  Game::initBonus(void) {
    std::list<IEntity *>::iterator it =
        this->_freePos.begin();
    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<int> choose(
        0,
        static_cast<int>(this->_freePos.size() - 1)
    );
    std::advance(it, choose(engine));
    IEntity *bonus = createEntity( (*it)->getPosX(), (*it)->getPosY(), Bonus, NoDir, tBonus );
    if (this->_bonus.size()) {
        this->listAdd(this->_freePos, createEntity(this->_bonus.front()->getPosX(), this->_bonus.front()->getPosY(), Free, NoDir, None));
        deleteEntity(this->_bonus.front());
        this->_bonus.pop_back();
    }
    this->listAdd(this->_bonus, bonus);
    this->listErase(this->_freePos, (*it)->getPosX(), (*it)->getPosY());
    return;
}

void  Game::initMap(unsigned int width, unsigned int height) {
    if (this->_freePos.size()) {
        std::list<IEntity *>::iterator it = this->_freePos.begin();
        while ( it != this->_freePos.end()) {
            deleteEntity((*it));
            it++;
        }
        this->_freePos.clear();
    }
    for(unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            IEntity *tmp = createEntity( x, y, Free, NoDir, None);
            this->listAdd(this->_freePos, tmp);
        }
    }
    return;
}

void Game::initGame(unsigned int width, unsigned int height, int mode) {
    delete this->_player;
    this->_engine->updateDirection(Right);
    this->_player = new Player(4, 0);
    this->initMap(width, height);
    this->initMode(mode);
    this->_player->initSnake();
    this->initFood();
}

void  Game::start(unsigned int width, unsigned int height, int mode) {
    Timer speed(200);
    Timer fire(4000);
    this->_engine = createEngine(width, height, Right);
    while (this->_engine->getStatus() != Exit) {
        this->_engine->handleEvent(speed.getDiff());
        switch(this->_engine->getStatus()) {
            case Play :
                if((this->_player->getSnake().size()) && !this->_player->checkDeath())
                    break;
            case Start :
                this->initGame(width, height, mode);
                speed.resetDiff(200);
                this->_engine->updateStatus(Play);
            default : break;
        }
        if ( this->_engine->getStatus() == Play ) {
            if (fire.update()) {
                 this->initFire();
            }
            if (speed.update()) {
                if (this->_player->getScoreChange()) {
                    if (this->_player->getScore() % 5 == 0 && speed.getDiff() >= 70)
                        speed.changeDiff(10);
                    if (this->_player->getScore() % 15 == 0)
                        this->initBonus();
                }
                this->_player->move(this->_engine->getDirection());
                this->refresh();
                this->_engine->reverseDirectionChecker();
            }
        }
        if (this->_engine->getStatus() == Pause) {
            if (this->_player->getSnake().size() &&this->_player->checkDeath())
                this->pause(this->_player->getScore());
            else
                this->pause(-1);
        }
        if (this->_engine->engineHasChanged()) {
            eStatus tmp = this->_engine->getStatus();
            this->_engine->updateStatus(Pause);
            switchEngine(
                this->_engine->getEngine(),
                this->_engine->getDirection()
            );
            this->_engine->updateStatus(tmp);
        }
    }
    return;
}


void  Game::pause(int score) {
    this->_engine->displayPause(score);
}

void  Game::refresh(void) {
    this->_engine->drawFrame(this->mergeEntities(), this->_player->getLife(), this->_player->getScore());
    return;
}

void Game::initMode(int mode) {
    switch(mode) {
        case 0 : break;
        case 1 : {
            for (unsigned int x = 0; x < this->_engine->getWidth(); x++) {
                this->listAdd(this->_walls, createEntity( x, 0, Wall, NoDir, tWall));
                this->listErase(this->_freePos, x, 0);
                this->listAdd(this->_walls, createEntity(x, this->_engine->getHeight() - 1, Wall, NoDir, tWall));
                this->listErase(this->_freePos, x, this->_engine->getHeight() - 1);
            }
            for (unsigned int y = 0; y < this->_engine->getHeight(); y++) {
                this->listAdd(this->_walls, createEntity( 0, y, Wall, NoDir, tWall));
                this->listErase(this->_freePos, 0, y);
                this->listAdd(this->_walls, createEntity( this->_engine->getWidth() - 1, y, Wall, NoDir, tWall));
                this->listErase(this->_freePos, this->_engine->getWidth() - 1, y);
            }
        }
        default: break;
    }
    return;
}

void    Game::switchEngine(eEngine engine, eDirection direction) {
    unsigned int tmpWidth = this->_engine->getWidth();
    unsigned int tmpHeight = this->_engine->getHeight();
    std::string path;
    if (this->_engine != NULL){
        deleteEngine(this->_engine);
    }
    switch(engine) {
        case SDL :
            path = "sdl.so";
            break;
        case SFML :
            path = "sfml.so";
            break;
        case GL :
            path = "glfw.so";
            break;
        default : break;
    }
    if (!openBinaryLib(const_cast<char*>(path.c_str()))) {
        throw Exception::Throw(LIB_FAIL);
    }
    this->_engine = createEngine(tmpWidth, tmpHeight, direction);
    return;
}

void    Game::listErase(std::list <IEntity *> &list, unsigned int x, unsigned int y) {
    std::list<IEntity *>::iterator check = list.begin();
    while (check != list.end()) {
        if ((*check)->getPosX() == x && (*check)->getPosY() == y) {
            deleteEntity((*check));
            check = list.erase(check);
        }
        else
            ++check;
    }
    return;
}

void    Game::listAdd(std::list <IEntity *> &list, IEntity *entity) {
    list.push_front(entity);
    return;
}

bool    Game::listCheck(std::list <IEntity *> &list, unsigned int x, unsigned int y) {
    std::list<IEntity *>::iterator check = list.begin();
    while (check != list.end()) {
        if ((*check)->getPosX() == x && (*check)->getPosY() == y) {
            return true;
        }
        check++;
    }
    return false;
}
