#include <glib.h>
#include <glib/gstdio.h>
#include <gio/gio.h>

GDataInputStream *get_input(const gchar *name)
{
	GError *err = NULL;
	GFile *file = g_file_new_for_path(name);
	GFileInputStream *stream = g_file_read(file, NULL, &err);
	if (err != NULL) {
		g_printerr("unable to read file: %s\n", err->message);
		return NULL;
	}
	return g_data_input_stream_new(G_INPUT_STREAM(stream));
}

gint main(void)
{
	GDataInputStream *stream = get_input("input.txt");
	if (stream == NULL) {
		return 1;
	}

	gulong total = 0;
	GError *err = NULL;
	gsize length;
	gchar *line;
	while ((line = g_data_input_stream_read_line(stream, &length, NULL, &err)) != NULL) {
		GScannerConfig config = {};
		GScanner *scanner = g_scanner_new(&config);
		g_scanner_input_text(scanner, line, length);

		gulong smallest = 0, biggest = 0;

		while (g_scanner_get_next_token(scanner) != G_TOKEN_EOF) {
			if (scanner->token != G_TOKEN_INT) {
				continue;
			}

			gulong v = scanner->value.v_int;
			if (smallest == 0 || v < smallest) {
				smallest = v;
			}
			if (biggest == 0 || v > biggest) {
				biggest = v;
			}
		}

		total += (biggest - smallest);
	}

	if (err != NULL) {
		g_printerr("unable to read from stream: %s\n", err->message);
		return 1;
	}

	g_print("%lu\n", total);

	return 0;
}
