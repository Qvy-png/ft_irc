/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:36:41 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/02/22 20:05:39 by rdel-agu         ###   ########.fr       */
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



int	main( int argc, char **argv ) {

	int			 		server_socket;
	struct sockaddr_in	server_address;
	int					addrlen = sizeof( server_address );

	// PARSING

	if ( argc != 3 )
		return ( printErr( RED "Wrong input, please use the following form : ./ircserv <port> <password>\n" CRESET ) );

	int					port = atoi(argv[1]);
	std::string 		password = argv[2];
	
	std::cout << BLU << port << CRESET " and " BLU << password << CRESET << std::endl;
	
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

	// TREATMENT

	int					num_open_fds = 0, current_client = -1;
	struct pollfd		pfds[MAX_CLIENTS];
	std::vector<int>	clients;
	// std::string			buffer;
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
			else 
			{
				client.push_back(new Client(current_client));
				num_open_fds++;
				clients.push_back( current_client );
				pfds[num_open_fds].fd = current_client;
				pfds[num_open_fds].events = POLLIN;
			}
		}
		
		// send(current_client, welcome.c_str(), welcome.size(), 0 );
		
		for ( int i = 1; i <= num_open_fds; i++ ) {
			 std::cout << RED"bonjour : "CRESET << num_open_fds << std::endl;
			if (pfds[i].revents & POLLIN ) {
				
				// char localhost[10] = "localhost";
				// char nick[5] = "nick";
				int valread = recv( pfds[i].fd, &buffer, 1024, 0 );
				std::cout << "je suis le client " << i << std::endl;
				std::cout << buffer << std::endl;
				
				client[i - 1]->setBuffer( buffer );			 

                if (valread >= 0 ) {
					// std::cout << RED "user gone" CRESET << std::endl, num_open_fds--;
					// std::cout << client[i].getBuffer() << std::endl;
					// DELETING CLIENT IF NOT RESPONDING / LEAVING
                    // close( pfds[i].fd );
                    // clients.erase( clients.begin() + 3i - 1 );
                    // pfds[i] = pfds[num_open_fds];
                    // num_open_fds--;
					// std::cout << RED "penis" CRESET << std::endl;
					// std::cout << BLU << clients[i - 1] << CRESET << std::endl;
					// std::cout << send_msg(ERR_PASSWDMISMATCH(localhost, nick), clients[i - 1]) << std::endl;
                } 
				// else {
				// for ( int j = 0; j < num_open_fds; j++ ) {
				//     if (clients[j] != pfds[i].fd) {
					// send( clients[i], buffer, strlen(buffer), 0 );
					
					
					// HANDSHAKE
					send( clients[i], RPL_WELCOME( localhost, nick ).c_str(), RPL_WELCOME( localhost, nick ).length(), 0 );
					send_msg(RPL_WELCOME( localhost, nick ), clients[i - 1] );
					std::cout << RPL_WELCOME( localhost, nick ) << std::endl;
					send_msg(RPL_YOURHOST( localhost ), clients[i - 1] );
					std::cout << RPL_YOURHOST( localhost ) << std::endl;
					send_msg(RPL_CREATED( localhost ), clients[i - 1] );
					std::cout << RPL_CREATED( localhost ) << std::endl;
					send_msg(RPL_MYINFO( localhost ), clients[i - 1] );
					std::cout << RPL_MYINFO( localhost ) << std::endl;

					// TEST
					
					// }
				
				bzero(buffer, 1025);
			}
		}
	}
	return ( 0 );
}