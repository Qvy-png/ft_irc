/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:48:31 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/02/03 14:59:36 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

#include "ft_irc.h"

class Server {

	private:

		int		 	_port;
		std::string	_password;
		
    public: 

		Server( void );
		Server( int port, std::string password);
		Server( const Server& );
		~Server( void );

		Server&	operator=( const Server& );
    
		void		setPort( int );
		void		setPassword( std::string );
		int			getPort( void ) const;
		std::string getPassword( void ) const;
		
};

#endif