#include "player_stable.h"

void    p_plot(t_xy xy, float opacity, int color, t_system *system)
{
	t_rgb	rgb0;
	t_rgb	rgb1;
	t_rgb	rgb;
	int		*pix;

	opacity = powf(opacity, 1 / 2.2f);
	pix = ((int*) system->surface->pixels) + (int)xy.y * W2 + (int)xy.x;
	rgb0.r = (*pix >> 16) & 0xFF;
	rgb1.r = (color >> 16) & 0xFF;
	rgb0.g = (*pix >>  8) & 0xFF;
	rgb1.g = (color >>  8) & 0xFF;
	rgb0.b = (*pix >>  0) & 0xFF;
	rgb1.b = (color >>  0) & 0xFF;
	rgb.r = max(rgb0.r, opacity*rgb1.r);
	rgb.g = max(rgb0.g, opacity*rgb1.g);
	rgb.b = max(rgb0.b, opacity*rgb1.b);
	*pix = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
}

void	plot(int x,int y, float opacity, int color, t_system *system)
{
	t_rgb	rgb0;
	t_rgb	rgb1;
	t_rgb	rgb;
	int		*pix;

	opacity = powf(opacity, 1 / 2.2f);
	pix = ((int*) system->surface->pixels) + y * W2 + x;
	rgb0.r = (*pix >> 16) & 0xFF;
	rgb1.r = (color >> 16) & 0xFF;
	rgb0.g = (*pix >>  8) & 0xFF;
	rgb1.g = (color >>  8) & 0xFF;
	rgb0.b = (*pix >>  0) & 0xFF;
	rgb1.b = (color >>  0) & 0xFF;
	rgb.r = max(rgb0.r, opacity*rgb1.r);
	rgb.g = max(rgb0.g, opacity*rgb1.g);
	rgb.b = max(rgb0.b, opacity*rgb1.b);
	*pix = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
}

void    main_loop(int steep, float gradient, t_iline *l, t_system *system)
{
	int     x;
	float   intery;

	x = (int)(l->one.x + 0.5f) + 1;
	intery = l->zero.y + gradient *
			((int)(l->one.x + 0.5f) - l->zero.x) + gradient; // first y-intersection for the main loop
	while (x < (int)(l->one.x + 0.5f))
	{
		if (steep)
		{
			p_plot((t_xy){(int) (intery), x}, rfpart(intery), l->color, system);
			p_plot((t_xy){(int) (intery) + 1, x}, rfpart(intery), l->color, system);
		}
		else
		{
			p_plot((t_xy){x, (int) (intery)}, rfpart(intery), l->color, system);
			p_plot((t_xy){x, (int) (intery) + 1}, rfpart(intery), l->color, system);
		}
		++x;
		intery += gradient;
	}
}

void    handle_second_endpoint(int steep, float gradient, t_iline *l, t_system *system)
{
	t_xy    end;
	float   xgap;

	end.x = (int)(l->one.x + 0.5f);
	end.y = l->one.y + gradient * (end.x - l->one.x);
	xgap = fpart(l->one.x + 0.5);
	if (steep)
	{
		p_plot((t_xy){(int)(end.y), end.x}, rfpart(end.y) * xgap, l->color, system);
		p_plot((t_xy){(int)(end.y)+1, end.x}, fpart(end.y) * xgap, l->color, system);
	}
	else
	{
		p_plot((t_xy){end.x, (int)(end.y)}, fpart(end.y) * xgap, l->color, system);
		p_plot((t_xy){end.x, (int)(end.y)+1}, fpart(end.y) * xgap, l->color, system);
	}
	main_loop(steep, gradient, l, system); 	// main loop
}

void    handle_first_endpoint(int steep, float gradient, t_iline *l, t_system *system)
{
	t_xy    end;
	float   xgap;

	end.x = (int)(l->zero.x + 0.5f);
	end.y = l->zero.y + gradient * (end.x - l->zero.x);
	xgap = rfpart(l->zero.x + 0.5f);
	if (steep)
	{
		p_plot((t_xy){(int)(end.y), end.x}, rfpart(end.y) * xgap, l->color, system);
		p_plot((t_xy){(int)(end.y)+1, end.x}, fpart(end.y) * xgap, l->color, system);
	}
	else
	{
		p_plot((t_xy){end.x, (int)(end.y)}, fpart(end.y) * xgap, l->color, system);
		p_plot((t_xy){end.x, (int)(end.y)+1}, fpart(end.y) * xgap, l->color, system);
	}
	handle_second_endpoint(steep, gradient, l, system); 	// handle second endpoint
}

