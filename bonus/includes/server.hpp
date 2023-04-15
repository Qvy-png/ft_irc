/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:26:47 by dasereno          #+#    #+#             */
/*   Updated: 2023/04/12 17:02:44 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "ft_irc.hpp"

class Server {
	private:

		int								port;
		std::string 					password;
	public:
		Server (std::string port, std::string pass): _addrlen(sizeof(server_address)), _canalManager(new CanalManager()), _port(port), _password(pass) {};
}