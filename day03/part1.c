#include <glib.h>
#include <glib/gstdio.h>
#include <gio/gio.h>
#include <math.h>

gchar *get_input(const gchar *name)
{
	gchar *input;
	GError *err = NULL;

	g_file_get_contents(name, &input, NULL, &err);
	g_assert((input == NULL && err != NULL) || (input != NULL && err == NULL));
	if (err != NULL) {
		g_assert(input == NULL);
		g_printerr("unable to read file: %s\n", err->message);
		return NULL;
	}
	g_strchomp(input);
	return input;
}

gint corner(gint k) {
	gint x = (2*k) + 1;
	return x*x;
}

gint main(void)
{
	gchar *input = g_strchomp(get_input("input.txt"));
	gint64 n = g_ascii_strtoll(input, NULL, 10);

	gint k = 0;
	while (corner(k) < n) {
		k++;
	}

	gint midpoint_1 = corner(k) - k;
	gint midpoint_2 = midpoint_1 - (2*k);
	gint midpoint_3 = midpoint_2 - (2*k);
	gint midpoint_4 = midpoint_3 - (2*k);

	gint delta = ABS(n - midpoint_1);
	delta = MIN(delta, ABS(n - midpoint_2));
	delta = MIN(delta, ABS(n - midpoint_3));
	delta = MIN(delta, ABS(n - midpoint_4));

	g_print("%d\n", k+delta);
}
