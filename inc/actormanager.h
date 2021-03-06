/*
 * actormanager.h
 *
 *  Created on: 28/1/2016
 *      Author: Usuario
 */

#ifndef INC_ACTORMANAGER_H_
#define INC_ACTORMANAGER_H_


#include <algorithm>
#include <list>
using namespace std;

class Actor;
class Game;

class ActorManager
{
	public:
		ActorManager(Game *g);
		~ActorManager();
		void add(Actor *a);
		void del(Actor *a);
		void rewind();
		Actor *next();
		Actor *current();
		void update();
		int num_actors();

		list<Actor*>::iterator get_begin_iterator();
		list<Actor*>::iterator get_end_iterator();

	protected:
		Game *game;
		list<Actor*> actors;
		list<Actor*>::iterator actors_iter;

		list<Actor*> to_del;
		list<Actor*> to_create;
		void add_all_to_create();
		void del_all_to_del();

};




#endif /* INC_ACTORMANAGER_H_ */
