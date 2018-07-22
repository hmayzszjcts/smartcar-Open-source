#ifndef __LDPY_TYPECHANGE_H__
#define __LDPY_TYPECHANGE_H__

/*
 * Misc. Defines
 */
#ifdef	FALSE
#undef	FALSE
#endif
#define 	FALSE		(0)

#ifdef	TRUE
#undef	TRUE
#endif
#define	TRUE		(1)

#ifdef	NULL
#undef	NULL
#endif
#define 	NULL		(0)

#ifdef  	ON
#undef  	ON
#endif
#define 	ON      	(1)

#ifdef  	OFF
#undef  	OFF
#endif
#define 	OFF     	(0)

/***********************************************************************/
/*
 * The basic data types
 */
typedef unsigned char		uint8;  /*  8 bits */
typedef unsigned short int	uint16; /* 16 bits */
typedef unsigned long int	uint32; /* 32 bits */

typedef char			int8;   /*  8 bits */
typedef short int	        	int16;  /* 16 bits */
typedef int		            int32;  /* 32 bits */

typedef volatile int8		vint8;  /*  8 bits */
typedef volatile int16		vint16; /* 16 bits */
typedef volatile int32		vint32; /* 32 bits */

typedef volatile uint8		vuint8;  /*  8 bits */
typedef volatile uint16		vuint16; /* 16 bits */
typedef volatile uint32		vuint32; /* 32 bits */

typedef unsigned char   boolean;          /* 8-bit*/

#endif


/*------------------------end of LDPY_TypeChange.h-----------------------------*/
