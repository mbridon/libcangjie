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

#include "cangjie-context.h"

struct _CangjieContextPrivate
{
    /* GObject properties */
    CangjieOrientation orientation;
    CangjieVersion version;
    guint32 char_families;

    /* Internal-only */
    GomFilter *filter;
};

G_DEFINE_TYPE_WITH_PRIVATE (CangjieContext, cangjie_context, G_TYPE_OBJECT)

enum {
    PROP_0,

    PROP_VERSION,
    PROP_ORIENTATION,
    PROP_CHAR_FAMILIES,

    LAST_PROP
};

static GParamSpec *gParamSpecs [LAST_PROP];

CangjieContext *
cangjie_context_new (CangjieVersion     version,
                     CangjieOrientation orientation,
                     guint32            char_family_flags)
{
    return g_object_new (CANGJIE_TYPE_CONTEXT,
                         "version", version, "orientation", orientation,
                         "char-families", char_family_flags,
                         NULL);
}

static void
cangjie_context_finalize (GObject *object)
{
    CangjieContextPrivate *priv = CANGJIE_CONTEXT (object)->priv;

    g_object_unref (priv->filter);

    G_OBJECT_CLASS (cangjie_context_parent_class)->finalize (object);
}

GomFilter *
cangjie_context_get_char_family_filter_or (CangjieContext *self,
                                             const gchar    *flag_nick,
                                             GomFilter      *current_filter)
{
    GomFilter *result_filter, *filter_eq;
    GValue value = { 0, };
    GFlagsClass *family_class = g_type_class_ref (CANGJIE_TYPE_CHAR_FAMILY_FLAGS);
    CangjieCharFamilyFlags flag_value =
            g_flags_get_value_by_nick (family_class, flag_nick)->value;

    if (self->priv->char_families & flag_value) {
        g_value_init (&value, G_TYPE_BOOLEAN);
        g_value_set_boolean (&value, TRUE);

        if (current_filter == NULL) {
            result_filter = gom_filter_new_eq (CANGJIE_TYPE_CHAR, flag_nick, &value);
        } else {
            filter_eq = gom_filter_new_eq (CANGJIE_TYPE_CHAR, flag_nick, &value);
            result_filter = gom_filter_new_or (current_filter, filter_eq);
            g_object_unref (filter_eq);
            g_object_unref (current_filter);
        }

        g_value_unset (&value);

        return result_filter;
    }

    return current_filter;
}

void
cangjie_context_reset_filter (CangjieContext *self)
{
    GomFilter *version_filter, *orientation_filter, *v_and_o_filter;
    GomFilter *family_filter = NULL;
    GValue value = { 0, };

    if (self->priv->filter != NULL) {
        g_object_unref (self->priv->filter);
    }

    /* Version filter */
    g_value_init (&value, CANGJIE_TYPE_VERSION);
    g_value_set_enum (&value, self->priv->version);
    version_filter = gom_filter_new_eq (CANGJIE_TYPE_CHAR, "version", &value);
    g_value_unset (&value);

    /* Orientation filter */
    g_value_init (&value, CANGJIE_TYPE_ORIENTATION);
    if (self->priv->orientation == CANGJIE_ORIENTATION_HORIZONTAL) {
        /* We want everything that IS NOT vertical-only */
        g_value_set_enum (&value, CANGJIE_ORIENTATION_VERTICAL);
    } else {
        /* We want everything that IS NOT horizontal-only */
        g_value_set_enum (&value, CANGJIE_ORIENTATION_HORIZONTAL);
    }
    orientation_filter = gom_filter_new_neq (CANGJIE_TYPE_CHAR, "orientation", &value);
    g_value_unset (&value);

    if (self->priv->char_families == CANGJIE_CHAR_FAMILY_ALL) {
        /* Don't bother with a family filter */
        self->priv->filter = gom_filter_new_and (version_filter, orientation_filter);
        g_object_unref (version_filter);
        g_object_unref (orientation_filter);
        return;
    }

    v_and_o_filter = gom_filter_new_and (version_filter, orientation_filter);
    g_object_unref (version_filter);
    g_object_unref (orientation_filter);

    /* Family filter */
    family_filter = cangjie_context_get_char_family_filter_or (self, "zh",
                                                               family_filter);
    family_filter = cangjie_context_get_char_family_filter_or (self, "big5",
                                                               family_filter);
    family_filter = cangjie_context_get_char_family_filter_or (self, "hkscs",
                                                               family_filter);
    family_filter = cangjie_context_get_char_family_filter_or (self, "zhuyin",
                                                               family_filter);
    family_filter = cangjie_context_get_char_family_filter_or (self, "kanji",
                                                               family_filter);
    family_filter = cangjie_context_get_char_family_filter_or (self, "hiragana",
                                                               family_filter);
    family_filter = cangjie_context_get_char_family_filter_or (self, "katakana",
                                                               family_filter);
    family_filter = cangjie_context_get_char_family_filter_or (self, "punctuation",
                                                               family_filter);
    family_filter = cangjie_context_get_char_family_filter_or (self, "symbol",
                                                               family_filter);

    /* The whole filter */
    self->priv->filter = gom_filter_new_and (v_and_o_filter, family_filter);
    g_object_unref (v_and_o_filter);
    g_object_unref (family_filter);
}

