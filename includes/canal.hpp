/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canal.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 16:58:43 by dasereno          #+#    #+#             */
/*   Updated: 2023/03/13 19:28:07 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.h"

class Client;
class Message;

class Canal {

	private:
		int	_mode;
		std::string	_name;
		std::string	pass;
		Client		&_op;

    public: 
		std::vector<Client *> clients;
		std::vector<Message > waitingMessages;

		// Canal( void ) { return ; };
		// Canal( std::string name ): _name(name) { };
		Canal( std::string name, Client & op ): _name(name), _op(op) { };
		// Canal( Canal const& ref );
		~Canal( void ) { return ;};

		std::string getName( void ) const { return (_name); };
		void setName( std::string name ) { _name = name; };

		Canal&	operator=( const Canal& rhs);
		bool	operator==( const Canal& rhs );
};
