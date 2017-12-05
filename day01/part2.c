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

gunichar get_halfway_char(gchar *input, gint length, gchar *start)
{
	gchar *ptr = start;
	for (int i = 0; i < length/2; i++) {
		ptr = g_utf8_next_char(ptr);
		if (*ptr == '\0') {
			ptr = input;
		}
	}
	return g_utf8_get_char(ptr);
}

int main(void)
{
	gchar *input = get_input("input.txt");
	if (input == NULL) {
		return 1;
	}
	gint input_length = g_utf8_strlen(input, -1);

	gint total = 0;
	gchar *current = input;
	while (*current != '\0') {
		gunichar c = g_utf8_get_char(current);
		g_assert(g_unichar_isdigit(c));

		gunichar next_c = get_halfway_char(input, input_length, current);
		if (c == next_c) {
			gint value = (gint)(next_c - g_utf8_get_char("0"));
			total += value;
		}

		current = g_utf8_next_char(current);
	}
	g_print("%d\n", total);
}
