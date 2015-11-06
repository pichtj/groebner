#ifndef DEBUG_H
#define DEBUG_H

#if (defined DEBUG || defined INFO)
#define I(statement) std::cout << __func__ << "[" << __LINE__ << "]: " << statement << std::endl;
#define II(statement, container) std::cout << __func__ << "[" << __LINE__ << "]: " << statement; print(std::string(__func__) + "[" + to_string(__LINE__) + "]: ", container);
#else
#define I(statement) ;
#define II(statement, container) ;
#endif // (defined DEBUG || defined INFO)

#ifdef DEBUG
#define D(statement) I(statement)
#define DD(statement, container) II(statement, container)
#else
#define D(statement) ;
#define DD(statement, container) ;
#endif // DEBUG

#endif // DEBUG_H
