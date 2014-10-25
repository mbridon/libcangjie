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


/* inclusion guard */
#ifndef CANGJIE_ENUMS_H
#define CANGJIE_ENUMS_H

#if !defined (CANGJIE_H) && !defined (CANGJIE_COMPILATION)
#error "Only <cangjie.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum {
    CANGJIE_ORIENTATION_HORIZONTAL = 0,
    CANGJIE_ORIENTATION_VERTICAL = 1,
    CANGJIE_ORIENTATION_BOTH = 2,
} CangjieOrientation;

#define CANGJIE_TYPE_ORIENTATION cangjie_orientation_get_type ()
GType cangjie_orientation_get_type (void);

G_END_DECLS

#endif
