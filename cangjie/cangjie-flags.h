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


#ifndef CANGJIE_FLAGS_H
#define CANGJIE_FLAGS_H

#if !defined (CANGJIE_H) && !defined (CANGJIE_COMPILATION)
#error "Only <cangjie.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum
{
  CANGJIE_CHAR_FAMILY_ALL         = 0,
  CANGJIE_CHAR_FAMILY_CHINESE     = (1<<0),
  CANGJIE_CHAR_FAMILY_BIG5        = (1<<1),
  CANGJIE_CHAR_FAMILY_HKSCS       = (1<<2),
  CANGJIE_CHAR_FAMILY_ZHUYIN      = (1<<3),
  CANGJIE_CHAR_FAMILY_KANJI       = (1<<4),
  CANGJIE_CHAR_FAMILY_HIRAGANA    = (1<<5),
  CANGJIE_CHAR_FAMILY_KATAKANA    = (1<<6),
  CANGJIE_CHAR_FAMILY_PUNCTUATION = (1<<7),
  CANGJIE_CHAR_FAMILY_SYMBOL      = (1<<8)
} CangjieCharFamilyFlags;

GType cangjie_char_family_flags_get_type (void);
#define CANGJIE_TYPE_CHAR_FAMILY_FLAGS (cangjie_char_family_flags_get_type ())

G_END_DECLS

#endif
