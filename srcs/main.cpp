/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:36:41 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/04/12 18:55:35 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

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

bool canalExist(std::vector<Canal *> canals, std::string canalName) {
	std::vector<Canal*>::iterator it = canals.begin();

	while (it != canals.end()) {
		if ((*it)->getName() == canalName)
			return (true);
		it++;
	}
	return (false);
}

Canal *getCanal(std::vector<Canal *> canals, std::string canalName) {
	std::vector<Canal *>::iterator it = canals.begin();

	while (it != canals.end()) {
		if ((*it)->getName() == canalName)
			return ((*it));
		it++;
	}
	return (NULL);
}

bool	validCanalName(std::string name) {
	if (name.size() > 200 || name.find(' ') != name.npos|| name.find(',') != name.npos)
		return (false);
	return (true);
}

bool	isClient(std::vector<Client *> clients, Client *cli ) {
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
		Client *tmp = (*it);
		if (tmp->getNick() == cli->getNick())
			return (true);
	}
	return (false);
}

bool	isClient(std::vector<Client *> clients, std::string cli ) {
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
		Client *tmp = (*it);
		if (tmp->getNick() == cli)
			return (true);
	}
	return (false);
}

bool	isCanal(std::string canalName) {
	return (canalName[0] == '#' || canalName[0] == '$');
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

Client	*getClient(std::vector<Client *> clients, std::string name) {
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
		Client *tmp = (*it);
		if (name == tmp->getNick()) {
			return tmp;
		}
	}
	return NULL;
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

// int	main( int argc, char **argv ) {
// 	int			 		server_socket;
// 	struct sockaddr_in6	server_address;
// 	socklen_t			addrlen = sizeof( server_address );
// 	CanalManager		*canalManager = new CanalManager();
	

// 	if ( argc != 3 )
// 		return ( printErr( "Wrong input, please use the following form : ./ircserv <int>port <string>password\n" ) );
// 	if ( checkOnlyNum( argv[1] ) == -1 )
// 		return ( printErr( "Wrong port format, please make sure only numbers\n" ) );
	
// 	int					port = atoi(argv[1]);
// 	std::string 		password = argv[2];
	
// 	if ( port < 1 || port > 65535 )
// 		return ( printErr( "Port not within range, please input a port within [1 - 65535]\n" ) );
	
// 	std::cout << "Port : " BLU << port << CRESET " and Password : " BLU << password << CRESET << std::endl;
	
// 	// TCP SOCKET CREATION

// 	Server data(port, password);
// 	// ===> SERVER LAUNCH
// 	server_socket = socket( AF_INET6, SOCK_STREAM, 0);
// 	if ( server_socket < 0 )
// 		return ( printErr( "Error creating socket" ) );
		
// 	server_address.sin6_family = AF_INET6;
// 	server_address.sin6_addr = in6addr_any;
// 	server_address.sin6_port = htons( port );
// 	server_address.sin6_flowinfo = 0;
// 	server_address.sin6_scope_id = 0; 
// 	int	n = 1;
// 	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(int)) < 0)
//     	return ( printErr( "Error option in socket" ) );
// 	// BINDING SOCKET AND LISTENING TO SOCKET CHECK
// 	if ( bind(server_socket, ( struct sockaddr* ) &server_address, sizeof( server_address ) ) < 0) 
// 		return ( printErr( "Error binding socket" ) );
// 	if ( listen( server_socket, 10 ) < 0 )
// 		return ( printErr( "Error listening to socket" ) );


// 	int					num_open_fds = 0, current_client = -1;
// 	struct pollfd		pfds[MAX_CLIENTS];
// 	std::vector<int>	clients;
// 	// std::vector<Canal *>	canals;
// 	char				buffer[1025];

// 	bzero(buffer, 1025);
	


// 	std::string			localhost, nick;
// 	localhost = "localhost";
// 	nick = "nick";
	
// 	pfds[0].fd = server_socket;
// 	pfds[0].events = POLLIN;
// 	// <=== SERVER LAUNCH
// 	while ( quit ) {
		
// 		signal(SIGINT, signal_callback_handler);

// 		int poll_count = poll(pfds, num_open_fds + 1, 10);

// 		if (poll_count == -1)
// 			return ( printErr( "Error creating Poll" ) );

// 		if (pfds[0].revents & POLLIN ) {
// 			if ( ( current_client = accept( server_socket, reinterpret_cast< sockaddr *>(&server_address), &addrlen ) ) < 0 )
// 				return ( printErr( "Can't accept" ) );
// 			else {
				
// 				client.push_back( new Client( current_client ) );
// 				num_open_fds++;
// 				clients.push_back( current_client );
// 				pfds[num_open_fds].fd = current_client;
// 				pfds[num_open_fds].events = POLLIN;
// 				pfds[num_open_fds].revents = 0;
// 				std::cout << "Accepted client #" << num_open_fds << std::endl;
// 			}
// 		}

// 		for (int i = 1; i < num_open_fds + 1; i++) {

// 			if (client[i - 1]->getHasTime() == true ) {
				
// 				std::time_t tmpTime = std::time(NULL);

// 				if ( tmpTime - client[i - 1]->getTime() > 90 ){
					
// 					std::cout << "Client #" << i << " timout." << std::endl;
// 					close( pfds[i].fd );
// 					client.erase( client.begin() + i - 1 );
// 					clients.erase( clients.begin() + i - 1 );
// 					for (int j = i; j + 1 <= num_open_fds + 1; j++)
// 						pfds[j] = pfds[j + 1];
// 					num_open_fds--;
// 				}
// 			}
// 			pfds[i].events = POLLIN | POLLOUT; 
// 			if ( pfds[i].revents & POLLIN ) {
				
// 				int valread = recv( pfds[i].fd, &buffer, 1024, 0 );
				
// 				if ( valread < 0 )
// 					std::cerr << "Error reading from client #" << i << std::endl;
// 				else if ( valread == 0) {
// 					// FAIRE QUITTER TOUT SES CANNAUX
// 					std::cout << "Client #" << i << " disconnected." << std::endl;
// 					close( pfds[i].fd );
// 					client.erase( client.begin() + i - 1 );
// 					clients.erase( clients.begin() + i - 1 );
// 					for (int j = i; j + 1 <= num_open_fds + 1; j++)
// 						pfds[j] = pfds[j + 1];
// 					num_open_fds--;
// 				}
// 				else {
					
// 					std::cout << YEL "je suis le client " CRESET << i << std::endl;
// 					std::cout << buffer << std::endl;
// 					client[i - 1]->addBuffer( buffer );
// 				}
// 			}

// 			if ( pfds[i].revents & POLLOUT ) {
// 				// LIMITE DE SEND DE 1024CHAR;
// 				std::map<std::string, Canal *> canals = canalManager->GetChannels();

// 				for (std::map<std::string, Canal *>::iterator it = canals.begin(); it != canals.end(); it++) {
// 					Canal *tmp = (*it).second;
// 					if (!tmp->hasClient(client[i - 1]))
// 						continue ;
// 					for (std::vector<Message *>::iterator it_msg = tmp->waitingMessages.begin(); it_msg != tmp->waitingMessages.end(); it_msg++) {
// 						Message *msg = (*it_msg);
// 						if (isClient(msg->clients, client[i - 1]) && msg->getSender().getNick() != client[i - 1]->getNick()) {
// 							std::cout << "msg = " << msg->getMessage() << std::endl;
// 							if (tmp->getName()[0] == '#') {
// 								std::cout << "ici mec " << std::endl; 
// 								if (tmp->isOp(msg->getSender().getNick()))
// 									send_msg(":" + msg->getSender().getNick() + " PRIVMSG " + tmp->getName() + " " + msg->getMessage() + "\r\n", clients[i - 1]);
// 								else
// 									send_msg(":" + msg->getSender().getNick() + " PRIVMSG " + tmp->getName() + " " + msg->getMessage() + "\r\n", clients[i - 1]);
// 								std::cout << ":" + msg->getSender().getNick() + " PRIVMSG " + tmp->getName() + " " + msg->getMessage() + "\r\n" << std::endl;
// 							}
// 							else {
// 								std::cout << "PRIV_MSG : " << RPL_PRIVMSG(getClient(client, tmp->getName())->getPrefix(), client[i - 1]->getNick(), msg->getMessage()) << std::endl;
// 								//send_msg("PRIVMSG" + std::string(" ") + msg->getMessage() + "\r\n", clients[i - 1]);
// 								// irctest : momo!root@localhost PRIVMSG coco :hey
// 								// momo envoie / coco recoit

// 								// moi : coco!root@localhost PRIVMSG coco :salut
// 								// momo envoie / coco recoit
// 								send_msg(RPL_PRIVMSG(getClient(client, tmp->getName())->getPrefix(), client[i - 1]->getNick(), msg->getMessage()), client[i - 1]->getFd());
// 							}
// 							//std::cout << "USER JOIN : " << client[i - 1]->getFullName() << " et " << client[i - 1]->getHost() << std::endl; 
// 							for (std::vector<Client *>::iterator cli_it = msg->clients.begin(); cli_it != msg->clients.end(); cli_it++) {
// 								Client *cli = (*cli_it);
// 								if (cli->getNick() == client[i - 1]->getNick())
// 								{
// 									msg->clients.erase(cli_it);
// 									break ;
// 								}
// 							}
// 							it_msg--;
// 							if (msg->clients.size() == 0)
// 								tmp->waitingMessages.erase(it_msg + 1);
// 						}
// 					}
					
// 				}
// 				// if (client[i - 1].ca)
// 			}

// 			if ( i <= static_cast<int>( client.size() ) && !client[i - 1]->getBuffer().empty() ) {
				
// 				std::string buffer1 = client[i - 1]->getBuffer();
					
// 				size_t pos = buffer1.find('\r');
// 				if ( pos != std::string::npos ) {
					
// 					std::string command = buffer1;
// 					std::cout << "Command: " GRN << command << CRESET << std::endl;
					
// 					// Extract first word
// 					std::string tmp;
// 					std::stringstream ss( buffer1 );
// 					ss >> tmp;
// 					std::cout << "First word: " << RED << tmp << CRESET << std::endl;

// 					// Extract rest of command
// 					std::string tmpRest;
// 					std::getline( ss, tmpRest, '\r' );
// 					tmpRest = tmpRest.erase( 0, 1 );
// 					std::cout << "Rest of command: " << RED << tmpRest << CRESET << std::endl;
// 					command = buffer1.substr( 0, pos );
// 					std::cout << "command to execute next :" << GRN << command << CRESET << std::endl;
// 					buffer1.erase( 0, pos + 1 );

// 					if ( tmp == "PASS" )
// 						pass(client, i, tmpRest);
// 					else if ( tmp == "NICK" )
// 						nickcmd(client, i, tmpRest, num_open_fds, localhost);
// 					else if ( tmp == "PING" )
// 						ping(clients, i, client);
// 					else if (tmp == "USER")
// 						user(client, i, tmpRest);
// 					else if (tmp == "JOIN")
// 						join(client, canalManager, i, tmpRest);
// 					else if (tmp == "INVITE")
// 						invite(client, canalManager, i, tmpRest);
// 					else if (tmp == "MODE")
// 						mode(client, canalManager, i, tmpRest);
// 					else if (tmp == "TOPIC")
// 						topic(client, canalManager, i, tmpRest);
// 					else if (tmp == "PART")
// 						part(client, canalManager, i, tmpRest);
// 					else if (tmp == "QUIT") {
// 						quitcmd(client, i, pfds, num_open_fds);
// 						num_open_fds--;
// 						break ;
// 					} else if (tmp == "PRIVMSG")
// 						privmsg(client, canalManager, i, tmpRest);
// 					else if (tmp == "KICK")
// 						kick(client, i, tmpRest, canalManager);
// 					else if (tmp == "OPER")
// 						oper(client, tmpRest);
// 					else if (tmp == "WHOIS")
// 						whois(client, i, tmpRest, num_open_fds);
// 					else if (tmp == "CAP")
// 						std::cout << "Cap command found, skipping" << std::endl;
// 					else
// 						send_msg(ERR_NOTIMPLEMENTED( client[i - 1]->getHost(), tmp) ,clients[i - 1]);
					
// 					if ( client[i - 1]->getHs() == false ) {
					
// 						if ( !client[i - 1]->getPass().empty() && !client[i - 1]->getNick().empty() && !client[i - 1]->getHost().empty() ) {
							
// 							if ( client[i - 1]->getPass() != password ) {
// 								std::cout << GRNHB << password << BLUHB << client[i - 1]->getNick() << CRESET << std::endl;
// 								send_msg(ERR_PASSWDMISMATCH( localhost, client[i - 1]->getNick() ), clients[i - 1] );

// 								//faire la deconnexion du client
// 								std::cout << "Client #" << i << " timeout." << std::endl;
// 								close( pfds[i].fd );
// 								client.erase( client.begin() + i - 1 );
// 								clients.erase( clients.begin() + i - 1 );
// 								for (int j = i; j + 1 <= num_open_fds + 1; j++)
// 									pfds[j] = pfds[j + 1];
// 								num_open_fds--;
// 							}
// 							else {

// 								// HANDSHAKE
// 								send_msg(RPL_WELCOME( client[i - 1]->getHost(), client[i - 1]->getNick() ), clients[ i - 1 ] );
// 								send_msg(RPL_YOURHOST( client[i - 1]->getHost() ), clients[ i - 1 ] );
// 								send_msg(RPL_CREATED( client[i - 1]->getHost() ), clients[ i - 1 ] );
// 								send_msg(RPL_MYINFO( client[i - 1]->getHost() ), clients[ i - 1 ] );
// 								client[i - 1]->setHs(true);
// 							}
// 						}
// 					}
// 					tmp.clear();
// 					tmpRest.clear();
// 				}
// 				client[i - 1]->setBuffer(const_cast<char*>( buffer1.c_str() ) );
// 				buffer1.clear();
// 				bzero(buffer, 1025);
// 			}
// 		}
// 	}


// 	return ( 0 );
// }
