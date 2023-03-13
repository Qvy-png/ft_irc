/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canal.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 17:39:26 by dasereno          #+#    #+#             */
/*   Updated: 2023/03/13 18:36:42 by dasereno         ###   ########.fr       */
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