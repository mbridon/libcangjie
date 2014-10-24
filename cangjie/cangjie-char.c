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


#include "cangjie-char.h"

struct _CangjieCharPrivate
{
    guint32 id;
    gchar *cjchar;
    gchar *simpchar;
    gboolean zh;
    gboolean big5;
    gboolean hkscs;
    gboolean zhuyin;
    gboolean kanji;
    gboolean hiragana;
    gboolean katakana;
    gboolean punctuation;
    gboolean symbol;
    gchar *orientation;
    gchar *code;
    gchar *shortcode;
    guint32 frequency;
};

G_DEFINE_TYPE_WITH_PRIVATE (CangjieChar, cangjie_char, GOM_TYPE_RESOURCE)

enum {
    PROP_0,

    PROP_ID,
    PROP_CJCHAR,
    PROP_SIMPCHAR,
    PROP_ZH,
    PROP_BIG5,
    PROP_HKSCS,
    PROP_ZHUYIN,
    PROP_KANJI,
    PROP_HIRAGANA,
    PROP_KATAKANA,
    PROP_PUNCTUATION,
    PROP_SYMBOL,
    PROP_ORIENTATION,
    PROP_CODE,
    PROP_SHORTCODE,
    PROP_FREQUENCY,

    LAST_PROP
};

static GParamSpec *gParamSpecs [LAST_PROP];

CangjieChar *
cangjie_char_new (GomRepository *repository,
                  gchar         *cjchar,
                  gchar         *simpchar,
                  gboolean       zh,
                  gboolean       big5,
                  gboolean       hkscs,
                  gboolean       zhuyin,
                  gboolean       kanji,
                  gboolean       hiragana,
                  gboolean       katakana,
                  gboolean       punctuation,
                  gboolean       symbol,
                  gchar         *orientation,
                  gchar         *code,
                  gchar         *shortcode,
                  guint32        frequency)
{
    return g_object_new (CANGJIE_TYPE_CHAR, "repository", repository,
                         "cjchar", cjchar, "simpchar", simpchar, "zh", zh,
                         "big5", big5, "hkscs", hkscs, "zhuyin", zhuyin,
                         "kanji", kanji, "hiragana", hiragana,
                         "katakana", katakana, "punctuation", punctuation,
                         "symbol", symbol, "orientation", orientation,
                         "code", code, "shortcode", shortcode,
                         "frequency", frequency,
                         NULL);
}

static void
cangjie_char_finalize (GObject *object)
{
    CangjieCharPrivate *priv = CANGJIE_CHAR (object)->priv;

    g_free (priv->cjchar);
    g_free (priv->simpchar);
    g_free (priv->code);
    g_free (priv->shortcode);

    G_OBJECT_CLASS (cangjie_char_parent_class)->finalize (object);
}

