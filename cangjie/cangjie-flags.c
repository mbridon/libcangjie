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


#include "cangjie-flags.h"


GType
cangjie_char_family_flags_get_type (void)
{
  static volatile gsize g_define_type_id__volatile = 0;

  if (g_once_init_enter (&g_define_type_id__volatile))
    {
      static const GFlagsValue values[] = {
        { CANGJIE_CHAR_FAMILY_ALL, "CANGJIE_CHAR_FAMILY_ALL", "all" },
        { CANGJIE_CHAR_FAMILY_CHINESE, "CANGJIE_CHAR_FAMILY_CHINESE", "zh" },
        { CANGJIE_CHAR_FAMILY_BIG5, "CANGJIE_CHAR_FAMILY_BIG5", "big5" },
        { CANGJIE_CHAR_FAMILY_HKSCS, "CANGJIE_CHAR_FAMILY_HKSCS", "hkscs" },
        { CANGJIE_CHAR_FAMILY_ZHUYIN, "CANGJIE_CHAR_FAMILY_ZHUYIN", "zhuyin" },
        { CANGJIE_CHAR_FAMILY_KANJI, "CANGJIE_CHAR_FAMILY_KANJI", "kanji" },
        { CANGJIE_CHAR_FAMILY_HIRAGANA, "CANGJIE_CHAR_FAMILY_HIRAGANA", "hiragana" },
        { CANGJIE_CHAR_FAMILY_KATAKANA, "CANGJIE_CHAR_FAMILY_KATAKANA", "katakana" },
        { CANGJIE_CHAR_FAMILY_PUNCTUATION, "CANGJIE_CHAR_FAMILY_PUNCTUATION", "punctuation" },
        { CANGJIE_CHAR_FAMILY_SYMBOL, "CANGJIE_CHAR_FAMILY_SYMBOL", "symbol" },
        { 0, NULL, NULL }
      };
      GType g_define_type_id =
        g_flags_register_static (g_intern_static_string ("CangjieCharFamilyFlags"), values);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }

  return g_define_type_id__volatile;
}
