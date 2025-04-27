#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, double startX, double startY, int dir, double size)
	: m_world(nullptr), m_health(NULL), m_stopsPea(false), m_isPassable(false),
	m_isPushable(false), m_isPeaProof(true), m_acceptsMarble(false),
	GraphObject(imageID, startX, startY, dir, size) {
	setDirection(dir);
	moveTo(startX, startY);
}

void Actor::setHealth(int health) {
	m_health = health;
}

int Actor::getHealth() const {
	return m_health;
}

void Actor::incHealth(int change) {
	m_health += change;
}

bool Actor::isAlive() const {
	if (m_health > 0)
		return true;
	else
		return false;
}

void Actor::died() {
	setHealth(0);
	setVisible(false);
	makePassable(true);
	setPeaProof(true);
	setPushable(false);
	setPeaStoppage(false);
	return;
}

bool Actor::isPassable() const  {
	return m_isPassable;
}

void Actor::makePassable(bool pass) {
	m_isPassable = pass;
}

bool Actor::isPushable() const {
	return m_isPushable;
}

bool Actor::push(int dir) {
	return false;
}

void Actor::setPushable(bool pushable) {
	m_isPushable = pushable;
}

void Actor::setWorld(StudentWorld* world) {
	m_world = world;
}

StudentWorld* Actor::getWorld() const {
	return m_world;
}

bool Actor::isPeaProof() const {
	return m_isPeaProof;
}

void Actor::setPeaProof(bool peaProof) {
	m_isPeaProof = peaProof;
}

bool Actor::stopsPea() {
	return m_stopsPea;
}

void Actor::setPeaStoppage(bool stoppage) {
	m_stopsPea = stoppage;
}

void Actor::setMarbleAcceptance(bool accepts) {
	m_acceptsMarble = accepts;
}

bool Actor::acceptsMarble() {
	return m_acceptsMarble;
}

bool Actor::isSwallowable() const {
	return false;
}

void Actor::increasePlayerScore(int increase) {
	getWorld()->increaseScore(increase);
}

void Actor::getAttacked() {
	return;
}

bool Actor::isPickable() {
	return false;
}

void Actor::setPickable(bool pickable) {
	return;
}

bool Actor::isInFactoryCensus() {
	return false;
}



Wall::Wall(double startX, double startY, double size) :
	Actor(IID_WALL, startX, startY, none, size) {
	setPeaStoppage(true);
	setHealth(1);
}

void Wall::doSomething() {
	return;
}



Pea::Pea(double startX, double startY, int dir, double size) :
	m_dx(0), m_dy(0), Actor(IID_PEA, startX, startY, dir, size) {
	makePassable(true);
	setHealth(1);
	switch (dir) {
		case up:
			m_dy = 1;
			break;
		case down:
			m_dy = -1;
			break;
		case right:
			m_dx = 1;
			break;
		case left:
			m_dx = -1;
			break;
	}
}

void Pea::doSomething() {
	if (!isAlive()) {
		return;
	}

	if (getWorld()->damageSomething(this, peaHitPoints))
		return;

	if (!getWorld()->existsPeaPath(getX(), getY(), 0, 0)) {
		died();
		return;
	}

	moveTo(getX() + m_dx, getY() + m_dy);

	if (getWorld()->damageSomething(this, peaHitPoints))
		return;

	if (!getWorld()->existsPeaPath(getX(), getY(), 0, 0)) {
		died();
		return;
	}
}



Pit::Pit(double startX, double startY, double size) :
	Actor(IID_PIT, startX, startY, size) {
	setHealth(1);
	setMarbleAcceptance(true);
	setPeaStoppage(false);
}

void Pit::doSomething() {

	if (!isAlive()) {
		return;
	}

	if (getWorld()->swallowSwallowable(this))
		died();
	
	return;
}



Exit::Exit(double startX, double startY, double size)
	: Actor(IID_EXIT, startX, startY, none, size) {
	setHealth(1);
	setVisible(false);
	makePassable(true);
}

