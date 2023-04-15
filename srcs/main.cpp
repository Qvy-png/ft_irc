/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:36:41 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/04/13 17:17:09 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

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

std::vector<Client*> client;

bool quit = true;

void signal_callback_handler( int signum ) {

	std::cout << GRN " \n Bye bye!" CRESET << std::endl;
	if ( signum == SIGQUIT ) {
		
		for (std::vector<Client*>::iterator it = client.begin(); it != client.end(); ++it) {
      		delete (*it);
    }
		client.clear();
	}
	exit(signum);
}

int	main( int argc, char **argv ) {
	if ( argc != 3 )
		return ( printErr( "Wrong input, please use the following form : ./ircserv <int>port <string>password\n" ) );
	if ( checkOnlyNum( argv[1] ) == -1 )
		return ( printErr( "Wrong port format, please make sure only numbers\n" ) );
	
	Server	*server = new Server(atoi(argv[1]), argv[2]);
	server->init();
	server->start();
}