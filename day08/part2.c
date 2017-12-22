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

gint register_value(GHashTable *registers, gchar *name)
{
	if (!g_hash_table_contains(registers, name))
		g_hash_table_insert(registers, g_strdup(name), GINT_TO_POINTER(0));

	return GPOINTER_TO_INT(g_hash_table_lookup(registers, name));
}

gint parse_int(gchar *str)
{
	gint sign = 1;
	if (str[0] == '-') {
		sign = -1;
		str = str+1;
	}
	return sign * (gint)(g_ascii_strtoll(str, NULL, 10));
}

gint main(void)
{
	GDataInputStream *stream = get_input("input.txt");
	if (stream == NULL) {
		return 1;
	}

	GError *err = NULL;
	GRegex *regex_line = g_regex_new("(?P<register>[a-z]+) (?P<operation>(inc|dec)) (?P<amount>[-\\d]+) if (?P<condition_register>[a-z]+) (?P<condition_comparator>[<>=!]+) (?P<condition_amount>[-\\d]+)", 0, 0, &err);
	if (err) {
		g_printerr("error creating regex: %s\n", err->message);
		return 1;
	}

	GHashTable *registers = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

	gsize length;
	gchar *line;
	GMatchInfo *info = NULL;
	gint highest_value_ever = 0;
	while ((line = g_data_input_stream_read_line(stream, &length, NULL, &err)) != NULL) {
		if (!g_regex_match(regex_line, line, 0, &info))
			continue;

		gchar *reg = g_match_info_fetch_named(info, "register");
		gchar *operation = g_match_info_fetch_named(info, "operation");
		gchar *sign = g_match_info_fetch_named(info, "sign");
		gchar *amount = g_match_info_fetch_named(info, "amount");
		gchar *condition_register = g_match_info_fetch_named(info, "condition_register");
		gchar *condition_comparator = g_match_info_fetch_named(info, "condition_comparator");
		gchar *condition_amount = g_match_info_fetch_named(info, "condition_amount");

		gint condition_register_value = register_value(registers, condition_register);
		gint condition_amount_value = parse_int(condition_amount);

		gboolean condition_met;
		if (!g_strcmp0(condition_comparator, ">")) {
			condition_met = condition_register_value > condition_amount_value;
		} else if (!g_strcmp0(condition_comparator, ">=")) {
			condition_met = condition_register_value >= condition_amount_value;
		} else if (!g_strcmp0(condition_comparator, "<")) {
			condition_met = condition_register_value < condition_amount_value;
		} else if (!g_strcmp0(condition_comparator, "<=")) {
			condition_met = condition_register_value <= condition_amount_value;
		} else if (!g_strcmp0(condition_comparator, "==")) {
			condition_met = condition_register_value == condition_amount_value;
		} else if (!g_strcmp0(condition_comparator, "!=")) {
			condition_met = condition_register_value != condition_amount_value;
		} else {
			g_printerr("unknown comparator: %s\n", condition_comparator);
			return 2;
		}

		if (condition_met) {
			gint new_value = register_value(registers, reg);
			gint mod_amount = parse_int(amount);
			if (!g_strcmp0(operation, "inc")) {
				new_value += mod_amount;
			} else if (!g_strcmp0(operation, "dec")) {
				new_value -= mod_amount;
			} else {
				g_printerr("unknown operation: %s\n", operation);
				return 2;
			}

			g_hash_table_insert(registers, g_strdup(reg), GINT_TO_POINTER(new_value));
			// g_print("new value for %s: %d\n", reg, new_value);

			if (new_value > highest_value_ever)
				highest_value_ever = new_value;
		}

		g_free(reg);
		g_free(operation);
		g_free(sign);
		g_free(amount);
		g_free(condition_register);
		g_free(condition_comparator);
		g_free(condition_amount);
	}

	g_print("%d\n", highest_value_ever);

	g_hash_table_unref(registers);
}
