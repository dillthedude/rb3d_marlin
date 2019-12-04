/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "MarlinConfig.h"

#if ENABLED(MESH_BED_LEVELING) || ENABLED(AUTO_BED_LEVELING_BILINEAR)

  #include "mesh_bed_leveling.h"
  #include "Marlin.h"
  #include "serial.h"

  mesh_bed_leveling mbl;

  float mesh_bed_leveling::z_offset,
        mesh_bed_leveling::z_values[GRID_MAX_POINTS_X_MANUAL][GRID_MAX_POINTS_Y_MANUAL],
        mesh_bed_leveling::index_to_xpos[GRID_MAX_POINTS_X_MANUAL],
        mesh_bed_leveling::index_to_ypos[GRID_MAX_POINTS_Y_MANUAL];

  mesh_bed_leveling::mesh_bed_leveling() {
    for (uint8_t i = 0; i < GRID_MAX_POINTS_X_MANUAL; ++i)
      index_to_xpos[i] = MESH_MIN_X + i * (MESH_X_DIST);
    for (uint8_t i = 0; i < GRID_MAX_POINTS_Y_MANUAL; ++i)
      index_to_ypos[i] = MESH_MIN_Y + i * (MESH_Y_DIST);
    reset();
  }

  void mesh_bed_leveling::reset() {
    z_offset = 0;
    ZERO(z_values);
  }

  // This function reports the saved mesh to the terminal program, like Pronterface. We don't save mesh data with MESH_BED_LEVELING, so we never call this function anyway.
  void mesh_bed_leveling::report_mesh() {
    SERIAL_PROTOCOLLNPGM("Num X,Y: " STRINGIFY(GRID_MAX_POINTS_X_MANUAL) "," STRINGIFY(GRID_MAX_POINTS_Y_MANUAL));
    SERIAL_PROTOCOLPGM("Z offset: "); SERIAL_PROTOCOL_F(z_offset, 5);
    SERIAL_PROTOCOLLNPGM("\nMeasured points:");
    print_2d_array(GRID_MAX_POINTS_X_MANUAL, GRID_MAX_POINTS_Y_MANUAL, 5,
      [](const uint8_t ix, const uint8_t iy) { return z_values[ix][iy]; }
    );
  }

#endif // MESH_BED_LEVELING