void Exit::doSomething() {
	if (getWorld()->noCrystals()) {
		getWorld()->playSound(SOUND_REVEAL_EXIT);
		setVisible(true);
		getWorld()->incNumItem(Level::crystal, -1);
	}

	if (isVisible() && getWorld()->sharesSquareWithPlayer(this)) {
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->increaseScore(2000);
		// if (getWorld()->getPlayerBonus() > 0)
			getWorld()->incPlayerBonus(getWorld()->getPlayerBonus());
		getWorld()->setLevelFinished(true);
	}
}



ThiefbotFactory::ThiefbotFactory(double startX, double startY, bool mean, double size) 
	: m_isMean(mean), Actor(IID_ROBOT_FACTORY, startX, startY, none, size) {
	setPeaProof(true);
	setPeaStoppage(true);
	setHealth(1);
}

void ThiefbotFactory::doSomething() {
	int count = 0;
	if (getWorld()->doFactoryCensus(getX(), getY(), 3, count) &&
		count < 3) {
		if (randInt(1, 50) == 25) {
			getWorld()->spawnNewThiefBot(getX(), getY(), m_isMean);
			getWorld()->playSound(SOUND_ROBOT_BORN);
		}
	}
}



PickableItem::PickableItem(int imageID, double startX, double startY, int dir, double size)
	: m_pickable(true), Actor(imageID, startX, startY, dir, size) {
	makePassable(true);
	setHealth(1);
}

bool PickableItem::pickedUpByPlayer(Actor* a) {
	if (getWorld()->sharesSquareWithPlayer(a) && isPickable())
		return true;
	return false;
}

void PickableItem::pickUpItem(char item, int increase) {
	getWorld()->incNumItem(item, increase);
}

void PickableItem::playPickupSound() {
	getWorld()->playSound(SOUND_GOT_GOODIE);
}

bool PickableItem::isPickable() {
	return m_pickable;
}

void PickableItem::setPickable(bool pickable) {
	m_pickable = pickable;

	if (m_pickable) {
		setVisible(true);
	}
	else {
		setVisible(false);
	}
}



Crystal::Crystal(double startX, double startY, double size) 
	: PickableItem(IID_CRYSTAL, startX, startY, none, size) {
	
}

void Crystal::doSomething() {
	if (!isAlive())
		return;

	if (pickedUpByPlayer(this)) {
		increasePlayerScore(50);
		died();
		pickUpItem(Level::crystal, -1);
		playPickupSound();
	}
}



ExtraLife::ExtraLife(double startX, double startY, double size)
	: PickableItem(IID_EXTRA_LIFE, startX, startY, none, size) {
	
}

void ExtraLife::doSomething() {
	if (!isAlive())
		return;

	if (pickedUpByPlayer(this)) {
		increasePlayerScore(1000);
		died();
		playPickupSound();
		getWorld()->incLives();
	}
}



RestoreHealth::RestoreHealth(double startX, double startY, double size)
	: PickableItem(IID_RESTORE_HEALTH, startX, startY, none, size) {

}

void RestoreHealth::doSomething() {
	if (!isAlive())
		return;

	if (pickedUpByPlayer(this)) {
		increasePlayerScore(500);
		died();
		playPickupSound();
		getWorld()->setPlayerHealthToFull();
	}
}



AmmoGoodie::AmmoGoodie(double startX, double startY, double size)
	: PickableItem(IID_AMMO, startX, startY, none, size) {

}

void AmmoGoodie::doSomething() {
	if (!isAlive())
		return;

	if (pickedUpByPlayer(this)) {
		increasePlayerScore(100);
		died();
		playPickupSound();
		getWorld()->addPlayerAmmo(20);
	}
}



Character::Character(int imageID, double startX, double startY, int dir, double size)
	: m_nPeas(NULL), Actor(imageID, startX, startY, dir, size) {
}

void Character::setPeas(int peas) {
	m_nPeas = peas;
}

int Character::getPeas() const {
	return m_nPeas;
}

void Character::incPeas(int increase) {
	m_nPeas += increase;
}



