/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 15:16:33 by dasereno          #+#    #+#             */
/*   Updated: 2023/04/12 17:32:58 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "ft_irc.h"

class Message {

	private:
		std::string	_message;
		Client		&_sender;

    public: 
		std::vector<Client *> clients;
		Message( void );
		Message (std::string msg, Client &sender): _message(msg), _sender(sender) { };
		// Message( const Message& );
		~Message( void ) { return ;};

		void		setMessage(std::string msg) { _message = msg; };
		std::string	getMessage(void) { return (_message); };

		Client		&getSender(void) { return (_sender); };

		bool	isClient(Client *cli ) {
			for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
				Client *tmp = (*it);
				if (tmp->getNick() == cli->getNick())
					return (true);
			}
			return (false);
		}

		bool	isClient(std::string cli ) {
			for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
				Client *tmp = (*it);
				if (tmp->getNick() == cli)
					return (true);
			}
			return (false);
		}

		Message&	operator=( const Message& ref) { 
			if ( this == &ref )
				return ( *this );
			*this = ref;
			return ( *this );
		};
};
