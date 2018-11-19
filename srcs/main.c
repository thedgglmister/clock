#include "clock.h"


// all times begin stopped
// all lines get a start time and a stop time
// on loop:
// 		if current_time is past stop_time:
//
// 			stopped == true;
// 			move time forward from prev_time to stop_time
// 			get new stop_time, get new start_time //start time based on stop time not current_time;
//
// 		if stopped and current_time is past(>=) start_time:
// 			stopped == false;
// 			move forward from start_time to current_time;
//     else if current_time is past start time:
// 		 	move forward from prev_time to current_time;


int   main(int ac, char **av)
{
	t_params params;
	int daylight_savings;

	daylight_savings = 0;
	if (ac > 1 && strcmp(av[1], "daylight_savings") == 0)
	{
		daylight_savings = 1;
	}
\
	ft_get_params(&params, daylight_savings);
\
	ft_clock(&params);
	//mlx_hook(params.win, 2, 0, &ft_on_keypress, &params);
	//mlx_hook(params.win, 3, 0, &ft_on_keyrelease, &params);
	mlx_loop_hook(params.mlx, &ft_on_loop, &params);
	mlx_loop(params.mlx);

  ac++;
  av++;
	return (0);
}


void	ft_get_params(t_params *params, int daylight_savings)
{
  params->win_h = 12 * OUTER_RADIUS;
  params->win_w = 2 * params->win_h;

	params->mlx = mlx_init();
	params->win = mlx_new_window(\
			params->mlx, params->win_w, params->win_h, "IT'S CLOCK TIME");
	params->img = mlx_new_image(params->mlx, params->win_w, params->win_h);
\
	params->img_str = ft_get_img_str(params->img, 32, 4 * params->win_w, 1);
\
	params->events = (t_events) {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	clock_gettime(CLOCK_REALTIME, &(params->init_time));
	params->curr_time = params->init_time;
	params->lines = ft_get_lines(params);
	params->daylight_savings = daylight_savings;
}

t_line  *ft_get_lines(t_params *params)
{
  int i;
	t_timespec stop_time;
  t_line  *lines;

  i = -1;
  lines = (t_line*)malloc(sizeof(t_line) * LINE_COUNT);

	stop_time = ft_get_stop_time(params->curr_time);
  while (++i < LINE_COUNT)
  {
    lines[i].radius = (i % 2 ? BIG_RADIUS : LITTLE_RADIUS);
    lines[i].p1 = (t_point){ .x = OUTER_RADIUS + (2 * OUTER_RADIUS) * ((i % 24) / 2), .y = OUTER_RADIUS + (2 * OUTER_RADIUS) * (i / 24)};
    lines[i].p2 = (t_point){ .x = OUTER_RADIUS + (2 * OUTER_RADIUS) * ((i % 24) / 2), .y = (OUTER_RADIUS - lines[i].radius) + (2 * OUTER_RADIUS) * (i / 24)};
    lines[i].rads = 3 * M_PI_2;
		lines[i].rad_goal = 0;
    lines[i].rads_per_msec = (i % 2 ? 12 * TWO_PI / (60 * 1000) : TWO_PI / (60 * 1000));
		lines[i].stop_time = stop_time;
		lines[i].stopped = 1;
		lines[i].start_time = ft_get_start_time(params->init_time, (int)ft_get_elapsed_msecs(params->init_time, stop_time), lines[i].rads_per_msec, lines[i].rads, lines[i].rad_goal);

  }

  return lines;
}

char	*ft_get_img_str(void *img, int bpp, int ls, int end)
{
	return (mlx_get_data_addr(img, &bpp, &ls, &end));
}

void ft_clock(t_params *params)
{
	mlx_clear_window(params->mlx, params->win);
	memset(params->img_str, 0, 4 * params->win_w * params->win_h);
  ft_draw(params);
  mlx_put_image_to_window(params->mlx, params->win, params->img, 0, 0);
}


void	ft_draw(t_params *params)
{
  int i;

  i = -1;
  while (++i < LINE_COUNT)
  {
    ft_draw_line(params, params->lines[i]);
  }
}

void ft_draw_line(t_params *params, t_line line)
{
  int dy;
  int dx;

  dy = ABS((line.p2.y - line.p1.y));
  dx = ABS((line.p2.x - line.p1.x));
\
  if (dy > dx)
    ft_bresenham_vertical(params, line.p1, line.p2);
  else
    ft_bresenham_horizontal(params, line.p1, line.p2);
}


void	ft_bresenham_horizontal(t_params *params, t_point p1, t_point p2)
{
  int m_new;
  int slope_error_new;
  int subtract_amount;
  int direction;

  if (p2.x < p1.x) {
    ft_swap_points(&p1, &p2);
  }
  m_new = 2 * ABS((p2.y - p1.y));
  slope_error_new = m_new - (p2.x - p1.x);
  subtract_amount = 2 * (p2.x - p1.x);
  direction = (p2.y < p1.y ? -1 : 1);
\
	while (p1.x < p2.x + 1) {
    ft_draw_pix(params, p1, 255);
    slope_error_new += m_new;
    if (slope_error_new >= 0)
    {
       p1.y += direction;
       slope_error_new -= subtract_amount;
    }
    p1.x++;
  }
}

void	ft_bresenham_vertical(t_params *params, t_point p1, t_point p2)
{
  int m_new;
  int slope_error_new;
  int subtract_amount;
  int direction;

  if (p2.y < p1.y) {
    ft_swap_points(&p1, &p2);
  }
  m_new = 2 * ABS((p2.x - p1.x));
  slope_error_new = m_new - (p2.y - p1.y);
  subtract_amount = 2 * (p2.y - p1.y);
  direction = (p2.x < p1.x ? -1 : 1);
\
	while (p1.y < p2.y + 1) {
    ft_draw_pix(params, p1, 255);
    slope_error_new += m_new;
    if (slope_error_new >= 0)
    {
       p1.x += direction;
       slope_error_new -= subtract_amount;
    }
    p1.y++;
  }
}

void ft_swap_points(t_point *p1, t_point *p2) {
  t_point temp;

  temp = *p1;
  *p1 = *p2;
  *p2 = temp;
}

void	ft_draw_pix(t_params *params, t_point p1, int color)
{
	int	x;
	int	y;
	int	pix_i;

	x = p1.x;
	y = p1.y;
	pix_i = y * params->win_w + x;
\
	if ((y >= 0 && y < params->win_h) && (x >= 0 && x < params->win_w))
	{
		((int*)(params->img_str))[pix_i] = color;
	}
}


// all times begin stopped
// all lines get a start time and a stop time
// on loop:
// 		if current_time is past stop_time:
//
// 			stopped == true;
// 			move time forward from prev_time to stop_time
// 			get new stop_time, get new start_time //start time based on stop time not current_time;
//
// 		if stopped and current_time is past(>=) start_time:
// 			stopped == false;
// 			move forward from start_time to current_time;
//     else if current_time is past start time:
// 		 	move forward from prev_time to current_time;

int ft_time_less_than(t_timespec t1, t_timespec t2)
{
	if (t1.tv_sec < t2.tv_sec)
	{
		return 1;
	}
	else if (t1.tv_sec == t2.tv_sec && t1.tv_nsec < t2.tv_nsec)
	{
		return 1;
	}
	return 0;
}

int		ft_on_loop(void *param)
{
	t_params	*params;
	t_line *line;
	t_timespec next_stop_time;
	double next_rad_goal;
	int i;

	params = (t_params*)param;
	next_stop_time = (t_timespec){-1, -1};

	params->prev_time = params->curr_time;
	clock_gettime(CLOCK_REALTIME, &(params->curr_time));
	ft_get_clock_time(params);

	i = -1;
  while (++i < LINE_COUNT)
	{
		line = &(params->lines[i]);
		if (ft_time_less_than(line->stop_time, params->curr_time))
		{
			if (!line->stopped)
			{
				ft_move_line_forward(params->prev_time, line->stop_time, line);
			}
			line->stopped = 1;
			if (next_stop_time.tv_sec < 0)
			{
				next_stop_time = ft_get_stop_time(params->curr_time);
			}
			next_rad_goal = ft_get_rad_goal(i, next_stop_time);
			line->start_time = ft_get_start_time(line->stop_time, 60000, line->rads_per_msec, line->rads, next_rad_goal);
			line->stop_time = next_stop_time;
		}
		if (line->stopped && ft_time_less_than(line->start_time, params->curr_time))
		{
			line->stopped = 0;
			ft_move_line_forward(line->start_time, params->curr_time, line);
		}
		else if (ft_time_less_than(line->start_time, params->curr_time))
		{
			ft_move_line_forward(params->prev_time, params->curr_time, line);
		}
	}



  //NEED TO BRESENHAM THE ROTATIONS!! LINEAR EQUATION THAT GOES FROM 0 to 360 (or 359) in whatever seconds. depending on the speed variable.

	// clock_gettime(CLOCK_REALTIME, &(params->current_time));
	// printf("%ld %ld\n", (params->current_time).tv_sec, (params->current_time).tv_nsec);


  //ft_move_time_forward(params);
  ft_clock(params);
	return (0);
}

double ft_get_rad_goal(int i, t_timespec next_stop_time)
{
	double next_rad_goal;

	next_rad_goal = (next_stop_time.tv_sec % 13) * i * M_PI_4;
	while(next_rad_goal >= TWO_PI)
	{
		next_rad_goal -= TWO_PI;
	}
	return next_rad_goal;
}


void ft_move_line_forward(t_timespec from_time, t_timespec to_time, t_line* line)
{
	double elapsed_msecs;

	elapsed_msecs = ft_get_elapsed_msecs(from_time, to_time);
	line->rads += (elapsed_msecs * line->rads_per_msec);
	while (line->rads >= TWO_PI) {
		line->rads -= TWO_PI;
	}

	line->p2.x = line->p1.x + (cos(line->rads) * line->radius);
	line->p2.y = line->p1.y + (sin(line->rads) * line->radius);
}


// void ft_move_time_forward(t_params *params)
// {
//   int i;
// 	t_timespec elapsed_time;
// 	double elapsed_msecs;
//
// 	elapsed_time = ft_get_elapsed_time(params); //rename or split up because it also sets other time vairables in params
// 	elapsed_msecs = 1000 * elapsed_time.tv_sec;
// 	elapsed_msecs += (double) elapsed_time.tv_nsec / 1000000;
//
//   i = -1;
//   while (++i < LINE_COUNT)
//   {
// 		if (i == 143) printf("stop time %ld prevtime %ld\n", params->lines[i].stop_time.tv_sec, params->prev_time.tv_sec);
// 		if (params->lines[i].stop_time.tv_sec < params->prev_time.tv_sec)
// 		{
// 			continue;
// 		}
// 		else if (params->lines[i].stop_time.tv_sec == params->prev_time.tv_sec && params->lines[i].stop_time.tv_nsec < params->prev_time.tv_nsec)
// 		{
// 			continue;
// 		}
// 		else
// 		{
// 			params->lines[i].rads += (elapsed_msecs * params->lines[i].rads_per_msec);
// 			while (params->lines[i].rads >= TWO_PI) {
// 				params->lines[i].rads -= TWO_PI;
// 			}
//
// 	    params->lines[i].p2.x = params->lines[i].p1.x + (cos(params->lines[i].rads) * params->lines[i].radius);
// 	    params->lines[i].p2.y = params->lines[i].p1.y + (sin(params->lines[i].rads) * params->lines[i].radius);
// 		}
//   }
// }



// t_timespec ft_get_elapsed_time(t_params *params)
// {
// 	t_timespec current_time;
// 	t_timespec new_prev_time;
// 	t_timespec elapsed_time;
//
// 	clock_gettime(CLOCK_REALTIME, &current_time);
// 	new_prev_time = current_time;
// 	printf("current %ld %ld\n", (current_time).tv_sec, (current_time).tv_nsec);
// 	printf("prevvvv %ld %ld\n", (params->prev_time).tv_sec, (params->prev_time).tv_nsec);
// 	if (current_time.tv_nsec < (params->prev_time).tv_nsec)
// 	{
// 		current_time.tv_sec--;
// 		current_time.tv_nsec += 1000000000;
// 	}
// 	elapsed_time.tv_nsec = current_time.tv_nsec - (params->prev_time).tv_nsec;
// 	elapsed_time.tv_sec = current_time.tv_sec - (params->prev_time).tv_sec;
// 	params->prev_time = new_prev_time;
//
// 	printf("%ld %ld\n", (elapsed_time).tv_sec, (elapsed_time).tv_nsec);
// 	return elapsed_time;
// }

double ft_get_elapsed_msecs(t_timespec from_time, t_timespec to_time)
{
	t_timespec elapsed_time;
	double elapsed_msecs;

	if (to_time.tv_nsec < from_time.tv_nsec)
	{
		to_time.tv_sec--;
		to_time.tv_nsec += 1000000000;
	}
	elapsed_time.tv_nsec = to_time.tv_nsec - from_time.tv_nsec;
	elapsed_time.tv_sec = to_time.tv_sec - from_time.tv_sec;
	elapsed_msecs = 1000 * elapsed_time.tv_sec;
	elapsed_msecs += (double) elapsed_time.tv_nsec / 1000000;

	return elapsed_msecs;
}





t_timespec ft_get_stop_time(t_timespec curr_time)
{
	t_timespec stop_time;

	stop_time.tv_sec = curr_time.tv_sec - (curr_time.tv_sec % 60) + 60;
	stop_time.tv_nsec = 0;

	return stop_time;
}

t_timespec ft_get_start_time(t_timespec from_time, int msec_to_next_stop, double rads_per_msec, double curr_rads, double rad_goal)
{
	t_timespec start_time;
	int msec_delay;
	int msecs_for_full_loop;
	int msecs_to_get_to_goal;
	double rads_to_go;

	rads_to_go = rad_goal - curr_rads < 0 ? rad_goal - curr_rads + TWO_PI : rad_goal - curr_rads;
	msecs_to_get_to_goal = (int) (rads_to_go / rads_per_msec);
	msecs_for_full_loop = (int) (TWO_PI / rads_per_msec);

	msec_delay = (msec_to_next_stop - msecs_to_get_to_goal) % msecs_for_full_loop;

	//if its too slow, it might not make it to its goal in a minute
	if(msec_delay >= 0)
	{
		start_time.tv_sec = from_time.tv_sec + msec_delay / 1000;
		start_time.tv_nsec = from_time.tv_nsec + (msec_delay % 1000) * 1000000;
	}
	else
	{
		start_time.tv_sec = from_time.tv_sec + 60;
		start_time.tv_nsec = from_time.tv_nsec;
	}
	if (start_time.tv_nsec >= 1000000000)
	{
		start_time.tv_nsec -= 1000000000;
		start_time.tv_sec += 1;
	}
	return start_time;
}

int ft_get_clock_time(t_params *params)
{
	int curr_secs;
	int hours;
	int mins;

	curr_secs = (params->curr_time).tv_sec;
	hours = ((curr_secs / (60 * 60)) + 3 + params->daylight_savings) % 12 + 1;
	mins = (curr_secs / 60) % 60;

	printf("%d:%d\n", hours, mins);
	return hours * 100 + mins;
}
//
// void	ft_display(t_params *params)
// {
// 	t_point ***copy;
// 	int		i;
//
// 	mlx_clear_window(params->mlx, params->win);
// 	ft_bzero(params->img_str, 4 * params->w_s * params->w_s);
// 	if (params->hidden)
// 	{
// 		i = -1;
// 		while (++i < params->w_s * params->w_s)
// 		{
// 			if (params->z_buf[i])
// 			{
// 				free(params->z_buf[i]);
// 				params->z_buf[i] = 0;
// 			}
// 		}
// 	}
// 	copy = ft_copy_map(params->map, params->map_l, params->map_w);
// 	ft_transform_map(copy, params);
// 	if (params->hidden)
// 		ft_draw_h_map(params, copy);
// 	else
// 		ft_draw_t_map(params, copy);
// 	mlx_put_image_to_window(params->mlx, params->win, params->img, 0, 0);
// 	ft_delete_map(copy);
//}
