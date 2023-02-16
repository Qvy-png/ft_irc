/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 14:12:48 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/02/03 14:24:49 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

Server::Server( void ) : _port(), _password() { }

Server::Server( int port, std::string password ) : _port( port ), _password( password ) { return ; }

Server::Server( const Server& ref ) { *this = ref; }

Server::~Server( void ) { return ;}

Server& Server::operator=( const Server& ref ) {

    if ( this == &ref )
        return ( *this );
    *this = ref;
    return ( *this );
}

void    Server::setPort( int port ) { _port = port; }

void    Server::setPassword( std::string str ) { _password = str; }

int Server::getPort( void ) const { return ( _port ); }

std::string Server::getPassword( void ) const { return ( _password ); }

