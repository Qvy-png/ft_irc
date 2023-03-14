/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:05:30 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/03/14 21:01:22 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

Client::Client( void ) : _hs( false ), _buffer("") { return ; }

Client::Client( int read ) :  _fd(read), _hs( false ), _buffer("")  { return ; }

Client::Client( const Client& ref ) { *this = ref; }

Client::~Client( void ) { return ; }

void    Client::printCanals(void) {
    std::cout << "Canal of " << _nick << std::endl;
    for (std::vector<Canal *>::iterator it = _canals.begin(); it != _canals.end(); it++)
    {
        Canal *tmp = (*it);
        std::cout << tmp->getName() << std::endl;
    }
}

Client& Client::operator=( const Client& ref ) {

    if ( this == &ref )
        return ( *this );
    *this = ref;
    return ( *this );
}

int Client::getFd( void ) { return( _fd ); }

void Client::setPass( std::string str ) { _pass = str; }

std::string Client::getPass( void ) { return( _pass ); }

void Client::setNick( std::string str ) { _nick = str; }

std::string Client::getNick( void ) { return( _nick ); }

void Client::setFullName( std::string str ) { _fullName = str; }

std::string Client::getFullName( void ) { return( _fullName ); }

void Client::setHost( std::string str ) { _host = str; }

std::string Client::getHost( void ) { return( _host ); }

void Client::setBuffer( char* str ) { _buffer = str; } // ajouter le buffer si non vide

void Client::addBuffer( char* str ) { _buffer += str; } // ajouter le buffer si non vide

std::string Client::getBuffer( void ) { return ( _buffer ); }

void Client::setHs( bool hs ) { _hs = hs; }

bool Client::getHs( void ) { return ( _hs ); }

bool Client::isInCanal(std::string canalName) {
    for (std::vector<Canal *>::iterator it = _canals.begin(); it != _canals.end(); it++) {
        Canal *tmp = *it;
        if (tmp->getName() == canalName)
            return (true);
    }
    return (false);
}

void Client::exitCanal(std::string canalName)
{
    for (std::vector<Canal *>::iterator it = _canals.begin(); it != _canals.end(); it++) {
        Canal *tmp = *it;
        if (tmp->getName() == canalName)
        {
            _canals.erase(it);
            return ;
        }
    }
}

Canal   *Client::getCanal(std::string canalName)
{
    for (std::vector<Canal *>::iterator it = _canals.begin(); it != _canals.end(); it++) {
        Canal *tmp = *it;
        if (tmp->getName() == canalName)
            return (tmp);
    }
    return (NULL);
}
