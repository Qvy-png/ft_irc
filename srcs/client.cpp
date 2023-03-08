/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:05:30 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/03/08 17:00:34 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

Client::Client( void ) : _hs( false ), _buffer("") { return ; }

Client::Client( int read ) :  _fd(read), _hs( false ), _buffer("") { return ; }

Client::Client( const Client& ref ) { *this = ref; }

Client::~Client( void ) { return ; }

Client& Client::operator=( const Client& ref ) {

    if ( this == &ref )
        return ( *this );
    *this = ref;
    return ( *this );
}

void Client::setPass( std::string str ) { _pass = str; }

std::string Client::getPass( void ) { return( _pass ); }

void Client::setNick( std::string str ) { _nick = str; }

std::string Client::getNick( void ) { return( _nick ); }

void Client::setUser( std::string str ) { _user = str; }

std::string Client::getUser( void ) { return( _user ); }

void Client::setBuffer( char* str ) { _buffer = str; }

std::string Client::getBuffer( void ) { return ( _buffer ); }

void Client::setHs( bool hs ) { _hs = hs; }

bool Client::getHs( void ) { return ( _hs ); }