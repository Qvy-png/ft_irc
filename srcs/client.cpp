/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:05:30 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/04/16 19:52:41 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

Client::Client(void) : _hs(false), _buffer(""), _hasTime(false), _modeI(false) { return; }

Client::Client(int read) : _fd(read), _hs(false), _buffer(""), _hasTime(false), _modeI(false) { return; }

Client::Client(const Client &ref) { *this = ref; }

Client::~Client(void)
{
    _canals.clear();
    _waitingMessages.clear();
    return;
}

Client &Client::operator=(const Client &ref)
{

    if (this == &ref)
        return (*this);
    *this = ref;
    return (*this);
}

int Client::getFd(void) { return (_fd); }

void Client::setPass(std::string str) { _pass = str; }

std::string Client::getPass(void) { return (_pass); }

void Client::setNick(std::string str) { _nick = str; }

std::string Client::getNick(void) { return (_nick); }

void Client::setFullName(std::string str) { _fullName = str; }

std::string Client::getFullName(void) { return (_fullName); }

void Client::setHost(std::string str) { _host = str; }

std::string Client::getHost(void) { return (_host); }

void Client::setBuffer(char *str) { _buffer = str; } // ajouter le buffer si non vide

void Client::addBuffer(char *str) {
    std::string tmp = _buffer;
    _buffer.clear() ;
    _buffer = tmp + str;
    tmp.clear();
 } // ajouter le buffer si non vide

std::string Client::getBuffer(void) { return (_buffer); }

void Client::setHs(bool hs) { _hs = hs; }

bool Client::getHs(void) { return (_hs); }

void Client::setTime(std::time_t time) { _time = time; }

std::time_t Client::getTime(void) { return (_time); }

void Client::setHasTime(bool hasTime) { _hasTime = hasTime; }

bool Client::getHasTime(void) { return (_hasTime); }