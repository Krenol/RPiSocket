#include <exception>
#include <string>

#ifndef RPISOCKET_EXCEPTIONS_H
#define RPISOCKET_EXCEPTIONS_H

class socket_exception: public std::exception
{
public:
    /** Constructor 
     *  @param message The error message.
     */
    socket_exception(const std::string& message):
      msg_(message)
      {}


    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *          is in posession of the Exception object. Callers must
     *          not attempt to free the memory.
     */
    virtual const char* what() const throw (){
       return msg_.c_str();
    }

protected:
    /** Error message.
     */
    std::string msg_;
};

#endif
