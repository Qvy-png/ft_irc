/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canal.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 17:39:26 by dasereno          #+#    #+#             */
/*   Updated: 2023/04/11 17:50:19 by dasereno         ###   ########.fr       */
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
        if (cli->getNick() == client->getNick()) {
            clients.erase(it);
            break ;
        }
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

void	Canal::broadcast(std::string msg) {
	std::vector<Client *>::iterator it = this->clients.begin();
	for (; it != this->clients.end(); it++) {
		Client *tmp = (*it);
	    send(tmp->getFd(), msg.c_str(), msg.length(), MSG_CONFIRM);
	}
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
    if (!isBanned(name))
        this->pushBanned(name);
}

bool    Canal::isBanned(Client *client) {
    std::vector<std::string>::iterator	it = _banned.begin();
    for (; it != _banned.end(); it++) {
        std::string tmp = (*it);
        if (tmp == client->getNick())
            return true;
    }
    return false;
}

bool    Canal::isBanned(std::string client) {
    std::vector<std::string>::iterator	it = _banned.begin();
    for (; it != _banned.end(); it++) {
        std::string tmp = (*it);
        if (tmp == client)
            return true;
    }
    return false;
}

void    Canal::printBanned(void) {
    std::vector<std::string >::iterator it = _banned.begin();

    for (; it != _banned.end(); it++) {
        std::string tmp = (*it);
        std::cout << tmp << std::endl;
    }    
}

bool	Canal::isOp( std::string name ) { 
    std::vector<Client *>::iterator	it = _chanops.begin();
    if (name == getOp().getNick())
        return true;
    for (; it != _chanops.end(); it++) {
        Client *tmp = (*it);
        if (tmp != NULL && tmp->getNick() == name)
            return true;
    }
    return false;
}

bool	Canal::isOp( Client *client ) { 
    std::vector<Client *>::iterator	it = _chanops.begin();
    if (client->getNick() == getOp().getNick())
        return true;
    for (; it != _chanops.end(); it++) {
        Client *tmp = (*it);
        if (tmp != NULL && tmp->getNick() == client->getNick())
            return true;
    }
    return false;
}

bool	Canal::isVoiced( std::string name ) { 
    std::vector<Client *>::iterator	it = _voiced.begin();
    for (; it != _voiced.end(); it++) {
        Client *tmp = (*it);
        if (tmp != NULL && tmp->getNick() == name)
            return true;
    }
    return false;
}

bool	Canal::isVoiced( Client *client ) { 
    std::vector<Client *>::iterator	it = _voiced.begin();
    for (; it != _voiced.end(); it++) {
        Client *tmp = (*it);
        if (tmp != NULL && tmp->getNick() == client->getNick())
            return true;
    }
    return false;
}

void	Canal::delOp( Client *client ) {
    std::vector<Client *>::iterator	it = _chanops.begin();
    for (; it != _chanops.end(); it++) {
        Client *tmp = (*it);
        if (tmp != NULL && tmp->getNick() == client->getNick()) {
            _chanops.erase(it);
            break ;
        }
    }
}

void	Canal::delVoiced( Client *client ) {
    std::vector<Client *>::iterator	it = _voiced.begin();
    for (; it != _voiced.end(); it++) {
        Client *tmp = (*it);
        if (tmp != NULL && tmp->getNick() == client->getNick()) {
            _voiced.erase(it);
            break ;
        }
    }
}


void	Canal::delBanned( Client *client ) {
    std::vector<std::string >::iterator	it = _banned.begin();
    for (; it != _banned.end(); it++) {
        std::string tmp = (*it);
        if (tmp == client->getNick()) {
            _banned.erase(it);
            break ;
        }
    }
}

void	Canal::delBanned( std::string client ) {
    std::vector<std::string >::iterator	it = _banned.begin();
    for (; it != _banned.end(); it++) {
        std::string tmp = (*it);
        if (tmp == client) {
            _banned.erase(it);
            break ;
        }
    }
}