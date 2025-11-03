/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#ifdef VIA_ENABLE
/* VIA configuration. */
#define DYNAMIC_KEYMAP_LAYER_COUNT 10
#define DYNAMIC_KEYMAP_MACRO_COUNT 32

#ifdef VIAL_ENABLE
/** Vial-specific configuration. */

#define VIAL_KEYBOARD_UID { 0x6D, 0xA5, 0xCD, 0x8D, 0xC7, 0x3D, 0x7B, 0xA8 }
#endif // VIAL_ENABLE
#endif     // VIA_ENABLE


#define RP2040_FLASH_GD25Q64CS

//W25Q080	Selected by default  默认选中
//AT25SF128A	#define RP2040_FLASH_AT25SF128A
//GD25Q64CS	#define RP2040_FLASH_GD25Q64CS
//W25X10CL	#define RP2040_FLASH_W25X10CL
//IS25LP080	#define RP2040_FLASH_IS25LP080
//Generic 03H flash  通用 03H 闪存	#define RP2040_FLASH_GENERIC_03H