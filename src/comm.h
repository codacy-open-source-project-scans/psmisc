/*
 * comm.h - command name length definition 
 *
 * Copyright 1995 Werner Almesberger
 * Copyright 2012-2024 Craig Small <csmall@dropbear.xyz>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef COMM_H
#define COMM_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/*
 * COMM_LEN should be the same size as TASK_COMM_LEN in the Linux source
 * at include/linux/sched.h
 */
#define COMM_LEN 64

/*
 * Older kernels had only 16 characters, which means we may have to check this
 * too
 */
#define OLD_COMM_LEN 16

#endif
