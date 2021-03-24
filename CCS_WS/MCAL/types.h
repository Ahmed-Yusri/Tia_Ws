/*
 * types.h
 *
 *  Created on: Aug ??, 2017
 *      Author: Ahmed Yusri Mohammed
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>
#include "Std_Types.h"

#define DIO_INPUT       			0
#define DIO_OUTPUT      			1

#ifndef STD_HIGH
#define STD_HIGH                    1
#endif

#ifndef STD_LOW
#define STD_LOW                     0
#endif

#ifndef E_OK
#define E_OK            			0
#endif

#ifndef E_NOT_OK
#define E_NOT_OK         			1
#endif

#define TRUE                        1
#define FALSE                       0

#ifndef NULL_PTR
#define NULL_PTR                   ((void*)0)
#endif

#endif /* TYPES_H_ */
