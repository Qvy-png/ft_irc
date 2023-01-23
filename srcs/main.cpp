/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:36:41 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/01/23 17:59:37 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

int	printErr( std::string str ) { std::cerr << str << std::end; }

int	main( int argc, char **argv ) {

	if ( argc != 4 )
		return ( printErr( REDHB "Wrong input, please use the following form : ./ircserv <port> <password>" CRESET ) );

	return ( 0 );
}