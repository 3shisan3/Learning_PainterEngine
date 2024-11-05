#include "PainterEngine.h"

int main()
{
	PainterEngine_Initialize(800, 600);
	PX_Object_Firework01Create(mp, root,200,600);
	PX_Object_Firework01Create(mp, root,400,600);
	PX_Object_Firework01Create(mp, root,600,600);

	return 0;
}