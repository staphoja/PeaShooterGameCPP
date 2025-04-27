#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Avatar;

class StudentWorld : public GameWorld
{
	public:
		static const int none = -1;
		static const int right = 0;
		static const int left = 180;
		static const int up = 90;
		static const int down = 270;


		StudentWorld(std::string assetPath);

		/// <summary>
		/// Creates objects based on the level's text file
		/// </summary>
		/// <returns></returns>
		virtual int init();

		/// <summary>
		/// Calls each actor's doSomething function
		/// </summary>
		/// <returns></returns>
		virtual int move();

		/// <summary>
		/// Removes all dynamically allocated objects 
		/// </summary>
		virtual void cleanUp();

		/// <summary>
		/// Determines if a walking actor can move to a specific spot
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <returns></returns>
		bool noObstruct(double x, double y);

		/// <summary>
		/// Determines if a pea can move past a certain spot
		/// </summary>
		bool noPeaStoppage(double x, double y);

		/// <summary>
		/// Determies if an object stops a marble, and destroys the marble if not
		/// </summary>
		bool allowsMarble(Actor* a, double x, double y);

		/// <summary>
		/// Swallows all marbles that are located above pits
		/// </summary>
		bool swallowSwallowable(Actor* a);

		/// <summary>
		/// Determies if a specific location has an object that can be pushed (marble)
		/// </summary>
		bool hasPushableObject(double x, double y, int dir);

		/// <summary>
		/// Determines if a pea can damage an object in a specific spot
		/// </summary>
		bool damageSomething(Actor* a, int damageAmt);

		/// <summary>
		/// Determines if a pea can be shot in a certain path to the player
		/// </summary>
		bool existsPeaPath(double x, double y, int dx, int dy);

		/// <summary>
		/// Creates a pea next to an actor that can shoot them
		/// </summary>
		void shootPea(double startX, double startY, int dir);

		/// <summary>
		/// Determines if an Actor is on the same square as a player
		/// </summary>
		bool sharesSquareWithPlayer(Actor* a);

		/// <summary>
		/// Alternate function to determine if the plater is on a square
		/// </summary>
		bool sharesSquareWithPlayer(double x, double y);

		/// <summary>
		/// Returns the player's bonus
		/// </summary>
		int getPlayerBonus();

		/// <summary>
		/// Increases the player's bonus
		/// </summary>
		void incPlayerBonus(int increase);

		/// <summary>
		/// Increases member variables based on object collected
		/// </summary>
		void incNumItem(char item, int increase);

		/// <summary>
		/// Determines if any crystals are left on the map
		/// </summary>
		bool anyCrystals();

		/// <summary>
		/// Determines if no Crystals are on the map
		/// </summary>
		bool noCrystals();

		/// <summary>
		/// Sets the level to completed
		/// </summary>
		void setLevelFinished(bool finished);

		/// <summary>
		/// Regenerates player's health
		/// </summary>
		void setPlayerHealthToFull();

		/// <summary>
		/// Adds ammo to the player
		/// </summary>
		void addPlayerAmmo(int ammo);

		/// <summary>
		/// Determines if a robot has a clear shot to the player
		/// </summary>
		bool existsClearShotToPlayer(double x, double y, int dx, int dy);

		/// <summary>
		/// Returns a pointer to a goodie on a specific square
		/// </summary>
		Actor* getColocatedStealable(double x, double y) const;

		/// <summary>
		/// Does a census and changes a reference to a count for how many
		/// thiefbots are in a given radius
		/// </summary>
		bool doFactoryCensus(double x, double y, int distance, int& count);

		/// <summary>
		/// Creates a new thiefbot on a specific point 
		/// </summary>
		void spawnNewThiefBot(double x, double y, bool mean);
		virtual ~StudentWorld();

	private:
		static const int m_maxActor = VIEW_HEIGHT * VIEW_WIDTH;
		int m_nActors;
		Actor* m_actors[m_maxActor];
		int m_nPlayers;
		Avatar* m_player;
		int m_nCrystals;
		bool m_finishedLevel;
};

#endif // STUDENTWORLD_H_