static void
cangjie_context_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
    CangjieContext *self = CANGJIE_CONTEXT (object);

    switch (prop_id)
    {
        case PROP_VERSION:
            g_value_set_enum (value, self->priv->version);
            break;

        case PROP_ORIENTATION:
            g_value_set_enum (value, self->priv->orientation);
            break;

        case PROP_CHAR_FAMILIES:
            g_value_set_flags (value, self->priv->char_families);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
cangjie_context_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
    CangjieContext *self = CANGJIE_CONTEXT (object);
    guint32 orientation;

    switch (prop_id)
    {
        case PROP_VERSION:
            self->priv->version = g_value_get_enum (value);
            cangjie_context_reset_filter (self);
            break;

        case PROP_ORIENTATION:
            orientation = g_value_get_enum (value);

            if (orientation == CANGJIE_ORIENTATION_BOTH) {
                g_error ("CANGJIE_ORIENTATION_BOTH is not a valid orientation"
                         " value for a CangjieContext");
            }

            self->priv->orientation = orientation;
            cangjie_context_reset_filter (self);
            break;

        case PROP_CHAR_FAMILIES:
            self->priv->char_families = g_value_get_flags (value);
            cangjie_context_reset_filter (self);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
cangjie_context_class_init (CangjieContextClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->finalize = cangjie_context_finalize;
    object_class->get_property = cangjie_context_get_property;
    object_class->set_property = cangjie_context_set_property;

    gParamSpecs[PROP_VERSION] =
            g_param_spec_enum ("version", "Version",
                               "The Cangjie version the user wants to input in",
                               CANGJIE_TYPE_VERSION,
                               CANGJIE_VERSION_3, G_PARAM_READWRITE);
    gParamSpecs[PROP_ORIENTATION] =
            g_param_spec_enum ("orientation", "Orientation",
                               "The orientation the user is inputing in",
                               CANGJIE_TYPE_ORIENTATION,
                               CANGJIE_ORIENTATION_HORIZONTAL, G_PARAM_READWRITE);
    gParamSpecs[PROP_CHAR_FAMILIES] =
            g_param_spec_flags ("char-families", "Char Families",
                                "Bitwise OR-ed flags representing the families of characters the user wants",
                                CANGJIE_TYPE_CHAR_FAMILY_FLAGS,
                                CANGJIE_CHAR_FAMILY_ALL, G_PARAM_READWRITE);

    g_object_class_install_properties (object_class, LAST_PROP, gParamSpecs);
}

static void
cangjie_context_init (CangjieContext *self)
{
    self->priv = cangjie_context_get_instance_private (self);
}
