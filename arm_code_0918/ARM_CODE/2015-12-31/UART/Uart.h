
#ifndef _UART_H
#define _UART_H

typedef char *va_list;

/*
 * Storage alignment properties
 */
#define  _AUPBND                (sizeof (long) - 1)
#define  _ADNBND                (sizeof (long) - 1)

/*
 * Variable argument list macro definitions
 */
#define _bnd(X, bnd)            (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T)           (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap)              (void) 0
#define va_start(ap, A)         (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))




#endif  //_UART_H
