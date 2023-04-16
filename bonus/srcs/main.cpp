/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:36:41 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/04/16 18:24:28 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"
bool							quit = true;

int		printErr( std::string str ) { std::cerr << REDHB << str << CRESET; return ( 1 ); }

int	checkOnlyNum( std::string str ) {

	if ( str.empty() )
		return ( -1 );
	else {

		int i = 0;

		while ( str[i] ) {
		
			if ( str[i] > '9' || str[i] < '0' )
				return ( -1 );
			i++;
		}
	}
	return ( 0 );
}

bool isValidNickname(const std::string& nickname) {
    if (nickname.length() < 1 || nickname.length() > 9) {
        return false;
    }
    
    if (!std::isalpha(nickname[0]) && nickname[0] != '[' && nickname[0] != ']' &&
        nickname[0] != '\\' && nickname[0] != '`' && nickname[0] != '^' && nickname[0] != '{' &&
        nickname[0] != '|') {
        return false;
    }
    
    for (size_t i = 1; i < nickname.length(); i++) {
        if ((!std::isalnum(nickname[i])&& nickname[i] != '-' && nickname[i] != '[' &&
            nickname[i] != ']' && nickname[i] != '\\' && nickname[i] != '`' && nickname[i] != '^' &&
            nickname[i] != '{' && nickname[i] != '|' && nickname[i] != '}') || std::isspace(nickname[i])) {
            return false;
        }
    }
	return true;
}

bool isValidChannelName(const std::string& channelName)
{
    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&')) {
        return false;
    }
    if (channelName.size() < 1 || channelName.size() > 200) {
        return false;
    }
    char secondChar = channelName[1];
    if (!((secondChar >= 'a' && secondChar <= 'z') || (secondChar >= 'A' && secondChar <= 'Z') || secondChar == '#' || secondChar == '&' || secondChar == '+' || secondChar == '-')) {
        return false;
    }
    for (size_t i = 2; i < channelName.size(); i++) {
        char c = channelName[i];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '.' || c == '_' || c == '~' || c == '@' || c == '#' || c == '&')) {
            return false;
        }
    }
    if (std::find_if(channelName.begin(), channelName.end(), ::isspace) != channelName.end()) {
        return false;
    }

	return true;
}

bool validateMessage(const std::string& message) {
    if (message.length() > 512) {
        return false;
    }
    for (size_t i = 0; i < message.length(); ++i) {
        char c = message[i];
        if ((c >= 0 && c <= 31) || c == 127) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

bool	str_isnum(std::string str) {
	for (int i = 0; str[i] ; i++) {
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}

	Server	*server = NULL;

int	main( int argc, char **argv ) {
	if ( argc != 3 )
		return ( printErr( "Wrong input, please use the following form : ./ircserv <int>port <string>password\n" ) );
	if ( checkOnlyNum( argv[1] ) == -1 )
		return ( printErr( "Wrong port format, please make sure only numbers\n" ) );
	
	server = new Server(atoi(argv[1]), argv[2]);
	server->init();
	server->start();
}