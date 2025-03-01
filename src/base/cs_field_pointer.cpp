/*============================================================================
 * Field pointers and ids for standard and model fields
 *============================================================================*/

/*
  This file is part of code_saturne, a general-purpose CFD tool.

  Copyright (C) 1998-2025 EDF S.A.

  This program is free software; you can redistribute it and/or modify it under
  the terms of the GNU General Public License as published by the Free Software
  Foundation; either version 2 of the License, or (at your option) any later
  version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
  details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
  Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

/*----------------------------------------------------------------------------*/

#include "base/cs_defs.h"

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Standard C library headers
 *----------------------------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------------------------------------------------------------------------
 * Local headers
 *----------------------------------------------------------------------------*/

#include "bft/bft_error.h"
#include "bft/bft_printf.h"

#include "base/cs_field.h"
#include "base/cs_mem.h"

/*----------------------------------------------------------------------------
 * Header for the current file
 *----------------------------------------------------------------------------*/

#include "base/cs_field_pointer.h"

/*----------------------------------------------------------------------------*/

BEGIN_C_DECLS

/*=============================================================================
 * Additional doxygen documentation
 *============================================================================*/

/*!
  \file cs_field_pointer.cpp
        Field pointers and ids for standard and model fields.
*/

/*! \fn CS_ENUMF_(e)
 * \brief Macro used for scoping of field pointer enums.
 *
 * This macro replaces CS_ENUMF_ by CS_FIELD_POINTER_ and allows
 * to rebuild a full enumerated field pointer id.
 *
 * \param [in] e suffix of enumerated field pointer id.
 */

/*! \fn CS_F_(e)
 * \brief Macro used to return a field pointer by its enumerated value.
 *
 * This macro replaces CS_F_ by an access to the global array of field pointers
 * \ref cs_glob_field_pointers using a rebuilt enumerated field pointer id.
 *
 * \param [in] e suffix of enumerated field pointer id.
 */

/*! \fn CS_FI_(e, i)
 * \brief Macro used to return a field pointer by its enumerated value.
 *
 * This macro replaces CS_FI_ by an access to the global array of field pointers
 * \ref cs_glob_field_pointers using a rebuilt enumerated field pointer id and
 * its field sublist index.
 *
 * \param [in] e suffix of enumerated field pointer id.
 * \param [in] i field enumerator value.
 */

/*! \cond DOXYGEN_SHOULD_SKIP_THIS */

/*=============================================================================
 * Local macro definitions
 *============================================================================*/

/*============================================================================
 * Type definitions
 *============================================================================*/

/*============================================================================
 * Static global variables
 *============================================================================*/

/* Number of pointers (initially fixed, but extensible in case
   user fields should be added after the model fields) */

static int                               _n_pointers = 0;
static struct cs_field_pointer_array_t  *_field_pointer = nullptr;

/* Handling of sublists */

static short int  *_sublist_size = nullptr;

/*! (DOXYGEN_SHOULD_SKIP_THIS) \endcond */

/*============================================================================
 * Global variables
 *============================================================================*/

/* Pointers */

struct cs_field_pointer_array_t  *cs_glob_field_pointers = nullptr;

/*! \cond DOXYGEN_SHOULD_SKIP_THIS */

/*============================================================================
 * Private function definitions
 *============================================================================*/

/*----------------------------------------------------------------------------
 * Initialize field pointers
 *----------------------------------------------------------------------------*/

static void
_init_pointers(void)
{
  assert(_field_pointer == nullptr);

  _n_pointers = CS_FIELD_N_POINTERS;
  CS_MALLOC(_field_pointer, _n_pointers, struct cs_field_pointer_array_t);
  CS_MALLOC(_sublist_size, _n_pointers, short int);

  for (int i = 0; i < _n_pointers; i++) {
    _field_pointer[i].f = nullptr;
    _field_pointer[i].p = &(_field_pointer[i].f);
    _sublist_size[i] = 0;
  }

  cs_glob_field_pointers = _field_pointer;
}

/*! (DOXYGEN_SHOULD_SKIP_THIS) \endcond */

/*=============================================================================
 * Public function definitions
 *============================================================================*/

/*----------------------------------------------------------------------------*/
/*!
 * \brief Ensure field pointer array is initialized.
 */
/*----------------------------------------------------------------------------*/

void
cs_field_pointer_ensure_init(void)
{
  if (_field_pointer == nullptr)
    _init_pointers();
}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Free all field pointer data.
 */
/*----------------------------------------------------------------------------*/

