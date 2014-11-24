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


#ifndef CANGJIE_CONTEXT_H
#define CANGJIE_CONTEXT_H

#if !defined (CANGJIE_H) && !defined (CANGJIE_COMPILATION)
#error "Only <cangjie.h> can be included directly."
#endif

#include <glib-object.h>
#include <gom/gom.h>
#include "cangjie-char.h"
#include "cangjie-enums.h"
#include "cangjie-flags.h"

G_BEGIN_DECLS

#define CANGJIE_TYPE_CONTEXT            (cangjie_context_get_type())
#define CANGJIE_CONTEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANGJIE_TYPE_CONTEXT, CangjieContext))
#define CANGJIE_CONTEXT_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), CANGJIE_TYPE_CONTEXT, CangjieContext const))
#define CANGJIE_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  CANGJIE_TYPE_CONTEXT, CangjieContextClass))
#define CANGJIE_IS_CONTEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CANGJIE_TYPE_CONTEXT))
#define CANGJIE_IS_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  CANGJIE_TYPE_CONTEXT))
#define CANGJIE_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  CANGJIE_TYPE_CONTEXT, CangjieContextClass))

typedef struct _CangjieContext        CangjieContext;
typedef struct _CangjieContextClass   CangjieContextClass;
typedef struct _CangjieContextPrivate CangjieContextPrivate;

struct _CangjieContext
{
    GObject parent;
    CangjieContextPrivate *priv;
};

struct _CangjieContextClass
{
    GObjectClass parent;
};

GType            cangjie_context_get_type                    (void);
CangjieContext  *cangjie_context_new                         (CangjieVersion     version,
                                                              CangjieOrientation orientation,
                                                              guint32            type_flags);

G_END_DECLS

#endif /* CANGJIE_CONTEXT_H */
