/*
 * Utility object for handling a pdf application / view
 * Takes care of PDF loading and displaying and navigation,
 * uses a number of callbacks to the GUI app.
 */
#include <fitz.h>
#include <mupdf.h>

enum { ARROW, HAND, WAIT };

struct pdfapp_s
{
	/* current document params */
	char *filename;
	char *doctitle;
	pdf_xref *xref;
	pdf_outline *outline;
	fz_renderer *rast;
	int pagecount;

	/* current view params */
	float zoom;
	int rotate;
	fz_pixmap *image;

	/* current page params */
	int pageno;
	pdf_page *page;

	/* snapback history */
	int hist[256];
	int histlen;

	/* window system sizes */
//	int winw, winh;
//	int scrw, scrh;
//	int shrinkwrap;

	/* event handling state */
	char number[256];
	int numberlen;

	int ispanning;
	int panx, pany;

	int iscopying;
	int selx, sely;
	fz_irect selr;

	/* client context storage */
	void *userdata;
};

typedef struct pdfapp_s pdfapp_t;
