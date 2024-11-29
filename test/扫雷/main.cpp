#include "PainterEngine.h"


#define GAME_MAP_SIZE 10 //扫雷地图大小
#define GAME_MINE_COUNT 10 //地雷数量

typedef struct
{
	px_int  x, y;//在按钮的坐标
	px_bool isOpen;//当前按钮是否被点击打开过
	px_bool isMine;//是不是地雷
}PX_Cell;


PX_Cell   game_cellmap[GAME_MAP_SIZE * GAME_MAP_SIZE];
PX_Object* ui_root = PX_NULL;
PX_Object* ui_cellbutton[GAME_MAP_SIZE * GAME_MAP_SIZE];
px_shape  shape_mine;
px_bool   game_isRunning;

//////////////////////////////////////////////////////////////////////////

//
PX_Cell* PX_CELL(px_int x, px_int y)
{
	if (x < 0 || x >= GAME_MAP_SIZE || y < 0 || y >= GAME_MAP_SIZE)
	{
		return PX_NULL;
	}
	return &game_cellmap[GAME_MAP_SIZE * y + x];
}

//扫描cell周围有多少个地雷
px_int PX_MineScan(px_int x, int y)
{
	px_int count = 0;
	PX_Cell* pCell;

	//上面三个
	pCell = PX_CELL(x - 1, y - 1);
	count += ((pCell && pCell->isMine) ? 1 : 0);
	pCell = PX_CELL(x, y - 1);
	count += ((pCell && pCell->isMine) ? 1 : 0);
	pCell = PX_CELL(x + 1, y - 1);
	count += ((pCell && pCell->isMine) ? 1 : 0);

	//中间两个
	pCell = PX_CELL(x - 1, y);
	count += ((pCell && pCell->isMine) ? 1 : 0);
	pCell = PX_CELL(x + 1, y);
	count += ((pCell && pCell->isMine) ? 1 : 0);

	//下面三个
	pCell = PX_CELL(x - 1, y + 1);
	count += ((pCell && pCell->isMine) ? 1 : 0);
	pCell = PX_CELL(x, y + 1);
	count += ((pCell && pCell->isMine) ? 1 : 0);
	pCell = PX_CELL(x + 1, y + 1);
	count += ((pCell && pCell->isMine) ? 1 : 0);

	return count;
}

//判断所有地雷是不是都已经清除
px_bool PX_GameIsClear()
{
	px_int i, j;
	for (j = 0; j < GAME_MAP_SIZE; j++)
	{
		for (i = 0; i < GAME_MAP_SIZE; i++)
		{
			//如果有一个格子不是地雷,且没有打开,那么游戏还需要继续
			if (!PX_CELL(i, j)->isMine && !PX_CELL(i, j)->isOpen)
			{
				return PX_FALSE;
			}
		}
	}
	return PX_TRUE;
}

//结束游戏
px_void PX_GameOver()
{
	px_int i, j;
	//显示所有地雷
	for (j = 0; j < GAME_MAP_SIZE; j++)
	{
		for (i = 0; i < GAME_MAP_SIZE; i++)
		{
			if (PX_CELL(i, j)->isMine)
			{
				PX_Object_PushButtonSetShape(ui_cellbutton[i + j * GAME_MAP_SIZE], &shape_mine);
				PX_Object_PushButtonSetTextColor(ui_cellbutton[i + j * GAME_MAP_SIZE], PX_COLOR(255, 255, 0, 0));
			}
			PX_ObjectSetEnabled(ui_cellbutton[i + j * GAME_MAP_SIZE], PX_FALSE);
		}
	}
	game_isRunning = PX_FALSE;
}