Avatar::Avatar(double startX, double startY, double size) :
	m_bonus(1000), Character(IID_PLAYER, startX, startY, right, size) {
	setHealth(20);
	setPeas(20);
	setPeaProof(false);
}

int Avatar::getBonus() const  {
	return m_bonus;
}

void Avatar::incBonus(int inc) {
	m_bonus += inc;
}

void Avatar::reduceBonusByOne() {
	if (getBonus() > 0)
		incBonus(-1);
}

void Avatar::doSomething() {
	if (!isAlive()) {
		getWorld()->decLives();
		return;
	}

	int ch;
	if (getWorld()->getKey(ch)) {
		switch (ch) {
		case KEY_PRESS_ESCAPE:
			setHealth(0);
			break;
		case KEY_PRESS_SPACE:
			if (getPeas() > 0) {
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				incPeas(-1);
				switch (getDirection()) {
					case up:
						getWorld()->shootPea(getX(), getY() + 1, up);
						break;
					case down:
						getWorld()->shootPea(getX(), getY() - 1, down);
						break;
					case right:
						getWorld()->shootPea(getX() + 1, getY(), right);
						break;
					case left:
						getWorld()->shootPea(getX() - 1, getY(), left);
						break;
				}
			}
			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (getWorld()->noObstruct(getX() - 1, getY()) || getWorld()->hasPushableObject(getX() - 1, getY(), left)) {
				moveAngle(left);
			}
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (getWorld()->noObstruct(getX() + 1, getY()) || getWorld()->hasPushableObject(getX() + 1, getY(), right)) {
				moveAngle(right);
			}
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if (getWorld()->noObstruct(getX(), getY() + 1) || getWorld()->hasPushableObject(getX(), getY() + 1, up)) {
				moveAngle(up);
			}
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (getWorld()->noObstruct(getX(), getY() - 1) || getWorld()->hasPushableObject(getX(), getY() - 1, down)) {
				moveAngle(down);
			}
			break;
		}
	}


}

void Avatar::getAttacked() {
	incHealth(-peaHitPoints);

	if (isAlive()) {
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
		return;
	}
	else {
		getWorld()->playSound(SOUND_PLAYER_DIE);

	}
}



Marble::Marble(double startX, double startY, double size) :
	Character(IID_MARBLE, startX, startY, none, size) {
	setWorld(nullptr);
	setHealth(10);
	setPushable(true);
	setPeaProof(false);
}

void Marble::doSomething() {
	return;
}

void Marble::getAttacked() {
	incHealth(-peaHitPoints);
}

bool Marble::push(int dir) {
	switch (dir) {
		case up:
			if (getWorld()->allowsMarble(this, getX(), getY() + 1)) {
				moveAngle(up);
				return true;
			}
			break;
		case down:
			if (getWorld()->allowsMarble(this, getX(), getY() - 1)) {
				moveAngle(down);
				return true;
			}
			break;
		case right:
			if (getWorld()->allowsMarble(this, getX() + 1, getY())) {
				moveAngle(right);
				return true;
			}
			break;
		case left:
			if (getWorld()->allowsMarble(this, getX() - 1, getY())) {
				moveAngle(left);
				return true;
			}
			break;
	}
	return false;
}

bool Marble::isSwallowable() const {
	return true;
}



Robot::Robot(int imageID, double startX, double startY, GameWorld* world, int dir, double size)
	: m_ticks(NULL), m_currTick(0), Character(imageID, startX, startY, dir, size) {
	setPeaProof(false);
	m_ticks = (28 - world->getLevel()) / 4;
	if (m_ticks < 3)
		m_ticks = 3;
}



int Robot::getCurrentTick() {
	return m_currTick;
}

int Robot::getNumTicks() {
	return m_ticks;
}

void Robot::incCurrentTick(int increase) {
	m_currTick++;
}

bool Robot::runOnCurrentTick() {
	return (getCurrentTick() % getNumTicks()) == 0;
}

void Robot::getAttacked() {
	incHealth(-peaHitPoints);
	if (isAlive()) {
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	}
	else {
		died();
		getWorld()->playSound(SOUND_ROBOT_DIE);
		increasePlayerScore(100);
	}

	return;
}



