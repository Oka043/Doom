#ifndef DOOM_NUKEM_WINDOW_H
# define DOOM_NUKEM_WINDOW_H

/* Define window size */
#define W2 1280 // width of screen
#define H 800  // height of screen
#define W 1280  // width of _game_ screen (in SplitScreen mode, map is drawn in the remaining space)


typedef struct	s_window
{
	int 		w;
	int 		h;
}				t_window;

#endif
