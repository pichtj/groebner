#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define D(statement) statement;
#else
#define D(statement) ;
#endif // DEBUG

#endif // DEBUG_H
