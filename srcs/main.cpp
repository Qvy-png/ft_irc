/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:36:41 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/02/03 15:04:11 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"
// #include "../includes/ft_irc.hpp"

int	printErr( std::string str ) { std::cerr << str; return ( 1 ); }

int	main( int argc, char **argv ) {

	int			 		server_socket;
	struct sockaddr_in	server_address;
	int					port = std::atoi(argv[1]);
	std::string 		password = argv[2];

	std::cout << BLU << port << CRESET " and " BLU << password << CRESET << std::endl;
	if ( argc != 3 )
		return ( printErr( RED "Wrong input, please use the following form : ./ircserv <port> <password>\n" CRESET ) );
	Server data(port, password);
	server_socket = socket( AF_INET, SOCK_STREAM, 0);
	if ( server_socket < 0 )
		return ( printErr( "Error creating socket" ) );
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl( INADDR_ANY );
	server_address.sin_port = htons( port );
	if ( bind(server_socket, ( struct sockaddr* ) &server_address, sizeof( server_address ) ) < 0) 
		return ( std::cerr << REDHB "Error binding socket" CRESET << std::endl, 133 );
	if ( listen( server_socket, 5 ) < 0 )
		return ( std::cerr << REDHB "Error listening to socket" CRESET << std::endl, 134 );
	
	
	return ( 0 );
}