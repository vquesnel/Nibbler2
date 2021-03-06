#include "WindowSfml.hpp"

Window::Window(void) { return; }

Window::Window(Window const &src) { *this = src; }

Window  &Window::operator=(Window const &) { return *this; }

Window::Window(unsigned int width, unsigned int height, eDirection direction) :
  direction(direction),
  status(Pause),
  engine(SFML),
  wWidth(width),
  wHeight(height)
{
    sf::Image icon;
    this->window = new sf::RenderWindow(
        sf::VideoMode(
            this->wWidth * CELL_UNITY,
            this->wHeight * CELL_UNITY + CELL_UNITY * 2),
        "Nibbler SFML",
        sf::Style::Titlebar | sf::Style::Close
    );
    this->initTextures();
    this->pFont.loadFromFile("./assets/roboto.ttf");
    icon.loadFromFile("./assets/appicon.bmp");
    this->window->setIcon(256, 256, icon.getPixelsPtr());
    return;
}

Window::~Window(void) {
    // this->window->close();
    delete this->window;
}

void        Window::handleEvent(int milliseconds) {
    (void)milliseconds;
    if (this->window->isOpen()) {
        sf::Event event;
        while(this->window->pollEvent(event)) {
            this->setEngine(event);
            this->setStatus(event);
            if (this->status == Play)
                this->setDirection(event);
            else if (this->status == Pause)
                this->handlePauseEvent(event);
        }
    }
}

void       Window::handlePauseEvent(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::R)
            { this->status = Play; return; }
        else if (event.key.code ==sf::Keyboard::S)
            { this->status = Start; return; }
    }
    return;
}

eDirection   Window::getDirection(void) const {
    return this->direction;
}

void   Window::setDirection(sf::Event event) {
    if (event.type == sf::Event::KeyPressed && !this->directionChecker) {
        if (event.key.code == sf::Keyboard::Up && this->direction != Down && this->direction!= Up)
            { this->directionChecker = true; this->direction = Up; }
        else if (event.key.code ==sf::Keyboard::Down  && this->direction != Up && this->direction!= Down)
            { this->directionChecker = true; this->direction = Down; }
        else if (event.key.code == sf::Keyboard::Left && this->direction != Right && this->direction!= Left)
            { this->directionChecker = true; this->direction = Left; }
        else if (event.key.code == sf::Keyboard::Right && this->direction != Left && this->direction!= Right)
            { this->directionChecker = true; this->direction = Right; }
    }
    return;
}

void    Window::updateDirection(eDirection direction){
    this->direction = direction;
}
bool   Window::directionHasChanged(void) const {
    return this->directionChecker;
}

void    Window::reverseDirectionChecker(void) {
    this->directionChecker = false;
}

eStatus   Window::getStatus(void) const {
    return this->status;
}

void    Window::setStatus(sf::Event event) {
    if (event.type == sf::Event::Closed) {
        this->status = Exit;
        this->window->close();
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->status = Exit;
            this->window->close();
        }
        else if (event.key.code == sf::Keyboard::Space)
           { this->status = Pause; }
    }
    return;
}

void    Window::updateStatus(eStatus status)  {
    this->status = status;
    return;
}

void    Window::setEngine(sf::Event event) {
    if (event.type == sf::Event::KeyPressed && !this->engineChecker) {
        if (event.key.code == sf::Keyboard::F && this->engine != SDL) {
            this->engine = SDL;
            this->engineChecker = true;
            this->window->close();
            return;
        }
        else if (event.key.code == sf::Keyboard::G && this->engine != GL) {
            this->engine = GL;
            this->engineChecker = true;
            this->window->close();
            return;
        }
    }
    return;
}

eEngine  Window::getEngine(void) const {
    return this->engine;
}

bool    Window::engineHasChanged(void) const{
    return this->engineChecker;
}

void    Window::setEngineChange(bool status) {
    this->engineChecker = status;
}

void      Window::drawFrame(std::list <IEntity *> data,int lives, int score) const {
    sf::Color color(22,22, 24, 0);
    this->window->clear(color);
    sf::Sprite sprite;
    std::list <IEntity *>::iterator iter = data.begin();

    while (iter != data.end()) {
        sprite.setPosition(sf::Vector2f((*iter)->getPosX() * CELL_UNITY, (*iter)->getPosY() * CELL_UNITY)); // position absolue
        eTexture img = (*iter)->getTexture();
        sprite.setTexture(this->_textures.find(img)->second);
        this->window->draw(sprite);
        iter++;
    }
    this->drawMenu(lives, score);
    this->window->display();
    return;
}

void    Window::drawMenu(int lives, int score) const {
    sf::Sprite  sprite;
    int x = CELL_UNITY;
    int y = this->wHeight * CELL_UNITY + CELL_UNITY;

    sprite.setPosition(sf::Vector2f(0, this->wHeight * CELL_UNITY));
    sprite.setTexture(this->_textures.find(NoImg)->second);
    sprite.setScale(CELL_UNITY, 0.2f);
    this->window->draw(sprite);
    while (lives != 0) {
        sprite.setPosition(sf::Vector2f(x, y - CELL_UNITY / 2));
        sprite.setTexture(this->_textures.find(Life)->second);
        sprite.setScale(1, 1);
        this->window->draw(sprite);
        x += CELL_UNITY + CELL_UNITY / 2;
        lives--;
    }
    sf::Text text;
    text.setFont(this->pFont);
    std::string sScore = "Score : ";
    sScore += std::to_string(score);
    text.setString(sScore.c_str());
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(
        this->wWidth * CELL_UNITY - 140,
        this->wHeight * CELL_UNITY + CELL_UNITY / 2
    ));
    this->window->draw(text);
  return;
}

bool            Window::displayPause(int score, int bestScore)  {
    sf::Color color(22,22, 24, 0);
    sf::Texture img;
    sf::Sprite background;
    background.setPosition(sf::Vector2f(0, 0));
    img.loadFromFile("./assets/menu.bmp");
    background.setTexture(img);
    background.setScale(static_cast<float>(CELL_UNITY * wWidth) / 512 , static_cast<float>(CELL_UNITY * wHeight) / 512 );
    this->window->clear(color);
    this->window->draw(background);
    if (score >= 0) {
        sf::Text text;
        text.setFont(this->pFont);
        std::string sScore = "You lose... Your score was: ";
        sScore += std::to_string(score);
        sScore +=  " Your best score is: ";
        sScore += std::to_string(bestScore);
        text.setString(sScore.c_str());
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(
            CELL_UNITY,
            this->wHeight * CELL_UNITY + CELL_UNITY / 2
        ));
        this->window->draw(text);
    }
    this->window->display();
    return true;
}


unsigned int    Window::getWidth(void) const {
    return this->wWidth;
}

unsigned int    Window::getHeight(void) const {
    return this->wHeight;
}

Window      *createWindow(unsigned int width, unsigned int height, eDirection direction) {
    return new Window(width, height, direction);
}

void        deleteWindow(Window *window) {
    delete window;
    return;
}

void       Window::initTextures(void) {
    for (int i = 1; i <= 24; i++) {
        sf::Texture texture;
        std::string name = "./assets/";
        name += std::to_string(i);
        name += ".bmp";
        texture.loadFromFile(name.c_str());
        this->_textures.insert(
            std::make_pair(static_cast<eTexture>(i), texture)
        );
    }
    return;
}
