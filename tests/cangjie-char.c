/* Copyright (c) 2014 - The libcangjie authors.
 *
 * This file is part of libcangjie.
 *
 * libcangjie is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcangjie is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libcangjie.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <glib.h>
#include <gom/gom.h>

#include <cangjie.h>


void
setup (GomAdapter **adapter, GomRepository **repository)
{
    const char *dburi = ":memory:";
    GList *object_types;

    GError *error = NULL;

    /* Connect to the DB */
    *adapter = gom_adapter_new ();
    gom_adapter_open_sync (*adapter, dburi, &error);
    g_assert_no_error (error);

    /* Create the table */
    *repository = gom_repository_new (*adapter);
    object_types = g_list_prepend (NULL, GINT_TO_POINTER (CANGJIE_TYPE_CHAR));
    gom_repository_automatic_migrate_sync (*repository, 1, object_types, &error);
    g_assert_no_error (error);
}

void
teardown (GomAdapter *adapter, GomRepository *repository)
{
    GError *error = NULL;

    gom_adapter_close_sync (adapter, &error);
    g_assert_no_error (error);

    g_object_unref (repository);
    g_object_unref (adapter);
}

void
test_new_cangjie_char (void)
{
    GomAdapter *adapter = NULL;
    GomRepository *repository = NULL;
    CangjieChar *c;

    GError *error = NULL;

    GomFilter *filter;
    GValue value = { 0, };
    gchar *str;
    gboolean b;
    guint32 i;

    setup (&adapter, &repository);

    /* Insert a CangjieChar */
    c = cangjie_char_new (repository, "來", "来", TRUE, TRUE, FALSE, FALSE,
                          TRUE, FALSE, FALSE, FALSE, FALSE,
                          CANGJIE_ORIENTATION_BOTH, CANGJIE_VERSION_3, "foo",
                          "", 22059);
    gom_resource_save_sync (GOM_RESOURCE (c), &error);
    g_assert_no_error (error);
    g_object_unref (c);

    /* Retrieve the CangjieChar */
    g_value_init (&value, G_TYPE_STRING);
    g_value_set_string (&value, "來");
    filter = gom_filter_new_eq (CANGJIE_TYPE_CHAR, "cjchar", &value);
    g_value_unset (&value);

    c = CANGJIE_CHAR (gom_repository_find_one_sync (repository,
                                                    CANGJIE_TYPE_CHAR,
                                                    filter, &error));
    g_assert_no_error (error);
    g_assert (c);

    /* And check its properties */
    g_object_get (c, "cjchar", &str, NULL);
    g_assert_cmpstr (str, ==, "來");
    g_free (str);

    g_object_get (c, "simpchar", &str, NULL);
    g_assert_cmpstr (str, ==, "来");
    g_free (str);

    g_object_get (c, "zh", &b, NULL);
    g_assert_true (b);

    g_object_get (c, "big5", &b, NULL);
    g_assert_true (b);

    g_object_get (c, "hkscs", &b, NULL);
    g_assert_false (b);

    g_object_get (c, "zhuyin", &b, NULL);
    g_assert_false (b);

    g_object_get (c, "kanji", &b, NULL);
    g_assert_true (b);

    g_object_get (c, "hiragana", &b, NULL);
    g_assert_false (b);

    g_object_get (c, "katakana", &b, NULL);
    g_assert_false (b);

    g_object_get (c, "punctuation", &b, NULL);
    g_assert_false (b);

    g_object_get (c, "symbol", &b, NULL);
    g_assert_false (b);

    g_object_get (c, "orientation", &i, NULL);
    g_assert_cmpuint (i, ==, CANGJIE_ORIENTATION_BOTH);

    g_object_get (c, "version", &i, NULL);
    g_assert_cmpuint (i, ==, CANGJIE_VERSION_3);

    g_object_get (c, "code", &str, NULL);
    g_assert_cmpstr (str, == , "foo");

    g_object_get (c, "shortcode", &str, NULL);
    g_assert_cmpstr (str, ==, "");

    g_object_get (c, "frequency", &i, NULL);
    g_assert_cmpuint (i, ==, 22059);

    teardown (adapter, repository);
}

void
test_cangjie_char_notnull_cjchar (void)
{
    GomAdapter *adapter = NULL;
    GomRepository *repository = NULL;
    CangjieChar *c;

    GError *error = NULL;

    setup (&adapter, &repository);

    /* Insert a CangjieChar without a cjchar */
    c = g_object_new (CANGJIE_TYPE_CHAR, "repository", repository,
                      "code", "foo",
                      NULL);
    gom_resource_save_sync (GOM_RESOURCE (c), &error);
    g_assert_error (error, GOM_ERROR, GOM_ERROR_COMMAND_SQLITE);
    g_assert_true (g_str_match_string ("NOT NULL", error->message, FALSE));
    g_assert_true (g_str_match_string ("chars.cjchar", error->message, FALSE));
    g_object_unref (c);
    g_clear_error (&error);

    teardown (adapter, repository);
}

void
test_cangjie_char_notnull_code (void)
{
    GomAdapter *adapter = NULL;
    GomRepository *repository = NULL;
    CangjieChar *c;

    GError *error = NULL;

    setup (&adapter, &repository);

    /* Insert a CangjieChar without a code */
    c = g_object_new (CANGJIE_TYPE_CHAR, "repository", repository,
                      "cjchar", "來",
                      NULL);
    gom_resource_save_sync (GOM_RESOURCE (c), &error);
    g_assert_error (error, GOM_ERROR, GOM_ERROR_COMMAND_SQLITE);
    g_assert_true (g_str_match_string ("NOT NULL", error->message, FALSE));
    g_assert_true (g_str_match_string ("chars.code", error->message, FALSE));
    g_object_unref (c);
    g_clear_error (&error);

    teardown (adapter, repository);
}

int
main (int argc, char** argv)
{
    g_test_init (&argc, &argv, NULL);

    g_test_add_func ("/cangjie-char/new", test_new_cangjie_char);
    g_test_add_func ("/cangjie-char/notnull-cjchar", test_cangjie_char_notnull_cjchar);
    g_test_add_func ("/cangjie-char/notnull-code", test_cangjie_char_notnull_code);

    return g_test_run ();
}