px_void PX_OnCellButtonExecute(PX_Object* pObject, PX_Object_Event e, px_void* ptr)
{
	PX_Cell* pcell = (PX_Cell*)ptr;

	//判断游戏是否运行
	if (!game_isRunning) return;

	//判断这个按钮是否已经打开
	if (pcell->isOpen) return;

	//是地雷游戏结束
	if (pcell->isMine)
	{
		PX_GameOver();
	}
	else
	{
		px_char content[2];
		//扫描周围地雷的数量
		px_int count = PX_MineScan(pcell->x, pcell->y);

		//将cell标记为选中
		pcell->isOpen = PX_TRUE;
		//将cell标记为灰色
		PX_ObjectSetEnabled(pObject, PX_FALSE);

		if (count == 0)
		{
			PX_Cell* pTargetCell;
			PX_Object* pTargetObject;
			//清除文本
			PX_Object_PushButtonSetBorderColor(pObject, PX_COLOR(32, 255, 255, 255));
			PX_Object_PushButtonSetText(pObject, "");

			//向周围的cell发送点击消息
			//上面三个
			pTargetCell = PX_CELL(pcell->x - 1, pcell->y - 1);
			if (pTargetCell)
			{
				pTargetObject = ui_cellbutton[pTargetCell->x + pTargetCell->y * GAME_MAP_SIZE];
				PX_ObjectExecuteEvent(pTargetObject, PX_OBJECT_BUILD_EVENT(PX_OBJECT_EVENT_EXECUTE));
			}
			pTargetCell = PX_CELL(pcell->x, pcell->y - 1);
			if (pTargetCell)
			{
				pTargetObject = ui_cellbutton[pTargetCell->x + pTargetCell->y * GAME_MAP_SIZE];
				PX_ObjectExecuteEvent(pTargetObject, PX_OBJECT_BUILD_EVENT(PX_OBJECT_EVENT_EXECUTE));
			}
			pTargetCell = PX_CELL(pcell->x + 1, pcell->y - 1);
			if (pTargetCell)
			{
				pTargetObject = ui_cellbutton[pTargetCell->x + pTargetCell->y * GAME_MAP_SIZE];
				PX_ObjectExecuteEvent(pTargetObject, PX_OBJECT_BUILD_EVENT(PX_OBJECT_EVENT_EXECUTE));
			}

			//中间两个
			pTargetCell = PX_CELL(pcell->x - 1, pcell->y);
			if (pTargetCell)
			{
				pTargetObject = ui_cellbutton[pTargetCell->x + pTargetCell->y * GAME_MAP_SIZE];
				PX_ObjectExecuteEvent(pTargetObject, PX_OBJECT_BUILD_EVENT(PX_OBJECT_EVENT_EXECUTE));
			}
			pTargetCell = PX_CELL(pcell->x + 1, pcell->y);
			if (pTargetCell)
			{
				pTargetObject = ui_cellbutton[pTargetCell->x + pTargetCell->y * GAME_MAP_SIZE];
				PX_ObjectExecuteEvent(pTargetObject, PX_OBJECT_BUILD_EVENT(PX_OBJECT_EVENT_EXECUTE));
			}

			//下面三个
			pTargetCell = PX_CELL(pcell->x - 1, pcell->y + 1);
			if (pTargetCell)
			{
				pTargetObject = ui_cellbutton[pTargetCell->x + pTargetCell->y * GAME_MAP_SIZE];
				PX_ObjectExecuteEvent(pTargetObject, PX_OBJECT_BUILD_EVENT(PX_OBJECT_EVENT_EXECUTE));
			}
			pTargetCell = PX_CELL(pcell->x, pcell->y + 1);
			if (pTargetCell)
			{
				pTargetObject = ui_cellbutton[pTargetCell->x + pTargetCell->y * GAME_MAP_SIZE];
				PX_ObjectExecuteEvent(pTargetObject, PX_OBJECT_BUILD_EVENT(PX_OBJECT_EVENT_EXECUTE));
			}
			pTargetCell = PX_CELL(pcell->x + 1, pcell->y + 1);
			if (pTargetCell)
			{
				pTargetObject = ui_cellbutton[pTargetCell->x + pTargetCell->y * GAME_MAP_SIZE];
				PX_ObjectExecuteEvent(pTargetObject, PX_OBJECT_BUILD_EVENT(PX_OBJECT_EVENT_EXECUTE));
			}

		}
		else
		{
			//设置cell文本
			PX_itoa(count, content, 2, 10);
			PX_Object_PushButtonSetBorderColor(pObject, PX_COLOR(96, 255, 255, 255));
			PX_Object_PushButtonSetText(pObject, content);
		}

		if (PX_GameIsClear())
		{
			PX_GameOver();
		}
	}

}



//////////////////////////////////////////////////////////////////////////
//初始化游戏
px_void PX_GameInitialize()
{
	px_int i, j;

	//初始化地图
	PX_memset(game_cellmap, 0, sizeof(game_cellmap));

	//删除之前的按钮
	if (ui_root)
	{
		PX_ObjectDelete(ui_root);
	}
	//创建根对象
	ui_root = PX_ObjectCreate(mp, root, 128, 128, 0, 0, 0, 0);

	//创建按钮
	for (j = 0; j < GAME_MAP_SIZE; j++)
	{
		for (i = 0; i < GAME_MAP_SIZE; i++)
		{
			ui_cellbutton[i + j * GAME_MAP_SIZE] = PX_Object_PushButtonCreate(mp, ui_root, i * 32, j * 32, 31, 31, "?", PX_NULL);
			PX_ObjectRegisterEvent(ui_cellbutton[i + j * GAME_MAP_SIZE], PX_OBJECT_EVENT_EXECUTE, PX_OnCellButtonExecute, &game_cellmap[i + j * GAME_MAP_SIZE]);
		}
	}

	//初始化地图数据
	for (j = 0; j < GAME_MAP_SIZE; j++)
	{
		for (i = 0; i < GAME_MAP_SIZE; i++)
		{
			game_cellmap[i + j * GAME_MAP_SIZE].x = i;
			game_cellmap[i + j * GAME_MAP_SIZE].y = j;
			game_cellmap[i + j * GAME_MAP_SIZE].isMine = PX_FALSE;
			game_cellmap[i + j * GAME_MAP_SIZE].isOpen = PX_FALSE;
		}
	}

	//挑选GAME_MINE_COUNT个幸运cell作为地雷
	i = 0;
	while (i < GAME_MINE_COUNT)
	{
		px_int index = (px_int)PX_randRange(0, GAME_MAP_SIZE * GAME_MAP_SIZE);
		if (!game_cellmap[index].isMine)
		{
			game_cellmap[index].isMine = PX_TRUE;
			i++;
		}
	}

	//开始游戏开关
	game_isRunning = PX_TRUE;
}

int main()
{
	PainterEngine_Initialize(600, 600);
	PX_LoadShapeFromFile(mp_static, &shape_mine, "assets/mine.png");
	PX_GameInitialize();
	return 0;
}