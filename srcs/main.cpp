/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:36:41 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/01/23 18:03:30 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

int	printErr( std::string str ) { std::cerr << str; return ( 1 ); }

int	main( int argc, char **argv ) {

	if ( argc != 3 )
		return ( printErr( RED "Wrong input, please use the following form : ./ircserv <port> <password>\n" CRESET ) );
	
	return ( 0 );
}