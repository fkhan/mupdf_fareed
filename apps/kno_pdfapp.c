#include <fitz.h>
#include <mupdf.h>
#include "pdfapp.h"
#include "kno_pdfapp.h"

int kno_addHighlight(pdfapp_t *app, int x0, int y0, int x1, int y1)
{
	app->selr.x0 = x0;
	app->selr.x1 = x1;
	app->selr.y0 = y0;
	app->selr.y1 = y1;
	kno_onselect(app);
	winrepaint(app);
	return 1;
}

void kno_setHighlightColor(pdfapp_t *app, unsigned long color)
{
	app->highlightedcolor = color;
}

void kno_UpdateHighlight(pdfapp_t *app, int x0, int y0, int x1, int y1)
{
	//bool abc = kno_addHighlight(app, x0, y0, x1, y1);
}

kno_highlightedtext *
kno_getHighlightedText(pdfapp_t *app)
{
kno_highlightedtext *result = fz_malloc(sizeof(kno_highlightedtext));
return result;
}


fz_textspan *pdfapp_gettextline(pdfapp_t *app, int linenum)
{
    /* lines numbered from 1 */
    fz_textspan *ln = app->text;
    int n = 1;
    while (ln && n!=linenum) {
        n++;
        ln = ln->next;
    }

    return ln;

}

void kno_clearselect(pdfapp_t *app)
{
    printf("clearSelect\n");
    app->firstline = app->lastline = 0;
    app->firstchar = app->firstchar = -1;
    app->sellen = 0;
}

void kno_applyselect(pdfapp_t *app)
{
    fz_textspan *firstline, *lastline, *ln;
    int i,n=0;

    if (!app->firstline)
        return;

    firstline = pdfapp_gettextline(app, app->firstline);
    if (firstline) {
        XRectangle box;
        box.x = firstline->text[app->firstchar].x;
        box.y = firstline->bbox.y0;
        box.height = firstline->bbox.y1 - firstline->bbox.y0 + 1;
        lastline = pdfapp_gettextline(app, app->lastline);
        if (lastline != firstline) {
            box.width = firstline->bbox.x1 - box.x + 1;

            app->selrects[n++] = box;

            ln = firstline->next;
            while (ln != lastline) {
                app->selrects[n].x = ln->bbox.x0;
                app->selrects[n].y = ln->bbox.y0;
                app->selrects[n].width = ln->bbox.x1 - ln->bbox.x0 + 1;
                app->selrects[n].height = ln->bbox.y1 - ln->bbox.y0 + 1;
                ln = ln->next;
                n++;
            }


            box.x = lastline->bbox.x0;
            box.width = lastline->text[app->lastchar].x - lastline->text[0].x +
                        lastline->text[app->lastchar].w;

            box.y = lastline->bbox.y0;
            box.height = lastline->bbox.y1 - lastline->bbox.y0 + 1;
            app->selrects[n++] = box;
        }
        else {
            box.width = lastline->text[app->lastchar].x - box.x +
                        lastline->text[app->lastchar].w;

            app->selrects[n++] = box;

        }

    }



    for (i=0; i<n; i++) {
        app->selrects[i].x += app->panx;
        app->selrects[i].y += app->pany;
    }

    app->sellen = n;
}


void kno_onselect(pdfapp_t *app)
{
    fz_textspan *ln;
    int x0 = app->image->x + app->selr.x0 - app->panx;
    int x1 = app->image->x + app->selr.x1 - app->panx;
    int y0 = app->image->y + app->selr.y0 - app->pany;
    int y1 = app->image->y + app->selr.y1 - app->pany;
    int n, i;

    int firstline, lastline, firstchar, lastchar;
    int linenum = 0;

    n=0;
    firstline = lastline = firstchar = lastchar = 0;
    for (ln = app->text; ln; ln = ln->next) {
        linenum++;
        if (ln->len > 0) {
            for (i = 0; i < ln->len; i++) {

                int x = ln->text[i].x;
                int y = ln->text[i].y;
                if (x >= x0 && x <= x1 && y >= y0 && y <= y1) {
                
                    if (!firstline) {
                        firstchar = i;
                        firstline = linenum;
                    }
                    lastchar = i;
                    lastline = linenum;

                }
            }
        }
    }

    app->firstline = firstline;
    app->lastline = lastline;
    app->firstchar = firstchar;
    app->lastchar = lastchar;

    kno_applyselect(app);
}

void kno_allocselection(pdfapp_t *app)
{
    /* JB: do something smarter */
    if (!app->selrects)
        app->selrects = malloc(sizeof(*app->selrects) * 16000);
}

int kno_ishighlightable(pdfapp_t *app, int x, int y, int *closestx, int *closesty)
{
    fz_textspan *ln;
    int lx0 = app->image->x + x - app->panx;
    int ly0 = app->image->y + y - app->pany;
    int i, difx, dify;
    difx = dify = 0;

    for (ln = app->text; ln; ln = ln->next) {
        if (ln->len > 0) {
            for (i = 0; i < ln->len; i++)
	    {
                int x0 = ln->text[i].x;
		int x1 = ln->text[i].x + ln->text[i].w;
                int y0 = ln->bbox.y0;//ln->text[i].y;
		int y1 = ln->bbox.y1;

                if (lx0 >= x0 && lx0 <= x1 && ly0 >= y0 && ly0 <= y1)
		{
			return 1; //Found
		}
		else
		{
			if (MAX(x0, lx0) - MIN(x0, lx0) < difx || difx==0)
			{
				difx = MAX(x0, lx0) - MIN(x0, lx0);
				*closestx = x0;
			}
			if (MAX(y0, ly0) - MIN(y0, ly0) < dify || dify == 0) 
			{
				dify = MAX(y0, ly0) - MIN(y0, ly0);
				*closesty = y0;
			}
		}
		//else if (x < lx0 || y < ly0) return 0; //not found 
		// it can't be acievable becuase lines are placed in list in reverse order
            }
        }
    }

    return 0; //not found
}

kno_hitdata *kno_gethitdata(pdfapp_t *app, int x, int y)
{
    fz_textspan *ln;
    kno_hitdata *hitdata;
    hitdata = fz_malloc(sizeof(kno_hitdata));
    hitdata->x = -1;
    hitdata->y = -1;
    hitdata->ucs = -1;

    int i;

    int lx = app->image->x + x - app->panx;
    int ly = app->image->y + y - app->pany;

    for (ln = app->text; ln; ln = ln->next) {
        if (ln->len > 0) {
            for (i = 0; i < ln->len; i++) {

                int x0 = ln->text[i].x;
		int x1 = ln->text[i].x + ln->text[i].w;
                int y0 = ln->text[i].y;
		int y1 = ln->bbox.y1;
                if (lx >= x0 && lx <= x1 && ly >= y0 && ly <= y1)
		{
			hitdata->x = x0;
			hitdata->y = y0;
			hitdata->ucs = ln->text[i].c;
			return hitdata; //found
		}
            }
        }
    }

    return hitdata; //not found
}


