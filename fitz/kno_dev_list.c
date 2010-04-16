#include "fitz.h"

//code change by kakai
int
kno_listnewid(fz_displaylist *list)
{
	if (list->last)
	{
		return list->last->id + 1;
	}
	else if (list->first)
	{
		return list->first->id + 1;
	}
	return 0;
}

void
kno_addhighlightednode(fz_displaynode *node)
{
	node->ishighlighted = 1;
}

void
kno_removehighlightednode(fz_displaynode *node)
{
	node->ishighlighted = 0;
}

void
kno_addlinknode(fz_displaynode *node)
{
	node->isunderline = 1;
}

void
kno_removelinknode(fz_displaynode *node)
{
	node->isunderline = 0;
}

void
kno_addbookmarknode(fz_displaynode *node)
{
	node->isbookmark = 1;
}

void
kno_removebookmarknode(fz_displaynode *node)
{
	node->isbookmark = 0;
}

//code change by kakai
