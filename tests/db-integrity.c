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

#include <locale.h>

#include <glib.h>
#include <gom/gom.h>

#include <cangjie.h>


void
setup (GomAdapter **adapter, GomRepository **repository)
{
    char *dburi = g_strjoin ("", "file://", CANGJIE_DB_PATH, "?mode=ro", NULL);
    GList *object_types;

    GError *error = NULL;

    /* Connect to the DB */
    *adapter = gom_adapter_new ();
    gom_adapter_open_sync (*adapter, dburi, &error);
    g_assert_no_error (error);
    g_free (dburi);

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
assert_cangjie_char_equal (CangjieChar *c1, CangjieChar *c2)
{
    gchar *cjchar1, *simpchar1;
    gboolean zh1, big51, hkscs1, zhuyin1, kanji1, hiragana1, katakana1, \
             punctuation1, symbol1;
    CangjieOrientation orientation1;

    gchar *cjchar2, *simpchar2;
    gboolean zh2, big52, hkscs2, zhuyin2, kanji2, hiragana2, katakana2, \
             punctuation2, symbol2;
    CangjieOrientation orientation2;

    g_object_get (c1, "cjchar", &cjchar1, "simpchar", &simpchar1, "zh", &zh1,
                  "big5", &big51, "hkscs", &hkscs1, "zhuyin", &zhuyin1,
                  "kanji", &kanji1, "hiragana", &hiragana1,
                  "katakana", &katakana1, "punctuation", &punctuation1,
                  "symbol", &symbol1, "orientation", &orientation1, NULL);

    g_object_get (c2, "cjchar", &cjchar2, "simpchar", &simpchar2, "zh", &zh2,
                  "big5", &big52, "hkscs", &hkscs2, "zhuyin", &zhuyin2,
                  "kanji", &kanji2, "hiragana", &hiragana2,
                  "katakana", &katakana2, "punctuation", &punctuation2,
                  "symbol", &symbol2, "orientation", &orientation2, NULL);

    if ((g_str_equal (cjchar1, cjchar2)) && \
        (g_str_equal (simpchar1, simpchar2)) && \
        (zh1 == zh2) && (big51 == big52) && (hkscs1 == hkscs2) && \
        (zhuyin1 == zhuyin2) && (kanji1 == kanji2) && \
        (hiragana1 == hiragana2) && (katakana1 == katakana2) && \
        (punctuation1 == punctuation2) && (symbol1 == symbol2) && \
        (orientation1 == orientation2)) {
        return;
    }

    g_error ("\n-%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n+%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
             cjchar1, simpchar1, zh1, big51, hkscs1, zhuyin1, kanji1,
             hiragana1, katakana1, punctuation1, symbol1, orientation1,
             cjchar2, simpchar2, zh2, big52, hkscs2, zhuyin2, kanji2,
             hiragana2, katakana2, punctuation2, symbol2, orientation2);
}


void
test_db_integrity (void)
{
    /* Our data source format is a bit complex, and might lead to easy
     * mistakes when maintaining it in the long run.
     *
     * This test ensures that we don't have inconsistencies on the parts that
     * are duplicated on every line, as it would be easy for example to change
     * the 'zh' column of a given 'cjchar' on one line but not on others.
     */
    GomAdapter *adapter = NULL;
    GomRepository *repository = NULL;

    GomResourceGroup *group;
    CangjieChar *c = NULL;
    gchar *cjchar;

    GHashTable *seen_chars = g_hash_table_new (g_str_hash, g_str_equal);
    CangjieChar *other;

    GError *error;

    guint count, i;

    setlocale (LC_ALL, "en_US.utf8");

    setup (&adapter, &repository);

    group = gom_repository_find_sync (repository, CANGJIE_TYPE_CHAR, NULL,
                                      &error);

    count = gom_resource_group_get_count (group);
    gom_resource_group_fetch_sync (group, 0, count, &error);

    for (i = 0; i < count; i++) {
        c = CANGJIE_CHAR (gom_resource_group_get_index (group, i));

        g_object_get (c, "cjchar", &cjchar, NULL);
        g_assert_nonnull (cjchar);

        other = g_hash_table_lookup (seen_chars, cjchar);

        if (other == NULL) {
            g_hash_table_insert (seen_chars, g_strdup (cjchar), c);
        } else {
            assert_cangjie_char_equal (c, other);
        }
    }

    g_object_unref (group);
    g_hash_table_destroy (seen_chars);

    teardown (adapter, repository);
}


int
main (int argc, char** argv)
{
    g_test_init (&argc, &argv, NULL);

    g_test_add_func ("/db-integrity", test_db_integrity);

    return g_test_run ();
}