void
cs_field_pointer_destroy_all(void)
{
  for (int i = 0; i < _n_pointers; i++) {
    if (_sublist_size[i] > 1)
      CS_FREE(_field_pointer[i].p);
  }
  CS_FREE(_field_pointer);
  CS_FREE(_sublist_size);

  cs_glob_field_pointers = nullptr;
}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Map a simple field to an enumerated pointer.
 *
 * The associated field pointer may then be retreived using \ref CS_F_(e).
 *
 * \param[in]  e   field enumerator value
 * \param[in]  f   pointer to field structure
 */
/*----------------------------------------------------------------------------*/

void
cs_field_pointer_map(cs_field_pointer_id_t   e,
                     cs_field_t             *f)
{
  cs_field_pointer_map_indexed(e, 0, f);
}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Map a field to an (enumerated pointer, index) couple.
 *
 * This sort of mapping may be used for sets of fields whose size
 * is not known in advance.
 *
 * The associated field pointer may then be retreived using \ref CS_FI_(e, i).
 *
 * \param[in]  e      field enumerator value
 * \param[in]  index  field enumerator index
 * \param[in]  f      pointer to field structure
 */
/*----------------------------------------------------------------------------*/

void
cs_field_pointer_map_indexed(cs_field_pointer_id_t   e,
                             int                     index,
                             cs_field_t             *f)
{
  assert(index >= 0);

  if (_field_pointer == nullptr)
    _init_pointers();

  assert(e < _n_pointers);

  if (index == 0 && _sublist_size[e] <= 1) {
    _field_pointer[e].f = f;
    _sublist_size[e] = 1;
  }

  else {
    if (_sublist_size[e] <= index) {

      int n_sub = index+1;

      if (_field_pointer[e].p == &(_field_pointer[e].f))
        CS_MALLOC(_field_pointer[e].p, n_sub, cs_field_t *);
      else
        CS_REALLOC(_field_pointer[e].p, n_sub, cs_field_t *);
      _field_pointer[e].p[0] = _field_pointer[e].f;

      for (int i = _sublist_size[e]; i < n_sub; i++)
        _field_pointer[e].p[i] = nullptr;

      _sublist_size[e] = n_sub;

    }

    _field_pointer[e].p[index] = f;
  }
}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Map base fields to enumerated pointers.
 */
/*----------------------------------------------------------------------------*/

void
cs_field_pointer_map_base(void)
{
  cs_field_pointer_map(CS_ENUMF_(dt),
                       cs_field_by_name_try("dt"));

  cs_field_pointer_map(CS_ENUMF_(hybrid_blend),
                       cs_field_by_name_try("hybrid_blend"));

  cs_field_pointer_map(CS_ENUMF_(h),
                       cs_field_by_name_try("enthalpy"));
  cs_field_pointer_map(CS_ENUMF_(t),
                       cs_field_by_name_try("temperature"));

  cs_field_pointer_map(CS_ENUMF_(cp),
                       cs_field_by_name_try("specific_heat"));

  cs_field_pointer_map(CS_ENUMF_(lambda),
                       cs_field_by_name_try("thermal_conductivity"));
  cs_field_pointer_map(CS_ENUMF_(th_diff),
                       cs_field_by_name_try("thermal_diffusivity"));

  cs_field_pointer_map(CS_ENUMF_(vism),
                       cs_field_by_name_try("mesh_viscosity"));

  cs_field_pointer_map(CS_ENUMF_(poro),
                       cs_field_by_name_try("porosity"));
  cs_field_pointer_map(CS_ENUMF_(t_poro),
                       cs_field_by_name_try("tensorial_porosity"));
}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Map some boundary fields to enumerated pointers.
 */
/*----------------------------------------------------------------------------*/

void
cs_field_pointer_map_boundary(void)
{
  cs_field_pointer_map(CS_ENUMF_(t_b),
                       cs_field_by_name_try("boundary_temperature"));
}

/*----------------------------------------------------------------------------*/
/*!
 * \brief Map base fields to enumerated pointers for atmospheric models
 *
 * \param[in]  n_chem_species  number of chemical species
 * \param[in]  species_f_id    field id for each chemical species
 */
/*----------------------------------------------------------------------------*/

void
cs_field_pointer_map_atmospheric(int        n_chem_species,
                                 const int  species_f_id[])
{
  cs_field_pointer_map(CS_ENUMF_(pot_t),
                       cs_field_by_name_try("temperature"));

  cs_field_pointer_map(CS_ENUMF_(ym_w),
                       cs_field_by_name_try("ym_water"));
  cs_field_pointer_map(CS_ENUMF_(ntdrp),
                       cs_field_by_name_try("number_of_droplets"));

  for (int i = 0; i < n_chem_species; i++)
    cs_field_pointer_map_indexed(CS_ENUMF_(chemistry),
                                 i,
                                 cs_field_by_id(species_f_id[i]));
}

/*----------------------------------------------------------------------------*/

END_C_DECLS
