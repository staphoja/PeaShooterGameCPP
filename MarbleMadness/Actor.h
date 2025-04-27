#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//#include "StudentWorld.h"

class GameWorld;
class StudentWorld;


static const int peaHitPoints = 2;


class Actor : public GraphObject {
	public:
		Actor(int imageID, double startX, double startY, int dir = 0, double size = 1.0);

		/// <summary>
		/// Sets the health value of the actor
		/// </summary>
		/// <param name="health"></param>
		void setHealth(int health);

		/// <summary>
		/// Returns the health value of the actor
		/// </summary>
		/// <returns></returns>
		int getHealth() const;

		/// <summary>
		/// Increases the health of the actor by the inputted amount
		/// </summary>
		/// <param name="change"></param>
		void incHealth(int change);

		/// <summary>
		/// Returns a bool indicating if the actor is alive (i.e. health is above 0)
		/// </summary>
		/// <returns></returns>
		bool isAlive() const;

		/// <summary>
		/// When called, runs the process for an object to be seen as dead
		/// </summary>
		void died();

		/// <summary>
		/// Returns a bool that determines if an actor can be passed over by a walking entity
		/// </summary>
		/// <returns></returns>
		virtual void doSomething() = 0;

		/// <summary>
		/// Changes an actor's passable status
		/// </summary>
		/// <param name="pass"></param>
		bool isPassable() const;

		/// <summary>
		/// Determines if an object can be pushed (only ever a marble)
		/// </summary>
		/// <returns></returns>
		bool isPushable() const;

		/// <summary>
		/// If an actor can be pushed, this function will return true and the actor will be moved.
		/// If not, this function returns false.
		/// </summary>
		/// <param name="dir"></param>
		/// <returns></returns>
		virtual bool push(int dir);

		/// <summary>
		/// This function changes the pushable status of an actor (i.e. when a marble is created)
		/// </summary>
		/// <param name="pushable"></param>
		void setPushable(bool pushable);

		/// <summary>
		/// Sets the world belonging to the actor, typically following creation of the actor
		/// </summary>
		/// <param name="world"></param>
		void makePassable(bool pass);

		/// <summary>
		/// Allows the actor to access a pointer to the StudentWorld to call external functions
		/// </summary>
		/// <param name="world"></param>
		void setWorld(StudentWorld* world);

		/// <summary>
		/// Determines if a pea can damage the actor
		/// </summary>
		/// <returns></returns>
		StudentWorld* getWorld() const;

		/// <summary>
		/// Changes if a pea can damage the actor
		/// </summary>
		/// <param name="peaProof"></param>
		bool isPeaProof() const;

		/// <summary>
		/// Determines if a pea stops once it meets the actor or if the pea continues
		/// </summary>
		/// <returns></returns>
		void setPeaProof(bool peaProof);

		/// <summary>
		/// Changes whether a pea can be stopped by the actor
		/// </summary>
		/// <param name="stoppage"></param>
		bool stopsPea();

		/// <summary>
		/// Changes whether a pea can be accepted/swallowed by the actor
		/// </summary>
		/// <param name="accepts"></param>
		void setPeaStoppage(bool stoppage);

		/// <summary>
		/// Returns true if the actor can accept/swallow a marble
		/// </summary>
		/// <returns></returns>
		void setMarbleAcceptance(bool accepts);

		/// <summary>
		/// Returns true if the actor can be swallowed
		/// </summary>
		/// <returns></returns>
		bool acceptsMarble();

		/// <summary>
		/// This function calls a separate function in the StudentWorld to increase the Player's score
		/// </summary>
		/// <param name="increase"></param>
		virtual bool isSwallowable() const;

		/// <summary>
		/// Does damage to an actor's health
		/// </summary>
		void increasePlayerScore(int increase);

		/// <summary>
		/// Returns true if the actor can be picked up
		/// </summary>
		/// <returns></returns>
		virtual void getAttacked();

		/// <summary>
		/// Sets whether an actor can be picked up
		/// </summary>
		/// <param name="pickable"></param>
		virtual bool isPickable();

