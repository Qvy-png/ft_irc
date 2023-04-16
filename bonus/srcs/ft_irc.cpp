/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 14:12:48 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/04/16 19:26:32 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

Server::Server( void ) : _port(), _password() { }

Server::Server( int port, std::string password ) : _num_open_fds(0), _addrlen(sizeof(_server_address)), _canalManager(new CanalManager()), _port(port), _password(password)
, _currentClient(-1), _commandManager(new CommandManager(this)) { 
    bzero(_buffer, 1025);
    return ;
}

Server::Server( const Server& ref ) { *this = ref; }

Server::~Server( void ) {
	std::cout << "bjr le amis je QUITEE" << std::endl;
	// _clients.clear();
    for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
        delete *it;
        *it = NULL;
    }
	_clients.clear();
	delete _canalManager;
	delete _commandManager;
	_ops.clear();
	return ;
}

Server& Server::operator=( const Server& ref ) {

    if ( this == &ref )
        return ( *this );
    *this = ref;
    return ( *this );
}

void Server::signal_callback_handler( int signum ) {
	std::cout << GRN " \n Bye bye!" CRESET << std::endl;
	if ( signum == SIGINT ) {
		delete server;
		exit (0);
		// Server~Server();	
		// for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
      		// delete (*it);
	}
}

void    Server::setPort( int port ) { _port = port; }

int Server::getPort( void ) const { return ( _port ); }

void    Server::setPassword( std::string str ) { _password = str; }

std::string Server::getPassword( void ) const { return ( _password ); }

Client		*Server::getClient(std::string name) {
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		Client *tmp = (*it);
		if (name == tmp->getNick()) {
			return tmp;
		}
	}
	return NULL;
}

int			Server::getClientIndex(Client *client) {
	int	i = 0;

	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		Client *tmp = (*it);
		if (client->getNick() == tmp->getNick()) {
			return i;
		}
		i++;
	}
	return -1;
}

bool	Server::isClient(Client *cli ) {
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		Client *tmp = (*it);
		if (tmp->getNick() == cli->getNick())
			return (true);
	}
	return (false);
}

bool	Server::isClient(std::string cli ) {
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		Client *tmp = (*it);
		if (tmp->getNick() == cli)
			return (true);
	}
	return (false);
}

int    Server::init( void ) {
	if ( _port < 1 || _port > 65535 )
		return ( printErr( "Port not within range, please input a port within [1 - 65535]\n" ) );
	
	std::cout << "Port : " BLU << _port << CRESET " and Password : " BLU << _password << CRESET << std::endl;
	
	_server_socket = socket( AF_INET6, SOCK_STREAM, 0);
	if ( _server_socket < 0 )
		return ( printErr( "Error creating socket" ) );
		
	_server_address.sin6_family = AF_INET6;
	_server_address.sin6_addr = in6addr_any;
	_server_address.sin6_port = htons( _port );
	_server_address.sin6_flowinfo = 0;
	_server_address.sin6_scope_id = 0; 
	int	n = 1;
	if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &n, sizeof(int)) < 0)
    	return ( printErr( "Error option in socket" ) );
	if (fcntl(_server_socket, F_SETFL, O_NONBLOCK))
		return ( printErr( "fcntl error") );
	// BINDING SOCKET AND LISTENING TO SOCKET CHECK
	if ( bind(_server_socket, ( struct sockaddr* ) &_server_address, sizeof( _server_address ) ) < 0) 
		return ( printErr( "Error binding socket" ) );
	if ( listen( _server_socket, 10 ) < 0 )
		return ( printErr( "Error listening to socket" ) );


	_localhost = "localhost";
	_nick = "nick";
	
	_pfds[0].fd = _server_socket;
	_pfds[0].events = POLLIN;
	(void)_num_open_fds;
	(void)_addrlen;
	(void)_currentClient;
	return (0);
}

void	Server::delOp( Client *client ) {
    std::vector<Client *>::iterator	it = _ops.begin();
    for (; it != _ops.end(); it++) {
        Client *tmp = (*it);
        if (tmp != NULL && tmp->getNick() == client->getNick()) {
            _ops.erase(it);
            break ;
        }
    }
}

bool	Server::isOp( std::string name ) { 
    std::vector<Client *>::iterator	it = _ops.begin();
    for (; it != _ops.end(); it++) {
        Client *tmp = (*it);
        if (tmp != NULL && tmp->getNick() == name)
            return true;
    }
    return false;
}

