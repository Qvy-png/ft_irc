/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:36:41 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/02/27 18:13:06 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

#define MAX_CLIENTS 128

int		printErr( std::string str ) { std::cerr << REDHB << str << CRESET; return ( 1 ); }

int	send_msg(std::string msg, int sfd)
{
	int res;
	res = send(sfd, msg.c_str(), msg.length(), MSG_CONFIRM);
	return (res);
}

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

int hasReturn( std::string str ) {

	if ( str.empty() )
		return ( -1 );
	else {
	
		int i = 0;
		int j = 0;
		
		while ( str[i] ) {

			if ( str[i++] == '\r')
				j++;
		}
		if ( j <= 0 )
			return ( -1 );
		else
			return ( 0 );
	}
}


int	main( int argc, char **argv ) {

	int			 		server_socket;
	struct sockaddr_in	server_address;
	int					addrlen = sizeof( server_address );

	if ( argc != 3 )
		return ( printErr( "Wrong input, please use the following form : ./ircserv <int>port <string>password\n" ) );
	if ( checkOnlyNum( argv[1] ) == -1 )
		return ( printErr( "Wrong port format, please make sure only numbers\n" ) );
	
	int					port = atoi(argv[1]);
	std::string 		password = argv[2];
	
	if ( port < 1 || port > 65535 )
		return ( printErr( "Port not within range, please input a port within [1 - 65535]\n" ) );
	
	std::cout << "Port : " BLU << port << CRESET " and Password : " BLU << password << CRESET << std::endl;
	
	// TCP SOCKET CREATION

	Server data(port, password);
	server_socket = socket( AF_INET, SOCK_STREAM, 0);
	if ( server_socket < 0 )
		return ( printErr( "Error creating socket" ) );
		
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons( port );
	
	// BINDING SOCKET AND LISTENING TO SOCKET CHECK

	if ( bind(server_socket, ( struct sockaddr* ) &server_address, sizeof( server_address ) ) < 0) 
		return ( printErr( "Error binding socket" ) );
	if ( listen( server_socket, 10 ) < 0 )
		return ( printErr( "Error listening to socket" ) );


	int					num_open_fds = 0, current_client = -1;
	struct pollfd		pfds[MAX_CLIENTS];
	std::vector<int>	clients;
	char				buffer[1025];
	bzero(buffer, 1025);
	
	std::vector<Client*> client;
	
	std::string			welcome = "Welcome to the IRC server!\n";
	std::string			localhost, nick;
	localhost = "localhost";
	nick = "nick";

	pfds[0].fd = server_socket;
	pfds[0].events = POLLIN;
	
	while ( true ) {
		
		int	poll_count = poll( pfds, num_open_fds + 1, -1);
		if( poll_count == -1 )
			return ( printErr( "Error creating Poll" ) );
		if (pfds[0].revents & POLLIN ) {

			if ( ( current_client = accept( server_socket, ( struct sockaddr * ) &server_address, ( socklen_t * ) &addrlen ) ) < 0 )
				return ( printErr( "Can't accept" ) ); 
			else {

				client.push_back(new Client(current_client));
				num_open_fds++;
				clients.push_back( current_client );
				pfds[num_open_fds].fd = current_client;
				pfds[num_open_fds].events = POLLIN; 
			}
		}
		for ( int i = 1; i <= num_open_fds; i++ ) {

			 std::cout << RED"bonjour : "CRESET << num_open_fds << std::endl;
			if (pfds[i].revents & POLLIN ) {
				
				int valread = recv( pfds[i].fd, &buffer, 1024, 0 );
				std::cout << "je suis le client " << i << std::endl;
				std::cout << buffer << std::endl;
				
				client[i - 1]->setBuffer( buffer );			 

                if (valread < 0 ) {

					std::cout << RED "user gone" CRESET << std::endl, num_open_fds--;
					// std::cout << client[i].getBuffer() << std::endl;
					// DELETING CLIENT IF NOT RESPONDING / LEAVING
                    close( pfds[i].fd );
                    clients.erase( clients.begin() + i - 1 );
                    pfds[i] = pfds[num_open_fds];
                    num_open_fds--;
					// std::cout << BLU << clients[i - 1] << CRESET << std::endl;
					// std::cout << send_msg(ERR_PASSWDMISMATCH(localhost, nick), clients[i - 1]) << std::endl;
					std::cout << BLU "Je ne reçois plus rien !" CRESET << std::endl;
                } 
				else {
						
					// HANDSHAKE
						// send_msg(RPL_WELCOME( localhost, nick ), clients[ i - 1 ] );
						// send_msg(RPL_YOURHOST( localhost ), clients[ i - 1 ] );
						// send_msg(RPL_CREATED( localhost ), clients[ i - 1 ] );
						// send_msg(RPL_MYINFO( localhost ), clients[ i - 1 ] );

					if ( hasReturn(client[i - 1]->getBuffer() ) == 0 ) {
						
						std::stringstream 	stream;
						std::stringstream	buffSplit;
						std::string			tmp;
						
						stream << client[i - 1]->getBuffer();
						std::getline( stream, tmp, '\r' );
						std::cout << tmp << std::endl;
						buffSplit << tmp;
						
						// std::cout << BLU "Hello" CRESET << std::endl;

					}
						
				}
				
				bzero(buffer, 1025);
			}
		}
	}
	return ( 0 );
}