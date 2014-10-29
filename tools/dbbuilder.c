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
#include <stdlib.h>
#include <string.h>

#include <gio/gio.h>
#include <glib.h>
#include <gom/gom.h>

#include <cangjie.h>


static GomAdapter *adapter;
static GomRepository *repository;
static GomResourceGroup *group;


void
usage (gchar *progname)
{
    g_print ("Usage: %s RESULTDB SOURCEFILE [SOURCEFILE ...]\n", progname);
}


void
create_db (gchar *dburi)
{
    GList *obj_types;
    GError *error = NULL;

    GTimer *timer = g_timer_new ();

    /* Connect to the DB */
    adapter = gom_adapter_new ();
    gom_adapter_open_sync (adapter, dburi, &error);
    g_assert_no_error (error);

    /* Create the table */
    repository = gom_repository_new (adapter);
    obj_types = g_list_prepend (NULL, GINT_TO_POINTER (CANGJIE_TYPE_CHAR));
    gom_repository_automatic_migrate_sync (repository, 1, obj_types, &error);
    g_assert_no_error (error);

    g_print ("Time taken to create the database: %f seconds\n", g_timer_elapsed (timer, NULL));
    g_timer_destroy (timer);
}


gchar *
get_version_nick (GFile *tablefile) {
    gchar *basename = g_file_get_basename (tablefile);

    /* All table files are called table-XXXX.txt */
    gchar *hyphen = strstr (basename, "-");
    gchar *dot = g_strrstr (basename, ".");

    return g_strndup (hyphen + 1, dot - hyphen - 1);
}


void
parse_and_insert_line (const gchar *line, CangjieVersion version)
{
    GEnumClass *orientation_class = g_type_class_ref (CANGJIE_TYPE_ORIENTATION);

    gchar **tokens = g_strsplit (line, "\t", 15);
    gchar *cjchar = tokens[0];
    gchar *simpchar = tokens[1];
    gboolean zh = atoi (tokens[2]);
    gboolean big5 = atoi (tokens[3]);
    gboolean hkscs = atoi (tokens[4]);
    gboolean zhuyin = atoi (tokens[5]);
    gboolean kanji = atoi (tokens[6]);
    gboolean hiragana = atoi (tokens[7]);
    gboolean katakana = atoi (tokens[8]);
    gboolean punctuation = atoi (tokens[9]);
    gboolean symbol = atoi (tokens[10]);
    CangjieOrientation orientation = g_enum_get_value_by_nick (orientation_class, tokens[11])->value;
    gchar *code = tokens[12];
    gchar *shortcode = tokens[13];
    guint frequency = atoi (tokens[14]);

    CangjieChar *c;

    if ((g_strcmp0 (code, "") == 0) && (g_strcmp0 (shortcode, "") == 0)) {
        /* This character would be useless in the database */
        return;
    }

    c = cangjie_char_new (repository, cjchar, simpchar, zh, big5, hkscs,
                          zhuyin, kanji, hiragana, katakana, punctuation,
                          symbol, orientation, version, code, shortcode,
                          frequency);

    gom_resource_group_append (group, GOM_RESOURCE (c));

    g_object_unref (c);
    g_strfreev (tokens);
    g_type_class_unref (orientation_class);
}


int
main (gint argc, gchar **argv)
{
    gint ret = 0;

    gchar *dbpath;

    gint i;
    gchar *tablepath;
    GFile *tablefile;
    GFileInputStream *stream;
    GDataInputStream *table;
    gchar *nick, *line;
    guint32 linenum = 0;
    gsize length;

    GEnumClass *version_class = g_type_class_ref (CANGJIE_TYPE_VERSION);
    CangjieVersion version;

    GError *error = NULL;
    GTimer *timer = g_timer_new ();

    if (argc < 3) {
        usage (argv[0]);
        return -1;
    }

    /* Hard-code a UTF-8 locale.
     *
     * Our source data actually **is** in UTF-8, and the printing here is only
     * for debugging purpose. Also, there is no need for i18n of this tool.
     */
    setlocale (LC_ALL, "en_US.utf8");

    dbpath = argv[1];
    if (g_file_test (dbpath, G_FILE_TEST_EXISTS)) {
        g_print ("DB file already exists: %s\n", dbpath);
        return 1;
    }

    create_db (dbpath);

    group = gom_resource_group_new (repository);

    for (i = 2; i < argc; i++) {
        tablepath = argv[i];
        tablefile = g_file_new_for_path (tablepath);

        nick = get_version_nick (tablefile);
        version = g_enum_get_value_by_nick (version_class, nick)->value;
        g_free (nick);

        stream = g_file_read (tablefile, NULL, &error);

        if (error != NULL) {
            ret = error->code;
            g_print ("%s: Cannot read %s: %s\n", argv[0], tablepath,
                     error->message);
            g_error_free (error);
            g_object_unref (tablefile);
            return ret;
        }

        table = g_data_input_stream_new (G_INPUT_STREAM (stream));

        g_timer_reset (timer);

        while (TRUE) {
            linenum += 1;
            line = g_data_input_stream_read_line_utf8 (table, &length, NULL, &error);

            if (error != NULL) {
                ret = error->code;
                g_print ("%s: Error reading line %d: %s\n", argv[0], linenum,
                         error->message);
                g_error_free (error);
                g_free (line);
                g_object_unref (table);
                g_object_unref (stream);
                g_object_unref (tablefile);
                return ret;
            }

            if (line == NULL) {
                /* We finished reading the file */
                break;
            }

            if (length == 0 || line[0] == '#') {
                /* Ignore empty and comment lines */
                g_free (line);
                continue;
            }

            parse_and_insert_line (line, version);

            g_free (line);
        }

        g_print ("Time taken to parse %s: %f seconds\n", tablepath, g_timer_elapsed (timer, NULL));

        g_object_unref (table);
        g_object_unref (stream);
        g_object_unref (tablefile);
    }

    g_timer_reset (timer);

    gom_resource_group_write_sync (group, &error);

    if (error != NULL) {
        ret = error->code;
        g_print ("%s: Error writing to the database: %s\n", argv[0],
                 error->message);
        g_error_free (error);
    }

    g_print ("Time taken to write to the DB: %f seconds\n", g_timer_elapsed (timer, NULL));
    g_timer_reset (timer);

    gom_adapter_close_sync (adapter, &error);

    if (error != NULL) {
        ret = error->code;
        g_print ("%s: Error closing the connection to the database: %s\n",
                 argv[0], error->message);
        g_error_free (error);
    }

    g_object_unref (repository);
    g_object_unref (adapter);

    g_print ("Time taken to close the DB: %f seconds\n", g_timer_elapsed (timer, NULL));
    g_timer_destroy (timer);

    return ret;
}