static void
cangjie_char_get_property (GObject    *object,
                           guint       prop_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
    CangjieChar *self = CANGJIE_CHAR (object);

    switch (prop_id)
    {
        case PROP_ID:
            g_value_set_uint (value, self->priv->id);
            break;

        case PROP_CJCHAR:
            g_value_set_string (value, self->priv->cjchar);
            break;

        case PROP_SIMPCHAR:
            g_value_set_string (value, self->priv->simpchar);
            break;

        case PROP_ZH:
            g_value_set_boolean (value, self->priv->zh);
            break;

        case PROP_BIG5:
            g_value_set_boolean (value, self->priv->big5);
            break;

        case PROP_HKSCS:
            g_value_set_boolean (value, self->priv->hkscs);
            break;

        case PROP_ZHUYIN:
            g_value_set_boolean (value, self->priv->zhuyin);
            break;

        case PROP_KANJI:
            g_value_set_boolean (value, self->priv->kanji);
            break;

        case PROP_HIRAGANA:
            g_value_set_boolean (value, self->priv->hiragana);
            break;

        case PROP_KATAKANA:
            g_value_set_boolean (value, self->priv->katakana);
            break;

        case PROP_PUNCTUATION:
            g_value_set_boolean (value, self->priv->punctuation);
            break;

        case PROP_SYMBOL:
            g_value_set_boolean (value, self->priv->symbol);
            break;

        case PROP_ORIENTATION:
            g_value_set_string (value, self->priv->orientation);
            break;

        case PROP_CODE:
            g_value_set_string (value, self->priv->code);
            break;

        case PROP_SHORTCODE:
            g_value_set_string (value, self->priv->shortcode);
            break;

        case PROP_FREQUENCY:
            g_value_set_uint (value, self->priv->frequency);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
cangjie_char_set_property (GObject      *object,
                           guint         prop_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
    CangjieChar *self = CANGJIE_CHAR (object);

    switch (prop_id)
    {
        case PROP_ID:
            self->priv->id = g_value_get_uint (value);
            break;

        case PROP_CJCHAR:
            g_free (self->priv->cjchar);
            self->priv->cjchar = g_value_dup_string (value);
            break;

        case PROP_SIMPCHAR:
            g_free (self->priv->simpchar);
            self->priv->simpchar = g_value_dup_string (value);
            break;

        case PROP_ZH:
            self->priv->zh = g_value_get_boolean (value);
            break;

        case PROP_BIG5:
            self->priv->big5 = g_value_get_boolean (value);
            break;

        case PROP_HKSCS:
            self->priv->hkscs = g_value_get_boolean (value);
            break;

        case PROP_ZHUYIN:
            self->priv->zhuyin = g_value_get_boolean (value);
            break;

        case PROP_KANJI:
            self->priv->kanji = g_value_get_boolean (value);
            break;

        case PROP_HIRAGANA:
            self->priv->hiragana = g_value_get_boolean (value);
            break;

        case PROP_KATAKANA:
            self->priv->katakana = g_value_get_boolean (value);
            break;

        case PROP_PUNCTUATION:
            self->priv->punctuation = g_value_get_boolean (value);
            break;

        case PROP_SYMBOL:
            self->priv->symbol = g_value_get_boolean (value);
            break;

        case PROP_ORIENTATION:
            g_free (self->priv->orientation);
            self->priv->orientation = g_value_dup_string (value);
            break;

        case PROP_CODE:
            g_free (self->priv->code);
            self->priv->code = g_value_dup_string (value);
            break;

        case PROP_SHORTCODE:
            g_free (self->priv->shortcode);
            self->priv->shortcode = g_value_dup_string (value);
            break;

        case PROP_FREQUENCY:
            self->priv->frequency = g_value_get_uint (value);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
cangjie_char_class_init (CangjieCharClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GomResourceClass *resource_class = GOM_RESOURCE_CLASS (klass);

    object_class->finalize = cangjie_char_finalize;
    object_class->get_property = cangjie_char_get_property;
    object_class->set_property = cangjie_char_set_property;

    gom_resource_class_set_table (resource_class, "chars");

    gParamSpecs[PROP_ID] =
            g_param_spec_uint ("id", "ID", "The identifier of the character",
                               0, G_MAXUINT, 0, G_PARAM_READWRITE);
    gParamSpecs[PROP_CJCHAR] =
            g_param_spec_string ("cjchar", "Character",
                                 "The character to input",
                                 NULL, G_PARAM_READWRITE);
    gParamSpecs[PROP_SIMPCHAR] =
            g_param_spec_string ("simpchar", "Simplified Character",
                                 "The Simplified Chinese equivalent of the character to input",
                                 NULL, G_PARAM_READWRITE);
    gParamSpecs[PROP_ZH] =
            g_param_spec_boolean ("zh", "Chinese",
                                  "Whether this is a Chinese character",
                                  FALSE, G_PARAM_READWRITE);
    gParamSpecs[PROP_BIG5] =
            g_param_spec_boolean ("big5", "Big 5",
                                  "Whether this character exists in Big 5",
                                  FALSE, G_PARAM_READWRITE);
    gParamSpecs[PROP_HKSCS] =
            g_param_spec_boolean ("hkscs", "HK HSCS",
                                  "Whether this character is part of the HK SCS",
                                  FALSE, G_PARAM_READWRITE);
    gParamSpecs[PROP_ZHUYIN] =
            g_param_spec_boolean ("zhuyin", "Zhuyin",
                                  "Whether this is character is part of the Zhuyin alphabet",
                                  FALSE, G_PARAM_READWRITE);
    gParamSpecs[PROP_KANJI] =
            g_param_spec_boolean ("kanji", "Kanji",
                                  "Whether this is a Japanese Kanji character",
                                  FALSE, G_PARAM_READWRITE);
    gParamSpecs[PROP_HIRAGANA] =
            g_param_spec_boolean ("hiragana", "Hiragana",
                                  "Whether this is a Japanese Hiragana character",
                                  FALSE, G_PARAM_READWRITE);
    gParamSpecs[PROP_KATAKANA] =
            g_param_spec_boolean ("katakana", "Katakana",
                                  "Whether this is a Japanese Katakana character",
                                  FALSE, G_PARAM_READWRITE);
    gParamSpecs[PROP_PUNCTUATION] =
            g_param_spec_boolean ("punctuation", "Punctuation",
                                  "Whether this is a Punctuation character",
                                  FALSE, G_PARAM_READWRITE);
    gParamSpecs[PROP_SYMBOL] =
            g_param_spec_boolean ("symbol", "Symbol",
                                  "Whether this is a symbol",
                                  FALSE, G_PARAM_READWRITE);
    gParamSpecs[PROP_ORIENTATION] =
            g_param_spec_string ("orientation", "Orientation",
                                 "The orientation of the character",
                                 NULL, G_PARAM_READWRITE);
    gParamSpecs[PROP_CODE] =
            g_param_spec_string ("code", "Code",
                                 "The Cangjie code of the character",
                                 NULL, G_PARAM_READWRITE);
    gParamSpecs[PROP_SHORTCODE] =
            g_param_spec_string ("shortcode", "Short Code",
                                 "The short code to input this character",
                                 NULL, G_PARAM_READWRITE);
    gParamSpecs[PROP_FREQUENCY] =
            g_param_spec_uint ("frequency", "Frequency",
                               "The frequency of the character",
                               0, G_MAXUINT, 0, G_PARAM_READWRITE);

    g_object_class_install_properties (object_class, LAST_PROP, gParamSpecs);

    gom_resource_class_set_primary_key (resource_class, "id");

    gom_resource_class_set_notnull (resource_class, "cjchar");
    gom_resource_class_set_notnull (resource_class, "code");
}

static void
cangjie_char_init (CangjieChar *self)
{
    self->priv = cangjie_char_get_instance_private (self);
}
