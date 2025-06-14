#ifndef __CS__ATMO_VARIABLES_H__
#define __CS__ATMO_VARIABLES_H__

/*============================================================================
 * Functions relative to atmospheric model fields.
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

#include "base/cs_defs.h"

/*----------------------------------------------------------------------------*/

BEGIN_C_DECLS

/*----------------------------------------------------------------------------
 * Add atmospheric variable fields
 *----------------------------------------------------------------------------*/

void
cs_atmo_add_variable_fields(void);

/*----------------------------------------------------------------------------
 * Add atmospheric property fields
 *----------------------------------------------------------------------------*/

void
cs_atmo_add_property_fields(void);

/*----------------------------------------------------------------------------*/
/*!
 * \brief Update the thermo physical properties fields for the humid air and
 *        the liquid \n
 *        Remarques :
 *        This function  is called at the beginning of each time step
 */
/*----------------------------------------------------------------------------*/

void
cs_atmo_physical_properties_update(void);

/*----------------------------------------------------------------------------*/
/*!
 * \brief Initialisation of variable options for the atmospheric module
 *        before what is done in cs_user_parameters functions
 */
/*----------------------------------------------------------------------------*/

void
cs_atmo_init_variables_1(void);

/*----------------------------------------------------------------------------*/
/*!
 * \brief Initialisation of variable options for the atmospheric module
 *        after what is done in cs_user_parameters functions
 */
/*----------------------------------------------------------------------------*/

void
cs_atmo_init_variables_2(void);

/*----------------------------------------------------------------------------*/

END_C_DECLS

#endif /* __CS_ATMO_VARIABLES_H__ */
