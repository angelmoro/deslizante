/*
 * tile.h
 *
 *  Created on: 23/3/2016
 *      Author: Usuario
 */

#ifndef INC_TILE_H_
#define INC_TILE_H_

#include <string>
#include <list>



class TileImage;
class TileAnimation;
class TileObjectGroup;
class TileProperty;


class Tile
{
public:
	Tile();
	~Tile();
private:

	int id; // The local tile ID within its tileset.
	std::string terrain; // Defines the terrain type of each corner of the tile, given as comma-separated indexes in the terrain types array in the order top-left, top-right, bottom-left, bottom-right. Leaving out a value means that corner has no terrain. (optional) (since 0.9)
	int probability; // A percentage indicating the probability that this tile is chosen when it competes with others while editing with the terrain tool. (optional) (since 0.9)
	std::list<TileProperty*> properties;
	std::list<TileProperty*>::iterator properties_iter;
	TileImage *image;
	TileAnimation * animation;
	TileObjectGroup * objectgroup;
};



#endif /* INC_TILE_H_ */
