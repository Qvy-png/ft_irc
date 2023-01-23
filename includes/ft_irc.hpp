/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:48:31 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/01/23 17:57:48 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

#include "ft_irc.h"

class Server {

	private:

		std::string _port;
		std::string	_password;
		
    public: 

		Server( void );
		Server( std::string, std::string );
		Server( const Server& );
		~Server( void );

		Server&	operator=( const Server& );
    
		void		setPort( std::string );
		void		setPassword( std::string );
		std::string	getPort( void ) const;
		std::string getPassword( void ) const;
		
};

#endif