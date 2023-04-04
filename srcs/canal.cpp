/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canal.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 17:39:26 by dasereno          #+#    #+#             */
/*   Updated: 2023/04/04 15:21:20 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/canal.hpp"

Canal& Canal::operator=( const Canal& ref ) {

    if ( this == &ref )
        return ( *this );
    *this = ref;
    return ( *this );
}

bool Canal::operator==( const Canal& rhs ) {
	return (this->_name == rhs.getName());
}

void    Canal::deleteClient(Client *client)  {
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
        Client *cli = (*it);
        if (cli->getNick() == client->getNick())
            clients.erase(it);
    }
    if (clients.size() == 0) {
        _canalManager->DeleteChannel(this);
    }
}

bool    Canal::hasClient(Client *client)  {
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
        Client *cli = (*it);
        if (cli->getNick() == client->getNick())
            return true;
    }
    return false;
}

bool    Canal::hasClient(std::string nick)  {
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
        Client *cli = (*it);
        if (cli->getNick() == nick)
            return true;
    }
    return false;
}

Client *Canal::getClient(std::string name) {
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
        Client *tmp = *it;
        if (tmp->getNick() == name)
            return (tmp);
    }
    return (NULL);
}

void    Canal::banClient(std::string name) {
    if (this->hasClient(name)) {
        this->deleteClient(this->getClient(name));
    }
}

void    Canal::printBanned(void) {
    std::vector<Client *>::iterator it = _banned.begin();

    for (; it != _banned.end(); it++) {
        Client *tmp = (*it);
        std::cout << tmp->getNick() << std::endl;
    }    
}