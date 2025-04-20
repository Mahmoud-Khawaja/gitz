#ifndef COMMANDS_H
#define COMMANDS_H

#include "repository.h"
#include "index.h"
#include <vector>
#include <string>

namespace gitz {

void aura(const Repository& repo, const std::vector<std::string>& paths); // git aura (add)
void hawktuah(const Repository& repo, const std::string& message); // git hawktuah (commit)
void fineshyte(const Repository& repo); // git fineshyte (diff)
void thesigma(const Repository& repo); // git thesigma (status)
void log(const Repository& repo); // git log 3ady

} 

#endif 