Ragebot::Ragebot(double startX, double startY, GameWorld* world, int dir, double size)
	: Robot(IID_RAGEBOT, startX, startY, world, dir, size) {
	setHealth(10);
}

void Ragebot::doSomething() {
	if (!isAlive())
		return;

	if (runOnCurrentTick()) {
		int dx = 0, dy = 0;

		switch (getDirection()) {
		case up:
			dy = 1;
			break;
		case down:
			dy = -1;
			break;
		case right:
			dx = 1;
			break;
		case left:
			dx = -1;
			break;
		}

		if (getWorld()->existsClearShotToPlayer(getX(), getY(), dx, dy)) {
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			getWorld()->shootPea(getX() + dx, getY() + dy, getDirection());
			incCurrentTick();
			return;
		}

		if (!move(getX(), getY(), dx, dy))
			flipDirection();
	}

	incCurrentTick();
}

bool Ragebot::move(double x, double y, int dx, int dy) {
	
	if (getWorld()->noObstruct(x + dx, y + dy)) {
		moveForward();
		return true;
	}


	return false;
}

void Ragebot::flipDirection() {
	switch (getDirection()) {
		case up:
			setDirection(down);
			break;
		case down:
			setDirection(up);
			break;
		case right:
			setDirection(left);
			break;
		case left:
			setDirection(right);
			break;
	}
}



Thiefbot::Thiefbot(int imageID, double startX, double startY, GameWorld* world, int dir, double size)
	: m_hasGoodie(false), m_myGoodie(nullptr), m_currentDistance(0), m_distanceBeforeTurning(NULL), Robot(imageID, startX, startY, world, dir, size) {

	m_distanceBeforeTurning = randInt(1, 6);
}

bool Thiefbot::hasGoodie() {
	return m_hasGoodie;
}

void Thiefbot::setGoodie(bool has) {
	m_hasGoodie = has;
}

Actor* Thiefbot::getStealable() {
	return m_myGoodie;
}

void Thiefbot::setStealable(Actor* stealable) {
	m_myGoodie = stealable;
}

int Thiefbot::getCurrentDistance() {
	return m_currentDistance;
}

void Thiefbot::setCurrentDistance(int distance) {
	m_currentDistance = distance;
}

void Thiefbot::incCurrentDistance(int increase) {
	m_currentDistance += increase;
}

int Thiefbot::getDistanceBeforeTurning() {
	return m_distanceBeforeTurning;
}

void Thiefbot::setDistanceBeforeTurning(int distance) {

}

bool Thiefbot::isInFactoryCensus() {
	return true;
}



RegularThiefbot::RegularThiefbot(double startX, double startY, GameWorld* world, double size)
	: Thiefbot(IID_THIEFBOT, startX, startY, world, right, size) {
	setHealth(5);
}

void RegularThiefbot::doSomething() {
	if (!isAlive())
		return;

	if (runOnCurrentTick()) {
		int dx = 0, dy = 0;

		switch (getDirection()) {
		case up:
			dy = 1;
			break;
		case down:
			dy = -1;
			break;
		case right:
			dx = 1;
			break;
		case left:
			dx = -1;
			break;
		}

		if (!hasGoodie() && randInt(1, 10) == 1) {
			Actor* stealable = getWorld()->getColocatedStealable(getX(), getY());

			if (stealable != nullptr) {
				stealable->setPickable(false);
				setGoodie(true);
				setGoodie(stealable);
				getWorld()->playSound(SOUND_ROBOT_MUNCH);
				incCurrentTick();
				return;
			}
		}
		else if (getCurrentDistance() < getDistanceBeforeTurning()) {
			getWorld()->noObstruct(getX() + dx, getY() + dy);
			incCurrentDistance();
			incCurrentTick();
			return;
		}
		else {
			setCurrentDistance(0);
			setDistanceBeforeTurning(randInt(1, 6));
			vector<int> directions = { up, down, left, right };
			while (directions.size() > 0 && !(getWorld()->noObstruct(getX() + dx, getY() + dy)))
			{
				int posInVec = randInt(1, directions.size()) - 1;
				int d = directions[posInVec];
				switch (d) {
				case up:
					dy = 1;
					break;
				case down:
					dy = -1;
					break;
				case right:
					dx = 1;
					break;
				case left:
					dx = -1;
					break;
				}
				if (getWorld()->noObstruct(getX() + dx, getY() + dy)) {
					setDirection(d);
					moveForward();
					incCurrentTick();
					incCurrentDistance();
					return;
				}
				else {
					if (directions.size() == 1) {
						setDirection(d);
						incCurrentTick();
						return;
					}

					directions.erase(directions.begin() + posInVec - 1);
				}
			}
		}
	}
	incCurrentTick();
}