		/// <summary>
		/// Determines if the actor can be counted in a factory census of Thiefbots
		/// </summary>
		/// <returns></returns>
		virtual void setPickable(bool pickable);

		virtual bool isInFactoryCensus();

		Actor& operator=(const Actor& src) {
			m_world = src.m_world;
			m_health = src.m_health;
			m_isPassable = src.m_isPassable;
			m_isPushable = src.m_isPushable;
			m_isPeaProof = src.m_isPeaProof;
			m_stopsPea = src.m_stopsPea;
			m_acceptsMarble = src.m_acceptsMarble;

			return *this;
		}

	private:
		StudentWorld* m_world;
		int m_health;
		bool m_isPassable;
		bool m_isPushable;
		bool m_isPeaProof;
		bool m_stopsPea;
		bool m_acceptsMarble;
};

class Wall : public Actor {
public:
	Wall(double startX, double startY, double size = 1.0);

	/// <summary>
	/// Does nothing
	/// </summary>
	virtual void doSomething();

	Wall& operator=(const Wall& src) {
		Actor::operator=(src);

		return *this;
	}

private:

};

class Pea : public Actor {
	public:
		Pea(double startX, double startY, int dir, double size = 1.0);

		/// <summary>
		/// Determines if a pea damages another actor and dies or if it can move
		/// </summary>
		virtual void doSomething();

		Pea& operator=(const Pea& src) {
			Actor::operator=(src);

			m_dx = src.m_dx;
			m_dy = src.m_dy;

			return *this;
		}

	private:
		int m_dx;
		int m_dy;
};

class Pit : public Actor {
	public:
		Pit(double startX, double startY, double size = 1.0);

		/// <summary>
		/// Does nothing except die when a marble is swallowed
		/// </summary>
		virtual void doSomething();

		Pit& operator=(const Pit& src) {
			Actor::operator=(src);

			return *this;
		}

	private:
};

class Exit : public Actor {
	public:
		Exit(double startX, double startY, double size = 1.0);

		/// <summary>
		/// Determines if all crystals have been collected and reveals itself once they have.
		/// Once this happens, the player can continue to the next level.
		/// </summary>
		virtual void doSomething();

		Exit& operator=(const Exit& src) {
			Actor::operator=(src);

			return *this;
		}

	private:
};

class ThiefbotFactory : public Actor {
public:
	ThiefbotFactory(double startX, double startY, bool mean, double size = 1.0);

	/// <summary>
	/// The factory determines if its square is empty and has a 1/50 chance of spawning a new thiefbot
	/// if the count is not exceeded.
	/// </summary>
	void doSomething();

private:
	bool m_isMean;
};

class PickableItem : public Actor {
	public:
		PickableItem(int imageID, double startX, double startY, int dir, double size = 1.0);

		/// <summary>
		/// Determines whether a player is on top of a goodie, and if so the goodie is collected.
		/// </summary>
		/// <param name="a"></param>
		/// <returns></returns>
		bool pickedUpByPlayer(Actor* a);

		/// <summary>
		/// Increases a member value in StudentWorld representing an object that has been collected.
		/// </summary>
		/// <param name="item"></param>
		/// <param name="increase"></param>
		void pickUpItem(char item, int increase);

		/// <summary>
		/// Plays a sound when a goodie is picked up
		/// </summary>
		void playPickupSound();

		/// <summary>
		/// Determines if an actor is able to be picked up
		/// </summary>
		/// <returns></returns>
		virtual bool isPickable();

		/// <summary>
		/// Sets whether an actor can be picked up
		/// </summary>
		/// <param name="pickable"></param>
		virtual void setPickable(bool pickable);

		PickableItem& operator=(const PickableItem& src) {
			Actor::operator=(src);

			m_pickable = src.m_pickable;

			return *this;
		}

	private:
		bool m_pickable;
};

class Crystal : public PickableItem {
	public:
		Crystal(double startX, double startY, double size = 1.0);

		/// <summary>
		/// Determies if the crystal has been picked up, and communicates this to StudentWorld
		/// </summary>
		virtual void doSomething();

