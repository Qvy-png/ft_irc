/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:24:36 by dasereno          #+#    #+#             */
/*   Updated: 2023/04/15 15:12:25 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

void	CommandManager::pass(std::string str, Client *cli) {
	cli->setPass( str );
}

void	CommandManager::nick(std::string str, Client *cli) {

	bool NickIsFree;

	NickIsFree = true;
	for (int j = 0; j < _server->getNumOpenFds(); j++) {
		
		if ( _server->getClient(j)->getNick() == str ) {
			
			cli->send_msg(ERR_NICKNAMEINUSE( _server->getLocalhost(), str));
			NickIsFree = false;
		}
	}
	if ( NickIsFree == true ) {

		std::string name = cli->getNick() + "!" + cli->getFullName() + "@" + cli->getHost();
		
		cli->setNick(str);
		cli->send_msg(NICK( name, str ));
	}
}

void	CommandManager::ping(Client *cli) {
	std::time_t time = std::time(NULL);

	if (cli->getHasTime() == true )
		std::cout << BLU <<  time - cli->getTime() << CRESET << std::endl;
	cli->setTime( time );
	std::cout << cli->getTime() << std::endl;
	cli->send_msg(PONG());
	cli->setHasTime( true );
}

void	CommandManager::user(std::string str, Client *cli) {
	std::stringstream	userSplitter( str );
	std::string			splitTmp;
	int					j = 0;

	while ( getline( userSplitter, splitTmp, ' ') ) {
		
		if ( j == 0 )
			cli->setFullName( splitTmp );
		if ( j == 2 )
			cli->setHost( splitTmp );
		j++;
	}
}

void	CommandManager::join(std::string str, Client *cli) {
	std::cout << "Client #" << cli->getNick() << " try to join " << str << std::endl;
	std::string name = str;
	std::string pass = "";

	if (str.find(' ') != str.npos) {
		name = str.substr(0, str.find(' '));
		pass = str.substr(str.find(' ') + 1, str.size());
	}
	// split str en 2
	if (name[0] != '#' && name[0] != '&')
	{
		cli->send_msg(ERR_BADCHANMASK(name));
		return ;
	}
	CanalManager *canalManager = _server->getCanalManager();
	Canal *canal = canalManager->GetChannel(name);
	if (canal == NULL) {
		canal = canalManager->CreateChannel(str, cli);
		std::cout << "chanoper is : " << canal->getOp().getNick() << std::endl;
		std::string users ("");
		std::vector<Client *> nicknames = canal->getClients();
		for (std::vector<Client *>::iterator it = nicknames.begin(); it != nicknames.end(); it++) {
			Client *tmp = *it;
			if (canal->isOp(tmp->getNick()))
				users.append("@" + tmp->getNick() + " ");
			else
				users.append(tmp->getNick() + " ");
		}
		cli->send_msg(RPL_NAMREPLY(cli->getNick(), canal->getName(), users));
		std::cout << "RPL_NAMREPLY: " << RPL_NAMREPLY(cli->getNick(), canal->getName(), users) << std::endl;
		cli->send_msg(RPL_ENDOFNAMES(cli->getNick(), canal->getName()));
		std::cout << "RPL_ENDOFNAMES : " << RPL_ENDOFNAMES(cli->getNick(), canal->getName()) << std::endl;
		canal->broadcast(RPL_JOIN(cli->getPrefix(), canal->getName()));
		std::cout << "RPL_JOIN : " << RPL_JOIN(cli->getPrefix(), canal->getName()) << std::endl;
		cli->send_msg(RPL_NOTOPIC(cli->getNick(), name));
	}
	if (canal->getModeK()) {
		if (pass.size() != 0 && pass == canal->getPass()) { }
		else {
			cli->send_msg(ERR_BADCHANNELKEY(cli->getNick(), name));
			std::cout << "pass tested = " << pass << std::endl;
			return ;
		}
	}
	if (canal->getModeB() && canal->isBanned(cli)) {
		cli->send_msg(ERR_BANNEDFROMCHAN(cli->getNick(), canal->getName()));
		std::cout << "YOU ARE BANNED" << std::endl;
		return ;
	}
	if (canal->getModeI() && !canal->isInvited(cli->getNick())) {
		cli->send_msg(ERR_INVITEONLYCHAN(cli->getNick(), canal->getName()));
		std::cout << "You are not invited to this channel" << std::endl;
		return ;
	}

	if (canal->getModeL() && canal->getNbClient() < canal->getMaxClient()) { }
	else if (canal->getModeL()) {
		cli->send_msg(ERR_CHANNELISFULL(cli->getNick(), canal->getName()));
		return ;
	}
		
	if (!canal->hasClient(cli)) {
		canal->pushClient(cli);
		std::string users ("");
		std::vector<Client *> nicknames = canal->getClients();
		for (std::vector<Client *>::iterator it = nicknames.begin(); it != nicknames.end(); it++) {
			Client *tmp = *it;
			if (canal->isOp(tmp->getNick()))
				users.append("@" + tmp->getNick() + " ");
			else
				users.append(tmp->getNick() + " ");
		}
		cli->send_msg(RPL_NAMREPLY(cli->getNick(), canal->getName(), users));
		std::cout << "RPL_NAMREPLY: " << RPL_NAMREPLY(cli->getNick(), canal->getName(), users) << std::endl;
		cli->send_msg(RPL_ENDOFNAMES(cli->getNick(), canal->getName()));
		std::cout << "RPL_ENDOFNAMES : " << RPL_ENDOFNAMES(cli->getNick(), canal->getName()) << std::endl;
		canal->broadcast(RPL_JOIN(cli->getPrefix(), canal->getName()));
		std::cout << "RPL_JOIN : " << RPL_JOIN(cli->getPrefix(), canal->getName()) << std::endl;
		if (canal->getTopic().size() > 0)
			cli->send_msg(RPL_TOPIC(cli->getNick(), name, canal->getTopic()));
		else
			cli->send_msg(RPL_NOTOPIC(cli->getNick(), name));
	}
}

void	CommandManager::invite(std::string str, Client *cli) {
	std::string channel = str.substr(str.find(' ') + 1, str.size());
	CanalManager	*canalManager = _server->getCanalManager();
	Canal	*canal = canalManager->GetChannel(channel);
	std::string invited = str.substr(0, str.find(' '));

	if (!canal->getModeI())
		return ;
	if (!canal) {
		cli->send_msg(ERR_NOSUCHCHANNEL(cli->getNick(), channel));
		return ;
	}
	if (!canal->getClient(cli->getNick())) {
		cli->send_msg(ERR_NOTONCHANNEL(cli->getNick(), channel));
		return ;
	}
	if (!canal->isOp(cli)) {
		cli->send_msg(ERR_CHANOPRIVSNEEDED(cli->getNick(), channel));
		return ;
	}
	if (canal->getClient(invited)) {
		cli->send_msg(ERR_USERONCHANNEL(cli->getNick(), invited, canal->getName()));
		return ;
	}
	canal->addInvite(invited);
	cli->send_msg(RPL_INVITING(cli->getNick(), invited, channel));
}

void	CommandManager::mode(std::string str, Client *cli) {
	std::vector<std::string>	args = split(str, " ");

	if (args.size() < 2)
	{
		// NOT ENOUGH ARGS
		return ;
	}
	if (args[1][0] != '+' && args[1][0] != '-') {
		// BAD ARGS
		return ;
	}
	if (args[0][0] == '#' || args[0][0] == '&') { // CHANNEL MODE
		CanalManager	*canalManager = _server->getCanalManager();
		Canal *canal = canalManager->GetChannel(args[0]);
		if (!canal) {
			cli->send_msg(ERR_NOSUCHCHANNEL(cli->getNick(), args[0]));
			return ;
		}
		if (!canal->hasClient(cli)) {
			// send_msg
		}
		bool block = false;
		bool sign = false; // true = '+' / false = '-'
		// if (args[1][0] == '+') { // ADD MODES
		if (args[1][0] != '+' && args[1][0] != '-') {
			cli->send_msg(ERR_UNKNOWNMODE(cli->getNick(), args[1][0]));
			return ;
		}
		for (int j = 0; args[1][j]; j++) {
			std::cout << "ICI" << std::endl;
			if (args[1][j] == '+') {
				sign = true;
				continue ;
			} else if (args[1][j] == '-') {
				sign = false;
				continue ;
			}
			if (sign) { // '+'
				switch (args[1][j])
				{
				case 'k':
					if (canal->getModeK() == true || args.size() < 3 || !canal->isOp(cli))
						break ;
					canal->setPass(args[2]);
					canal->setModeK(true);
					std::cout << "Pass set to : " << args[2] << std::endl; 
					block = true;
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+k", args[2]));
					std::cout << args[2] << std::endl;
					break;
				case 'b': // IF ARGS size == 2 => PRINT BANNED USERS
					if (args.size() < 3 || !canal->isOp(cli))
						break ;
					canal->banClient(args[2]);
					canal->printBanned();
					canal->setModeB(true);
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+b", args[2]));
					block = true;
					std::cout << "mode B activated" << std::endl;
					break ;
				case 'o' : // IF ARGS size == 2 => PRINT OPERATOR USERS
					if (args.size() < 3 || !canal->isOp(cli))
						break ;
					if (canal->hasClient(args[2])) {
						canal->addOp(canal->getClient(args[2]));
					} else
						break ;
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+o", args[2]));
					block = true;
					break ;
				case 'i' :
					if (canal->getModeI() || !canal->isOp(cli)) {
						std::cout << canal->getOp().getNick() << std::endl;
						std::cout << cli->getNick() << std::endl;
						std::cout << "yo c ici ca quitte" << std::endl;
						std::cout << canal->isOp(cli) << std::endl;
						break ;
					}
					canal->setModeI(true);
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+i", ""));
					break ;
				case 'm' :
					if (canal->getModeM() || !canal->isOp(cli))
						break ;
					canal->setModeM(true);
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+m", ""));
					break ;
				case 'l' :
					if (!canal->isOp(cli) || args.size() < 3 || !str_isnum(args[2]))
						break ;
					canal->setModeL(true);
					canal->setMaxClient(atoi(args[2].c_str()));
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+l", args[2]));
					block = true;
					break ;
				case 'v' :
					if (!canal->isOp(cli) || args.size() != 3) // IF ARGS size == 2 => PRINT VOICED USERS
						break ;
					canal->setModeV(true);
					if (canal->hasClient(args[2])) {
						canal->addVoiced(canal->getClient(args[2]));
					}
					block = true;
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+v", args[2]));
					break ;
				default:
					cli->send_msg(ERR_UNKNOWNMODE(cli->getNick(), args[1][j]));
					return ;
				}
			} else {
				switch (args[1][j])
				{
				case 'k':
					if (!canal->isOp(cli))
						break ;
					canal->setPass("");
					canal->setModeK(false);
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-k", ""));
					break;
				case 'b': // IF ARGS size == 2 => PRINT BANNED USERS
					if (!canal->isOp(cli))
						break ;
					if (args.size() != 3) {
						canal->setModeB(false);
						cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-b", ""));
						canal->resetBanned();
					} else if (args.size() == 3) {
						canal->delBanned(args[2]);
						cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-b", args[2]));
						block = true;
					}
					break ;
				case 'o' : // IF ARGS size == 2 => PRINT OPERATOR USERS
					if (!canal->isOp(cli))
						break ;
					if (args.size() == 3 && canal->hasClient(args[2])) {
						canal->delOp(canal->getClient(args[2]));
						cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-o", args[2]));
						block = true;
					} else 
						break ;
					break ;
				case 'i' :
					if (!canal->getModeI() || !canal->isOp(cli))
						break ;
					canal->setModeI(false);
					canal->resetInvited();
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-i", ""));
					break ;
				case 'm' :
					if (!canal->getModeM() || !canal->isOp(cli))
						break ;
					canal->setModeM(false);
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-m", ""));
					break ;
				case 'l' :
					if (!canal->getModeL() || !canal->isOp(cli))
						break ;
					canal->setModeL(false);
					canal->setMaxClient(0);
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-l",  ""));
					block = true;
					break ;
				case 'v' :
					if (!canal->isOp(cli)) // IF ARGS size == 2 => PRINT VOICED USERS
						break ;
					if ( args.size() < 3) {
						canal->setModeV(false);
						cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-v", ""));
						canal->resetVoiced();
					} else if (args.size() == 3 && canal->getClient(args[2])) {
						canal->delVoiced(canal->getClient(args[2]));
						cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-v", args[2]));
						block = true;
					}
					break ;
				default:
					cli->send_msg(ERR_UNKNOWNMODE(cli->getNick(), args[1][j]));
					return ;
				}
			}
			if (block)
					return ;
		}
		// }
	} else { // USER MODE
		// if (args[2][0] == '+') { // ADD MODES
		// 	for (int i = 1; args[2][i]; i++) {
		// 		if (user_modes.find(args[2][i], 0) == chan_modes.npos)
		// 		{
		// 			// BAD MODE
		// 			return ;
		// 		}
		// 	}
		// } else if (args[2][0] == '-') { // REMOVE MODES

		// }
	}
}

void	CommandManager::topic(std::string str, Client *cli) {
	CanalManager *canalManager = _server->getCanalManager();
	if (str.find(":") == str.npos) { // TODO
		// get topic
	} else if (str.find(":") == str.size() || str.find(":") + 1 == str.size()) {
		// clear topic
		std::string channel = str.substr(0, str.find(":") - 1);
		Canal *canal = canalManager->GetChannel(channel);
		if (channel.size() == 0 || (channel[0] != '#' && channel[0] != '&') || !canal) {
			cli->send_msg(ERR_NOSUCHCHANNEL(cli->getNick(), channel));
			return ;
		}
		if (!canal->hasClient(cli->getNick())) {
			cli->send_msg(ERR_NOTONCHANNEL(cli->getNick(), channel));
			return ;
		}
		if (!canal->isOp(cli)) {
			cli->send_msg(ERR_CHANOPRIVSNEEDED(cli->getNick(), channel));
			return ;
		}
		canal->broadcast(RPL_NOTOPIC(cli->getNick(), channel));
		canal->setTopic("");
		return ;
	} else {
		std::string channel = str.substr(0, str.find(":")- 1);
		std::string topic = str.substr(str.find(":") + 1, str.size());
		std::cout << "channel : " << channel << std::endl;
		std::cout << "topic : " << topic << std::endl;
		Canal *canal = canalManager->GetChannel(channel);
		if (channel.size() == 0 || (channel[0] != '#' && channel[0] != '&') || !canal) {
			cli->send_msg(ERR_NOSUCHCHANNEL(cli->getNick(), channel));
			return ;
		}
		if (!canal->hasClient(cli->getNick())) {
			cli->send_msg(ERR_NOTONCHANNEL(cli->getNick(), channel));
			return ;
		}
		if (!canal->isOp(cli)) {
			cli->send_msg(ERR_CHANOPRIVSNEEDED(cli->getNick(), channel));
			return ;
		}
		canal->setTopic(topic);
		canal->broadcast(RPL_TOPIC(cli->getNick(), channel, topic));
	}
}

void	CommandManager::part(std::string str, Client *cli) { // TO DO: ALL ERRORS
	std::cout << "Client #" << cli->getNick() << " PART channel "<< str << std::endl;
	std::string args = str.substr(str.find(':') + 1, str.size());
	std::vector<std::string> channels = split(args, " ");
	CanalManager	*canalManager = _server->getCanalManager();
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
		std::string canalName = (*it);
		Canal *canal = canalManager->GetChannel(canalName);
		if (canal && canal->hasClient(cli) && canal->getName()[0] == '#')
		{
			canal->broadcast(RPL_PART(cli->getPrefix(), canal->getName()));
			canal->deleteClient(cli);
		}
	}
}

void	CommandManager::quit(int i) { // ERROR HANDLING???
	close( _server->getPfds(i).fd );
	_server->eraseClient(i - 1);
	// client.erase( client.begin() + i - 1 );
	for (int j = i; j + 1 <= _server->getNumOpenFds() + 1; j++)
	{
		_server->getPfds(j) = _server->getPfds(j + 1);
	}
}

void	CommandManager::_privmsgClient(std::string str, CanalManager *canalManager, Client *cli)
{
	std::string receiver = str.substr(0, str.find(' '));
	if (!_server->isClient(receiver)) {
		cli->send_msg(ERR_NOSUCHNICK(cli->getNick(), receiver));
		return ;
	}
	std::string msg = str.substr(str.find(':'), str.size());
	Message *newMsg = new Message (msg, *cli);
	Canal *newCanal = canalManager->CreateChannel(cli->getNick(), cli);
	newCanal->pushClient(_server->getClient(receiver));
	newCanal->waitingMessages.push_back(newMsg);
	(*newCanal->waitingMessages.rbegin())->clients = newCanal->clients; 
	std::cout << "Client :" << cli->getNick() << " send a message prive." << std::endl;
}

void	CommandManager::privmsg(std::string str, Client *cli) {
	std::string channel = str.substr(0, str.find(' '));
	CanalManager	*canalManager = _server->getCanalManager();
	Canal	*canal = canalManager->GetChannel(channel);
	std::cout << "channel = " << channel << std::endl;
	if (channel[0] == '#' && !canal) {
		cli->send_msg(ERR_NOSUCHCHANNEL(cli->getNick(), channel));
		return ;
	}
	if (!canal || (canal && channel[0] != '#') || channel[0] != '#') {
		return (_privmsgClient(str, canalManager, cli));
	}
	if (!canal->hasClient(cli))
		return ;
	if (canal->getModeM() && !canal->isVoiced(cli)) {
		cli->send_msg(ERR_CANNOTSENDTOCHAN(cli->getNick(), canal->getName()));
		return ;
	}
	std::string msg = str.substr(str.find(':'), str.size());
	Message *newMsg = new Message (msg, *cli);
	canal->waitingMessages.push_back(newMsg);
	for (std::vector<Client *>::iterator it = canal->clients.begin(); it != canal->clients.end(); it++) {
		Client *cli = (*it);
		std::cout << "->" + cli->getNick() << std::endl;
	}
	(*canal->waitingMessages.rbegin())->clients = canal->clients; 
	std::cout << "Client :" << cli->getNick() << " send a message." << std::endl;
}

void	CommandManager::kick(std::string str, Client *cli) {
	std::string channel = str.substr(0, str.find(' '));
	std::string toKick = str.substr(str.find(' ') + 1, str.find(':') - str.find(' ') - 2);
	std::string reason = str.substr(str.find(':') + 1, str.size());
	// if (toKick.find(':') != toKick.npos)
		// toKick.erase(toKick.find(':'));
	// if (toKick.find(' ') != toKick.npos)
		// toKick.erase(toKick.find(' '));
	if (channel.size() == 0 || (channel[0] != '#' && channel[0] != '&' )|| str.find(':') == str.npos) {
		cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "KICK"));
		return ;
	}
	CanalManager *canalManager = _server->getCanalManager();
	std::cout << str << std::endl;
	std::cout << channel << std::endl;
	std::cout << "(" << toKick << ")" << std::endl;
	std::cout  << "(" << reason << ")" << std::endl;
	std::cout << str.find(':') << std::endl;
	Canal *canal = canalManager->GetChannel(channel);
	if (canal == NULL)
		return ;
	if (!canal->hasClient(cli->getNick())) {
		cli->send_msg(ERR_NOTONCHANNEL(cli->getNick(), canal->getName()));
		return ;
	}
	if (!canal->isOp(cli)) {
		cli->send_msg(ERR_CHANOPRIVSNEEDED(cli->getNick(), channel));
		return ;
	}
	if (!canal->hasClient(toKick)) {
		cli->send_msg(ERR_USERNOTINCHANNEL(cli->getNick(), toKick, channel));
		return ;
	}
	Client *tmp = canal->getClient(toKick);
	if (tmp == NULL) {
		cli->send_msg(ERR_USERNOTINCHANNEL(cli->getNick(), toKick, channel));
		return ;
	}
	if (canal->hasClient(tmp))
	{
		canal->broadcast(KICK(cli->getNick(), channel, toKick, reason));
		canal->deleteClient(tmp);
	} else {
		cli->send_msg(ERR_USERNOTINCHANNEL(cli->getNick(), toKick, channel));
	}
}

void	CommandManager::oper(std::string str) {
	std::string	name = str.substr(0, str.find(' '));
	std::string pass = str.substr(str.find(' ') + 1, str.size());
	std::cout << name << " : " << pass << std::endl;
	if (pass == "6432") {
		Client *tmp = _server->getClient(name);
		if (tmp != NULL) {
			tmp->setOp(true);
			std::cout << "op state : " << tmp->getOp() << std::endl;
		}
	}
}

void	CommandManager::whois(std::string str, Client *cli) {
	int j = 1;
	//TODO
	while ( j <= _server->getNumOpenFds() ) {
		Client *tmp =  _server->getClient(j - 1);
		if ( tmp->getNick() == str ) {

			std::string name = tmp->getNick() + "!" + tmp->getFullName() + "@" + tmp->getHost();
			std::string requesterName = cli->getNick() + "!" + cli->getFullName() + "@" + cli->getHost();
			std::string nick = tmp->getNick();
			std::string fullName = tmp->getFullName();
			// std::cout << tmp->getFullName() << " et " << client[i-1]->getNick() << std::endl;
			std::cout << RPL_WHOISUSER( requesterName, cli->getNick(), nick, name, tmp->getHost(), fullName ) << std::endl;
			std::cout << RPL_WHOISSERVER( requesterName, cli->getNick(), tmp->getNick(), requesterName, "This server") << std::endl;
			std::cout << RPL_ENDOFWHOIS( name, str) << std::endl;

			cli->send_msg( RPL_WHOISUSER( requesterName, cli->getNick(), nick, name, tmp->getHost(), fullName ) + RPL_WHOISSERVER( requesterName, cli->getNick(), tmp->getNick(), requesterName, "This server") + RPL_ENDOFWHOIS( name, str));
		}
		j++;
	}
}
