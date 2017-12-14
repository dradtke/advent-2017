#include <glib.h>
#include <glib/gstdio.h>
#include <fcntl.h>


gchar *key(GList *list, gint size)
{
	gchar **elems = g_malloc((size+1) * sizeof(gchar*));
	elems[size] = NULL;
	for (int i = 0; i < size; i++) {
		gchar str[256];
		g_snprintf(str, 256, "%d", GPOINTER_TO_INT(list->data));
		elems[i] = g_strdup(str);
		list = list->next;
	}
	gchar *result = g_strjoinv(",", elems);
	g_strfreev(elems);
	return result;
}


GList *get_biggest(GList *list)
{
	gint value = GPOINTER_TO_INT(list->data);
	GList *biggest = list;

	while ((list = list->next)) {
		gint v = GPOINTER_TO_INT(list->data);
		if (v > value) {
			value = v;
			biggest = list;
		}
	}

	return biggest;
}


GList *next_or_wrap(GList *current, GList *start)
{
	GList *next = current->next;
	return next ? next : start;
}


void cycle(GList *list)
{
	GList *current = get_biggest(list);
	gint value = GPOINTER_TO_INT(current->data);
	current->data = GINT_TO_POINTER(0);
	while (value > 0) {
		current = next_or_wrap(current, list);
		gint v = GPOINTER_TO_INT(current->data);
		current->data = GINT_TO_POINTER(v+1);
		value--;
	}
}


gint main(void)
{
	gint fd = g_open("input.txt", O_RDONLY, 0);
	if (fd < 0) {
		g_printerr("unable to open input\n");
		return 1;
	}

	GList *list = NULL;
	GScanner *scanner = g_scanner_new(NULL);
	g_scanner_input_file(scanner, fd);
	gint size = 0;

	while (g_scanner_get_next_token(scanner) != G_TOKEN_EOF) {
		if (scanner->token != G_TOKEN_INT) {
			continue;
		}

		list = g_list_prepend(list, GINT_TO_POINTER((gint)(scanner->value.v_int)));
		size++;
	}

	list = g_list_reverse(list);

	GHashTable *seen = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
	gchar *set = NULL;
	while (!g_hash_table_lookup(seen, (set = key(list, size)))) {
		g_hash_table_insert(seen, set, GINT_TO_POINTER(1));
		cycle(list);
	}

	gint count = 0;
	do {
		cycle(list);
		count++;
	} while(g_strcmp0(set, key(list, size)));

	g_hash_table_unref(seen);
	g_print("%d\n", count);
}