		Crystal& operator=(const Crystal& src) {
			PickableItem::operator=(src);

			return *this;
		}

	private:

};

class ExtraLife : public PickableItem {
	public:
		ExtraLife(double startX, double startY, double size = 1.0);

		/// <summary>
		/// Determies if the extra life has been picked up.
		/// Also adds a life to the player
		/// </summary>
		virtual void doSomething();

		ExtraLife& operator=(const ExtraLife& src) {
			PickableItem::operator=(src);

			return *this;
		}

	private:
};

class RestoreHealth : public PickableItem {
	public:
		RestoreHealth(double startX, double startY, double size = 1.0);

		/// <summary>
		/// Determines if restore health goodie has been picked up. If so,
		/// tells StudentWorld to set the player's health to full.
		/// </summary>
		virtual void doSomething();

		RestoreHealth& operator=(const RestoreHealth& src) {
			PickableItem::operator=(src);

			return *this;
		}

	private:

};

class AmmoGoodie : public PickableItem {
	public:
		AmmoGoodie(double startX, double startY, double size = 1.0);

		/// <summary>
		/// Determines if ammo goodie has been picked up. If so, calls function in StudentWorld that increases
		/// the player's ammo.
		/// </summary>
		virtual void doSomething();

		AmmoGoodie& operator=(const AmmoGoodie& src) {
			PickableItem::operator=(src);

			return *this;
		}

	private:
};

class Character : public Actor {
	public:
		Character(int imageID, double startX, double startY, int dir = 0, double size = 1.0);

		virtual void getAttacked() = 0;

		/// <summary>
		/// Sets the amount of peas a Character has
		/// </summary>
		/// <param name="peas"></param>
		void setPeas(int peas);

		/// <summary>
		/// Returns the integer amount of peas a Character has
		/// </summary>
		/// <returns></returns>
		int getPeas() const;

		/// <summary>
		/// Increases the amount of peas a Character has (either positive or negative)
		/// </summary>
		/// <param name="increase"></param>
		void incPeas(int increase);

		Character& operator=(const Character& src) {
			Actor::operator=(src);

			m_nPeas = src.m_nPeas;

			return *this;
		}

	private:
		int m_nPeas;
};

class Avatar : public Character {
	public:
		Avatar(double startX, double startY, double size = 1.0);

		/// <summary>
		/// Determines the key the player has pressed. Moves the player and shoots peas
		/// if the space bar has been pressed.
		/// Sets health to zero if escape is pressed.
		/// </summary>
		virtual void doSomething();
		
		/// <summary>
		/// Decreases the player's health if it has been struck by a pea
		/// </summary>
		virtual void getAttacked();
		
		/// <summary>
		/// Returns an int representing the bonus the player has
		/// </summary>
		int getBonus() const;
		
		/// <summary>
		/// Changes the value of the player's bonus
		/// </summary>
		void incBonus(int inc);

		/// <summary>
		/// Reduces the player's bonus by one, function is called every tick
		/// </summary>
		void reduceBonusByOne();

		Avatar& operator=(const Avatar& src) {
			Character::operator=(src);

			m_bonus = src.m_bonus;

			return *this;
		}

	private:
		int m_bonus;
};

class Marble : public Character {
	public:
		Marble(double startX, double startY, double size = 1.0);

		/// <summary>
		/// Does nothing
		/// </summary>
		void doSomething();

		/// <summary>
		/// Once hit by a pea, marble's health is decreased and dies one health is zero
		/// </summary>
		virtual void getAttacked();

		/// <summary>
		/// Determines if a marble can be pushed in a given direction. Moves marble
		/// and returns true if so.
		/// </summary>
		virtual bool push(int dir);

		/// <summary>
		/// Allows other actors to determine marbles are swallowable
		/// </summary>
		virtual bool isSwallowable() const;

		Marble& operator=(const Marble &src) {
			Character::operator=(src);

			return *this;
		}

	private:

};

class Robot : public Character {
	public:
		Robot(int imageID, double startX, double startY, GameWorld* world, int dir, double size = 1.0);

		/// <summary>
		/// Determies if robot can act during a given tick
		/// </summary>
		int getCurrentTick();

