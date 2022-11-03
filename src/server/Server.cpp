#include "../../include/xchat/Server.hpp"

void Server::SaveMessageCommand(uint32_t command, uint32_t second_command) {
    SavePrimary(command);
    SaveSecondary(second_command);
}
    
void Server::GetMessageCommand(uint32_t* command, uint32_t* second_command) {
    *command = GetPrimary();
    *second_command = GetSecondary();
}