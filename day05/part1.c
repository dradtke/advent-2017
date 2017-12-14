#include <glib.h>
#include <glib/gstdio.h>
#include <fcntl.h>


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

	gint negative = FALSE, n = 0;
	while (g_scanner_get_next_token(scanner) != G_TOKEN_EOF) {
		if (scanner->token != G_TOKEN_INT) {
			if (scanner->token == '-') {
				negative = TRUE;
			}
			continue;
		}

		n = (gint)(scanner->value.v_int);
		if (negative) {
			n *= -1;
		}
		negative = FALSE;
		list = g_list_prepend(list, GINT_TO_POINTER(n));
	}

	gint count = 0;
	list = g_list_reverse(list);
	while (list) {
		gint instruction = GPOINTER_TO_INT(list->data);
		list->data = GINT_TO_POINTER(instruction+1);
		if (instruction < 0) {
			for (int i = instruction; list && i < 0; i++) {
				list = list->prev;
			}
		} else if (instruction > 0) {
			for (int i = 0; list && i < instruction; i++) {
				list = list->next;
			}
		}
		count++;
	}

	g_print("%d\n", count);
}
