/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canal.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dasereno <dasereno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 16:58:43 by dasereno          #+#    #+#             */
/*   Updated: 2023/04/05 14:39:58 by dasereno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.h"

class Client;
class Message;
class CanalManager;

// typedef enum USER_MODE {
//     USER_I = 0,
//     USER_S = 1,
//     USER_W = 2,
//     USER_O = 3
// } user_mode ;


class Canal {

	private:
		std::string		_name;
		std::string		_pass;
		Client			&_op;
		std::vector<Client *>	_chanops;
		CanalManager	*_canalManager;
		std::vector<Client *>	_banned;
		std::vector<Client *>	_voiced;
		size_t				_maxClients;
		bool			_modeO;
		bool			_modeI;
		bool			_modeT;
		bool			_modeM;
		bool			_modeN;
		bool			_modeL;
		bool			_modeB;
		bool			_modeV;
		bool			_modeK;

    public: 
		std::vector<Client *> clients;
		std::vector<Message *> waitingMessages;

		Canal( std::string name, Client & op, CanalManager *cm ): _name(name), _pass(""), _op(op), _canalManager(cm), 
		_modeO(false), _modeI(false), _modeT(false), _modeM(false), _modeN(false), _modeL(false), _modeB(false), _modeK(false) { _chanops.push_back(&op); };
		~Canal( void ) { return ;};

		std::string getName( void ) const { return (_name); };
		void setName( std::string name ) { _name = name; };

		Client *getClient(std::string name);

		void	deleteClient(Client *);
		bool	hasClient(Client *);
		bool	hasClient(std::string);
		void	pushClient(Client *client) { clients.push_back(client); };
		void	pushBanned(Client *client) { _banned.push_back(client); };
		void	delBanned( Client *client );
		bool	isBanned(Client *client);
		Client &getOp( void ) { return _op; };

		bool	isOp( std::string name );
		bool	isOp( Client *client );

		void	addOp( Client *client ) {
			_chanops.push_back(client);
		}
		void	delOp( Client *client );

		size_t	getNbClient ( void ) { return clients.size(); };

		void	setMaxClient( size_t max ) { _maxClients = max;};
		size_t		getMaxClient( void ) { return _maxClients; };

		void	addVoiced( Client *client ) {
			_voiced.push_back(client);
		}
		void	delVoiced( Client *client );

		void			banClient(std::string name);
		void			printBanned(void);

		bool			getModeO( void ) { return (_modeO);};
		bool			getModeI( void ) { return (_modeI);};
		bool			getModeT( void ) { return (_modeT);};
		bool			getModeM( void ) { return (_modeM);};
		bool			getModeN( void ) { return (_modeN);};
		bool			getModeL( void ) { return (_modeL);};
		bool			getModeB( void ) { return (_modeB);};
		bool			getModeV( void ) { return (_modeV);};
		bool			getModeK( void ) { return (_modeK);};
		std::string		getPass( void ) { return (_pass);};

		void			setModeO( bool mode ) { _modeO = mode;};
		void			setModeI( bool mode ) { _modeI = mode;};
		void			setModeT( bool mode ) { _modeT = mode;};
		void			setModeM( bool mode ) { _modeM = mode;};
		void			setModeN( bool mode ) { _modeN = mode;};
		void			setModeL( bool mode ) { _modeL = mode;};
		void			setModeB( bool mode ) { _modeB = mode;};
		void			setModeV( bool mode ) { _modeV = mode;};
		void			setModeK( bool mode ) { _modeK = mode;};
		void			setPass( std::string pass ) { _pass = pass;};

		Canal&	operator=( const Canal& rhs);
		bool	operator==( const Canal& rhs );
};
