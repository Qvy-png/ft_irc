/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:36:41 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/02/20 18:14:05 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

#define MAX_CLIENTS 128

int		printErr( std::string str ) { std::cerr << REDHB << str << CRESET; return ( 1 ); }

// struct	Client {

// 	int			sock;
// 	std::string	nickname;
// 	bool		active;
// };

int	main( int argc, char **argv ) {

	int			 		server_socket;
	struct sockaddr_in	server_address;
	int					addrlen = sizeof( server_address );
	int					port = atoi(argv[1]);
	std::string 		password = argv[2];

	// PARSING

	std::cout << BLU << port << CRESET " and " BLU << password << CRESET << std::endl;
	if ( argc != 3 )
		return ( printErr( RED "Wrong input, please use the following form : ./ircserv <port> <password>\n" CRESET ) );
	
	// TCP SOCKET CREATION

	Server data(port, password);
	server_socket = socket( AF_INET, SOCK_STREAM, 0);
	if ( server_socket < 0 )
		return ( printErr( "Error creating socket" ) );

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons( port );
	if ( bind(server_socket, ( struct sockaddr* ) &server_address, sizeof( server_address ) ) < 0) 
		return ( printErr( "Error binding socket" ) );
	if ( listen( server_socket, 10 ) < 0 )
		return ( printErr( "Error listening to socket" ) );

	// TREATMENT

	// fd_set				read_fds;
	// std::vector<Client> clients(128);
	// for ( int i = 0; i < 128; i++ )
	// 	clients[ i ].active = false;

	int				num_open_fds = 0, current_client;
	struct pollfd	pfds[MAX_CLIENTS];
	std::vector<int> clients;

	pfds[0].fd = server_socket;
	pfds[0].events = POLLIN;
	
	while ( true ) {
		
		std::cout << "bonjour" << std::endl;
		int	poll_count = poll( pfds, num_open_fds + 1, -1);

		if( poll_count == -1 )
			return ( printErr( "Error creating Poll" ) );
		
		if (pfds[0].revents & POLLIN ) {

			if ( ( current_client = accept( server_socket, ( struct sockaddr * ) &server_address, ( socklen_t * ) &addrlen ) ) < 0 )
				return ( printErr( "Can't accept" ) ); 
		}
		num_open_fds++;
		clients.push_back( current_client );
		
		
		// FD_ZERO( &read_fds );
		
		// FD_SET(server_socket, &read_fds );
		// for (int i = 0; i < 128; i++) {
		
		// 	if ( clients[i].active )
		// 		FD_SET( clients[i].sock, &read_fds );
		// }
		// if ( FD_ISSET(server_socket, &read_fds ) )
		// {
		// 	struct sockaddr_in client_address;
		// 	socklen_t client_len = sizeof( client_address );
		// 	int client_socket = accept( server_socket, ( struct sockaddr* ) &client_address, &client_len );
		// 	if ( client_socket < 0 )
		// 		return ( printErr( "Error accepting connection" ) );
		// 	for ( int i = 0; i < 128; i++ )
		// 	{
		// 		if ( !clients[i].active )
		// 		{
		// 			clients[i].sock = client_socket;
		// 			clients[i].active = true;
		// 			break;
		// 		}
		// 	}
		// }

		// for (int i = 0; i < 128; i++) {
		
		// 	if ( FD_ISSET( clients[i].sock, &read_fds ) ) {
			
		// 		char	buffer[4096];
		// 		int r = read(clients[i].sock, buffer, 4096);
		// 		if ( r < 0 )
		// 			std::cerr << "Error reading from client" << std::endl;
		// 		else if ( r == 0 )
		// 			std::cerr << "Client disconnected" << std::endl;
		// 		else {
				
		// 			std::cout << buffer << std::endl;
		// 			std::cout << "hello there" << std::endl;
		// 		}
		// 	}
		// }
	}
	return ( 0 );
}