bool	Server::isOp( Client *client ) {
    std::vector<Client *>::iterator	it = _ops.begin();
    for (; it != _ops.end(); it++) {
        Client *tmp = (*it);
        if (tmp != NULL && tmp->getNick() == client->getNick())
            return true;
    }
    return false;
}

int	Server::_callCommands(Client *client, CommandManager *cmd, int i) {
	if ( i <= static_cast<int>( getClientSize() ) && !client->getBuffer().empty() ) {
					
		std::string buffer1 = client->getBuffer();
			
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
				cmd->pass(tmpRest, client);
			else if ( tmp == "NICK" )
				cmd->nick(tmpRest, client);
			else if ( tmp == "PING" )
				cmd->ping(client);
			else if (tmp == "USER")
				cmd->user(tmpRest, client);
			else if (tmp == "JOIN")
				cmd->join(tmpRest, client);
			else if (tmp == "INVITE")
				cmd->invite(tmpRest, client);
			else if (tmp == "MODE")
				cmd->mode(tmpRest, client);
			else if (tmp == "TOPIC")
				cmd->topic(tmpRest, client);
			else if (tmp == "PART")
				cmd->part(tmpRest, client);
			else if (tmp == "QUIT") {
				cmd->quit(i);
				return (1);
			} else if (tmp == "PRIVMSG")
				cmd->privmsg(tmpRest, client);
			else if (tmp == "KICK")
				cmd->kick(tmpRest, client);
			else if (tmp == "NOTICE")
				cmd->notice(tmpRest, client);
			else if (tmp == "OPER")
				cmd->oper(tmpRest, client);
			else if (tmp == "WHOIS")
				cmd->whois(tmpRest, client);
			else if (tmp == "CAP")
				std::cout << "Cap command found, skipping" << std::endl;
			else
				client->send_msg(ERR_NOTIMPLEMENTED( client->getHost(), tmp));
			
			if ( client->getHs() == false ) {
			
				if ( !client->getPass().empty() && !client->getNick().empty() && !client->getHost().empty() ) {
					
					if ( client->getPass() != _password ) {
						std::cout << GRNHB << _password << BLUHB << client->getNick() << CRESET << std::endl;
						client->send_msg(ERR_PASSWDMISMATCH( _localhost, client->getNick() ) );

						//faire la deconnexion du client
						std::cout << "Client #" << i << " timeout." << std::endl;
						close( _pfds[i].fd );
						eraseClient( i - 1 );
						for (int j = i; j + 1 <= _num_open_fds + 1; j++)
							_pfds[j] = _pfds[j + 1];
						_num_open_fds--;
					}
					else {

						// HANDSHAKE
						client->send_msg(RPL_WELCOME( client->getHost(), client->getNick() ) );
						client->send_msg(RPL_YOURHOST( client->getHost() ) );
						client->send_msg(RPL_CREATED( client->getHost() ) );
						client->send_msg(RPL_MYINFO( client->getHost() ) );
						client->setHs(true);
					}
				}
			}
			tmp.clear();
			tmpRest.clear();
		}
		client->setBuffer(const_cast<char*>( buffer1.c_str() ) );
		buffer1.clear();
		bzero(_buffer, 1025);
	}
	return (0);
}

bool isFeur(std::string &sent) {
	
    std::string lowerSent;
    std::transform(sent.begin(), sent.end(), std::back_inserter(lowerSent), ::tolower);
    const std::string word = "quoi";
    const std::string delimiters = " .,;:!?\n\t\r";
    std::size_t pos = lowerSent.rfind(word);

    while (pos != std::string::npos) {

        const std::size_t endpos = pos + word.length();
        if (endpos == lowerSent.length() || delimiters.find(lowerSent[endpos]) != std::string::npos) {
            return true;
        }
        pos = lowerSent.rfind(word, pos - 1);
    }
    return false;
}

bool isNotice(std::string &sent) {
   	return !(sent.find("NOTICE") == sent.npos);
}


