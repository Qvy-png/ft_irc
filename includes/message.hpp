/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 15:16:33 by dasereno          #+#    #+#             */
/*   Updated: 2023/03/14 18:33:47 by dasereno         ###   ########.fr       */
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

		Message&	operator=( const Message& ref) { 
			if ( this == &ref )
				return ( *this );
			*this = ref;
			return ( *this );
		};
};