		/// <summary>
		/// Determies how many ticks a robot has to wait before acting
		/// </summary>
		int getNumTicks();

		/// <summary>
		/// Increases the current tick 
		/// </summary>
		void incCurrentTick(int increase = 1.0);

		/// <summary>
		/// Determies if a robot has waited enough ticks
		/// </summary>
		bool runOnCurrentTick();

		/// <summary>
		/// Decreases a robot's health when attaked by a pea
		/// </summary>
		virtual void getAttacked();

		Robot& operator=(const Robot& src) {
			Character::operator=(src);

			m_ticks = src.m_ticks;
			m_currTick = src.m_currTick;

			return *this;
		}

	private:
		int m_ticks;
		int m_currTick;
};

class Ragebot : public Robot {
	public:
		Ragebot(double startX, double startY, GameWorld* world, int dir, double size = 1.0);

		/// <summary>
		/// Moves if clear, switches directions if there is an obstruction. Shoots at
		/// player if they are in the Ragebot's path
		/// </summary>
		virtual void doSomething();

		/// <summary>
		/// Determines if it can move in the current direction
		/// </summary>
		virtual bool move(double x, double y, int dx, int dy);

		/// <summary>
		/// Flips the current direction when called to the opposite one
		/// </summary>
		void flipDirection();

		Ragebot& operator=(const Ragebot& src) {
			Robot::operator=(src);

			return *this;
		}

	private:
};

class Thiefbot : public Robot {
	public:
		Thiefbot(int imageID, double startX, double startY, GameWorld* world, int dir, double size = 1.0);

		Thiefbot& operator=(const Thiefbot& src) {
			Robot::operator=(src);

			m_hasGoodie = src.m_hasGoodie;
			m_myGoodie = src.m_myGoodie;

			return *this;
		}

		/// <summary>
		/// Returns true if a goodie has been picked up
		/// </summary>
		bool hasGoodie();

		/// <summary>
		/// Sets bool to true when goodie is picked up
		/// </summary>
		void setGoodie(bool has);

		/// <summary>
		/// Returns pointer to the goodie that has been picked up
		/// </summary>
		Actor* getStealable();

		/// <summary>
		/// Sets the data member stealable to the goodie that has been picked up
		/// </summary>
		void setStealable(Actor* stealable);

		/// <summary>
		/// Determines how far it has traveled in the current direction
		/// </summary>
		int getCurrentDistance();

		/// <summary>
		/// Sets the distance that can be traveled in the current direction
		/// </summary>
		void setCurrentDistance(int distance);

		/// <summary>
		/// Increases once the robot has moved
		/// </summary>
		void incCurrentDistance(int increase = 1);

		/// <summary>
		/// Determines how far the robot must go in a given direction
		/// </summary>
		int getDistanceBeforeTurning();

		/// <summary>
		/// Sets the distance a robot must go in a given direction
		/// </summary>
		void setDistanceBeforeTurning(int distance);

		/// <summary>
		/// Returns true so that thiefbots are counted in the census
		/// </summary>
		virtual bool isInFactoryCensus();

	private:
		bool m_hasGoodie;
		Actor* m_myGoodie;
		int m_distanceBeforeTurning;
		int m_currentDistance;
};

class RegularThiefbot : public Thiefbot {
	public:
		RegularThiefbot(double startX, double startY, GameWorld* world, double size = 1.0);

		/// <summary>
		/// Moves, changes directions, and steals goodies
		/// </summary>
		virtual void doSomething();

		/// <summary>
		/// Reduces the health of the thiefbot
		/// </summary>
		virtual void getAttacked();

	private:

};

class MeanThiefbot : public Thiefbot {
	public:
		MeanThiefbot(double startX, double startY, GameWorld* world, double size = 1.0);

		/// <summary>
		/// Moves, changes directions, shoots player when possible, and steals goodies
		/// </summary>
		virtual void doSomething();

		/// <summary>
		/// Reduces the health of the thiefbot
		/// </summary>
		virtual void getAttacked();

	private:

};




#endif // ACTOR_H_
