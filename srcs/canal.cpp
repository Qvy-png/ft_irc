/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canal.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 17:39:26 by dasereno          #+#    #+#             */
/*   Updated: 2023/03/14 20:35:31 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/canal.hpp"

Canal& Canal::operator=( const Canal& ref ) {

    if ( this == &ref )
        return ( *this );
    *this = ref;
    return ( *this );
}

bool Canal::operator==( const Canal& rhs ) {
	return (this->_name == rhs.getName());
}

// Canal::Canal( const Canal& ref ) { *this = ref; }


Client *Canal::getClient(std::string name) {
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
        Client *tmp = *it;
        if (tmp->getNick() == name)
            return (tmp);
    }
    return (NULL);
}