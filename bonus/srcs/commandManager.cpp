/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:24:36 by dasereno          #+#    #+#             */
/*   Updated: 2023/04/16 19:54:05 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.h"

void	CommandManager::pass(std::string str, Client *cli) {
	if (str.empty()) {
		cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "PASS"));
		return ;
	}
	cli->setPass( str );
}

void	CommandManager::nick(std::string str, Client *cli) {

	bool NickIsFree;
	if (str.empty()) {
		cli->send_msg(ERR_NONICKNAMEGIVEN(_server->getLocalhost()));
		return ;
	}
	if (!isValidNickname(str)) {
		cli->send_msg(ERR_ERRONEUSNICKNAME(_server->getLocalhost(), str));
		return ;
	}
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

	if (cli->getPass().empty()) {
		cli->send_msg(ERR_PASSWDMISMATCH( _server->getLocalhost(), cli->getNick() ) );
		quit(cli);
		return ;
	}
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
	if (str.size() == 0) {
		cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "JOIN"));
		return ;
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
		if (!isValidChannelName(str)) {
			cli->send_msg(ERR_BADCHANMASK(str));
			return ;
		}
		canal = canalManager->CreateChannel(str, cli);
		std::string users ("");
		std::vector<Client *> nicknames = canal->getClients();
		for (std::vector<Client *>::iterator it = nicknames.begin(); it != nicknames.end(); it++) {
			Client *tmp = *it;
			if (canal->isOp(tmp->getNick()) && !tmp->getModeI())
				users.append("@" + tmp->getNick() + " ");
			else if (!tmp->getModeI())
				users.append(tmp->getNick() + " ");
		}
		cli->send_msg(RPL_NAMREPLY(cli->getNick(), canal->getName(), users));
		cli->send_msg(RPL_ENDOFNAMES(cli->getNick(), canal->getName()));
		canal->broadcast(RPL_JOIN(cli->getPrefix(), canal->getName()));
		cli->send_msg(RPL_NOTOPIC(cli->getNick(), name));
	}
	if (canal->getModeK()) {
		if (pass.size() != 0 && pass == canal->getPass()) { }
		else {
			cli->send_msg(ERR_BADCHANNELKEY(cli->getNick(), name));
			return ;
		}
	}
	if (canal->getModeB() && canal->isBanned(cli)) {
		cli->send_msg(ERR_BANNEDFROMCHAN(cli->getNick(), canal->getName()));
		return ;
	}
	if (canal->getModeI() && !canal->isInvited(cli->getNick())) {
		cli->send_msg(ERR_INVITEONLYCHAN(cli->getNick(), canal->getName()));
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
		cli->send_msg(RPL_ENDOFNAMES(cli->getNick(), canal->getName()));
		canal->broadcast(RPL_JOIN(cli->getPrefix(), canal->getName()));
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

	if (str.size() == 0) {
		cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "INVITE"));
		return ;
	}
	if (!canal) {
		cli->send_msg(ERR_NOSUCHCHANNEL(cli->getNick(), channel));
		return ;
	}
	if (!canal->getModeI())
		return ;
	if (!canal->getClient(cli->getNick())) {
		cli->send_msg(ERR_NOTONCHANNEL(cli->getNick(), channel));
		return ;
	}
	if (!canal->isOp(cli)) {
		cli->send_msg(ERR_CHANOPRIVSNEEDED(cli->getNick(), channel));
		return ;
	}
	if (canal->hasClient(invited)) {
		cli->send_msg(ERR_USERONCHANNEL(cli->getNick(), invited, canal->getName()));
		return ;
	}
	canal->addInvite(invited);
	cli->send_msg(RPL_INVITING(cli->getNick(), invited, channel));
	Client *receiver = _server->getClient(invited);
	if (!receiver)
		return ;
	// :michou!~dasereno@shittyIRC INVITE dadou :#test
	receiver->send_msg(":" + cli->getPrefix() + " INVITE " + cli->getNick() + " :" + canal->getName() + "\r\n");
}

void	CommandManager::mode(std::string str, Client *cli) {
	std::vector<std::string>	args = split(str, " ");

	if (args.size() < 2)
	{
		cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "MODE"));
		return ;
	}
	bool sign = false; // true = '+' / false = '-'
	bool block = false;
	if (args[0][0] == '#' || args[0][0] == '&') { // CHANNEL MODE
		CanalManager	*canalManager = _server->getCanalManager();
		Canal *canal = canalManager->GetChannel(args[0]);
		if (!canal) {
			cli->send_msg(ERR_NOSUCHCHANNEL(cli->getNick(), args[0]));
			return ;
		}
		if (!canal->hasClient(cli)) {
			cli->send_msg(ERR_NOTONCHANNEL(cli->getNick(), args[0]));
		}
		// if (args[1][0] == '+') { // ADD MODES
		if (args[1][0] != '+' && args[1][0] != '-') {
			cli->send_msg(ERR_UNKNOWNMODE(cli->getNick(), args[1][0]));
			return ;
		}
		for (int j = 0; args[1][j]; j++) {
			if (args[1][j] == '+') {
				sign = true;
				continue ;
			} else if (args[1][j] == '-') {
				sign = false;
				continue ;
			}
			if (!canal->isOp(cli)) {
				cli->send_msg(ERR_CHANOPRIVSNEEDED(cli->getNick(), canal->getName()));
				return ;
			}
			if (sign) { // '+'
				switch (args[1][j])
				{
				case 'k':
					if (canal->getModeK() == true)
						break ;
					if (args.size() < 3) {
						cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "TOPIC"));
						return ;
					}
					canal->setPass(args[2]);
					canal->setModeK(true);
					block = true;
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+k", args[2]));
					break;
				case 'b': // IF ARGS size == 2 => PRINT BANNED USERS
					if (args.size() < 3) {
						cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "TOPIC"));
						return ;
					}
					canal->banClient(args[2]);
					canal->printBanned();
					canal->setModeB(true);
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+b", args[2]));
					block = true;
					break ;
				case 'o' : // IF ARGS size == 2 => PRINT OPERATOR USERS
					if (args.size() < 3) {
						cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "TOPIC"));
						return ;
					}
					if (canal->hasClient(args[2])) {
						canal->addOp(canal->getClient(args[2]));
					} else
						break ;
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+o", args[2]));
					block = true;
					break ;
				case 'i' :
					if (canal->getModeI()) {
						break ;
					}
					canal->setModeI(true);
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+i", ""));
					break ;
				case 'm' :
					if (canal->getModeM())
						break ;
					canal->setModeM(true);
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+m", ""));
					break ;
				case 'l' :
					if (args.size() < 3 || !str_isnum(args[2])) {
						cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "TOPIC"));
						return ;
					}
					canal->setModeL(true);
					canal->setMaxClient(atoi(args[2].c_str()));
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "+l", args[2]));
					block = true;
					break ;
				case 'v' :
					if (args.size() != 3) { // IF ARGS size == 2 => PRINT VOICED USERS
						cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "TOPIC"));
						return ;
					}
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
					canal->setPass("");
					canal->setModeK(false);
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-k", ""));
					break;
				case 'b': // IF ARGS size == 2 => PRINT BANNED USERS
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
					if (args.size() == 3 && canal->hasClient(args[2])) {
						canal->delOp(canal->getClient(args[2]));
						cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-o", args[2]));
						block = true;
					} else {
						// PRINT OP
						break ;
					}
					break ;
				case 'i' :
					if (!canal->getModeI())
						break ;
					canal->setModeI(false);
					canal->resetInvited();
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-i", ""));
					break ;
				case 'm' :
					if (!canal->getModeM())
						break ;
					canal->setModeM(false);
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-m", ""));
					break ;
				case 'l' :
					if (!canal->getModeL())
						break ;
					canal->setModeL(false);
					canal->setMaxClient(0);
					cli->send_msg(RPL_MODE(cli->getPrefix(), canal->getName(), "-l",  ""));
					block = true;
					break ;
				case 'v' :
					// IF ARGS size == 2 => PRINT VOICED USERS
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
		Client *tmp = _server->getClient(args[0]);
		if (!tmp) {
			cli->send_msg(ERR_NOSUCHNICK(cli->getNick(), args[0]));
			return ;
		}
			for (int j = 0; args[1][j]; j++) {
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
				case 'i' :
					if (tmp->getModeI() || !_server->isOp(cli))
						return ;
					tmp->setModeI(true);
					cli->send_msg(RPL_MODE(cli->getPrefix(), tmp->getNick(), "+i", ""));
					break ;
				default:
					cli->send_msg(ERR_UNKNOWNMODE(cli->getNick(), args[1][j]));
					return ;
				}
			} else {
				switch (args[1][j])
				{
				case 'i' :
					if (tmp->getModeI() || !_server->isOp(cli))
						return ;
					tmp->setModeI(false);
					cli->send_msg(RPL_MODE(cli->getPrefix(), tmp->getNick(), "-i", ""));
					break ;
				default:
					cli->send_msg(ERR_UNKNOWNMODE(cli->getNick(), args[1][j]));
					return ;
				}
			}
			if (block)
					return ;
		}
	}
}

void	CommandManager::topic(std::string str, Client *cli) {
	if (str.empty()) {
		cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "TOPIC"));
		return ;
	}
	CanalManager *canalManager = _server->getCanalManager();
	if (str.find(":") == str.npos) { // TODO
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
		if (canal->getTopic().size() > 0)
			cli->send_msg(RPL_TOPIC(cli->getNick(), channel, canal->getTopic()));
		else
			cli->send_msg(RPL_NOTOPIC(cli->getNick(), channel));
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

void	CommandManager::part(std::string str, Client *cli) { // TO DO: ALL ERRORS // HANDLE MULTIPLE PART	
	std::cout << "Client #" << cli->getNick() << " PART channel "<< str << std::endl;
	if (str.empty())
	{
		cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "PART"));
		return ;
	}
	std::string args = str.substr(str.find(':') + 1, str.size());
	std::vector<std::string> channels = split(args, ",");
	CanalManager	*canalManager = _server->getCanalManager();
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
		std::string canalName = (*it);
		Canal *canal = canalManager->GetChannel(canalName);
		if (!canal || (canal->getName()[0] != '#' && canal->getName()[0] != '&')) {
			cli->send_msg(ERR_NOSUCHCHANNEL(cli->getNick(), canalName));
		} else if (!canal->hasClient(cli)) {
			cli->send_msg(ERR_NOTONCHANNEL(cli->getNick(), canalName));
		}
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
	_server->setNumOpenFds(_server->getNumOpenFds() - 1);
}

void	CommandManager::quit(Client *cli) { // ERROR HANDLING???
	int	i = _server->getClientIndex(cli) + 1;
	std::cout << "client #" << i << " quit because bad pass" << std::endl;
	close( _server->getPfds(i).fd );
	_server->eraseClient(i - 1);
	// client.erase( client.begin() + i - 1 );
	for (int j = i; j + 1 <= _server->getNumOpenFds() + 1; j++)
	{
		_server->getPfds(j) = _server->getPfds(j + 1);
	}
	_server->setNumOpenFds(_server->getNumOpenFds() - 1);
}

void	CommandManager::_privmsgClient(std::string msg, CanalManager *canalManager, Client *cli, std::string receiver)
{
	if (receiver.empty()) {
		cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "PRIVMSG"));
		return ;
	}
	// std::string str = str.substr(0, str.find(' '));
	if (!_server->isClient(receiver)) {
		cli->send_msg(ERR_NOSUCHNICK(cli->getNick(), receiver));
		return ;
	}
	// std::string msg = str.substr(str.find(':'), str.size());
	if (!validateMessage(msg))
		return ;
	Message *newMsg = new Message (RPL_PRIVMSG(cli->getPrefix(), cli->getNick(), msg), *cli);
	Canal *newCanal = canalManager->CreateChannel(cli->getNick(), cli);
	newCanal->pushClient(_server->getClient(receiver));
	newCanal->waitingMessages.push_back(newMsg);
	(*newCanal->waitingMessages.rbegin())->clients = newCanal->clients; 
	std::cout << "Client :" << cli->getNick() << " send a private msg." << std::endl;
}

void	CommandManager::privmsg(std::string str, Client *cli) {
	if (str.find(':') == str.npos || str.find(' ') == str.npos || str.empty()) {
		cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "PRIVMSG"));
		return ;
	}
	std::string channel = str.substr(0, str.find(' '));
	std::vector<std::string> canals = split (channel, ",");
	for (std::vector<std::string>::iterator it = canals.begin(); it != canals.end(); it++) {
		std::string tmp = (*it);
		CanalManager	*canalManager = _server->getCanalManager();
		Canal	*canal = canalManager->GetChannel(tmp);
		std::string msg = str.substr(str.find(':'), str.size());
		if (tmp[0] == '#' && !canal) {
			cli->send_msg(ERR_NOSUCHCHANNEL(cli->getNick(), tmp));
			continue ;
		}
		if (!canal || (canal && tmp[0] != '#') || tmp[0] != '#') {
			_privmsgClient(msg, canalManager, cli, tmp);
			continue ;
		}
		if (!canal->hasClient(cli))
			continue ;
		if (canal->getModeM() && !canal->isVoiced(cli)) {
			cli->send_msg(ERR_CANNOTSENDTOCHAN(cli->getNick(), canal->getName()));
			continue ;
		}
		if (!validateMessage(msg))
			return ;
		Message *newMsg = new Message (":" + cli->getNick() + " PRIVMSG " + tmp + " " + msg+ "\r\n", *cli);
		canal->waitingMessages.push_back(newMsg);
		(*canal->waitingMessages.rbegin())->clients = canal->clients;
		
	}
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
	std::vector<std::string> toKicks = split(toKick, ",");
	if (channel.size() == 0 || (channel[0] != '#' && channel[0] != '&' ) || str.find(':') == str.npos) {
		cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "KICK"));
		return ;
	}
	CanalManager *canalManager = _server->getCanalManager();
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
	for (std::vector<std::string>::iterator i = toKicks.begin(); i != toKicks.end(); i++) {
		std::string kicked = (*i);
		if (!canal->hasClient(kicked)) {
			cli->send_msg(ERR_USERNOTINCHANNEL(cli->getNick(), kicked, channel));
			continue ;
		}
		Client *tmp = canal->getClient(kicked);
		if (tmp == NULL) {
			cli->send_msg(ERR_USERNOTINCHANNEL(cli->getNick(), kicked, channel));
			continue ;
		}
		if (canal->hasClient(tmp))
		{
			canal->broadcast(KICK(cli->getNick(), channel, kicked, reason));
			canal->deleteClient(tmp);
		} else {
			cli->send_msg(ERR_USERNOTINCHANNEL(cli->getNick(), kicked, channel));
		}
	}
}

void	CommandManager::oper(std::string str, Client *cli) {
	std::string	name = str.substr(0, str.find(' '));
	std::string pass = str.substr(str.find(' ') + 1, str.size());
	if (str.empty() || pass.empty() || name.empty()) {
		cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "OPER"));
		return ;
	}
	if (pass == "6432") {
		Client *tmp = _server->getClient(name);
		if (tmp != NULL) {
			tmp->setOp(true);
			_server->addOp(tmp);
			tmp->send_msg(RPL_YOUREOPER(_server->getLocalhost()));
			return ;
		}
	} else {
		cli->send_msg(ERR_PASSWDMISMATCH( _server->getLocalhost(), cli->getNick()));
		return ;
	}
}
void	CommandManager::_noticeClient(std::string msg, CanalManager *canalManager, Client *cli, std::string receiver)
{
	if (receiver.empty()) {
		return ;
	}
	// std::string str = str.substr(0, str.find(' '));
	if (!_server->isClient(receiver)) {
		return ;
	}
	// std::string msg = str.substr(str.find(':'), str.size());
	Message *newMsg = new Message (NOTICE(cli->getPrefix(), cli->getNick(), msg), *cli);
	Canal *newCanal = canalManager->CreateChannel(cli->getNick(), cli);
	newCanal->pushClient(_server->getClient(receiver));
	newCanal->waitingMessages.push_back(newMsg);
	(*newCanal->waitingMessages.rbegin())->clients = newCanal->clients; 
	std::cout << "Client :" << cli->getNick() << " send a message prive." << std::endl;
}

void	CommandManager::notice(std::string str, Client *cli) {
	if (str.find(':') == str.npos || str.find(' ') == str.npos || str.empty()) {
		cli->send_msg(ERR_NEEDMOREPARAMS(cli->getNick(), "NOTICE"));
		return ;
	}
	std::string channel = str.substr(0, str.find(' '));
	std::vector<std::string> canals = split (channel, ",");
	for (std::vector<std::string>::iterator it = canals.begin(); it != canals.end(); it++) {
		std::string tmp = (*it);
		CanalManager	*canalManager = _server->getCanalManager();
		Canal	*canal = canalManager->GetChannel(tmp);
		std::string msg = str.substr(str.find(':'), str.size());
		if (tmp[0] == '#' && !canal) {
			continue ;
		}
		if (!canal || (canal && tmp[0] != '#') || tmp[0] != '#') {
			_noticeClient(msg, canalManager, cli, tmp);
			continue ;
		}
		if (!canal->hasClient(cli))
			continue ;
		if (canal->getModeM() && !canal->isVoiced(cli)) {
			continue ;
		}
		Message *newMsg = new Message (":" + cli->getNick() + " NOTICE " + tmp + " " + msg+ "\r\n", *cli);
		canal->waitingMessages.push_back(newMsg);
		(*canal->waitingMessages.rbegin())->clients = canal->clients; 
		
	}
	std::cout << "Client :" << cli->getNick() << " send a message." << std::endl;
}

void	CommandManager::whois(std::string str, Client *cli) {
	int j = 1;
	while ( j <= _server->getNumOpenFds() ) {
		Client *tmp =  _server->getClient(j - 1);
		if ( tmp->getNick() == str && !tmp->getModeI()) {

			std::string name = tmp->getNick() + "!" + tmp->getFullName() + "@" + tmp->getHost();
			std::string requesterName = cli->getNick() + "!" + cli->getFullName() + "@" + cli->getHost();
			std::string nick = tmp->getNick();
			std::string fullName = tmp->getFullName();
			cli->send_msg( RPL_WHOISUSER( requesterName, cli->getNick(), nick, name, tmp->getHost(), fullName ) + RPL_WHOISSERVER( requesterName, cli->getNick(), tmp->getNick(), requesterName, "This server") + RPL_ENDOFWHOIS( name, str));
		}
		j++;
	}
}
