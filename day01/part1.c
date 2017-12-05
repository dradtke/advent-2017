#include <glib.h>
#include <glib/gstdio.h>

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

int main(void)
{
	gchar *input = get_input("input.txt");
	if (input == NULL) {
		return 1;
	}

	gint total = 0;
	gchar *ptr = input;
	while (*ptr != '\0') {
		gunichar c = g_utf8_get_char(ptr);
		g_assert(g_unichar_isdigit(c));

		ptr = g_utf8_next_char(ptr);
		gunichar next_c = g_utf8_get_char(*ptr == '\0' ? input : ptr);
		if (c == next_c) {
			gint value = (gint)(next_c - g_utf8_get_char("0"));
			total += value;
		}
	}
	g_print("%d\n", total);
}
