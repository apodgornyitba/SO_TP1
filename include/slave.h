#ifndef SLAVE_H
#define SLAVE_H

#include "library.h"

#define COMMAND_ERROR "Minisat Error, couldn't solve file."
#define COMMAND_FORMAT "minisat %s | /bin/grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\""

#endif