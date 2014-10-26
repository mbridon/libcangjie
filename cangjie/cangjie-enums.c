/* Copyright (c) 2013-2014 - The libcangjie authors.
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


#include "cangjie-enums.h"


static const GEnumValue _cangjie_orientation_values[] = {
  { CANGJIE_ORIENTATION_HORIZONTAL, "CANGJIE_ORIENTATION_HORIZONTAL", "H" },
  { CANGJIE_ORIENTATION_VERTICAL, "CANGJIE_ORIENTATION_VERTICAL", "V" },
  { CANGJIE_ORIENTATION_BOTH, "CANGJIE_ORIENTATION_BOTH", "" },
  { 0, NULL, NULL }
};

GType
cangjie_orientation_get_type (void)
{
  static GType type = 0;

  if (!type)
    type = g_enum_register_static ("CangjieOrientation", _cangjie_orientation_values);

  return type;
}


static const GEnumValue _cangjie_version_values[] = {
  { CANGJIE_VERSION_3, "CANGJIE_VERSION_3", "cj3" },
  { CANGJIE_VERSION_5, "CANGJIE_VERSION_5", "cj5" },
  { 0, NULL, NULL }
};

GType
cangjie_version_get_type (void)
{
  static GType type = 0;

  if (!type)
    type = g_enum_register_static ("CangjieVersion", _cangjie_version_values);

  return type;
}
