/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:37:21 by rdel-agu          #+#    #+#             */
/*   Updated: 2023/02/16 15:48:59 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_H
# define FT_IRC_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include "color.hpp"
#include "ft_irc.hpp"

#endif