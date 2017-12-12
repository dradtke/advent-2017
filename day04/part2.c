#include <glib.h>
#include <glib/gstdio.h>
#include <gio/gio.h>
#include <string.h>

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

	GHashTable *words = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
	gint count = 0;

	GError *err = NULL;
	gsize length;
	gchar *line;
	while ((line = g_data_input_stream_read_line(stream, &length, NULL, &err)) != NULL) {
		GScanner *scanner = g_scanner_new(NULL);
		g_scanner_input_text(scanner, line, length);
		gint valid = 1;

		while (g_scanner_get_next_token(scanner) != G_TOKEN_EOF) {
			if (scanner->token != G_TOKEN_IDENTIFIER) {
				g_printerr("found a non-identifier token: %d\n", scanner->token);
				return 1;
			}


			gchar *word = g_strdup(scanner->value.v_identifier);

			gint word_len = (gint)strlen(word);
			for (gint i = 0; i < word_len; i++) {
				for (gint j = 0; j < word_len; j++) {
					if (word[i] < word[j]) {
						gchar temp = word[j];
						word[j] = word[i];
						word[i] = temp;
					}
				}
			}

			if (g_hash_table_lookup(words, word) != NULL) {
				valid = 0;
			}
			g_hash_table_insert(words, word, GINT_TO_POINTER(1));
		}

		if (valid) {
			count++;
		}

		g_hash_table_remove_all(words);
	}

	if (err != NULL) {
		g_printerr("unable to read from stream: %s\n", err->message);
		return 1;
	}

	g_print("%d\n", count);
}
