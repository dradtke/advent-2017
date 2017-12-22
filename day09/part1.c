#include <glib.h>
#include <gio/gio.h>


GInputStream *get_input(const gchar *name)
{
	GError *err = NULL;
	GFile *file = g_file_new_for_path(name);
	GFileInputStream *stream = g_file_read(file, NULL, &err);
	if (err != NULL) {
		g_printerr("unable to read file: %s\n", err->message);
		return NULL;
	}
	return G_INPUT_STREAM(stream);
}

gint main(void)
{
	GInputStream *stream = get_input("input.txt");
	if (stream == NULL) {
		return 1;
	}

	gchar *buffer = g_malloc(2);
	buffer[1] = '\0';
	GError *error = NULL;

	gint score = 0, total_score = 0;
	gboolean skip_next = FALSE, is_junk = FALSE;
	while (g_input_stream_read(stream, buffer, 1, NULL, &error) && !error) {
		if (skip_next) {
			skip_next = FALSE;
			continue;
		}

		if (buffer[0] == '!') {
			skip_next = TRUE;
			continue;
		}

		if (is_junk) {
			if (buffer[0] == '>')
				is_junk = FALSE;

			continue;
		} else {
			if (buffer[0] == '<') {
				is_junk = TRUE;
				continue;
			}
		}

		if (buffer[0] == '{') {
			score++;
			total_score += score;
		} else if (buffer[0] == '}') {
			score--;
		}
	}

	if (error) {
		g_printerr("error: %s\n", error->message);
		return 1;
	}

	g_print("%d\n", total_score);
}
