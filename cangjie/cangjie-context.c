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
};

G_DEFINE_TYPE_WITH_PRIVATE (CangjieContext, cangjie_context, G_TYPE_OBJECT)

enum {
    PROP_0,


    LAST_PROP
};

static GParamSpec *gParamSpecs [LAST_PROP];

CangjieContext *
cangjie_context_new ()
{
    return g_object_new (CANGJIE_TYPE_CONTEXT,
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

    g_object_class_install_properties (object_class, LAST_PROP, gParamSpecs);
}

static void
cangjie_context_init (CangjieContext *self)
{
    self->priv = cangjie_context_get_instance_private (self);
}
