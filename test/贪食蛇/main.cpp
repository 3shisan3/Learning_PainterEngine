#include "PainterEngine.h"

int snake_x = 10, snake_y = 10, incx = 1, incy = 0;
int snake_len = 2;
int x, y;
char map[25 * 25];
int gameover = 0;
int elpased = 0;

PX_OBJECT_EVENT_FUNCTION(OnKeydown)
{
	px_byte key = PX_Object_Event_GetKeyDown(e);
	switch(key)
	{
	case PX_VK_LEFT: { incx = -1, incy = 0; } break;
	case PX_VK_RIGHT: { incx = 1, incy = 0; } break;
	case PX_VK_UP: { incx = 0, incy = -1; } break;
	case PX_VK_DOWN: { incx = 0, incy = 1; } break;
	default:break;
	}
}

PX_OBJECT_RENDER_FUNCTION(MyRenderFunction)
{
	elpased++;
	if (!gameover && elpased >= 20)
	{
		elpased = 0;
		snake_x = snake_x + incx;
		snake_y = snake_y + incy;
		if (map[snake_x + snake_y * 25] == 0)
		{
			map[snake_x + snake_y * 25] = snake_len;
			for (x = 0; x < 25 * 25; x++)
				if (map[x] > 0)
					map[x]--;
		}
		else if (map[snake_x + snake_y * 25] == -1)
		{
			int fx, fy;
			map[snake_x + snake_y * 25] = snake_len;
			snake_len++;
			while (1)
			{
				fx = PX_rand() % 25;
				fy = PX_rand() % 25;
				if (map[fx + fy * 25] == 0)
				{
					map[fx + fy * 25] = -1;
					break;
				}
			}
		}
		else
		{
			gameover = 1;
		}
	}
	//绘制地图
	PX_SurfaceClear(render_surface, 0, 0, 499, 499, PX_COLOR(255, 255, 255, 255));
	for (y = 0; y < 25; y++)
		for (x = 0; x < 25; x++)
		{
			px_char num[16];
			PX_itoa(map[x + y * 25], num, 16, 10);

			if (map[x + y * 25] > 0 || map[x + y * 25] == -2)
			{
				PX_GeoDrawRect(render_surface, x * 20, y * 20, x * 20 + 19, y * 20 + 19, PX_COLOR(255, 0, 0, 255));
			}
			else if (map[x + y * 25] == -1)//wall
			{
				PX_GeoDrawRect(render_surface, x * 20, y * 20, x * 20 + 19, y * 20 + 19, PX_COLOR(255, 255, 0, 0));
			}
			PX_FontModuleDrawText(render_surface, PX_NULL, x * 20 + 10, y * 20 + 10, PX_ALIGN_CENTER, num, PX_COLOR(255, 0, 128, 64));
		}
}




int main()
{
	PainterEngine_Initialize(500, 500);
	PX_memset(map, 0, sizeof(map));
	for (x = 0; x < 25; x++)map[x] = -2;
	for (x = 0; x < 25; x++)map[x * 25] = -2;
	for (x = 0; x < 25; x++)map[x * 25 + 24] = -2;
	for (x = 0; x < 25; x++)map[24 * 25 + x] = -2;
	map[366] = -1;

	PX_ObjectCreateFunction(mp, root, 0, MyRenderFunction, 0);
	PX_ObjectRegisterEvent(root, PX_OBJECT_EVENT_KEYDOWN, OnKeydown, PX_NULL);
	return 0;
}