/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:48:31 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/04/13 18:08:51 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

#include "ft_irc.h"
#include "color.hpp"

#define MAX_CLIENTS 128

class CanalManager;
class CommandManager;
class Client;

class Server {

	private:
		int								_num_open_fds;
		struct	pollfd					_pfds[MAX_CLIENTS];
		std::vector<Client *>			_clients;
		int			 					_server_socket;
		struct sockaddr_in6				_server_address;
		socklen_t						_addrlen;
		CanalManager					*_canalManager;
		int		 						_port;
		std::string						_password;
		int								_currentClient;
		char							_buffer[1025];
		std::string						_localhost;
		std::string 					_nick;
		CommandManager					*_commandManager;

		int	_callCommands(Client *client, CommandManager *cmd, int i);
		int	_polloutHandler(Client *client);
    public: 

		Server( void );
		Server( int port, std::string password);
		Server( const Server& );
		~Server( void );

		Server&	operator=( const Server& );
    
		void			setPort( int );
		void			setPassword( std::string );
		
		CanalManager	*getCanalManager( void ) { return _canalManager; };
		CommandManager	*getCommandManager( void ) { return _commandManager; };
		int				getPort( void ) const;
		std::string 	getPassword( void ) const;
		Client			*getClient(int i) { return _clients[i]; };
		Client			*getClient(std::string name);
		int				getClientIndex(Client *client);
		int				getNumOpenFds( void ) { return _num_open_fds; };
		std::string		getLocalhost( void ) { return _localhost; };
		struct pollfd	&getPfds(int i) { return _pfds[i]; };

		int				printErr( std::string str ) { std::cerr << REDHB << str << CRESET; return ( 1 ); }
		
		std::vector<Client *>	getClients(void) { return _clients; };
		void			pushClient(Client *client) { _clients.push_back(client); };
		void			eraseClient(int pos) { _clients.erase(_clients.begin() + pos); };
		size_t			getClientSize(void) { return _clients.size(); };
		bool			isClient(Client *cli );
		bool			isClient(std::string cli );
		void			setNumOpenFds( int fds ) { _num_open_fds = fds; };

		int	send_msg(std::string msg, int sfd) {
			int res;
			
			res = send(sfd, msg.c_str(), msg.length(), MSG_CONFIRM);
			return (res);
		} // A DELETE DES QUE PLUS APPELE

		int				init ( void );
		int				start ( void );

};

#endif