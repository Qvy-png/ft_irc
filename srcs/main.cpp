/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:36:41 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/03/09 16:26:40 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

#define MAX_CLIENTS 128

int		printErr( std::string str ) { std::cerr << REDHB << str << CRESET; return ( 1 ); }

int	send_msg(std::string msg, int sfd) {

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
	socklen_t					addrlen = sizeof( server_address );

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

	std::string			localhost, nick;
	localhost = "localhost";
	nick = "nick";

	pfds[0].fd = server_socket;
	pfds[0].events = POLLIN;
	
	int i = 1;
	while ( true ) {

		if (i > num_open_fds) {
			i = 1;
		}
		int poll_count = poll(pfds, num_open_fds + 1, 10);

		if (poll_count == -1)
			return ( printErr( "Error creating Poll" ) );

		if (pfds[0].revents & POLLIN ) {
			if ( ( current_client = accept( server_socket, reinterpret_cast< sockaddr *>(&server_address), &addrlen ) ) < 0 )
				return ( printErr( "Can't accept" ) );
			else {
				
				client.push_back( new Client( current_client ) );
				num_open_fds++;
				clients.push_back( current_client );
				pfds[num_open_fds].fd = current_client;
				pfds[num_open_fds].events = POLLIN; 
				std::cout << "Accepted client #" << num_open_fds << std::endl;
			}
		}

		if ( pfds[i].revents & POLLIN ) {
			
			int valread = recv( pfds[i].fd, &buffer, 1024, 0 );
			
			if ( valread < 0 )
				std::cerr << "Error reading from client #" << i << std::endl;
			else {
				
				std::cout << YEL "je suis le client " CRESET << i << std::endl;
				std::cout << buffer << std::endl;
				client[i - 1]->setBuffer( buffer );
			}
		}

		if ( i <= static_cast<int>( client.size() ) && !client[i - 1]->getBuffer().empty() ) {
			
			std::string buffer1 = client[i - 1]->getBuffer();
				
			size_t pos = buffer1.find('\r');
			if ( pos != std::string::npos ) {
				
				std::string command = buffer1;
				std::cout << "Command: " GRN << command << CRESET << std::endl;
				
				// Extract first word
				std::string tmp;
				std::stringstream ss( buffer1 );
				ss >> tmp;
				std::cout << "First word: " << RED << tmp << CRESET << std::endl;

				// Extract rest of command
				std::string tmpRest;
				std::getline( ss, tmpRest, '\r' );
				tmpRest = tmpRest.erase( 0, 1 );
				std::cout << "Rest of command: " << RED << tmpRest << CRESET << std::endl;
				
				command = buffer1.substr( 0, pos );
				buffer1.erase( 0, pos + 1 );

				if ( tmp == "PASS" )
					client[i - 1]->setPass( tmpRest );

				else if ( tmp == "NICK" ) {
				
				//TODO CHECK POUR SAVOIR SI LE NICK EST DEJA PRIT OU NON
				
					client[i - 1]->setNick( tmpRest );
				}
				else if ( tmp == "PING" )
					send_msg(PONG(), clients[i - 1]);
					
				else if (tmp == "USER") {
					
					std::stringstream	userSplitter( tmpRest );
					std::string			splitTmp;
					int					j = 0;

					client[i - 1]->setFullName( tmpRest );
					while ( getline( userSplitter, splitTmp, ' ') ) {
						
						if ( j == 2 )
							client[i - 1]->setHost( splitTmp );
						j++;
					}			
				}
				
				if ( client[i - 1]->getHs() == false ) {
				
					if ( !client[i - 1]->getPass().empty() && !client[i - 1]->getNick().empty() && !client[i - 1]->getHost().empty() ) {
						
						if ( client[i - 1]->getPass() != password ) {
							std::cout << GRNHB << password << BLUHB << client[i - 1]->getNick() << CRESET << std::endl;
							ERR_PASSWDMISMATCH( client[i - 1]->getHost(), client[i - 1]->getNick() );}
						else {

							// HANDSHAKE
							send_msg(RPL_WELCOME( client[i - 1]->getHost(), client[i - 1]->getNick() ), clients[ i - 1 ] );
							send_msg(RPL_YOURHOST( client[i - 1]->getHost() ), clients[ i - 1 ] );
							send_msg(RPL_CREATED( client[i - 1]->getHost() ), clients[ i - 1 ] );
							send_msg(RPL_MYINFO( client[i - 1]->getHost() ), clients[ i - 1 ] );
							client[i - 1]->setHs(true);
						}
					}
				}
				tmp.clear();
				tmpRest.clear();
			}
			client[i - 1]->setBuffer(const_cast<char*>( buffer1.c_str() ) );
			buffer1.clear();
			bzero(buffer, 1025);
			
		}
		i++;
	}


	return ( 0 );
}
