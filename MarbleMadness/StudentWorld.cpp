#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;



GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : m_nActors(0), m_nPlayers(0), m_nCrystals(0), m_finishedLevel(false), 
    GameWorld(assetPath)
{
    
}

int StudentWorld::init() {

    m_finishedLevel = false;
    m_nCrystals = 0;

    Level lev(assetPath());

    ostringstream oss;
    oss.fill('0');
    oss << "level" << setw(2) << getLevel() << ".txt";
    string curLevel = oss.str();

    Level::LoadResult result = lev.loadLevel(curLevel);
    if (result == Level::load_fail_file_not_found && getLives() > 0)
        return GWSTATUS_PLAYER_WON;
    if (getLevel() > 99 && getLives() > 0)
        return GWSTATUS_PLAYER_WON;
    if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;

    
    
    for (int y = 0; y < VIEW_HEIGHT; y++) {
        for (int x = 0; x < VIEW_WIDTH; x++) {
            
            Level::MazeEntry item = lev.getContentsOf(x, y);
            
            switch (tolower(item))
            {
            case Level::empty: 
                break;
            case Level::exit:
                m_actors[m_nActors] = new Exit(x, y);
                m_actors[m_nActors]->setWorld(this);
                m_nActors++;
                break;
            case Level::player:
                m_player = new Avatar(x, y);
                m_player->setWorld(this);
                m_nPlayers++;
                break;
            case Level::horiz_ragebot:
                m_actors[m_nActors] = new Ragebot(x, y, this, right);
                m_actors[m_nActors]->setWorld(this);
                m_nActors++;
                break;
            case Level::vert_ragebot:
                m_actors[m_nActors] = new Ragebot(x, y, this, down);
                m_actors[m_nActors]->setWorld(this);
                m_nActors++;
                break;
            case Level::thiefbot_factory:
                m_actors[m_nActors] = new ThiefbotFactory(x, y, false);
                m_actors[m_nActors]->setWorld(this);
                m_nActors++;
                break;
            case Level::mean_thiefbot_factory:
                m_actors[m_nActors] = new ThiefbotFactory(x, y, true);
                m_actors[m_nActors]->setWorld(this);
                m_nActors++;
                break;
            case Level::wall:
                m_actors[m_nActors] = new Wall(x, y);
                m_actors[m_nActors]->setWorld(this);
                m_nActors++;
                break;
            case Level::marble:
                m_actors[m_nActors] = new Marble(x, y);
                m_actors[m_nActors]->setWorld(this);
                m_nActors++;
                break;
            case Level::pit:
                m_actors[m_nActors] = new Pit(x, y);
                m_actors[m_nActors]->setWorld(this);
                m_nActors++;
                break;
            case Level::crystal:
                m_actors[m_nActors] = new Crystal(x, y);
                m_actors[m_nActors]->setWorld(this);
                m_nActors++;
                m_nCrystals++;
                break;
            case Level::restore_health:
                m_actors[m_nActors] = new RestoreHealth(x, y);
                m_actors[m_nActors]->setWorld(this);
                m_nActors++;
                break;
            case Level::extra_life:
                m_actors[m_nActors] = new ExtraLife(x, y);
                m_actors[m_nActors]->setWorld(this);
                m_nActors++;
                break;
            case Level::ammo:
                m_actors[m_nActors] = new AmmoGoodie(x, y);
                m_actors[m_nActors]->setWorld(this);
                m_nActors++;
                break;
            }
        }
    }
        
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
    // This code is here merely to allow the game to build, run, and terminate after you type q
    /*setGameStatText("Game will end when you type q");
    
	return GWSTATUS_CONTINUE_GAME;*/

    ostringstream board;
    board.fill('0');
    board << "Score: " << setw(7) << getScore() << "  ";
    board << "Level: " << setw(2) << getLevel() << "  ";
    board.fill(' ');
    board << "Lives: " << setw(2) << getLives() << "  ";
    board << "Health: " << setw(3) << (m_player->getHealth() / 20.0) * 100 << "%  ";
    board << "Ammo: " << setw(3) << m_player->getPeas() << "  ";
    board << "Bonus: " << setw(4) << m_player->getBonus();

    setGameStatText(board.str());

    for (int i = 0; i < m_nActors; i++) {
        if (!(m_actors[i]->isAlive())) {
            swap(m_actors[i], m_actors[m_nActors - 1]);
            delete m_actors[m_nActors - 1];
            m_nActors--;
        }
    }

    for (int i = 0; i < m_nActors; i++) {
        if (m_actors[i] != nullptr && m_actors[i]->isAlive())
            m_actors[i]->doSomething();
        if (m_player != nullptr && !m_player->isAlive()) {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (m_finishedLevel == true)
            return GWSTATUS_FINISHED_LEVEL;
    }
    
    if (m_player != nullptr && m_player->isAlive())
        m_player->doSomething();

    if (m_player != nullptr)
        m_player->reduceBonusByOne();

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    int temp = m_nActors;
    for (int i = 0; i < temp; i++) {
        if (m_actors[i] != nullptr) {
            delete m_actors[i];
            m_nActors--;
        }
    }
    if (m_player != nullptr && m_nPlayers > 0) {
        delete m_player;
        m_nPlayers--;
    }
}

bool StudentWorld::noObstruct(double x, double y) {
    for (int i = 0; i < m_nActors; i++) {
        if (round(m_actors[i]->getX()) == round(x) &&
            round(m_actors[i]->getY()) == round(y) &&
            !m_actors[i]->isPassable())
        {
            return false;
        }
    }
    if (!sharesSquareWithPlayer(x, y))
        return true;
    else 
        return false;
}

bool StudentWorld::noPeaStoppage(double x, double y) {
    for (int i = 0; i < m_nActors; i++) {
        if (round(m_actors[i]->getX()) == round(x) &&
            round(m_actors[i]->getY()) == round(y) &&
            m_actors[i]->stopsPea())
        {
            return false;
        }
    }
    if (!sharesSquareWithPlayer(x, y))
        return true;
    else
        return false;
}

bool StudentWorld::allowsMarble(Actor* a, double x, double y) {
    for (int i = 0; i < m_nActors; i++) {
        if (round(m_actors[i]->getX()) == round(x) &&
            round(m_actors[i]->getY()) == round(y) &&
            m_actors[i]->acceptsMarble())
        {
            
            return true;
        }
    }
    return noObstruct(x, y);
}

bool StudentWorld::swallowSwallowable(Actor* a) {

    for (int i = 0; i < m_nActors; i++) {
        if (round(m_actors[i]->getX()) == round(a->getX()) &&
            round(m_actors[i]->getY()) == round(a->getY()) &&
            m_actors[i]->isSwallowable())
        {
            m_actors[i]->died();
            return true;
        }
    }

    return false;
}

bool StudentWorld::hasPushableObject(double x, double y, int dir) {
    for (int i = 0; i < m_nActors; i++) {
        if (round(m_actors[i]->getX()) == round(x) &&
            round(m_actors[i]->getY()) == round(y) &&
            m_actors[i]->isPushable())
            if(m_actors[i]->push(dir))
                return true;
    }
    return false;
}

bool StudentWorld::damageSomething(Actor* a, int damageAmt) {
    for (int i = 0; i < m_nActors; i++) {
        if (round(m_actors[i]->getX()) == round(a->getX()) &&
            round(m_actors[i]->getY()) == round(a->getY()) &&
            !m_actors[i]->isPeaProof()) {
            m_actors[i]->getAttacked();
            a->died();
            return true;
        }
    }
    if (round(m_player->getX()) == round(a->getX()) &&
        round(m_player->getY()) == round(a->getY()) &&
        !m_player->isPeaProof()) {
        m_player->getAttacked();
        a->died();
        return true;
    }
    return false;
}

bool StudentWorld::existsPeaPath(double x, double y, int dx, int dy) {
    for (int i = 0; i < m_nActors; i++) {
        if (round(m_actors[i]->getX()) == round(x + dx) &&
            round(m_actors[i]->getY()) == round(y + dy) &&
            m_actors[i]->stopsPea() == true) {
            return false;
        }
    }
    return true;
}

void StudentWorld::shootPea(double startX, double startY, int dir) {
    m_actors[m_nActors] = new Pea(startX, startY, dir);
    m_actors[m_nActors]->setWorld(this);
    m_nActors++;
}

bool StudentWorld::sharesSquareWithPlayer(Actor* a) {
    if (m_player != nullptr &&
        round(m_player->getX()) == round(a->getX()) &&
        round(m_player->getY()) == round(a->getY())) {
        return true;
    }
    return false;
}

bool StudentWorld::sharesSquareWithPlayer(double x, double y) {
    if (m_player != nullptr &&
        round(m_player->getX()) == round(x) &&
        round(m_player->getY()) == round(y)) {
        return true;
    }
    return false;
}

int StudentWorld::getPlayerBonus() {
    return m_player->getBonus();
}

void StudentWorld::incPlayerBonus(int increase) {
    m_player->incBonus(increase);
}

void StudentWorld::incNumItem(char item, int increase) {
    switch (item) {
    case Level::crystal:
        m_nCrystals += increase;
        break;
    }
}

bool StudentWorld::anyCrystals()
{
    return false;
}

bool StudentWorld::noCrystals() {
    return m_nCrystals == 0;
}

void StudentWorld::setLevelFinished(bool finished) {
    m_finishedLevel = finished;
}

void StudentWorld::setPlayerHealthToFull() {
    if (m_player != nullptr &&
        m_player->getHealth() < 20)
        m_player->setHealth(20);
}

void StudentWorld::addPlayerAmmo(int ammo) {
    if (m_player != nullptr)
        m_player->setPeas(m_player->getPeas() + ammo);
}

bool StudentWorld::existsClearShotToPlayer(double x, double y, int dx, int dy) {
    double currentX = x + dx, currentY = y + dy;

    while (!sharesSquareWithPlayer(currentX, currentY) && 
            noPeaStoppage(currentX, currentY) &&
        (currentX >= 0 && currentX < VIEW_WIDTH &&
            currentY >= 0 && currentY < VIEW_HEIGHT)) {
        currentX += dx;
        currentY += dy;
    }

    if (sharesSquareWithPlayer(currentX, currentY)) {
        return true;
    }
    
    return false;
}

Actor* StudentWorld::getColocatedStealable(double x, double y) const {
    for (int i = 0; i < m_nActors; i++) {
        if (round(m_actors[i]->getX()) == round(x) &&
            round(m_actors[i]->getY()) == round(y) &&
            m_actors[i]->isPickable()) {
            return m_actors[i];
        }
    }
    return nullptr;
}

bool StudentWorld::doFactoryCensus(double x, double y, int distance, int& count) {
    for (int i = 0; i < m_nActors; i++) {
        if (m_actors[i]->isInFactoryCensus() &&
            round(m_actors[i]->getX()) - distance >= round(x) &&
            round(m_actors[i]->getX()) + distance <= round(x) &&
            round(m_actors[i]->getY()) - distance >= round(y) &&
            round(m_actors[i]->getY()) + distance <= round(y)) {
            if (m_actors[i]->isInFactoryCensus() &&
                round(m_actors[i]->getX()) == round(x) &&
                round(m_actors[i]->getY()) == round(y)) {
                return false;
            }
            count++;
        }
    }
    return true;
}

void StudentWorld::spawnNewThiefBot(double x, double y, bool mean) {
    if (mean) {
        m_actors[m_nActors] = new MeanThiefbot(x, y, this);
    }
    else {
        m_actors[m_nActors] = new RegularThiefbot(x, y, this);
    }
    m_nActors++;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}