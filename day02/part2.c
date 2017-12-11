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
		GScanner *scanner = g_scanner_new(NULL);
		g_scanner_input_text(scanner, line, length);

		GList *numbers = NULL;

		while (g_scanner_get_next_token(scanner) != G_TOKEN_EOF) {
			if (scanner->token != G_TOKEN_INT) {
				continue;
			}

			gint v = (gint)scanner->value.v_int;
			numbers = g_list_prepend(numbers, GINT_TO_POINTER(v));
		}

		GList *a = numbers;
		while (a != NULL) {
			GList *b = numbers;
			while (b != NULL) {
				gint av = GPOINTER_TO_INT(a->data), bv = GPOINTER_TO_INT(b->data);
				gint high = MAX(av, bv), low = MIN(av, bv);;
				if (high > low && high%low == 0) {
					total += high/low;
					goto done;
				}
				b = b->next;
			}
			a = a->next;
		}
done:
		;
	}

	if (err != NULL) {
		g_printerr("unable to read from stream: %s\n", err->message);
		return 1;
	}

	g_print("%lu\n", total);

	return 0;
}