void RegularThiefbot::getAttacked() {
	incHealth(-peaHitPoints);
	if (isAlive()) {
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	}
	else {
		died();
		getStealable()->moveTo(getX(), getY());
		getStealable()->setPickable(true);
		getWorld()->playSound(SOUND_ROBOT_DIE);
		increasePlayerScore(10);
	}

	return;
}



MeanThiefbot::MeanThiefbot(double startX, double startY, GameWorld* world, double size)
	: Thiefbot(IID_MEAN_THIEFBOT, startX, startY, world, right, size) {
	setHealth(8);
}

void MeanThiefbot::doSomething() {
	if (!isAlive())
		return;

	/*if (runOnCurrentTick()) {
		int dx = 0, dy = 0;

		switch (getDirection()) {
		case up:
			dy = 1;
			break;
		case down:
			dy = -1;
			break;
		case right:
			dx = 1;
			break;
		case left:
			dx = -1;
			break;
		}

		if (this != nullptr &&
			getWorld()->existsClearShotToPlayer(getX(), getY(), dx, dy)) {
			getWorld()->playSound(SOUND_ENEMY_FIRE);
			getWorld()->shootPea(getX() + dx, getY() + dy, getDirection());
			incCurrentTick();
			return;
		}


		if (!hasGoodie() && randInt(1, 10) == 1) {
			Actor* stealable = getWorld()->getColocatedStealable(getX(), getY());

			if (stealable != nullptr) {
				stealable->setPickable(false);
				setGoodie(true);
				setGoodie(stealable);
				getWorld()->playSound(SOUND_ROBOT_MUNCH);
				incCurrentTick();
				return;
			}
		}
		else if (getCurrentDistance() < getDistanceBeforeTurning()) {
			getWorld()->noObstruct(getX() + dx, getY() + dy);
			incCurrentDistance();
			incCurrentTick();
			return;
		}
		else {
			setCurrentDistance(0);
			setDistanceBeforeTurning(randInt(1, 6));
			vector<int> directions = { up, down, left, right };
			while (directions.size() > 0 && !(getWorld()->noObstruct(getX() + dx, getY() + dy)))
			{
				int posInVec = randInt(1, directions.size()) - 1;
				int d = directions[posInVec];
				switch (d) {
				case up:
					dy = 1;
					break;
				case down:
					dy = -1;
					break;
				case right:
					dx = 1;
					break;
				case left:
					dx = -1;
					break;
				}
				if (getWorld()->noObstruct(getX() + dx, getY() + dy)) {
					setDirection(d);
					moveForward();
					incCurrentTick();
					incCurrentDistance();
					return;
				}
				else {
					if (directions.size() == 1) {
						setDirection(d);
						incCurrentTick();
						return;
					}

					directions.erase(directions.begin() + posInVec - 1);
				}
			}
		}
	}
	incCurrentTick();*/
}

void MeanThiefbot::getAttacked() {
	incHealth(-peaHitPoints);
	if (isAlive()) {
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
	}
	else {
		died();
		getStealable()->moveTo(getX(), getY());
		getStealable()->setPickable(true);
		getWorld()->playSound(SOUND_ROBOT_DIE);
		increasePlayerScore(20);
	}

	return;
}

