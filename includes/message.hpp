/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 15:16:33 by dasereno          #+#    #+#             */
/*   Updated: 2023/03/13 19:28:28 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "ft_irc.h"

class Message {

	private:
		std::string	_message;

    public: 
		std::vector<Client *> clients;
		// Message( void );
		Message (std::string msg): _message(msg) { };
		// Message( const Message& );
		~Message( void ) { return ;};

		void		setMessage(std::string msg) { _message = msg; };
		std::string	getMessage(void) { return (_message); };

		Message&	operator=( const Message& );
};