int     line_algorithm(t_xy *zero, t_xy *one)
{
	int steep;

	steep = abs(one->x - zero->x) < abs(one->y - zero->y);
	if (steep)
	{
		ft_swap(&zero->x, &zero->y);
		ft_swap(&one->x, &one->y);
	}
	if (zero->x > one->x)
	{
		ft_swap(&zero->x, &one->x);
		ft_swap(&zero->x, &one->y);
	}
	return (steep);
}

void	line_l(t_iline *l, t_system *system)
{
	int steep;
	float gradient;
	t_xy    zero;
	t_xy    one;
	t_xy    d;

	zero.x = l->zero.x;
	zero.y = l->zero.y;
	one.x = l->one.x;
	one.y = l->one.y;
	steep = line_algorithm(&zero, &one);
	d.x = one.x - zero.x;
	d.y = one.y - zero.y;
	gradient = d.y/d.x;
	handle_first_endpoint(steep, gradient, l, system); // handle first endpoint
}

void    line(float x0,float y0, float x1,float y1, int color, t_system *system)
{
//	float x0;
//	float y0;
//	float x1;
//	float y1;
//	int color;

	int steep;//, steep1;
	float dx;
	float dy;
	float gradient;

//    x0 = l->x0;
//    y0 = l->y0;
//	x1 = l->x1;
//	y1 = l->y1;
//	color = l->color;


	// Xiaolin Wu's antialiased line algorithm from Wikipedia.

//    steep1 = fabsf(y1-y0) > fabsf(x1-x0);
	steep = abs(y1 - y0) > abs(x1-x0);

//    printf("abs L = %f > abs R = %f; steep1 %d\nfabs L = %f > fabs R = %f ; steep %d\n\n",abs(y1-y0) , abs(x1-x0),steep1,  fabsf(y1-y0) , fabsf(x1-x0), steep);
//	if (steep != steep1)
//		SDL_Log("ALERT steep not %d == %d", steep, steep1);
	if(steep)
	{
		ft_swap(&x0, &y0);
		ft_swap(&x1, &y1);
	}
	if(x0 > x1)
	{
		ft_swap(&x0, &x1);
		ft_swap(&y0, &y1);
	}
	dx = x1-x0, dy = y1-y0, gradient = dy/dx;
	// handle first endpoint
	int xend = (int)(x0 + 0.5f);
	int yend = y0 + gradient * (xend - x0);
	float xgap = rfpart(x0 + 0.5f);
	int xpxl1 = xend; // this will be used in the main loop
	int ypxl1 = (int)(yend);
	if(steep)
	{
		plot(ypxl1,   xpxl1, rfpart(yend) * xgap, color, system);
		plot(ypxl1+1, xpxl1,  fpart(yend) * xgap, color, system);
	}
	else
	{
		plot(xpxl1, ypxl1  , rfpart(yend) * xgap, color, system);
		plot(xpxl1, ypxl1+1,  fpart(yend) * xgap, color, system);
	}
	float intery = yend + gradient; // first y-intersection for the main loop
	// handle second endpoint
	xend = (int)(x1 + 0.5f);
	yend = y1 + gradient * (xend - x1);
	xgap = fpart(x1 + 0.5);
	int xpxl2 = xend; //this will be used in the main loop
	int ypxl2 = (int)(yend);
	if(steep)
	{
		plot(ypxl2  , xpxl2, rfpart(yend) * xgap, color, system);
		plot(ypxl2+1, xpxl2,  fpart(yend) * xgap, color, system);
	}
	else
	{
		plot(xpxl2, ypxl2,  rfpart(yend) * xgap, color, system);
		plot(xpxl2, ypxl2+1, fpart(yend) * xgap, color, system);
	}
	// main loop
	for(int x = xpxl1 + 1; x < xpxl2; ++x, intery += gradient)
		if(steep)
		{
			plot((int)(intery)  , x, rfpart(intery), color, system);
			plot((int)(intery)+1, x,  fpart(intery), color, system);
		}
		else
		{
			plot(x, (int)(intery),  rfpart(intery), color, system);
			plot(x, (int)(intery)+1, fpart(intery), color, system);
		}
}


