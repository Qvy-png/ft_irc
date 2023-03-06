/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:05:30 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/03/06 14:38:56 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

Client::Client( void ) : _buffer("") { return ; }

Client::Client( int read ) : _fd(read), _buffer("") { return ; }

Client::Client( const Client& ref ) { *this = ref; }

Client::~Client( void ) { return ; }

Client& Client::operator=( const Client& ref ) {

    if ( this == &ref )
        return ( *this );
    *this = ref;
    return ( *this );
}

void Client::setBuffer( char* str ) { _buffer = str; }

std::string Client::getBuffer( void ) { return (_buffer); }