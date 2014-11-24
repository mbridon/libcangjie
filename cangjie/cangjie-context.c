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

    /* TODO: Free all private resources */

    G_OBJECT_CLASS (cangjie_context_parent_class)->finalize (object);
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

    switch (prop_id)
    {
        case PROP_VERSION:
            self->priv->version = g_value_get_enum (value);
            break;

        case PROP_ORIENTATION:
            self->priv->orientation = g_value_get_enum (value);
            break;

        case PROP_CHAR_FAMILIES:
            self->priv->char_families = g_value_get_flags (value);
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
