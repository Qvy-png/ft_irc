/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 17:58:14 by dasereno          #+#    #+#             */
/*   Updated: 2023/04/16 18:29:32 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "ft_irc.h"

class Server;

class CommandManager {
	private:
		Server					*_server;
		void	_privmsgClient(std::string msg, CanalManager *canalManager, Client *cli, std::string receiver);
		void	_noticeClient(std::string msg, CanalManager *canalManager, Client *cli, std::string receiver);
	public:
		CommandManager	(Server *server): _server(server) {};
		~CommandManager (void) {};

		void	pass(std::string str, Client *cli);
		void	nick(std::string str, Client *cli);
		void	ping(Client *cli);
		void	user(std::string str, Client *cli);
		void	join(std::string str, Client *cli);
		void	invite(std::string str, Client *cli);
		void	mode(std::string str, Client *cli);
		void	topic(std::string str, Client *cli);
		void	part(std::string str, Client *cli);
		void	quit(int i);
		void	quit(Client *cli);
		void	privmsg(std::string str, Client *cli);
		void	kick(std::string str, Client *cli);
		void	oper(std::string str, Client *cli);
		void	notice(std::string str, Client *cli);
		void	whois(std::string str, Client *cli);
};
