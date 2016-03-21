/*
 * mask.cpp
 *
 *  Created on: 6/2/2016
 *      Author: Usuario
 */

#include <allegro.h>
#include "game.h"
#include "mask.h"

Mask::Mask()
{
	sp_mask=NULL;
	bb_height=0;
	bb_width=0;
	max_chunk=0;
	num_y=0;
}
Mask::~Mask()
{
	if (sp_mask!=NULL)
	{
		for (int j=0; j<num_y; j++)
		delete[] sp_mask[j];
		delete[] sp_mask;
	}
}

void Mask::create(ALLEGRO_BITMAP *bmp)
{
	// Creando la memoria para la mascara
	int height;
	int width;

	height = al_get_bitmap_height(bmp);
	width = al_get_bitmap_width(bmp);

	sp_mask = new unsigned long int *[height];
	for (int j=0; j<height; j++)
	sp_mask[j]=new unsigned long int[((width)>>5)+1];
	// creaci�n de la mascara de bits
	// Desde aqu� hacia abajo este m�todo funciona como "caja negra" para mi
	int x1, y1, z; /* Used to span through pixels within the sprite */
	ALLEGRO_COLOR p; /* Holds return value from getpixel() */

	//bloqueamos el bitmap por rendimiento, la operacion al_get_pixel()
	//puede ser muy lenta sin este bloqueo
	al_lock_bitmap(bmp,al_get_bitmap_format(bmp),ALLEGRO_LOCK_READONLY);

	for (y1=0; y1<(height); y1++) /* Now go through each pixel of the sprite
	*/
	{
		for(z=0; z<(int)((width)/32)+1; z++){
			sp_mask[y1][z]=0;
			for (x1=(z*32); x1<((z*32)+32); x1++)
			{
				//p=getpixel(bmp,x1,y1);

				p=al_get_pixel(bmp, x1, y1);
/*
 * Esta condicion es para no incluir en la mascara el color que se haya establecido como
 * transparente para esta imagen.
 * no tengo claro como hacerlo en allegro 5 por eso lo dejo comentado


				if ((p!=bitmap_mask_color(bmp)) && (p>=0))
				{
*/
					if (z>max_chunk) (max_chunk = z);
					if (y1>bb_height) bb_height=y1;
					if (x1>bb_width) bb_width=x1;
					sp_mask[y1][z]+=0x80000000 >> (x1 - ((z+1)*32));
/*
				}
*/
			}
		}
	}
	//desbloqueamos el bitmap
	al_unlock_bitmap(bmp);
	num_y=height;
}

int Mask::check_bbcollision(Mask *m1, Mask *m2, int x1, int y1, int x2, int
y2)
{
	if ((x1>x2+m2->get_bb_width()) || (x2 > x1+m1->get_bb_width()) ||
	(y1>y2+m2->get_bb_height()) || (y2> y1+m1->get_bb_height()))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int Mask::check_ppcollision(Mask *m1, Mask *m2, int x1, int y1, int x2, int y2)
{
	int dx1, dx2, dy1, dy2, ddy1, ddy2;
	int spr1_chunk, spr2_chunk;
	int dx1_chunk, dx2_chunk;
	if ((x1>x2+m2->get_bb_width()) || (x2 > x1+m1->get_bb_width()) ||
	(y1>y2+m2->get_bb_height()) || (y2> y1+m1->get_bb_height()))
	{
		return 0; /* No collision */
	}
	else /* if bounding box reports collision do pixel-perfect check*/
	{
		if (x1>x2) {
			dx1=0; //don't need to shift sprite 1
			dx2=x1-x2; //we need to shift sprite 2 left
		}
		else {
			dx1=x2-x1; //we need to shift sprite 1 left
			dx2=0; //don't need to shift sprite 2
		}
		if (y1>y2) {
			dy1=0; //we don't need to skip any rows on sprite 1
			dy2=y1-y2; //we need to skip this many rows on sprite 2
		}
		else {
			dy1=(y2-y1); //we need to skip this many rows on sprite 1
			dy2=0; // we don't need to skip any rows on sprite 2
		}
		spr1_chunk = dx1 / 32;
		spr2_chunk = dx2 / 32;
		dx1_chunk = dx1 - (32 * spr1_chunk);
		dx2_chunk = dx2 - (32 * spr2_chunk);
		while((spr1_chunk <= m1->get_max_chunk()) & (spr2_chunk <= m2->get_max_chunk())){
			ddy1 = dy1;
			ddy2 = dy2;
			while((ddy1<=m1->get_bb_height())&&(ddy2<=m2->get_bb_height())) {
				if ((m1->get_sp_mask(ddy1,spr1_chunk)<<dx1_chunk)&(m2->get_sp_mask(ddy2,spr2_chunk)<<dx2_chunk))
				return 1;
				ddy1++;
				ddy2++;
			}
			if((!dx1_chunk) && (!dx2_chunk)){ /*In case both sprites are lined up on
			x axis */
				spr1_chunk++;
				spr2_chunk++;
			}
			else{
				if(!dx1_chunk){ /*Set up the next 32 bit chunk in the mask for
				detection*/
					spr2_chunk++;
					dx1_chunk = 32 - dx2_chunk;
					dx2_chunk = 0;
				}
				else if(!dx2_chunk){
					spr1_chunk++;
					dx2_chunk = 32 - dx1_chunk;
					dx1_chunk = 0;
				}
			}
		}
		return 0; /* no collision */
	}
}
