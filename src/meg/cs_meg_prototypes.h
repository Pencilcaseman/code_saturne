#ifndef __CS_MEG_PROTOTYPES_H__
#define __CS_MEG_PROTOTYPES_H__

/*============================================================================
 * Prototypes for MEG (Mathematical Expression Generator) functions
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

/*----------------------------------------------------------------------------
 *  Local headers
 *----------------------------------------------------------------------------*/

#include "base/cs_base.h"
#include "base/cs_field.h"
#include "base/cs_volume_zone.h"
#include "base/cs_boundary_zone.h"
#include "base/cs_ibm.h"

/*----------------------------------------------------------------------------*/

BEGIN_C_DECLS

typedef void
(cs_ibm_volume_func_t)(const cs_lnum_t    c_id,
                       const cs_real_3_t  xyz,
                       const cs_real_t    t,
                       cs_real_t         *retval);

typedef void
(cs_ibm_fsi_func_t)(cs_real_t  *retval);

/*============================================================================
 * MEG function prototypes
 *============================================================================*/

/*----------------------------------------------------------------------------*/
/*!
 * \brief This function is used to compute user defined values for fields over
 *        a given boundary zone. The mathematical expression is defined in the
 *        GUI.
 *
 * \param[in]  zone_name    name of a boundary zone
 * \param[in]  n_elts       number of elements related to the zone
 * \param[in]  elt_ids      list of element ids related to the zone
 * \param[in]  xyz          list of coordinates related to the zone
 * \param[in]  field_name   name of the variable field
 * \param[in]  condition    condition type defined as a string
 * \param[out] retvals      array of computed values
 */
/*----------------------------------------------------------------------------*/

void
cs_meg_boundary_function(const char       *zone_name,
                         const cs_lnum_t   n_elts,
                         const cs_lnum_t  *elt_ids,
                         const cs_real_t   xyz[][3],
                         const char       *field_name,
                         const char       *condition,
                         cs_real_t        *retvals);

/*----------------------------------------------------------------------------*/
/*!
 * \brief This function is used to compute user defined values for fields over
 *        a given volume zone. The mathematical expression is defined in the
 *        GUI.
 *
 * \param[in]      zone_name  name of a volume zone
 * \param[in]      n_elts     number of elements related to the zone
 * \param[in]      elt_ids    list of element ids related to the zone
 * \param[in]      xyz        list of coordinates related to the zone
 * \param[in, out] f          array of pointers to cs_field_t structures
 */
/*----------------------------------------------------------------------------*/

void
cs_meg_volume_function(const char        *zone_name,
                       const cs_lnum_t    n_elts,
                       const cs_lnum_t   *elt_ids,
                       const cs_real_t    xyz[][3],
                       const char        *fields_names,
                       cs_real_t         *fvals[]);

/*----------------------------------------------------------------------------*/
/*!
 * \brief This function is used for the initalization of fields over a given
 *        volume zone. The mathematical expression is defined in the GUI.
 *
 * The caller is responsible for freeing the associated array.
 *
 * \param[in]  zone_name    name of a volume zone
 * \param[in]  n_elts       number of elements related to the zone
 * \param[in]  elt_ids      list of element ids related to the zone
 * \param[in]  xyz          list of coordinates related to the zone
 * \param[in]  field_name   associated variable field name
 * \param[out] retvals      array of computed values
 *
 */
/*----------------------------------------------------------------------------*/

void
cs_meg_initialization(const char       *zone_name,
                      const cs_lnum_t   n_elts,
                      const cs_lnum_t  *elt_ids,
                      const cs_real_t   xyz[][3],
                      const char       *field_name,
                      cs_real_t        *retvals);

