/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:36:41 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/02/16 16:20:25 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

int		printErr( std::string str ) { std::cerr << REDHB << str << CRESET; return ( 1 ); }

struct	Client {

	int			sock;
	std::string	nickname;
	bool		active;
};

int	main( int argc, char **argv ) {

	int			 		server_socket;
	fd_set				read_fds;
	struct sockaddr_in	server_address;
	int					port = atoi(argv[1]);
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
		return ( printErr( "Error binding socket" ) );
	if ( listen( server_socket, 5 ) < 0 )
		return ( printErr( "Error listening to socket" ) );

	std::vector<Client> clients(128);
	for ( int i = 0; i < 128; i++ )
		clients[ i ].active = false;
	while ( 1 ) {
		FD_ZERO( &read_fds );
		FD_SET(server_socket, &read_fds );
		for (int i = 0; i < 128; i++) {
		
			if ( clients[i].active )
				FD_SET( clients[i].sock, &read_fds );
		}
		if ( FD_ISSET(server_socket, &read_fds ) )
		{
			struct sockaddr_in client_address;
			socklen_t client_len = sizeof( client_address );
			int client_socket = accept( server_socket, ( struct sockaddr* ) &client_address, &client_len );
			if ( client_socket < 0 )
				return ( printErr( "Error accepting connection" ) );
			for ( int i = 0; i < 128; i++ )
			{
				if ( !clients[i].active )
				{
					clients[i].sock = client_socket;
					clients[i].active = true;
					break;
				}
			}
		}

		int		r = 0;
		char	buffer[4096];
		for (int i = 0; i < 128; i++) {
		
			if ( FD_ISSET( clients[i].sock, &read_fds ) ) {
			
				r = read(clients[i].sock, buffer, 4096);
				if ( r < 0 )
					std::cout << "Error reading from client" << std::endl;
				else if ( r == 0 )
					std::cout << "Client disconnected" << std::endl;
				else {
				
					std::cout << buffer << std::endl;
				}
			}
		}
	}
	return ( 0 );
}