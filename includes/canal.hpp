/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canal.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 16:58:43 by dasereno          #+#    #+#             */
/*   Updated: 2023/04/03 14:34:17 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.h"

class Client;
class Message;
class CanalManager;

class Canal {

	private:
		// int	_mode;
		std::string		_name;
		std::string		pass;
		Client			&_op;
		CanalManager	*_canalManager;

    public: 
		std::vector<Client *> clients;
		std::vector<Message *> waitingMessages;

		Canal( std::string name, Client & op, CanalManager *cm ): _name(name), _op(op), _canalManager(cm) { };
		~Canal( void ) { return ;};

		std::string getName( void ) const { return (_name); };
		void setName( std::string name ) { _name = name; };

		Client *getClient(std::string name);

		void	deleteClient(Client *);
		bool	hasClient(Client *);
		bool	hasClient(std::string);
		void	pushClient(Client *client) { clients.push_back(client); };
		Client &getOp( void ) { return _op; };

		Canal&	operator=( const Canal& rhs);
		bool	operator==( const Canal& rhs );
};
