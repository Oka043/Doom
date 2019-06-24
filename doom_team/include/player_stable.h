#ifndef DOOM_NUKEM_PLAYER_H
#define DOOM_NUKEM_PLAYER_H

#include <math.h>
#include <SDL2/SDL_image.h>
#include <SDL.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "get_next_line.h"
#include "player_struct.h"
#include "defines.h"
#include "window.h"

void        UnloadData(unsigned *NumSectors, t_sector *sectors);
//void        LoadData_stable(t_player *player, t_system *system);
int         Scaler_Next(t_scaler* i);
void        plot(int x,int y, float opacity, int color, t_system *system);
void        BloomPostprocess(t_system *system);
void        fillpolygon(t_sector* sect, int color, t_system *system);
int         vert_compare(const t_xy* a, const t_xy* b);
void        vline(int x, int y1,int y2, int top,int middle,int bottom, t_system *system);
void        MovePlayer(float dx, float dy, t_player *player, t_sector *sectors);
void        save_frame2(t_system *system);
void        save_frame1(t_system *system);
void        verify_map(t_system *system, t_sector *sectors);
void    	LoadData_stable(t_player *player, t_system *system);
int         IntersectLineSegments(float x0,float y0, float x1,float y1,
                          float x2,float y2, float x3,float y3);


void		DrawScreen(t_player *player, t_system *system);

//void		acquire_coords(t_system *system);
void		draw_screen_init(t_system *system, int *renderedsectors, t_player *player);
void		transform_vertices(t_system *system, t_player *player);
void		view_frustrum_next(t_system *system);
void		view_frustrum(t_system *system);
void		scales(t_system *system);
void		transform_floor_ceiling(t_system *system, t_player *player);
void		check_the_edge_type(t_system *system, t_player *player);
void		acquire_coords(t_system *system);
//void		clamp_ya_yb(t_system *system);
//void		render_upper_wall(t_system *system);
//void		another_sector(t_system *system);
//void		render_wall(t_system* system);
//void		schedule_sector(t_system *system);
//void		render_each_wall(t_system *system, t_player *player);

//void        DrawMap(t_player *player, t_system *system, t_sector *sectors); // WTF? не нужно, но когда-то работало...но не сейчас
int         done(t_system *system);
void		ft_swap_d(double *x, double *y);
void		ft_swap_f(float *x, float *y);
void	    line_l(t_iline *l, t_system *system);
void        line(float x0,float y0, float x1,float y1, int color, t_system *system);

int			key_event(SDL_Event *ev, t_player *player, t_system *system);
void		init_player(t_player *player);
void 		mouse_event(t_player *player, t_system *system, float *yaw);
void		move(t_player *player);
void		mvs(t_player *player, t_system *system, float const *eyeheight, int s);
void		mvs1(int s, t_sector *sector, float const *eyeh, t_system *system);
void		init_window(t_system *system);
void		falling(t_player *player, t_system *system, float const eyeheight);
int			main_cycle(t_player *player, t_system *system);
void		pos_m(t_system *system, t_player *player, float *eyeheight);

int			LoadTexture(t_system *system);

//function used to sort the sprites
void		combSort(int* order, double* dist, int amount);

void		sprite_casting(t_system *system);
//void 		loadImage(t_sprite *texture, int w, int h, char *str);

void	load_textures(t_system *system, char *string, Texture *texture);
int 		load_items(t_system *system, char *string);

// read file
t_system 		*init_system(void);
void    		sdl_init(t_system *system);

/*
work_with_file
*/
//void transform_ver_arr(t_system *system);
void 			read_file(t_system *system);
void 			print_error(const char *msg);
void			tmp_list(t_list **lst, const void *content,
						 const size_t content_size);
void        	free_tmp_list(t_list **head); // если будет отдельный файл free можно перенсти туда
//void			init_all(t_system *system);

/*
parse_vertex
*/
void			parse_vertex(t_system *system, t_list *list);
t_list			*find_elem(t_list *list, char *elem);
size_t 			content_len(t_list *list, char *content);
t_xy  			*allocate_vertex(size_t size); 					//t_vertex
t_xy			*pars_ver_par(char *data, t_system *system, size_t *len);

/*
validate_vertex
*/
t_bool			validate_vertex(char *vertex_data);
t_bool			validate_dilims(char *vertex_data);
t_bool			validate_coords(char *vertex_data);
t_bool			validate_num(char *vertex_data);
double			*allocate_vertex_x_array(size_t size);

/*
parse_vertex2
*/
float			parse_ver_y(char *vertex_line);
double			*parse_ver_x(char *vertex_line, size_t *size_arr);
char			*find_x_values(char *vertex_line);
size_t			two_dim_len(char **array);
void			two_dim_del(char ***array);

/*
parse_sector
*/
void			parse_sec(t_system *system, t_list *list);
t_sector		*allocate_sec(size_t size);
t_sector		processing(t_system *system, size_t index, char *sec_data);
t_bool			int_error(char **data);
//double			**allocate_vertex_array(size_t size);

/*
parse_sector2
*/
t_sector		parse_height_params(char *data);
t_xy			*parse_vertex_pair(t_system *system, char **data, size_t size);
t_xy			*search_vertex_pair(t_system *system, int32_t pair);
double			*allocate_vertex_pair();

/*
parse_player
*/
void			parse_player(t_system *system, t_list *list);
t_player		parse_player_parameters(char *data, t_system *system);
t_xyz 			define_position(char *data);

int32_t			*parse_neighbours(char *data, t_sector *sector);
//t_sector 			*parse_extra_pair(char **data, char **data1, t_sector *sector); //char **data1
//t_sector 			*allocate_transit_arr(size_t size);


#endif

