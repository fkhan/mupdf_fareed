/*
 * Utility object for handling a pdf application / view
 * Takes care of PDF loading and displaying and navigation,
 * uses a number of callbacks to the GUI app.
 */

#include <X11/Xlib.h>

typedef struct kno_highlightedtext_s kno_highlightedtext;
typedef struct kno_hitdata_s kno_hitdata;

struct kno_highlightedtext_s
{
	fz_textspan *text;
};

struct kno_hitdata_s
{
	int x, y;
	int ucs;
};

void kno_onselect(pdfapp_t *app);
void kno_clearselect(pdfapp_t *app);
void kno_applyselect(pdfapp_t *app);
int kno_addHighlight(pdfapp_t *app, int x0, int y0, int x1, int y1);
void kno_setHighlightColor(pdfapp_t *app, unsigned long color);
void kno_UpdateHighlight(pdfapp_t *app, int x0, int y0, int x1, int y1);
kno_highlightedtext *getHighlightedText(pdfapp_t *app);
void kno_allocselection(pdfapp_t *app);

kno_hitdata *kno_gethitdata(pdfapp_t *app, int x, int y);
int kno_ishighlightable(pdfapp_t *app, int x, int y, int *closestx, int *closesty);
//kno_highlightid *kno_gethighlightid(int x, int y);
//kno_highlightinfo *kno_gethighlightinfo(kno_highlightid id);