/*----------------------------------------------------------------------------*/
/*!
 * \brief This function is used to compute source terms over a volume zone. The
 *        mathematical expression is defined in the GUI.
 *
 * The caller is responsible for freeing the returned array.
 *
 * \param[in]  zone_name     name of a volume zone
 * \param[in]  n_elts        number of elements related to the zone
 * \param[in]  elt_ids       list of element ids related to the zone
 * \param[in]  xyz           list of coordinates related to the zone
 * \param[in]  field_name    variable field name
 * \param[in]  source_type   source term type
 * \param[out] retvals       array of computed values
 *
 */
/*----------------------------------------------------------------------------*/

void
cs_meg_source_terms(const char       *zone_name,
                    const cs_lnum_t   n_elts,
                    const cs_lnum_t  *elt_ids,
                    const cs_real_t   xyz[][3],
                    const char       *name,
                    const char       *source_type,
                    cs_real_t        *retvals);

/*----------------------------------------------------------------------------*/
/*!
 * \brief This function is used to query FSI internal coupling structure values
 *        for a given boundary and structure.
 *
 * \param[in]      object_type   name of object type
 * \param[in]      name          name of matching boundary
 * \param[in]      fluid_f       array of fluid forces on the object
 * \param[in, out] val[]         matrix or vector coefficients
 */
/*----------------------------------------------------------------------------*/

void
cs_meg_fsi_struct(const char       *object_type,
                  const char       *name,
                  const cs_real_t   fluid_f[],
                  cs_real_t         val[]);

/*----------------------------------------------------------------------------*/
/*!
 * \brief This function is used to activate postprocessing writers.
 */
/*----------------------------------------------------------------------------*/

void
cs_meg_post_activate(void);

/*----------------------------------------------------------------------------*/
/*!
 * \brief This function is used to define profile coordinates.
 *
 * \param[in]      name          name of matching profile
 * \param[in]      n_coords      number of point coordinates
 * \param[in, out] coords        point coordinates
 */
/*----------------------------------------------------------------------------*/

void
cs_meg_post_profiles(const char   *name,
                     int           n_coords,
                     cs_real_t     coords[][3]);

/*----------------------------------------------------------------------------*/
/*!
 * \brief This function is used to compute user defined calculator formulae.
 *        The mathematical expression is defined in the GUI.
 *
 * \param[in]  field_name   function name
 * \param[in]  n_elts       number of elements related to the zone
 * \param[in]  elt_ids      list of element ids related to the zone
 * \param[in]  xyz          list of coordinates related to the zone
 * \param[out] retvals      array of computed values
 */
/*----------------------------------------------------------------------------*/

void
cs_meg_post_calculator(const char       *name,
                       const cs_lnum_t   n_elts,
                       const cs_lnum_t  *elt_ids,
                       const cs_real_t   xyz[][3],
                       cs_real_t        *retvals);

/*----------------------------------------------------------------------------*/
/*!
 * \brief This function is used to get the cs_cutcell_func_t pointer for a
 * given object.
 *
 * \return pointer to corresponding function.
 */
/*----------------------------------------------------------------------------*/

cs_cutcell_func_t *
cs_meg_ibm_func_by_name
(
  const char *object_name /*!<[in] name of the object */
);

/*----------------------------------------------------------------------------*/
/*!
 *
 * \brief This function is used to get the cs_ibm_volume_func_t pointer for a
 *        given object
 *
 * \return pointer to corresponding function.
 */
/*----------------------------------------------------------------------------*/

cs_ibm_volume_func_t *
cs_meg_ibm_volume_func_by_name(const char *object_name,
                               const char *gui_var_name);

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*!
 *
 * \brief This function is used to get the cs_ibm_fsi_func_t pointer for a
 *        given object
 *
 * \return pointer to corresponding function.
 */
/*----------------------------------------------------------------------------*/

cs_ibm_fsi_func_t *
cs_meg_ibm_fsi_func_by_name(const char *object_name,
                            const char *gui_var_name);

/*----------------------------------------------------------------------------*/

END_C_DECLS

#endif /* __CS_MEG_PROTOTYPES_H__ */
