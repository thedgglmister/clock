#ifndef CLOCK_H

# define CLOCK_H

# include "mlx.h"
# include <math.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <time.h>

# define ABS(x) (x < 0 ? -x : x)
# define TO_RAD(x) ((double) x * M_PI / 180)
# define TWO_PI (2 * M_PI)
# define OUTER_RADIUS 50
# define BIG_RADIUS 45
# define LITTLE_RADIUS 35
# define LINE_COUNT 144

typedef struct timespec t_timespec;

typedef struct	s_rgb
{
	int red;
	int green;
	int blue;
}				t_rgb;

typedef struct	s_point
{
	int x;
	int y;
	int c;
}				t_point;

typedef struct	s_line
{
	t_point p1;
	t_point p2;
	int 		radius;
	int			stopped;
	double	rads;
	double	rad_goal;
	double	rads_per_msec;
	t_timespec stop_time;
	t_timespec start_time;
}				t_line;

typedef struct	s_events
{
	int o_zoom;
	int i_zoom;
	int cc_xrot;
	int c_xrot;
	int c_zrot;
	int cc_zrot;
	int down;
	int up;
	int right;
	int left;
}				t_events;

typedef struct	s_params
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*img_str;
	int			win_w;
	int			win_h;
	int 		daylight_savings;
	t_line	*lines;
	t_events	events;
	t_timespec init_time;
	t_timespec prev_time;
	t_timespec curr_time;
}				t_params;











void ft_clock(t_params *params);
void ft_draw(t_params *params);
void ft_draw_line(t_params *params, t_line line);
void ft_bresenham_horizontal(t_params *params, t_point p1, t_point p2);
void ft_bresenham_vertical(t_params *params, t_point p1, t_point p2);
void ft_swap_points(t_point *p1, t_point *p2);
void ft_move_line_forward(t_timespec from_time, t_timespec to_time, t_line* line);
t_line  *ft_get_lines(t_params *params);
double ft_get_elapsed_msecs(t_timespec from_time, t_timespec to_time);
t_timespec ft_get_stop_time(t_timespec curr_time);
t_timespec ft_get_start_time(t_timespec from_time, int msec_to_next_stop, double rads_per_msec, double curr_rads, double rad_goal);
double ft_get_rad_goal(int i, t_timespec next_stop_time);
int ft_get_clock_time(t_params *params);





















/*
**main.c
*/
void			ft_get_params(t_params *params, int daylight_savings);
char			*ft_get_img_str(void *img, int bpp, int ls, int end);
void			ft_display(t_params *params);

/*
**handle_flags.c
*/
void			ft_handle_flags(t_params *params, char **av, int ac);
int				ft_usage(void);

/*
**event_handlers.c
*/
int				ft_on_keypress(int keycode, void *param);
int				ft_on_keyrelease(int keycode, void *param);
int				ft_on_loop(void *param);
void			ft_handle_zoom(t_params *params, int *refresh);
void			ft_exit(t_params *params);

/*
**create_map.c
*/
t_point			***ft_create_map(char *file_name);
//char			**ft_get_lines(int fd);
char			***ft_split_lines(char **lines);
t_point			***ft_lines_to_map(char ***split);

/*
**map_functions.c
*/
t_point			***ft_copy_map(t_point ***orig, int l, int w);
void			ft_delete_map(t_point ***map);
t_point			*ft_create_point(int x, int y, char *p_data);
t_point			*ft_pointdup(t_point *p);

/*
**transform_map.c
*/
void			ft_transform_map(t_point ***map, t_params *params);
void			ft_transform(\
					t_point ***map, void (*f)(double, t_point *), double val);

/*
**transformations.c
*/
void			ft_rotate_x(double rads, t_point *p);
void			ft_rotate_z(double rads, t_point *p);
void			ft_dilate(double scalar, t_point *p);
void			ft_translate_x(double x_shift, t_point *p);
void			ft_translate_y(double y_shift, t_point *p);

/*
**draw_map.c
*/
void			ft_draw_t_map(t_params *params, t_point ***map);
void			ft_draw_h_map(t_params *params, t_point ***map);
int				ft_in_win(\
					t_params *params, t_point *p1, t_point *p2, t_point *p3);

/*
**rasterize.c
*/
void			ft_rasterize_b(t_params *params, t_line *a, t_line *b);
void			ft_rasterize_c(\
					t_params *params, t_line *a, t_line *b, t_line *c);
void			ft_draw_triangle(\
					t_params *params, t_point p1, t_point p2, t_point p3);
void			ft_scanline(t_params *params, t_point p1, t_point p2);
void			ft_scan_middle(\
					t_params *params, t_line *a, t_line *b, t_line *c);

/*
**bresenham.c
*/
void			ft_bresenham_x(t_params *params, t_line *l);
void			ft_bresenham_y(t_params *params, t_line *l);
void			ft_pre_bres(t_params *params, t_line *l);
//void			ft_draw_line(t_params *params, t_point p1, t_point p2);
void			ft_draw_pix(t_params *params, t_point p1, int color);

/*
**get_line.c
*/
t_line			ft_get_line(t_point p1, t_point p2);
void			ft_order_points(t_point *p1, t_point *p2, t_point *p3);
void			ft_point_swap(t_point *p1, t_point *p2);
int				ft_get_octant(int dx, int dy, int absx);
void			ft_delete_gradients(t_line *a, t_line *b, t_line *c);

/*
**get_gradient.c
*/
int				*ft_get_gradient(int dx, int from, int to);
t_rgb			ft_get_rgb(int color);
int				ft_combine_rgb(int red, int green, int blue);

#endif