int	Server::_polloutHandler(Client *client) {
					// LIMITE DE SEND DE 1024CHAR;
	std::map<std::string, Canal *> canals = _canalManager->GetChannels();
	int i = 0;
	for (std::map<std::string, Canal *>::iterator it = canals.begin(); it != canals.end(); it++) {
		Canal *tmp = (*it).second;
		if (!tmp->hasClient(client))
			continue ;
		for (std::vector<Message *>::iterator it_msg = tmp->waitingMessages.begin(); it_msg != tmp->waitingMessages.end(); it_msg++) {
			Message *msg = (*it_msg);
			if (msg->isClient(client) && msg->getSender().getNick() != client->getNick()) {
				std::cout << "msg = " << msg->getMessage() << std::endl;
				std::string str = msg->getMessage();
				// if (tmp->getName()[0] == '#') {
					// std::cout << "ici mec " << std::endl; 
					client->send_msg(msg->getMessage());
					if (isFeur(str) && !isNotice(str)) {
						
						client->send_msg(":bot PRIVMSG " + tmp->getName() + " " + "feur lol" + "\r\n");
						if ( i == 0 )
							msg->getSender().send_msg(":bot PRIVMSG " + tmp->getName() + " " + "feur lol" + "\r\n"), i++;
					}
					// if (tmp->isOp(msg->getSender().getNick()))
					// 	client->send_msg(":" + msg->getSender().getNick() + " PRIVMSG " + tmp->getName() + " " + msg->getMessage() + "\r\n");
					// else
					// 	client->send_msg(":" + msg->getSender().getNick() + " PRIVMSG " + tmp->getName() + " " + msg->getMessage() + "\r\n");
					// std::cout << ":" + msg->getSender().getNick() + " PRIVMSG " + tmp->getName() + " " + msg->getMessage() + "\r\n" << std::endl;
				// }
				// else {
				// 	std::cout << "PRIV_MSG : " << RPL_PRIVMSG(getClient(tmp->getName())->getPrefix(), client->getNick(), msg->getMessage()) << std::endl;
				// 	client->send_msg(RPL_PRIVMSG(getClient(tmp->getName())->getPrefix(), client->getNick(), msg->getMessage()));
				// }
				//std::cout << "USER JOIN : " << client->getFullName() << " et " << client->getHost() << std::endl; 
				for (std::vector<Client *>::iterator cli_it = msg->clients.begin(); cli_it != msg->clients.end(); cli_it++) {
					Client *cli = (*cli_it);
					if (cli->getNick() == client->getNick())
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
	return (0);
}

int	Server::start( void ) {
	CommandManager	*cmd = getCommandManager();

	while ( quit ) {
		
		signal(SIGINT, Server::signal_callback_handler);

		if (!quit)
			std::cout << "JE DOIT QUITTER" << std::endl;
		int poll_count = poll(_pfds, _num_open_fds + 1, 10);

		if (poll_count == -1)
			return ( printErr( "Error creating Poll" ) );

		if (_pfds[0].revents & POLLIN ) {
			if ( ( _currentClient = accept( _server_socket, reinterpret_cast< sockaddr *>(&_server_address), &_addrlen ) ) < 0 )
				return ( printErr( "Can't accept" ) );
			else {
				pushClient(new Client( _currentClient ));
				_num_open_fds++;
				_pfds[_num_open_fds].fd = _currentClient;
				_pfds[_num_open_fds].events = POLLIN | POLLOUT;
				_pfds[_num_open_fds].revents = 0;
				std::cout << "Accepted client #" << _num_open_fds << std::endl;
			}
		}

		for (int i = 1; i < _num_open_fds + 1; i++) {
			Client *client = getClient(i - 1);
			if (client->getHasTime() == true ) {
				
				std::time_t tmpTime = std::time(NULL);

				if ( tmpTime - client->getTime() > 90 ){
					
					std::cout << "Client #" << i << " timout." << std::endl;
					close( _pfds[i].fd );
					eraseClient( i - 1 );
					for (int j = i; j + 1 <= _num_open_fds + 1; j++)
						_pfds[j] = _pfds[j + 1];
					_num_open_fds--;
				}
			}
			// _pfds[i].events = POLLIN | POLLOUT; 
			if (_pfds[i].revents & POLLIN ) {
				
				int valread = recv( _pfds[i].fd, &_buffer, 1024, 0 );
				
				if ( valread < 0 )
					std::cerr << "Error reading from client #" << i << std::endl;
				else if ( valread == 0) {
					// FAIRE QUITTER TOUT SES CANNAUX
					std::cout << "Client #" << i << " disconnected." << std::endl;
					close( _pfds[i].fd );
					eraseClient( i - 1 );
					for (int j = i; j + 1 <= _num_open_fds + 1; j++)
						_pfds[j] = _pfds[j + 1];
					_num_open_fds--;
				}
				else {
					
					std::cout << YEL "je suis le client " CRESET << i << std::endl;
					std::cout << _buffer << std::endl;
					client->addBuffer( _buffer );
				}
			}

			if (i <= _num_open_fds &&  _pfds[i].revents & POLLOUT )
				_polloutHandler(client);
			if (i <= _num_open_fds && _callCommands(client, cmd, i))
				break ;
		}
	}

	return (0);
}
