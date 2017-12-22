#include <glib.h>
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

	GError *err = NULL;

	GRegex *regex = g_regex_new("(?P<id>[a-z]+) \\((?P<weight>\\d+)\\)( -> (?P<above>[a-z ,]+))?", 0, 0, &err);
	if (err) {
		g_printerr("regex error: %s\n", err->message);
		return 1;
	}

	GHashTable *carry_counts = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

	gsize length;
	gchar *line;
	GMatchInfo *info = NULL;
	while ((line = g_data_input_stream_read_line(stream, &length, NULL, &err)) != NULL) {
		if (!g_regex_match(regex, line, 0, &info))
			continue;

		gchar *id = g_match_info_fetch_named(info, "id");
		if (!g_hash_table_contains(carry_counts, id))
			g_hash_table_insert(carry_counts, g_strdup(id), GINT_TO_POINTER(0));

		gchar *above = g_match_info_fetch_named(info, "above");
		if (above) {
			gchar **above_list = g_strsplit(above, ", ", 0);
			for (int i = 0; above_list[i]; i++) {
				g_hash_table_insert(carry_counts, g_strdup(above_list[i]), GINT_TO_POINTER(1));
			}
			g_strfreev(above_list);
		}
	}

	if (err) {
		g_printerr("input error: %s\n", err->message);
		return 1;
	}

	GHashTableIter iter;
	gpointer key, value;
	g_hash_table_iter_init(&iter, carry_counts);
	while (g_hash_table_iter_next(&iter, &key, &value)) {
		if (!GPOINTER_TO_INT(value)) {
			g_print("%s\n", (gchar*)key);
			break;
		}
	}

	g_hash_table_unref(carry_counts);
}
