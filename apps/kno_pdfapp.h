/*
 * Utility object for handling a pdf application / view
 * Takes care of PDF loading and displaying and navigation,
 * uses a number of callbacks to the GUI app.
 */

#include <X11/Xlib.h>

typedef struct kno_highlightedtext_s kno_highlightedtext;

struct kno_highlightedtext_s
{
	fz_textspan *text;
};

void kno_onselect(pdfapp_t *app);
void kno_clearselect(pdfapp_t *app);
void kno_applyselect(pdfapp_t *app);
void kno_addHighlight(pdfapp_t *app, int x0, int y0, int x1, int y1);
void kno_changeHighlightColor(pdfapp_t *app, unsigned long color);
void kno_UpdateHighlight(pdfapp_t *app, int x0, int y0, int x1, int y1);
kno_highlightedtext *getHighlightedText(pdfapp_t *app);
void kno_allocselection(pdfapp_t *app);
