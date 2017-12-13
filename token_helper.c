void print_token(GTokenType token)
{
	switch (token) {
		case G_TOKEN_EOF:
			g_print("%d: EOF\n", token);
			break;

		case G_TOKEN_LEFT_PAREN:
			g_print("%d: LEFT_PAREN\n", token);
			break;

		case G_TOKEN_RIGHT_PAREN:
			g_print("%d: RIGHT_PAREN\n", token);
			break;

		case G_TOKEN_LEFT_CURLY:
			g_print("%d: LEFT_CURLY\n", token);
			break;

		case G_TOKEN_RIGHT_CURLY:
			g_print("%d: RIGHT_CURLY\n", token);
			break;

		case G_TOKEN_LEFT_BRACE:
			g_print("%d: LEFT_BRACE\n", token);
			break;

		case G_TOKEN_RIGHT_BRACE:
			g_print("%d: RIGHT_BRACE\n", token);
			break;

		case G_TOKEN_EQUAL_SIGN:
			g_print("%d: EQUAL_SIGN\n", token);
			break;

		case G_TOKEN_COMMA:
			g_print("%d: COMMA\n", token);
			break;

		case G_TOKEN_NONE:
			g_print("%d: NONE\n", token);
			break;

		case G_TOKEN_ERROR:
			g_print("%d: ERROR\n", token);
			break;

		case G_TOKEN_CHAR:
			g_print("%d: CHAR\n", token);
			break;

		case G_TOKEN_BINARY:
			g_print("%d: BINARY\n", token);
			break;

		case G_TOKEN_OCTAL:
			g_print("%d: OCTAL\n", token);
			break;

		case G_TOKEN_INT:
			g_print("%d: INT\n", token);
			break;

		case G_TOKEN_HEX:
			g_print("%d: HEX\n", token);
			break;

		case G_TOKEN_FLOAT:
			g_print("%d: FLOAT\n", token);
			break;

		case G_TOKEN_STRING:
			g_print("%d: STRING\n", token);
			break;

		case G_TOKEN_SYMBOL:
			g_print("%d: SYMBOL\n", token);
			break;

		case G_TOKEN_IDENTIFIER:
			g_print("%d: IDENTIFIER\n", token);
			break;

		case G_TOKEN_IDENTIFIER_NULL:
			g_print("%d: IDENTIFIER_NULL\n", token);
			break;

		case G_TOKEN_COMMENT_SINGLE:
			g_print("%d: COMMENT_SINGLE\n", token);
			break;

		case G_TOKEN_COMMENT_MULTI:
			g_print("%d: COMMENT_MULTI\n", token);
			break;

		case G_TOKEN_LAST:
			g_print("%d: LAST\n", token);
			break;

		default:
			g_print("%d: unknown\n", token);
			break;
	}
}
