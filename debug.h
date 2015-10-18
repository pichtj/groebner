#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define D(statement) std::cout << __func__ << "[" << __LINE__ << "]: " << statement << std::endl;
#define DD(statement, container) std::cout << __func__ << "[" << __LINE__ << "]: " << statement; print(string(__func__) + "[" + to_string(__LINE__) + "]: ", container);
#else
#define D(statement) ;
#define DD(statement, container) ;
#endif // DEBUG

#endif // DEBUG_H