// BloomPostprocess adds some bloom to the 2D map image. It is merely a cosmetic device.
void BloomPostprocess(t_system *system)
{
	int blur_width;
	int blur_height;

	blur_width  = W/120;
	blur_height = H/90;
	float blur_kernel[blur_height*2+1][blur_width*2+1];

	int y;
	int x;

	y = -blur_height;
	while (++y <= blur_height)
	{
		x = -blur_width;
		while (++x <= blur_width)
		{
			float value;

			value = expf(-(x*x+y*y) / (2.f*(0.5f*max(blur_width,blur_height))));
			blur_kernel[y+blur_height][x+blur_width] = value * 0.3f;
			//printf("%10.3f", value);
		}
		//printf("\n");
	}
	t_pixel *img;

	img = (t_pixel*)ft_memalloc((sizeof(t_pixel) *W2*H));

	int pixels_original[W2*H];

	memcpy(pixels_original, system->surface->pixels, sizeof(pixels_original));

	int *pix;

	pix = ((int*) system->surface->pixels);

	t_xy    iter;
	t_pixel rgb;
	int original_pixel;
	float wanted_br;

	iter.y = 0;
	while (++iter.y < H)
	{
		iter.x = 0;
		while (++iter.x < W2)
		{
			original_pixel = pixels_original[y*W2+x];
			rgb.r = (original_pixel >> 16) & 0xFF;
			rgb.g = (original_pixel >> 8) & 0xFF;
			rgb.b = (original_pixel >>  0) & 0xFF;
			wanted_br = original_pixel == 0xFFFFFF ? 1 : original_pixel == 0x55FF55 ? 0.6 : original_pixel == 0xFFAA55 ? 1 : 0.1;
			rgb.brightness = powf((rgb.r*0.299f + rgb.g*0.587f + rgb.b*0.114f) / 255.f, 12.f / 2.2f);
			rgb.brightness = (rgb.brightness*0.2f + wanted_br * 0.3f + max(max(rgb.r,rgb.g),rgb.b)*0.5f/255.f);
			img[y*W2+x] = (t_pixel) { rgb.r,rgb.g,rgb.b,rgb.brightness };
		}
	}

#pragma omp parallel for schedule(static) collapse(2)
	for(unsigned y=0; y<H; ++y)
#ifdef SplitScreen
			for(unsigned x=W; x<W2; ++x)
#else
				for(unsigned x=0; x<W; ++x)
#endif

	{
		t_xy    pmin;
		t_xy    pmax;
		t_rgb   sum;
		t_xy    p;

		pmin.x = max(0, (int)x - blur_width);
		pmin.y  = max(0, (int)y - blur_height);
		pmax.y = min(H-1, (int)y + blur_height);
		pmax.x = min(W-1, (int)x + blur_width);
		sum.r = (int)img[(int)y*W2+(int)x].r;
		sum.g = (int)img[(int)y*W2+(int)x].g;
		sum.b = (int)img[(int)y*W2+(int)x].b;
		p.y = pmin.y;
		while (++p.y <= pmax.y)
		{
			p.x = pmin.x;
			while (++p.x <= pmax.x)
			{
				float r;
				float g;
				float b;
				float brightness;
				float value;

				r = img[(int)p.y*W2+(int)p.x].r;
				g = img[(int)p.y*W2+(int)p.x].g;
				b = img[(int)p.y*W2+(int)p.x].b;
				brightness = img[(int)p.y*W2+(int)p.x].brightness;
				value = brightness * blur_kernel[(int)p.y+blur_height-(int)y][(int)p.x+blur_width-(int)x];
				sum.r += (int)(r * value);
				sum.g += (int)(g * value);
				sum.b += (int)(b * value);
			}
		}
		int color;

		color = (((int)clamp(sum.r,0,255)) << 16)
		        + (((int)clamp(sum.g,0,255)) <<  8)
		        + (((int)clamp(sum.b,0,255)) <<  0);
		pix[y*W2+x] = color;
	}
}






