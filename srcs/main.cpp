/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:36:41 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/03/27 14:38:39 by dasereno         ###   ########.fr       */
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

bool quit = true;

void signal_callback_handler( int signum ) {

	std::cout << GRN " \n Bye bye!" CRESET << std::endl;
	// if ( signum == SIGQUIT )
	// 	quit = false;
	exit(signum);
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

void	kick(std::vector<Client *> client, int i, std::string tmpRest, CanalManager *canalManager) {
	std::cout << "Client #" << i << " KICK channel "<< tmpRest << std::endl;
	// std::string args = tmpRest.substr(tmpRest.find(':') + 1, tmpRest.size());
	std::string channel = tmpRest.substr(0, tmpRest.find(' '));
	std::string toKick = tmpRest.substr(tmpRest.find(' ') + 1, tmpRest.size());
	if (toKick.find(':') != toKick.npos)
		toKick.erase(toKick.find(':'));
	if (toKick.find(' ') != toKick.npos)
		toKick.erase(toKick.find(' '));
	std::cout << tmpRest << std::endl;
	std::cout << channel << std::endl;
	std::cout << toKick << std::endl;
	Canal *canal = canalManager->GetChannel(channel);
	if (canal == NULL)
		return ;
	if (canal->getOp().getNick() != client[i - 1]->getNick())
		return ;
	Client *cli = canal->getClient(toKick);
	if (cli == NULL)
		return ;
	if (canal->hasClient(cli))
	{
		canal->deleteClient(cli);
		std::cout << "il a quitte" << std::endl;
	} else {
		std::cout << "non kickage" << std::endl;
	}
						
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

void	privmsg_client(std::vector<Client *> client, int i, std::string tmpRest)
{
	std::string receiver = tmpRest.substr(0, tmpRest.find(' '));
	if (!isClient(client, receiver))
		return ;
	std::string msg = tmpRest.substr(tmpRest.find(':'), tmpRest.size());
	Message *newMsg = new Message (msg, *client[i - 1]);
	Client *cli = getClient(client, receiver);
	if (cli == NULL)
		return ;
	cli->pushMessage(newMsg);

}

void	privmsg(std::vector<Client *> client, CanalManager *canalManager, int i, std::string tmpRest) {
	// MMARCHE QUE POUR LES CHANNELS ACTUELLEMENT
	std::cout << "Client #" << i << " send a message." << std::endl;
	std::string channel = tmpRest.substr(0, tmpRest.find(' '));
	Canal	*canal = canalManager->GetChannel(channel);
	if (!canal)
		return (privmsg_client(client, i, tmpRest));
	if (!canal->hasClient(client[i - 1]))
		return ;
	std::string msg = tmpRest.substr(tmpRest.find(':'), tmpRest.size());
	Message *newMsg = new Message (msg, *client[i - 1]);
	if (!canal)
		return ;
	canal->waitingMessages.push_back(newMsg);
	for (std::vector<Client *>::iterator it = canal->clients.begin(); it != canal->clients.end(); it++) {
		Client *cli = (*it);
		std::cout << "->" + cli->getNick() << std::endl;
	}
	(*canal->waitingMessages.rbegin())->clients = canal->clients; 
}



int	main( int argc, char **argv ) {

	int			 		server_socket;
	struct sockaddr_in6	server_address;
	socklen_t			addrlen = sizeof( server_address );
	CanalManager		*canalManager = new CanalManager();
	

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
	server_socket = socket( AF_INET6, SOCK_STREAM, 0);
	if ( server_socket < 0 )
		return ( printErr( "Error creating socket" ) );
		
	server_address.sin6_family = AF_INET6;
	server_address.sin6_addr = in6addr_any;
	server_address.sin6_port = htons( port );
	server_address.sin6_flowinfo = 0;
	server_address.sin6_scope_id = 0; 
	int	n = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(int)) < 0)
    	return ( printErr( "Error option in socket" ) );
	// BINDING SOCKET AND LISTENING TO SOCKET CHECK
	if ( bind(server_socket, ( struct sockaddr* ) &server_address, sizeof( server_address ) ) < 0) 
		return ( printErr( "Error binding socket" ) );
	if ( listen( server_socket, 10 ) < 0 )
		return ( printErr( "Error listening to socket" ) );


	int					num_open_fds = 0, current_client = -1;
	struct pollfd		pfds[MAX_CLIENTS];
	std::vector<int>	clients;
	// std::vector<Canal *>	canals;
	char				buffer[1025];

	bzero(buffer, 1025);
	
	std::vector<Client*> client;

	std::string			localhost, nick;
	localhost = "localhost";
	nick = "nick";

	short event[2] = {POLLIN, POLLOUT};
	int	ev = 0;

	pfds[0].fd = server_socket;
	pfds[0].events = POLLIN;
	while ( quit ) {
		
		signal(SIGINT, signal_callback_handler);

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
				pfds[num_open_fds].revents = 0;
				std::cout << "Accepted client #" << num_open_fds << std::endl;
			}
		}

		for (int i = 1; i < num_open_fds + 1; i++) {

			if (client[i - 1]->getTime()) {
				
				std::time_t tmpTime = std::time(NULL);

				if ( tmpTime - client[i - 1]->getTime() > 90 ){
					
					std::cout << "Client #" << i << " timout." << std::endl;
					close( pfds[i].fd );
					client.erase( client.begin() + i - 1 );
					clients.erase( clients.begin() + i - 1 );
					for (int j = i; j + 1 <= num_open_fds + 1; j++)
						pfds[j] = pfds[j + 1];
					num_open_fds--;
				}
			}
			pfds[i].events = event[ev]; 
			if ( pfds[i].revents & POLLIN ) {
				
				int valread = recv( pfds[i].fd, &buffer, 1024, 0 );
				
				if ( valread < 0 )
					std::cerr << "Error reading from client #" << i << std::endl;
				else if ( valread == 0) {
					// FAIRE QUITTER TOUT SES CANNAUX
					std::cout << "Client #" << i << " disconnected." << std::endl;
					close( pfds[i].fd );
					client.erase( client.begin() + i - 1 );
					clients.erase( clients.begin() + i - 1 );
					for (int j = i; j + 1 <= num_open_fds + 1; j++)
						pfds[j] = pfds[j + 1];
					num_open_fds--;
				}
				else {
					
					std::cout << YEL "je suis le client " CRESET << i << std::endl;
					std::cout << buffer << std::endl;
					client[i - 1]->addBuffer( buffer );
				}
			}

			if ( pfds[i].revents & POLLOUT ) {
				// LIMITE DE SEND DE 1024CHAR;
				std::map<std::string, Canal *> canals = canalManager->GetChannels();

				for (std::map<std::string, Canal *>::iterator it = canals.begin(); it != canals.end(); it++) {
					Canal *tmp = (*it).second;
					if (!tmp->hasClient(client[i - 1]))
						continue ;
					for (std::vector<Message *>::iterator it_msg = tmp->waitingMessages.begin(); it_msg != tmp->waitingMessages.end(); it_msg++) {
						Message *msg = (*it_msg);
						if (isClient(msg->clients, client[i - 1]) && msg->getSender().getNick() != client[i - 1]->getNick()) {
							std::cout << "client nb #" << i << " has to receive a message" << std::endl;
							send_msg(":" + msg->getSender().getNick() + " PRIVMSG " + tmp->getName() + " " + msg->getMessage() + "\r\n", clients[i - 1]);
							// std::cout << client[i - 1]->getNick() + " :"+ "PRIVMSG " + tmp->getName() + " " + msg.getMessage() << std::endl;
							for (std::vector<Client *>::iterator cli_it = msg->clients.begin(); cli_it != msg->clients.end(); cli_it++) {
								Client *cli = (*cli_it);
								if (cli->getNick() == client[i - 1]->getNick())
								{
									msg->clients.erase(cli_it);
									break ;
								}
							}
							it_msg--;
							if (msg->clients.size() == 0)
								tmp->waitingMessages.erase(it_msg + 1);
						}
					}
					
				}
				// if (client[i - 1].ca)
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
					std::cout << "command to execute next :" << GRN << command << CRESET << std::endl;
					buffer1.erase( 0, pos + 1 );

					if ( tmp == "PASS" )
						client[i - 1]->setPass( tmpRest );

					else if ( tmp == "NICK" ) {
					
						bool NickIsFree;

						NickIsFree = true;
						if (client[i - 1]->getNick().empty()) {
							
							for (int j = 0; j < num_open_fds; j++) {
								
								if ( client[j]->getNick() == tmpRest ) {
									
									send_msg(ERR_NICKNAMEINUSE( localhost, tmpRest), clients[i - 1]);
									NickIsFree = false;
								}
							}
						}
						if ( NickIsFree == true )
							client[i - 1]->setNick(tmpRest);
					}
					else if ( tmp == "PING" ) {
						
						// send_msg(PONG(), clients[i - 1]);
						
						std::time_t time = std::time(NULL);

						if (client[i - 1]->getHasTime() == true )
							std::cout << BLU <<  time - client[i - 1]->getTime() << CRESET << std::endl;
						client[i - 1]->setTime( time );
						std::cout << client[i - 1]->getTime() << std::endl;
						send_msg(PONG(), clients[i - 1]);
						client[i - 1]->setHasTime( true );
					}
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
					} else if (tmp == "JOIN") {
						
						std::cout << "Client #" << i << " try to join " << tmpRest << std::endl;
						Canal *canal = canalManager->GetChannel(tmpRest);
						if (canal == NULL) {
							canal = canalManager->CreateChannel(tmpRest, client[i - 1]);
						} else if (!canal->hasClient(client[i - 1])) {
							canal->pushClient(client[i - 1]);
						}
					} else if (tmp == "PART") {

						std::cout << "Client #" << i << " PART channel "<< tmpRest << std::endl;
						std::string args = tmpRest.substr(tmpRest.find(':') + 1, tmpRest.size());
						std::vector<std::string> channels = split(args, " ");
						std::map<std::string, Canal *> canals = canalManager->GetChannels();
						for (std::map<std::string, Canal *>::iterator it = canals.begin(); it != canals.end(); it++) {
							Canal *canal = (*it).second;
							if (canal->hasClient(client[i - 1]))
							{
								canal->deleteClient(client[i - 1]);
								send_msg(":" + client[i - 1]->getNick() + "!user@host PART " + canal->getName() + " " + "\r\n", clients[i - 1]);
							}
						}
					}
					else if (tmp == "QUIT") {
						
						// FAIRE QUITTER TOUT SES CANAUX
						std::cout << "Client #" << i << " disconnected. QUIT" << std::endl;
						close( pfds[i].fd );
						client.erase( client.begin() + i - 1 );
						clients.erase( clients.begin() + i - 1 );
						for (int j = i; j + 1 <= num_open_fds + 1; j++)
						{
							pfds[j] = pfds[j + 1];
						}
						num_open_fds--;
						break ;
					} else if (tmp == "PRIVMSG") {
						privmsg(client, canalManager, i, tmpRest);
					} else if (tmp == "KICK") {
						kick(client, i, tmpRest, canalManager);
					} else if (tmp == "OPER") {
						std::string	name = tmpRest.substr(0, tmpRest.find(' '));
						std::string pass = tmpRest.substr(tmpRest.find(' ') + 1, tmpRest.size());
						std::cout << name << " : " << pass << std::endl;
						if (pass == "6432") {
							Client *cli = getClient(client, name);
							if (cli != NULL) {
								cli->setOp(true);
								std::cout << "op state : " << cli->getOp() << std::endl;
							}
						}
					}
					
					if ( client[i - 1]->getHs() == false ) {
					
						if ( !client[i - 1]->getPass().empty() && !client[i - 1]->getNick().empty() && !client[i - 1]->getHost().empty() ) {
							
							if ( client[i - 1]->getPass() != password ) {
								std::cout << GRNHB << password << BLUHB << client[i - 1]->getNick() << CRESET << std::endl;
								send_msg(ERR_PASSWDMISMATCH( localhost, client[i - 1]->getNick() ), clients[i - 1] );

								//faire la deconnexion du client
								std::cout << "Client #" << i << " timout." << std::endl;
								close( pfds[i].fd );
								client.erase( client.begin() + i - 1 );
								clients.erase( clients.begin() + i - 1 );
								for (int j = i; j + 1 <= num_open_fds + 1; j++)
									pfds[j] = pfds[j + 1];
								num_open_fds--;
							}
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
		}
		ev++;
		if (ev == 2)
			ev = 0;
	}


	return ( 0 );
}
